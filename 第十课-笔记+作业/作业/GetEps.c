/*
 * @Author: your name
 * @Date: 2021-04-30 11:26:41
 * @LastEditTime: 2021-05-25 18:28:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \edk2-UDK2018\AppPkg\UniTen\GetEps.c
 */
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>
#include <Library/MemoryAllocationLib.h>

UINTN
StrLen1 (
   CHAR8     *String
  ) 
  {
  UINTN   Length;
  for (Length = 0; *String != '\0'; String++, Length++) {
    //
    // If PcdMaximumUnicodeStringLength is not zero,
    // length should not more than PcdMaximumUnicodeStringLength
    //
  }
  return Length;
}

typedef struct 
{
  UINT8  Num;
  CHAR8  *Data;
} StrData;

EFI_STATUS GetEps (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
  {
    UINTN               StructureSize = 0;
    UINTN               TotalSize = 0;
    UINTN               StrSize = 0;
    UINTN               i = 0;  
    EFI_STATUS          Status;
    EFI_SMBIOS_PROTOCOL *Smbios;
    EFI_SMBIOS_STRING   *StrPtr;
    SMBIOS_TABLE_TYPE13 *SmbiosTableType13;
    StrData  LanguageData[7]= {
                              {1, "en_US"}, 
                              {2, "fr_FR"},
                              {3, "ja_JP"},
                              {4, "ko_KR"},
                              {5, "zh_CHI"},
                              {6, "zhu_CHS"},
                              {7, "ru_RU"}
                              };

    Status = gBS->LocateProtocol (
              &gEfiSmbiosProtocolGuid,
              NULL,
              (VOID **) &Smbios
              );
    if (EFI_ERROR (Status)) {
      return EFI_PROTOCOL_ERROR ;
    }  
    // Calculate size of DMI type 16
     StructureSize = sizeof (SMBIOS_TABLE_TYPE13);
     for ( i = 0; i < 7; i++)
     {
       StrSize += StrLen1 (LanguageData[i].Data) + 1; 
       Print(L"the length 0f %d number is %d\n", i, StrSize);
     }
     TotalSize = StructureSize + StrSize + 1;
     Print(L"Totalsize is %d\n", TotalSize);
     
    // allocate zero pool
    SmbiosTableType13 = NULL;
    SmbiosTableType13 = AllocateZeroPool (TotalSize);

    // Generate DMI type 13 
    SmbiosTableType13->Hdr.Type               = 13;
    SmbiosTableType13->Hdr.Length             = 22;
    SmbiosTableType13->Hdr.Handle             = 43;
    SmbiosTableType13->InstallableLanguages   = 7;
    SmbiosTableType13->Flags                  = 1;
    SmbiosTableType13->Reserved                  ;
    StrPtr = (EFI_SMBIOS_STRING*) (((UINT8*) SmbiosTableType13) + StructureSize + 1);
    for ( i = 0; i < 7; i++)
    {

      gBS->CopyMem (
              StrPtr,
              LanguageData[i].Data,
              StrLen1(LanguageData[i].Data) 
               
            );
      StrPtr =StrPtr + StrLen1 (LanguageData[i].Data)+ 2;  //Point to the first address of the next string
    }
    Status = Smbios->Add (
                      Smbios,
                      NULL,                    
                      &SmbiosTableType13->Hdr.Handle, 
                      (EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType13
                      );
    return EFI_SUCCESS;
    
  }