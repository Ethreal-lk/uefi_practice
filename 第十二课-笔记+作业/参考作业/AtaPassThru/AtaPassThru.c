#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/AtaPassThru.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IndustryStandard/Atapi.h>
#include <Library/BaseMemoryLib.h>

#define ATA_TIMEOUT 3*1000*10000
/**
  Macro that returns the number of 100 ns units for a specified number of seconds.
  This is useful for managing EFI timer events.

  @param  Seconds                The number of seconds.

  @return The number of 100 ns units equivalent to the number of seconds specified
          by Seconds.

**/
#define EFI_TIMER_PERIOD_SECONDS(Seconds)           MultU64x32((UINT64)(Seconds), 10000000)
#define BLOCK_SIZE              512

EFI_STATUS
UefiMain (
  IN EFI_HANDLE *ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  ) {
    EFI_ATA_PASS_THRU_PROTOCOL        *AtaPassThru;
    EFI_HANDLE                        *AtaPassThruHandleBuff;
    UINT16                            Port = 0;
    EFI_STATUS                        Status = 0;
    EFI_ATA_PASS_THRU_COMMAND_PACKET  Packet;
    EFI_ATA_STATUS_BLOCK              Asb;
    EFI_ATA_COMMAND_BLOCK             Acb;
    UINT16                            PortMultiplierPort = 0;
    UINT8                             *Buff = NULL;
    UINT16                             Index = 0;
    UINTN HandleCount = 0;

    ZeroMem (&Asb, sizeof (Asb));
    ZeroMem (&Acb, sizeof (Acb));
    Acb.AtaCommand = ATA_CMD_READ_SECTORS; ///< 28-bit LBA; PIO read
    Acb.AtaSectorCount = 1;
    Acb.AtaSectorNumber = 0; ///< LBA0
    Acb.AtaCylinderLow = 0; ///< LBA1
    Acb.AtaCylinderHigh = 0; ///< LBA2
    ZeroMem (&Packet, sizeof(Packet));
    Status = gBS->AllocatePool(EfiBootServicesData, BLOCK_SIZE, (VOID**)&Buff);
    Print(L"Allocate Memory: %r\n", Status);
    // in as read,out as write relative to Memory
    Packet.Protocol = EFI_ATA_PASS_THRU_PROTOCOL_PIO_DATA_IN;
    Packet.Length = EFI_ATA_PASS_THRU_LENGTH_BYTES;
    Packet.Asb = &Asb;
    Packet.InDataBuffer = Buff;
    Packet.InTransferLength = 512;
    Packet.Timeout = ATA_TIMEOUT;
    Status = gBS->LocateHandleBuffer (
                      ByProtocol,
                      &gEfiAtaPassThruProtocolGuid,
                      NULL,
                      &HandleCount,
                      &AtaPassThruHandleBuff
  );
    Print(L"HandleCount: %d\n", HandleCount);
    Print (L"EfiAtaPassThruProtocolLocateHandleBuffer: %r\n", Status);
    for (Index = 0; Index < HandleCount; Index++) {
      Status = gBS->HandleProtocol (
                      AtaPassThruHandleBuff[Index],
                      &gEfiAtaPassThruProtocolGuid,
                      (VOID**)&AtaPassThru
      );
      Print (L"EfiAtaPassThruProtocol: %r\n", Status);
      Port = 0xFFFF;
      while (TRUE) {
        Status = AtaPassThru->GetNextPort (AtaPassThru, &Port);
        if (EFI_ERROR (Status)) {
          break;
        }
        PortMultiplierPort = 0xFFFF;
        while (TRUE) {
          Status = AtaPassThru->GetNextDevice(
                                    AtaPassThru,
                                    Port,
                                    &PortMultiplierPort
          );
          if (EFI_ERROR (Status)) {
            break;
          }
          Acb.AtaDeviceHead = (UINT8)(BIT7 | BIT6| BIT5 | (PortMultiplierPort << 4));
          Packet.Acb = &Acb;
          Status = AtaPassThru->PassThru(
                                  AtaPassThru,
                                  Port,
                                  PortMultiplierPort,
                                  &Packet,
                                  NULL
          );
          Print(L"AtaPassThru: %r  Port: 0x%04x  PortMultiplierPort: 0x%04x\n", Status,Port, PortMultiplierPort);
          if (!EFI_ERROR(Status)) {
            for (Index = 0; Index < BLOCK_SIZE; Index++) {
              Print(L"%02x ", Buff[Index]);
              if ((Index + 1) % 16 == 0) {
                Print (L"\n");
              }
            }
          }
          Print (L"\n");
          //ZeroMem (Buff, BLOCK_SIZE);
        }
      }
    }
    Status = gBS->FreePool (Buff);
    Print(L"FreeMemory: %r\n", Status);
    return EFI_SUCCESS;
  }