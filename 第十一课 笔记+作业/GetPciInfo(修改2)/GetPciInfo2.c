/*
 * @Author: your name
 * @Date: 2021-06-10 16:04:09
 * @LastEditTime: 2021-06-11 14:43:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \edk2-UDK2018\AppPkg\Unit11\GetPciINfo2.c
 */

#include <Uefi.h>
#include <Base.h>
#include <Protocol/PciIo.h>
#include <IndustryStandard/Pci.h> 
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS GetClassname(
            IN UINT8  Data_ClassCode,
            IN UINT8  Data_SubClassCode
);
  

EFI_STATUS GetPciInfo2 (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_HANDLE    *PciHandleBuffer;
  PCI_TYPE00    Pci;
  UINTN         HandleIndex = 0;
  UINTN         HandleCount;
  UINTN         SegmentNumber;
  UINTN         BusNumber;
  UINTN         DeviceNumber;
  UINTN         FunctionNumber;
  // UINT32        Address;
  EFI_PCI_IO_PROTOCOL   *PciIo = NULL;

  //get the handles which supports 
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiPciIoProtocolGuid,
		NULL,
		&HandleCount,
		&PciHandleBuffer
		);
    
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
                  PciHandleBuffer[HandleIndex],
                  &gEfiPciIoProtocolGuid,
                  (VOID**)&PciIo
                  );
    Status = PciIo->GetLocation (
                      PciIo,
                      &SegmentNumber,
                      &BusNumber,
                      &DeviceNumber,
                      &FunctionNumber
                      );
    Print(L"Bus: %d Device: %d Function: %d\n", BusNumber, DeviceNumber, FunctionNumber);
    
    // Read the entire config header for the device  
    Status = PciIo->Pci.Read (
                              PciIo,
                              EfiPciIoWidthUint32,
                              0,
                              sizeof (PCI_TYPE00) / sizeof (UINT32),
                              &Pci
                              );  
    Print(L"VenderId : %04x DeviceId : %04x\n", Pci.Hdr.VendorId, Pci.Hdr.DeviceId);  
    Print(L"Status: %04x Command: %04x\nRevison ID: %02x\n",Pci.Hdr.Status, Pci.Hdr.Command, Pci.Hdr.RevisionID);
    Print(L"Classcode: %02x%02x%02x\n", Pci.Hdr.ClassCode[0], Pci.Hdr.ClassCode[1], Pci.Hdr.ClassCode[2]);
    GetClassname(Pci.Hdr.ClassCode[2], Pci.Hdr.ClassCode[1]);
    Print(L"\nCacheLineSize: %02x LatencyTimer: %02x \nHeaderType: %02x BIST: %02x\n", Pci.Hdr.CacheLineSize, Pci.Hdr.LatencyTimer, Pci.Hdr.HeaderType, Pci.Hdr.BIST);
    Print(L"BaseAddress0 = %08x\n",Pci.Device.Bar[0]);
    Print(L"BaseAddress2 = %08x\n",Pci.Device.Bar[1]);
    Print(L"BaseAddress2 = %08x\n",Pci.Device.Bar[2]);
    Print(L"BaseAddress3 = %08x\n",Pci.Device.Bar[3]);
    Print(L"BaseAddress4 = %08x\n",Pci.Device.Bar[4]);
    Print(L"BaseAddress5 = %08x\n",Pci.Device.Bar[5]);
    Print(L"Subsystem ID: %04x  Subsystem Vender ID: %04x\n", Pci.Device.SubsystemID, Pci.Device.SubsystemVendorID);
    Print(L"Expansion ROM Base Address: %08x\n",Pci.Device.ExpansionRomBar);
    Print(L"Interrupt PIn: %02x Interrupt Line: %02x\n", Pci.Device.InterruptPin, Pci.Device.InterruptLine);
    Print(L"Max_lat: %02x MIn_Gnt: %02x\n\n\n",Pci.Device.MinGnt, Pci.Device.MaxLat);
	}

  return EFI_SUCCESS;
}

/**
 * @description: Determine the type of equipment
 * @param  Data_ClassCode             base class
 * @param  Data_SubClassCode          sub class
 * @return EFI_SUCCESS
 */
