//#include <stdio.h>
//#include <Uefi.h>
//#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
//#include <Library/UefiApplicationEntryPoint.h>




#define PCIEX_BASE_ADDRESS  0xE0000000

UINT32
EFIAPI
MmioRead32_test (
      IN  UINTN                     Address
  )
{
  return *(volatile UINT32*)Address;
}






UINT8 bus,dev,func,reg;
UINT32 PciMmioBaseaddr,MmioPciData,Data_ClassCode,Data_SubClassCode,Data_ProgInterface;
UINT8 registerNum;




EFI_STATUS
EFIAPI PciDetail1(CHAR16 **argv){
bus = (CHAR8)StrHexToUintn (argv[1]);
dev = (CHAR8)StrHexToUintn (argv[2]);
func = 0x00;
Print(L"PCI Bus:%02x Device:%02x Function:%02x",bus,dev,func);

for(registerNum=0x00;registerNum<=0x3F;registerNum++){   //3ff
	reg = registerNum;
	PciMmioBaseaddr =(UINT32)(PCIEX_BASE_ADDRESS | bus << 20 | dev << 15 | func << 12 | reg << 2);
    MmioPciData = MmioRead32_test(PciMmioBaseaddr);
	if (registerNum%0x04 == 0)
		{
		Print(L"\n");
		}
	if (registerNum%0x01 == 0)
		{		
		Print(L" - ");
		}
		Print(L"%08x",MmioPciData);
}
Print(L"\n");
return EFI_SUCCESS;
}




EFI_STATUS
EFIAPI PciDetail2(CHAR16 **argv){
bus = (CHAR8)StrHexToUintn (argv[1]);
dev = (CHAR8)StrHexToUintn (argv[2]);
func = (CHAR8)StrHexToUintn (argv[3]);
Print(L"PCI Bus:%02x Device:%02x Function:%02x",bus,dev,func);
/*
The test case:

     PciMmioBaseaddr =(UINT32)( PCIEX_BASE_ADDRESS|bus << 20|dev << 15|func << 12);

	 Print(L"test_PciMmioBaseaddr:%x\n",PciMmioBaseaddr);
	 MmioPciData = MmioRead32_test(PciMmioBaseaddr);
	 Print(L"test_MmioPciData:%x\n",MmioPciData);
	 
	 PciMmioBaseaddr1=PciMmioBaseaddr | (0x01<<2);
	 MmioPciData = MmioRead32_test(PciMmioBaseaddr1); 
     Print(L"test_01hPciMmioBaseaddr:%x\n",PciMmioBaseaddr1);
	 Print(L"test_01h:%x\n",MmioPciData);

	 PciMmioBaseaddr1=PciMmioBaseaddr | (0x02<<2);
	 MmioPciData = MmioRead32_test(PciMmioBaseaddr1); 
	 Print(L"test_02hPciMmioBaseaddr:%x\n",PciMmioBaseaddr1);
     Print(L"test_02h:%x\n",MmioPciData);
	 
	 PciMmioBaseaddr1=PciMmioBaseaddr | (0x03<<2);
	 MmioPciData = MmioRead32_test(PciMmioBaseaddr1); 
	 Print(L"test_03hPciMmioBaseaddr:%x\n",PciMmioBaseaddr1);
	 Print(L"test_03h:%x\n",MmioPciData);

     PciMmioBaseaddr1=PciMmioBaseaddr | (0x04<<2);
	 MmioPciData = MmioRead32_test(PciMmioBaseaddr1); 
	 Print(L"test_04hPciMmioBaseaddr:%x\n",PciMmioBaseaddr1);
	 Print(L"test_04h:%x\n",MmioPciData);

	 PciMmioBaseaddr2=PciMmioBaseaddr | (0x08<<2) ;
	 MmioPciData = MmioRead32_test(PciMmioBaseaddr2 );    
	 Print(L"test_08hPciMmioBaseaddr:%x\n",PciMmioBaseaddr2);
	 Print(L"test_08h:%x\n",MmioPciData);

	 PciMmioBaseaddr3=PciMmioBaseaddr | (0x0C<<2)  ;
	 MmioPciData = MmioRead32_test(PciMmioBaseaddr3 );   
	 Print(L"test_0ChPciMmioBaseaddr:%x\n",PciMmioBaseaddr3);
	 Print(L"test_0Ch:%x\n",MmioPciData);

	 PciMmioBaseaddr4=PciMmioBaseaddr | (0x10<<2);
	 MmioPciData = MmioRead32_test(PciMmioBaseaddr4);
	 Print(L"test_10hPciMmioBaseaddr:%x\n",PciMmioBaseaddr4);
	 Print(L"test_10h:%x\n",MmioPciData);
*/

for(registerNum=0x00;registerNum<=0x3F;registerNum++){   //3ff
	reg = registerNum;
	PciMmioBaseaddr =(UINT32)(PCIEX_BASE_ADDRESS | bus << 20 | dev << 15 | func << 12 | reg << 2);
    // Print(L"addr:%x\n",PciMmioBaseaddr);
    MmioPciData = MmioRead32_test(PciMmioBaseaddr);
	//Print(L"Data:%08x",MmioPciData);

	if (registerNum%0x04 == 0)
		{
		Print(L"\n");
		}
	if (registerNum%0x01 == 0)
		{		
		Print(L" - ");
		}
		Print(L"%08x",MmioPciData);
}
Print(L"\n");
return EFI_SUCCESS;
}





