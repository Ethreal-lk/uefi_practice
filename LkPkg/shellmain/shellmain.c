#include <Uefi.h>
#include  <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include  <Library/ShellCEntryLib.h>

#define     DELAY_1_S       1000000


#define     DEFAULT_YEAR    2021
#define     DEFAULT_MOUTH   1
#define     DEFAULT_DAY     1
#define     DEFAULT_HOUR    0
#define     DEFAULT_MINUTE  0
#define     DEFAULT_SECOND  0

VOID SetDefaultTime(EFI_TIME * Time) {
    Time->Year  = DEFAULT_YEAR;
    Time->Month = DEFAULT_MOUTH;
    Time->Day   = DEFAULT_DAY;
    Time->Hour  = DEFAULT_HOUR;
    Time->Minute= DEFAULT_MINUTE;
    Time->Second= DEFAULT_SECOND;
    gRT->SetTime(Time);
    
}
VOID PrintTIme(EFI_TIME curTime) {
    Print(L"CurrentTime : %d-%d-%d  %02d:%02d:%02d\n", \
    curTime.Year, curTime.Month, curTime.Day, \
    curTime.Hour, curTime.Minute, curTime.Second);
}

INTN
EFIAPI
ShellAppMain (
    IN UINTN Argc,
    IN CHAR16 **Argv)
{
    EFI_TIME curTime;
    INTN  index;

    Print(L"hello ,this is Entry of shellMain!\n");
    gBS->Stall(DELAY_1_S);
    gRT->GetTime(&curTime,NULL);
    PrintTIme(curTime);
    SetDefaultTime(&curTime);
    for (index = 0; index < PRINT_COUNT; index++) {
        gRT->GetTime(&curTime,NULL);
        PrintTIme(curTime);
        gBS->Stall(DELAY_1_S);
    }


    gST->ConOut->OutputString(gST->ConOut, L"Test Systemtable ....\n\r");
    return 0;

}

