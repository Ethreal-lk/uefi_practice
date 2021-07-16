/*
 * @Author: Running snail
 * @Date: 2021-07-15 15:03:43
 * @LastEditTime: 2021-07-16 16:11:16
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \edk2-UDK2018\AppPkg\Homework\PracticeExamples\1GerKeyInfo\GetKeyInfo.c
 */
#include <Uefi.h>
EFI_STATUS GetKeyInfo(
              IN EFI_HANDLE ImageHandle, 
              IN EFI_SYSTEM_TABLE *gST
              ){
  EFI_STATUS Status;
  UINTN      Index;
  EFI_INPUT_KEY  Key;
  CHAR16 StrBUffer[3] = {0};

	gST->ConOut->OutputString(gST->ConOut,L"========= EDKII Test Samples ========\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Author: Running snail \n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Data: 2021-07-15 15:11:33\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Context: Control Keyboard input--\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"======================================\n\r");
  gST->BootServices->WaitForEvent (1, &gST->ConIn->WaitForKey, &Index);

  Status = gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
  StrBUffer[0] = Key.UnicodeChar;
  StrBUffer[1] = '\n';
  gST->ConOut->OutputString (gST->ConOut, StrBUffer);


  

  

  return EFI_SUCCESS;
}              