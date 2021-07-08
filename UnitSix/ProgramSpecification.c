#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>


EFI_STATUS TestMMap()
{
  EFI_STATUS             Status = 0;
  UINTN                  MemoryMapSize = 0;
  EFI_MEMORY_DESCRIPTOR  *MemoryMap = 0;
  UINTN                  MapKey = 0;
  UINTN                  DescriptorSize = 0;
  UINTN                  DescriptorVersion = 0;
  UINTN                  i = 0;
  EFI_MEMORY_DESCRIPTOR *MMap = 0;

//Get the buffer size
  Status = gBS->GetMemoryMap (
                  &MemoryMapSize,
                  MemoryMap,
                  &MapKey,
                  &DescriptorSize,
                  &DescriptorVersion
  );
  if(Status != EFI_BUFFER_TOO_SMALL) {
    return Status;
  }

//Allocate memory
  Status = gBS->AllocatePool (
                EfiBootServicesData,
                MemoryMapSize,
                &MemoryMap
);
  Status = gBS->GetMemoryMap (
                &MemoryMapSize,
                MemoryMap,
                &MapKey,
                &DescriptorSize,
                &DescriptorVersion
);
  for ( i = 0; i < MemoryMapSize / DescriptorSize; i++) {
    MMap = (EFI_MEMORY_DESCRIPTOR*)(((CHAR8*)MemoryMap) + i * DescriptorSize);
    Print(L"MemoryMap %4d %10d :",MMap[0].Type, MMap[0].NumberOfPages);
    Print(L"%101x<->",MMap[0].PhysicalStart);
    Print(L"%101x\n", MMap[0].VirtualStart);

  }
  Status = gBS->FreePool(MemoryMap);
  return Status;

  

}


