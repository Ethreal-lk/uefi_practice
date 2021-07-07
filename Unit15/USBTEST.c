/*
 * @Author: your name
 * @Date: 2021-07-06 13:02:31
 * @LastEditTime: 2021-07-06 13:04:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \edk2-UDK2018\AppPkg\unittest\udb.c
 */



#include "UsbBus.h"
/**
  Process the events on the port.

  @param  HubIf                 The HUB that has the device connected.
  @param  Port                  The port index of the hub (started with zero).

  @retval EFI_SUCCESS           The device is enumerated (added or removed).
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate resource for the device.
  @retval Others                Failed to enumerate the device.

**/
EFI_STATUS
UsbEnumeratePort (
  IN USB_INTERFACE        *HubIf,
  IN UINT8                Port
  )
{
  USB_HUB_API             *HubApi;
  USB_DEVICE              *Child;
  EFI_USB_PORT_STATUS     PortState;
  EFI_STATUS              Status;

  Child   = NULL;
  HubApi  = HubIf->HubApi;
  //
  // Host learns of the new device by polling the hub for port changes.
  //
  Status = HubApi->GetPortStatus (HubIf, Port, &PortState);

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "UsbEnumeratePort: failed to get state of port %d\n", Port));
    return Status;
  }

  //
  // Only handle connection/enable/overcurrent/reset change.
  // Usb super speed hub may report other changes, such as warm reset change. Ignore them.
  //
  if ((PortState.PortChangeStatus & (USB_PORT_STAT_C_CONNECTION | USB_PORT_STAT_C_ENABLE | USB_PORT_STAT_C_OVERCURRENT | USB_PORT_STAT_C_RESET)) == 0) {
    return EFI_SUCCESS;
  }

  DEBUG (( EFI_D_INFO, "UsbEnumeratePort: port %d state - %02x, change - %02x on %p\n",
              Port, PortState.PortStatus, PortState.PortChangeStatus, HubIf));

  //
  // This driver only process two kinds of events now: over current and
  // connect/disconnect. Other three events are: ENABLE, SUSPEND, RESET.
  // ENABLE/RESET is used to reset port. SUSPEND isn't supported.
  //
  
  if (USB_BIT_IS_SET (PortState.PortChangeStatus, USB_PORT_STAT_C_OVERCURRENT)) {     

    if (USB_BIT_IS_SET (PortState.PortStatus, USB_PORT_STAT_OVERCURRENT)) {
      //
      // Case1:
      //   Both OverCurrent and OverCurrentChange set, means over current occurs, 
      //   which probably is caused by short circuit. It has to wait system hardware
      //   to perform recovery.
      //
      DEBUG (( EFI_D_ERROR, "UsbEnumeratePort: Critical Over Current\n", Port));
      return EFI_DEVICE_ERROR;
      
    } 
    //
    // Case2:
    //   Only OverCurrentChange set, means system has been recoveried from 
    //   over current. As a result, all ports are nearly power-off, so
    //   it's necessary to detach and enumerate all ports again. 
    //
    DEBUG (( EFI_D_ERROR, "UsbEnumeratePort: 2.0 device Recovery Over Current\n", Port)); 
  }

  if (USB_BIT_IS_SET (PortState.PortChangeStatus, USB_PORT_STAT_C_ENABLE)) {  
    //
    // Case3:
    //   1.1 roothub port reg doesn't reflect over-current state, while its counterpart
    //   on 2.0 roothub does. When over-current has influence on 1.1 device, the port 
    //   would be disabled, so it's also necessary to detach and enumerate again.
    //
    DEBUG (( EFI_D_ERROR, "UsbEnumeratePort: 1.1 device Recovery Over Current\n", Port));
  }
  
  if (USB_BIT_IS_SET (PortState.PortChangeStatus, USB_PORT_STAT_C_CONNECTION)) {
    //
    // Case4:
    //   Device connected or disconnected normally. 
    //
    DEBUG ((EFI_D_INFO, "UsbEnumeratePort: Device Connect/Disconnect Normally\n", Port));
  }

  // 
  // Following as the above cases, it's safety to remove and create again.
  //
  Child = UsbFindChild (HubIf, Port);
  
  if (Child != NULL) {
    DEBUG (( EFI_D_INFO, "UsbEnumeratePort: device at port %d removed from root hub %p\n", Port, HubIf));
    UsbRemoveDevice (Child);
  }
  
  if (USB_BIT_IS_SET (PortState.PortStatus, USB_PORT_STAT_CONNECTION)) {
    //
    // Now, new device connected, enumerate and configure the device 
    //
    DEBUG (( EFI_D_INFO, "UsbEnumeratePort: new device connected at port %d\n", Port));
    if (USB_BIT_IS_SET (PortState.PortChangeStatus, USB_PORT_STAT_C_RESET)) {
      Status = UsbEnumerateNewDev (HubIf, Port, FALSE);
    } else {
      Status = UsbEnumerateNewDev (HubIf, Port, TRUE);
    }
  
  } else {
    DEBUG (( EFI_D_INFO, "UsbEnumeratePort: device disconnected event on port %d\n", Port));
  }
  
  HubApi->ClearPortChange (HubIf, Port);
  return Status;
}