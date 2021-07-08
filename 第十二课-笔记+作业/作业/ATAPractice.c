/*
 * @Author: your name
 * @Date: 2021-06-05 15:47:13
 * @LastEditTime: 2021-06-22 14:32:01
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \edk2-UDK2018\AppPkg\Unit12\ATAPractice.c
 */
#include <Uefi.h>
#include <Base.h>
#include <Protocol/PciIo.h>
#include <Protocol/AtaPassThru.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <IndustryStandard/Atapi.h>

#define ATA_TIMEOUT 30000000

EFI_STATUS ATAPractice (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
  {
    
    EFI_STATUS Status;
    UINTN      HandleIndex;
    UINTN      HandleCount;
    UINT16     Port; 
    UINT16     PortMultiplierPort;
    UINT16     i;
    EFI_ATA_PASS_THRU_PROTOCOL          *AtaPassThru = NULL;
    EFI_ATA_PASS_THRU_COMMAND_PACKET    Packet;
    EFI_HANDLE                          *AtaPassThruHandleBuffer;
    EFI_ATA_COMMAND_BLOCK               Acb;
    EFI_ATA_STATUS_BLOCK                Asb;
    ATA_IDENTIFY_DATA                   IdentifyData;
    UINT32                              *PtrData;


    ZeroMem(&Acb, sizeof(Acb));
    ZeroMem(&Asb, sizeof(Asb));
  
    //Command data block
    Acb.AtaSectorCount  = 1;
    Acb.AtaSectorNumber = 0;
    Acb.AtaCylinderLow  = 0;
    Acb.AtaCylinderHigh = 0;
    Acb.AtaCommand = ATA_CMD_READ_SECTORS;  // 0x20h
    
    ZeroMem (&Packet, sizeof(Packet));
    Packet.Protocol      = EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_IN;
    Packet.Length        = EFI_ATA_PASS_THRU_LENGTH_BYTES;
    Packet.Asb           = &Asb;
    Packet.Acb           = &Acb; 
    Packet.InDataBuffer  = &IdentifyData;
    Packet.InTransferLength = sizeof(ATA_IDENTIFY_DATA );
    Packet.Timeout       = ATA_TIMEOUT;  //Delay three seconds

    // get the handles which supports
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiAtaPassThruProtocolGuid,
                    NULL,
                    &HandleCount,
                    &AtaPassThruHandleBuffer
                    );
    for(HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
    {
      Status = gBS->HandleProtocol (
                      AtaPassThruHandleBuffer[HandleIndex],
                      &gEfiAtaPassThruProtocolGuid,
                      (VOID**)&AtaPassThru
                      );                     
      Port = 0xFFFF;
      while (TRUE)
      {
        Status = AtaPassThru->GetNextPort (AtaPassThru, &Port);
        if ( EFI_ERROR(Status)) {
          break;
        }
        PortMultiplierPort = 0xFFFF;
        while (TRUE)
        {
          Status = AtaPassThru->GetNextDevice (AtaPassThru, Port, &PortMultiplierPort);
          if (EFI_ERROR(Status))
          {
            break;
          }
        
          Acb.AtaDeviceHead  = (UINT8) (BIT7 | BIT6 | BIT5 | PortMultiplierPort << 4);  //0 represent master ，1 represent slave
          Status = AtaPassThru->PassThru (
                                  AtaPassThru,
                                  Port,
                                  PortMultiplierPort,
                                  &Packet,
                                  NULL
                                  );
          Print(L"Port is %04x PortMultiplierPort is %04x\n", Port, PortMultiplierPort);
          // Read all hard disks’s first block data 
          PtrData = (UINT32*)&IdentifyData;
          for (i = 1; i <= sizeof(IdentifyData) / sizeof(UINT32); i++) {

              Print(L"%08x ", *PtrData);
              PtrData++;
              if(i % 4 == 0) {
                Print(L"\n");
              }
          }
          Print(L"\n\n");
          
        } 
      }
    }

    return EFI_SUCCESS;
  }