EFI_STATUS
EFIAPI
PciDirectory (){
Print(L"bus  dev  func\n");
Print(L"--   --   --  \n");
for (bus = 0; bus <= 60; bus++)       //Maximum 255, but not necessary
{
	for (dev = 0; dev <= 32; dev++)
	{
		for (func = 0; func <= 7; func++)
		{
	    PciMmioBaseaddr =(UINT32)( PCIEX_BASE_ADDRESS|bus << 20|dev << 15|func << 12);
	    MmioPciData = MmioRead32_test(PciMmioBaseaddr);
		   if(MmioPciData != 0xffffffff)
			{
			Print(L"%02x   %02x   %02x ==>",bus,dev,func);
			Print(L"Vendor:%x Device:%04x\n",MmioPciData&0xFFFF,(MmioPciData&0xffff0000)>>16);
			MmioPciData = MmioRead32_test(PciMmioBaseaddr|(0x02<<2));
			Data_ClassCode = (MmioPciData&0xff000000)>>24;
			Data_SubClassCode = (MmioPciData&0xff0000)>>16;
			switch(Data_ClassCode){ 
			case 0x00:
				Print(L"Pre 2.0 device");
				Print(L" - ");	
				/*
				if(Data_SubClassCode==0x00){
				Print(L"All devices other than VGA");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"VGA-compatible devices");
				}
				else{
				Print(L"Error");
				}
				*/
				break;
			case 0x01:
				Print(L"Mass Storage Controller");
				Print(L" - ");
				/*
				if(Data_SubClassCode==0x00){
				Print(L"SCSI");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"IDE controller");
				}
				else if(Data_SubClassCode==0x02){
				Print(L"Floppy disk controller");
				}
				else if(Data_SubClassCode==0x03){
				Print(L"IPI controller");
				}
				else if(Data_SubClassCode==0x04){
				Print(L"RAID controller");
				}
				else if(Data_SubClassCode==0x05){
				Print(L"ATA controller with ADMA interface");
				}
				else if(Data_SubClassCode==0x06){
				Print(L"Serial ATA controller");
				}
				else if(Data_SubClassCode==0x07){
				Print(L"Serial Attached SCSI (SAS) controller");
				}			
				else if(Data_SubClassCode==0x08){
				Print(L"Non-volatile memory subsystem");
				}					
				else if(Data_SubClassCode==0x09){
				Print(L"Universal Flash Storage (UFS) controller");
				}
				else if(Data_SubClassCode==0x80){
				Print(L"Other mass storage controller");
				}
				else{
				Print(L"Error");
				}	
				*/
				break;
			case 0x02:
				Print(L"Network Controller");
				Print(L" - ");

				if(Data_SubClassCode==0x00){
				Print(L"Ethernet controller");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"Token ring controller");
				}
				else if(Data_SubClassCode==0x02){
				Print(L"FDDI controller");
				}
				else if(Data_SubClassCode==0x03){
				Print(L"ATM controller");
				}
				else if(Data_SubClassCode==0x04){
				Print(L"ISDN controller");
				}
				else if(Data_SubClassCode==0x05){
				Print(L"WorldFip controller");
				}
				else if(Data_SubClassCode==0x06){
				Print(L"PICMG 2.14 Multi Computing");
				}
				else if(Data_SubClassCode==0x07){
				Print(L"InfiniBand controller");
				}			
				else if(Data_SubClassCode==0x80){
				Print(L"Other network controller");
				}
				else{
				Print(L"Error");
				}
				
				break;
			case 0x03:
				Print(L"Display Controller");
				Print(L" - ");
			
			    if(Data_SubClassCode==0x00){
				Print(L"VGA/8514 controller");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"XGA controller");
				}
				else if(Data_SubClassCode==0x02){
				Print(L"3D controller");
				}
				else if(Data_SubClassCode==0x80){
				Print(L"Other display controller");
				}
				else{
				Print(L"Error");
				}
				
				break;
			case 0x04:
				Print(L"Multimedia Device");
				Print(L" - ");

				if(Data_SubClassCode==0x00){
				Print(L"Video device");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"Audio device");
				}
				else if(Data_SubClassCode==0x02){
				Print(L"Computer Telephony device");
				}
				else if(Data_SubClassCode==0x03){
				Print(L"Mixed mode device");
				}
				else if(Data_SubClassCode==0x80){
				Print(L"Other multimedia device");
				}
				else{
				Print(L"Error");
				}
				
				break;
			case 0x05:
				Print(L"Memory Controller");
				Print(L" - ");
				/*
				if(Data_SubClassCode==0x00){
				Print(L"RAM memory controller");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"Flash memory controller");
				}
				else if(Data_SubClassCode==0x80){
				Print(L"Other memory controller");
				}
				else{
				Print(L"Error");
				}
				*/
				break;
	        case 0x06:
				Print(L"Bridge Device");
				Print(L" - ");
			
				if(Data_SubClassCode==0x00){
				Print(L"Host/PCI bridge");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"PCI/ISA bridge");
				}
				else if(Data_SubClassCode==0x02){
				Print(L"PCI/EISA bridge");
				}
				else if(Data_SubClassCode==0x03){
				Print(L"PCI/Micro Channel bridge");
				}
				else if(Data_SubClassCode==0x04){
				Print(L"PCI/PCI bridge");
				}
				else if(Data_SubClassCode==0x05){
				Print(L"PCI/PCMCIA bridge");
				}
				else if(Data_SubClassCode==0x06){
				Print(L"NuBus bridge");
				}
				else if(Data_SubClassCode==0x07){
				Print(L"CardBus bridge");
				}			
				else if(Data_SubClassCode==0x08){
				Print(L"RACEway bridge");
				}					
				else if(Data_SubClassCode==0x09){
				Print(L"Semi-transparent PCI-to-PCI bridge");
				}
				else if(Data_SubClassCode==0x0A){
				Print(L"InfiniBand-to-PCI host bridge");
				}
				else if(Data_SubClassCode==0x0B){
				Print(L"Advanced Switching to PCI host bridge");
				}
				else if(Data_SubClassCode==0x80){
				Print(L"Other bridge type");
				}
				else{
				Print(L"Error");
				}					
			
				break;			
			case 0x07:
				Print(L"Simple Communications Controllers");
				Print(L" - ");
				/*
				if(Data_SubClassCode==0x00){
				Print(L"Serial controller");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"Parallel port");
				}
				else if(Data_SubClassCode==0x02){
				Print(L"Multiport serial controller");
				}
				else if(Data_SubClassCode==0x03){
				Print(L"Modem");
				}
				else if(Data_SubClassCode==0x04){
				Print(L"GPIB (IEEE 488.1/2) controller");
				}
				else if(Data_SubClassCode==0x05){
				Print(L"Smart Card");
				}
				else if(Data_SubClassCode==0x80){
				Print(L"Other communication device");
				}
				else{
				Print(L"Error");
				}	
				*/
				break;			
			case 0x08:
				Print(L"Base System Peripherals");
				Print(L" - ");

				if(Data_SubClassCode==0x00){
				Print(L"PIC");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"DMA controller");
				}
				else if(Data_SubClassCode==0x02){
				Print(L"System timer");
				}
				else if(Data_SubClassCode==0x03){
				Print(L"RTC controller");
				}
				else if(Data_SubClassCode==0x04){
				Print(L"Generic PCI Hot-Plug controller");
				}
				else if(Data_SubClassCode==0x05){
				Print(L"SD Host controller");
				}
				else if(Data_SubClassCode==0x06){
				Print(L"IOMMU");
				}
				else if(Data_SubClassCode==0x07){
				Print(L"Root Complex Event Collector");
				}			
				else if(Data_SubClassCode==0x80){
				Print(L"Other system peripheral");
				}
				else{
				Print(L"Error");
				}
				
				break;			
			case 0x09:
				Print(L"Input Devices");
				Print(L" - ");
				/*
				if(Data_SubClassCode==0x00){
				Print(L"Keyboard controller");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"Digitizer (pen)");
				}
				else if(Data_SubClassCode==0x02){
				Print(L"Mouse controller");
				}
				else if(Data_SubClassCode==0x03){
				Print(L"Scanner controller");
				}
				else if(Data_SubClassCode==0x04){
				Print(L"Gameport controller");
				}	
				else if(Data_SubClassCode==0x80){
				Print(L"Other input controller");
				}
				else{
				Print(L"Error");
				}
				*/
				break;			
			case 0x0a:
				Print(L"Docking Stations");
				Print(L" - ");
				/*
				if(Data_SubClassCode==0x00){
				Print(L"Generic docking station");
				}	
				else if(Data_SubClassCode==0x80){
				Print(L"Other type of docking station");
				}
				else{
				Print(L"Error");
				}
				*/
				break;			
			case 0x0b:
				Print(L"Processors");
				Print(L" - ");
			    /*
				if(Data_SubClassCode==0x00){
				Print(L"386");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"486");
				}
				else if(Data_SubClassCode==0x02){
				Print(L"Pentium");
				}
				else if(Data_SubClassCode==0x10){
				Print(L"Alpha");
				}
				else if(Data_SubClassCode==0x20){
				Print(L"PowerPC");
				}	
				else if(Data_SubClassCode==0x30){
				Print(L"MIPS");
				}
				else if(Data_SubClassCode==0x40){
				Print(L"Co-processor");
				}
				else if(Data_SubClassCode==0x80){
				Print(L"Other processor");
				}
				else{
				Print(L"Error");
				}
				*/
				break;			
			case 0x0c:
				Print(L"Serial Bus Controllers");
				Print(L" - ");

				if(Data_SubClassCode==0x00){
				Print(L"IEEE 1394");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"ACCESS.bus");
				}
				else if(Data_SubClassCode==0x02){
				Print(L"SSA");
				}
				else if(Data_SubClassCode==0x03){
				Print(L"USB");
				}
				else if(Data_SubClassCode==0x04){
				Print(L"Fibre Channel");
				}
				else if(Data_SubClassCode==0x05){
				Print(L"System Management Bus");
				}
				else if(Data_SubClassCode==0x06){
				Print(L"InfiniBand");
				}
				else if(Data_SubClassCode==0x07){
				Print(L"IPMI");
				}
				else if(Data_SubClassCode==0x08){
				Print(L"SERCOS Interface Standard (IEC 61491)");
				}	
				else if(Data_SubClassCode==0x09){
				Print(L"CANbus");
				}		
				else if(Data_SubClassCode==0x80){
				Print(L"Other bus type");
				}
				else{
				Print(L"Error");
				}
				
				break;			
			case 0x0d:
				Print(L"Wireless Controllers");
				Print(L" - ");
				/*
				if(Data_SubClassCode==0x00){
				Print(L"iRDA compatible controller");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"");
				}
				else if(Data_SubClassCode==0x10){
				Print(L"RF controller");
				}
				else if(Data_SubClassCode==0x11){
				Print(L"Bluetooth");
				}
				else if(Data_SubClassCode==0x12){
				Print(L"Broadband");
				}
				else if(Data_SubClassCode==0x20){
				Print(L"Ethernet (802.11a - 5 GHz)");
				}
				else if(Data_SubClassCode==0x21){
				Print(L"Ethernet (802.11b - 2.4 GHz)");
				}
				else if(Data_SubClassCode==0x80){
				Print(L"Other type of wireless controller");
				}
				else{
				Print(L"Error");
				}
				*/
				break;			
			case 0x0e:
				Print(L"Intelligent IO Controllers");
				Print(L" - ");
				/*
				if(Data_SubClassCode==0x00){
				Print(L"I2O Architecture");
				}
				else{
				Print(L"Error");
				}
				*/
				break;			
			case 0x0f:
				Print(L"Satellite Communications Controllers");
				Print(L" - ");
				/*
				if(Data_SubClassCode==0x01){
				Print(L"TV");
				}
				else if(Data_SubClassCode==0x02){
				Print(L"Audio");
				}
				else if(Data_SubClassCode==0x03){
				Print(L"Voice");
				}
				else if(Data_SubClassCode==0x04){
				Print(L"Data");
				}
				else if(Data_SubClassCode==0x80){
				Print(L"Other satellite communication controller");
				}
				else{
				Print(L"Error");
				}
				*/
				break;			
			case 0x10:
				Print(L"Encryption/Decryption Controllers");
				Print(L" - ");

				if(Data_SubClassCode==0x00){
				Print(L"Network & computing Encrypt/Decrypt");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"Entertainment Encrypt/Decrypt");
				}
				else if(Data_SubClassCode==0x80){
				Print(L"Other Encrypt/Decrypt");
				}
				else{
				Print(L"Error");
				}
				
				break;			
			case 0x11:
				Print(L"Data Acquisition & Signal Processing Controllers");
				Print(L" - ");
				/*
				if(Data_SubClassCode==0x00){
				Print(L"DPIO modules");
				}
				else if(Data_SubClassCode==0x01){
				Print(L"Performance Counters");
				}
				else if(Data_SubClassCode==0x10){
				Print(L"Communications synchronization plus time and frequency test/measurement");
				}
				else if(Data_SubClassCode==0x20){
				Print(L"Management card");
				}
				else if(Data_SubClassCode==0x80){
				Print(L"Other DAQ & SP controllers");
				}
				else{
				Print(L"Error");
				}
				*/
				break;			
			case 0x12:
				Print(L"Processing Accelerators");
				Print(L" - ");
				break;			
			case 0x13:
				Print(L"Non-Essential Instrumentation");
				Print(L" - ");
				break;			
			case 0xff:
				Print(L"Device does not fit in any defined classes");
				break;			
			default:
				Print(L"Error");
				break;				
			}             

			//Print(L"\nVendor:%x Device:%04x",MmioPciData&0xFFFF,(MmioPciData&0xffff0000)>>16);
			PciMmioBaseaddr =(UINT32)( PCIEX_BASE_ADDRESS|bus << 20|dev << 15|func << 12);
			MmioPciData = MmioRead32_test(PciMmioBaseaddr|(0x02<<2));
			Data_ProgInterface = (MmioPciData&0xff00)>>8;
			Print(L"  Prog Interface:%x\n",Data_ProgInterface);
			Print(L"=============================================================\n");
			}   
		}
	}
}



return EFI_SUCCESS;
}



INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
	
   switch (Argc) {
	  case 1:
	  PciDirectory ();
	  break;
	  case 3:
      PciDetail1 (Argv);
      break;
	  case 4:
      PciDetail2 (Argv);
      break;      
	  default:
      Print(L" Error:Too few arguments specified.\n");
      break;
  }
   return 0;
}




