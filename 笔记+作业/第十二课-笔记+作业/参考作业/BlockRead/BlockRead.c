#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/BlockIo.h>

#define BLOCK_SIZE              512

EFI_STATUS
UefiMain (
  IN EFI_HANDLE *ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status = 0;
  UINTN HandleCount = 0;
  EFI_HANDLE *BlkBuff;
  UINT8 Index = 1;
  UINT16 i = 0;
  EFI_BLOCK_IO_PROTOCOL *BlkIo;
  UINT8 BootSector[BLOCK_SIZE];

  Status = gBS->LocateHandleBuffer (
                      ByProtocol,
                      &gEfiBlockIoProtocolGuid,
                      NULL,
                      &HandleCount,
                      &BlkBuff
  );
  Print (L"FindBlockIoProtocol:%r \n",Status);
  Print (L"HandleCount:%d \n",HandleCount);
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                     BlkBuff[Index],
                     &gEfiBlockIoProtocolGuid,
                     (VOID**)&BlkIo
    );
    if (BlkIo->Media->LogicalPartition == FALSE && BlkIo->Media->MediaPresent) {
      Print (L"%d ",Index);
      Print (L"OpenBlockIoProtocol:%r \n",Status);
      BlkIo->ReadBlocks (
                BlkIo,
                BlkIo->Media->MediaId,
                0, ///< LBA0 this is block address starting
                BLOCK_SIZE,
                BootSector
      );
      for (i = 0; i < BLOCK_SIZE; i++) {
        Print (L"%02x ",BootSector[i]);
        if ((i + 1) % 16 == 0) {
          Print (L"\n");
        }
      }
      Print (L"\n");
      Print (L"\n");
    }
  }


  return EFI_SUCCESS;
}