EFI_STATUS GetClassname(
            IN UINT8  Data_ClassCode,
            IN UINT8  Data_SubClassCode
)
{
  Print(L"Classname: ");
  
  switch(Data_ClassCode){ 
    case 0x00:
      Print(L"Pre 2.0 device");
      Print(L" - ");	
      if(Data_SubClassCode==0x00){
      Print(L"All devices other than VGA");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"VGA-compatible devices");
      }
      else{
      Print(L"Error");
      }
      break;
    case 0x01:
      Print(L"Mass Storage Controller");
      Print(L" - ");
      
      if(Data_SubClassCode==0x00){
      Print(L"SCSI");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"IDE controller");
      }
      else if(Data_SubClassCode==0x02){
      Print(L"Floppy disk controller");
      }
      else if(Data_SubClassCode==0x03){
      Print(L"IPI controller");
      }
      else if(Data_SubClassCode==0x04){
      Print(L"RAID controller");
      }
      else if(Data_SubClassCode==0x05){
      Print(L"ATA controller with ADMA interface");
      }
      else if(Data_SubClassCode==0x06){
      Print(L"Serial ATA controller");
      }
      else if(Data_SubClassCode==0x07){
      Print(L"Serial Attached SCSI (SAS) controller");
      }			
      else if(Data_SubClassCode==0x08){
      Print(L"Non-volatile memory subsystem");
      }					
      else if(Data_SubClassCode==0x09){
      Print(L"Universal Flash Storage (UFS) controller");
      }
      else if(Data_SubClassCode==0x80){
      Print(L"Other mass storage controller");
      }
      else{
      Print(L"Error");
      }	
      
      break;
    case 0x02:
      Print(L"Network Controller");
      Print(L" - ");

      if(Data_SubClassCode==0x00){
      Print(L"Ethernet controller");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"Token ring controller");
      }
      else if(Data_SubClassCode==0x02){
      Print(L"FDDI controller");
      }
      else if(Data_SubClassCode==0x03){
      Print(L"ATM controller");
      }
      else if(Data_SubClassCode==0x04){
      Print(L"ISDN controller");
      }
      else if(Data_SubClassCode==0x05){
      Print(L"WorldFip controller");
      }
      else if(Data_SubClassCode==0x06){
      Print(L"PICMG 2.14 Multi Computing");
      }
      else if(Data_SubClassCode==0x07){
      Print(L"InfiniBand controller");
      }			
      else if(Data_SubClassCode==0x80){
      Print(L"Other network controller");
      }
      else{
      Print(L"Error");
      }
      
      break;
    case 0x03:
      Print(L"Display Controller");
      Print(L" - ");
    
        if(Data_SubClassCode==0x00){
      Print(L"VGA/8514 controller");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"XGA controller");
      }
      else if(Data_SubClassCode==0x02){
      Print(L"3D controller");
      }
      else if(Data_SubClassCode==0x80){
      Print(L"Other display controller");
      }
      else{
      Print(L"Error");
      }
      
      break;
    case 0x04:
      Print(L"Multimedia Device");
      Print(L" - ");

      if(Data_SubClassCode==0x00){
      Print(L"Video device");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"Audio device");
      }
      else if(Data_SubClassCode==0x02){
      Print(L"Computer Telephony device");
      }
      else if(Data_SubClassCode==0x03){
      Print(L"Mixed mode device");
      }
      else if(Data_SubClassCode==0x80){
      Print(L"Other multimedia device");
      }
      else{
      Print(L"Error");
      }
      
      break;
    case 0x05:
      Print(L"Memory Controller");
      Print(L" - ");
      if(Data_SubClassCode==0x00){
      Print(L"RAM memory controller");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"Flash memory controller");
      }
      else if(Data_SubClassCode==0x80){
      Print(L"Other memory controller");
      }
      else{
      Print(L"Error");
      }
      break;
        case 0x06:
      Print(L"Bridge Device");
      Print(L" - ");
    
      if(Data_SubClassCode==0x00){
      Print(L"Host/PCI bridge");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"PCI/ISA bridge");
      }
      else if(Data_SubClassCode==0x02){
      Print(L"PCI/EISA bridge");
      }
      else if(Data_SubClassCode==0x03){
      Print(L"PCI/Micro Channel bridge");
      }
      else if(Data_SubClassCode==0x04){
      Print(L"PCI/PCI bridge");
      }
      else if(Data_SubClassCode==0x05){
      Print(L"PCI/PCMCIA bridge");
      }
      else if(Data_SubClassCode==0x06){
      Print(L"NuBus bridge");
      }
      else if(Data_SubClassCode==0x07){
      Print(L"CardBus bridge");
      }			
      else if(Data_SubClassCode==0x08){
      Print(L"RACEway bridge");
      }					
      else if(Data_SubClassCode==0x09){
      Print(L"Semi-transparent PCI-to-PCI bridge");
      }
      else if(Data_SubClassCode==0x0A){
      Print(L"InfiniBand-to-PCI host bridge");
      }
      else if(Data_SubClassCode==0x0B){
      Print(L"Advanced Switching to PCI host bridge");
      }
      else if(Data_SubClassCode==0x80){
      Print(L"Other bridge type");
      }
      else{
      Print(L"Error");
      }					
    
      break;			
    case 0x07:
      Print(L"Simple Communications Controllers");
      Print(L" - ");
      if(Data_SubClassCode==0x00){
      Print(L"Serial controller");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"Parallel port");
      }
      else if(Data_SubClassCode==0x02){
      Print(L"Multiport serial controller");
      }
      else if(Data_SubClassCode==0x03){
      Print(L"Modem");
      }
      else if(Data_SubClassCode==0x04){
      Print(L"GPIB (IEEE 488.1/2) controller");
      }
      else if(Data_SubClassCode==0x05){
      Print(L"Smart Card");
      }
      else if(Data_SubClassCode==0x80){
      Print(L"Other communication device");
      }
      else{
      Print(L"Error");
      }	
      break;			
    case 0x08:
      Print(L"Base System Peripherals");
      Print(L" - ");

      if(Data_SubClassCode==0x00){
      Print(L"PIC");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"DMA controller");
      }
      else if(Data_SubClassCode==0x02){
      Print(L"System timer");
      }
      else if(Data_SubClassCode==0x03){
      Print(L"RTC controller");
      }
      else if(Data_SubClassCode==0x04){
      Print(L"Generic PCI Hot-Plug controller");
      }
      else if(Data_SubClassCode==0x05){
      Print(L"SD Host controller");
      }
      else if(Data_SubClassCode==0x06){
      Print(L"IOMMU");
      }
      else if(Data_SubClassCode==0x07){
      Print(L"Root Complex Event Collector");
      }			
      else if(Data_SubClassCode==0x80){
      Print(L"Other system peripheral");
      }
      else{
      Print(L"Error");
      }
      
      break;			
    case 0x09:
      Print(L"Input Devices");
      Print(L" - ");
      if(Data_SubClassCode==0x00){
      Print(L"Keyboard controller");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"Digitizer (pen)");
      }
      else if(Data_SubClassCode==0x02){
      Print(L"Mouse controller");
      }
      else if(Data_SubClassCode==0x03){
      Print(L"Scanner controller");
      }
      else if(Data_SubClassCode==0x04){
      Print(L"Gameport controller");
      }	
      else if(Data_SubClassCode==0x80){
      Print(L"Other input controller");
      }
      else{
      Print(L"Error");
      }
      break;			
    case 0x0a:
      Print(L"Docking Stations");
      Print(L" - ");
      
      if(Data_SubClassCode==0x00){
      Print(L"Generic docking station");
      }	
      else if(Data_SubClassCode==0x80){
      Print(L"Other type of docking station");
      }
      else{
      Print(L"Error");
      }
      
      break;			
    case 0x0b:
      Print(L"Processors");
      Print(L" - ");
      if(Data_SubClassCode==0x00){
      Print(L"386");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"486");
      }
      else if(Data_SubClassCode==0x02){
      Print(L"Pentium");
      }
      else if(Data_SubClassCode==0x10){
      Print(L"Alpha");
      }
      else if(Data_SubClassCode==0x20){
      Print(L"PowerPC");
      }	
      else if(Data_SubClassCode==0x30){
      Print(L"MIPS");
      }
      else if(Data_SubClassCode==0x40){
      Print(L"Co-processor");
      }
      else if(Data_SubClassCode==0x80){
      Print(L"Other processor");
      }
      else{
      Print(L"Error");
      }
      
      break;			
    case 0x0c:
      Print(L"Serial Bus Controllers");
      Print(L" - ");

      if(Data_SubClassCode==0x00){
      Print(L"IEEE 1394");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"ACCESS.bus");
      }
      else if(Data_SubClassCode==0x02){
      Print(L"SSA");
      }
      else if(Data_SubClassCode==0x03){
      Print(L"USB");
      }
      else if(Data_SubClassCode==0x04){
      Print(L"Fibre Channel");
      }
      else if(Data_SubClassCode==0x05){
      Print(L"System Management Bus");
      }
      else if(Data_SubClassCode==0x06){
      Print(L"InfiniBand");
      }
      else if(Data_SubClassCode==0x07){
      Print(L"IPMI");
      }
      else if(Data_SubClassCode==0x08){
      Print(L"SERCOS Interface Standard (IEC 61491)");
      }	
      else if(Data_SubClassCode==0x09){
      Print(L"CANbus");
      }		
      else if(Data_SubClassCode==0x80){
      Print(L"Other bus type");
      }
      else{
      Print(L"Error");
      }
      
      break;			
    case 0x0d:
      Print(L"Wireless Controllers");
      Print(L" - ");
      if(Data_SubClassCode==0x00){
      Print(L"iRDA compatible controller");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"");
      }
      else if(Data_SubClassCode==0x10){
      Print(L"RF controller");
      }
      else if(Data_SubClassCode==0x11){
      Print(L"Bluetooth");
      }
      else if(Data_SubClassCode==0x12){
      Print(L"Broadband");
      }
      else if(Data_SubClassCode==0x20){
      Print(L"Ethernet (802.11a - 5 GHz)");
      }
      else if(Data_SubClassCode==0x21){
      Print(L"Ethernet (802.11b - 2.4 GHz)");
      }
      else if(Data_SubClassCode==0x80){
      Print(L"Other type of wireless controller");
      }
      else{
      Print(L"Error");
      }
      break;			
    case 0x0e:
      Print(L"Intelligent IO Controllers");
      Print(L" - ");
      if(Data_SubClassCode==0x00){
      Print(L"I2O Architecture");
      }
      else{
      Print(L"Error");
      }
      break;			
    case 0x0f:
      Print(L"Satellite Communications Controllers");
      Print(L" - ");
      if(Data_SubClassCode==0x01){
      Print(L"TV");
      }
      else if(Data_SubClassCode==0x02){
      Print(L"Audio");
      }
      else if(Data_SubClassCode==0x03){
      Print(L"Voice");
      }
      else if(Data_SubClassCode==0x04){
      Print(L"Data");
      }
      else if(Data_SubClassCode==0x80){
      Print(L"Other satellite communication controller");
      }
      else{
      Print(L"Error");
      }
      break;			
    case 0x10:
      Print(L"Encryption/Decryption Controllers");
      Print(L" - ");

      if(Data_SubClassCode==0x00){
      Print(L"Network & computing Encrypt/Decrypt");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"Entertainment Encrypt/Decrypt");
      }
      else if(Data_SubClassCode==0x80){
      Print(L"Other Encrypt/Decrypt");
      }
      else{
      Print(L"Error");
      }
      
      break;			
    case 0x11:
      Print(L"Data Acquisition & Signal Processing Controllers");
      Print(L" - ");
      if(Data_SubClassCode==0x00){
      Print(L"DPIO modules");
      }
      else if(Data_SubClassCode==0x01){
      Print(L"Performance Counters");
      }
      else if(Data_SubClassCode==0x10){
      Print(L"Communications synchronization plus time and frequency test/measurement");
      }
      else if(Data_SubClassCode==0x20){
      Print(L"Management card");
      }
      else if(Data_SubClassCode==0x80){
      Print(L"Other DAQ & SP controllers");
      }
      else{
      Print(L"Error");
      }
      break;			
    case 0x12:
      Print(L"Processing Accelerators");
      Print(L" - ");
      break;			
    case 0x13:
      Print(L"Non-Essential Instrumentation");
      Print(L" - ");
      break;			
    case 0xff:
      Print(L"Device does not fit in any defined classes");
      break;			
    default:
      Print(L"Error");
      break;				
  }  

  return EFI_SUCCESS;
}