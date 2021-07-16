#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS AllocteAddress (
  IN EFI_HANDLE ImageHandle ,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_PHYSICAL_ADDRESS pages  = 0x28E0000 ; 
  EFI_STATUS           Status = EFI_SUCCESS;
  CHAR16*              str    = NULL;
  CHAR16*              Array  = L"Byosoft BIOS";
  // allocate memory
  Status = gBS->AllocatePages (
           AllocateAddress, 
           EfiBootServicesData, 
           3, 
           &pages
); 
  Print (L"AllocateAddress:%r %x\n", Status, pages);

  if (Status == 0) {
      str = (CHAR16*)(UINTN)pages;
      StrCpy (str,Array);         // Copy the data in the array 
      Print (L"%s\n",str);
      Status = gBS->FreePages (pages, 3);
}
  return EFI_SUCCESS;
    
}