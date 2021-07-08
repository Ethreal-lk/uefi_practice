#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>

EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle , IN EFI_SYSTEM_TABLE *SystemTable)
{

    UINT8  Year   = 0;
    UINT8  Month  = 0;
    UINT8  Day    = 0;
    UINT8  Hour   = 0;
    UINT8  Minute = 0;
    UINT8  Second = 0;
    IoWrite8(0x70,0x09);
    Year=IoRead8(0x71) ;

    IoWrite8(0x70,0x08);
    Month=IoRead8(0x71);

    IoWrite8(0x70,0x07);
    Day=IoRead8(0x71)  ;

    IoWrite8(0x70,0x04);
    Hour=IoRead8(0x71) ;

    IoWrite8(0x70,0x02) ;
    Minute=IoRead8(0x71);

    IoWrite8(0x70,0x00) ;
    Second=IoRead8(0x71);

    Print(L"time is :%d/%d/%d  %2d:%2d:%2d\n",Year,Month,Day,Hour,Minute,Second);
    SystemTable->ConOut ->OutputString(SystemTable->ConOut, L"hell0world!!\n");
    
    return EFI_SUCCESS;

    
}