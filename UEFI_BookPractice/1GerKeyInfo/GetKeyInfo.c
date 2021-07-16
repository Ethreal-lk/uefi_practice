/*
 * @Author: Running snail
 * @Date: 2021-07-15 15:03:43
 * @LastEditTime: 2021-07-16 17:15:34
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \edk2-UDK2018\AppPkg\Homework\PracticeExamples\1GerKeyInfo\GetKeyInfo.c
 */
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState);
EFI_STATUS GetKeyInfo(
              IN EFI_HANDLE ImageHandle, 
              IN EFI_SYSTEM_TABLE *gST
              ){
  EFI_STATUS Status;
  UINTN      Index;
  EFI_INPUT_KEY  Key;
  CHAR16 StrBUffer[3] = {0};
  UINT16 ScanCode = 0;
  UINT16 UniChar = 0;
  UINT32 ShiftState;
  UINT32 count = 0 ;
  EFI_KEY_TOGGLE_STATE ToggleState;

	gST->ConOut->OutputString(gST->ConOut,L"========= EDKII Test Samples ========\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Author: Running snail \n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Data: 2021-07-15 15:11:33\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Context: Control Keyboard input--\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"======================================\n\r");

  
	while(ScanCode!=0x17)	//ESC
	{
    Status=GetKeyEx(&ScanCode,&UniChar,&ShiftState,&ToggleState);
		if (EFI_ERROR (Status)) 
		{
			Print(L"Call GetKeyEx() Error!\n");
			break;
		}
		else
		{
			Print(L"NO.%08d\n",count);
			++count;
			Print(L"  ScanCode=%04x",ScanCode);
			Print(L"  UnicodeChar=%04x",UniChar);
			Print(L"  ShiftState=%08x",ShiftState);
			Print(L"  ToggleState=%02x",ToggleState);
			Print(L"\n");
		}
	}

  gST->BootServices->WaitForEvent (1, &gST->ConIn->WaitForKey, &Index);

  Status = gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
  StrBUffer[0] = Key.UnicodeChar;
  StrBUffer[1] = '\n';
  gST->ConOut->OutputString (gST->ConOut, StrBUffer);

  return EFI_SUCCESS;
} 


EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState)
{
	EFI_STATUS                        Status;
	EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx;
  EFI_KEY_DATA                      KeyData;
  EFI_HANDLE                        *Handles;
  UINTN                             HandleCount;
  UINTN                             HandleIndex;
  UINTN															Index;
  
  Status = gBS->LocateHandleBuffer (
              ByProtocol,							
              &gEfiSimpleTextInputExProtocolGuid,
              NULL,
              &HandleCount,
              &Handles
              );
  if(EFI_ERROR (Status))
  	return Status;

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (Handles[HandleIndex], &gEfiSimpleTextInputExProtocolGuid, (VOID **) &SimpleEx);
    
    if (EFI_ERROR(Status))	
    	continue;
		else
		{
			gBS->WaitForEvent(1,&(SimpleEx->WaitForKeyEx),&Index);
    	Status = SimpleEx->ReadKeyStrokeEx(SimpleEx,&KeyData);
    	if(!EFI_ERROR(Status))
    	{
    		*ScanCode=KeyData.Key.ScanCode;
  			*UniChar=KeyData.Key.UnicodeChar;
  			*ShiftState=KeyData.KeyState.KeyShiftState;
  			*ToggleState=KeyData.KeyState.KeyToggleState;
  			return EFI_SUCCESS;
  		}
    }
  }	 
	return Status;
}