#include <Uefi.h>
#include "time.h"
UINT8 TimeGet(UINT8 Num, UINT8 port)
{
	IoWrite8(0x70, port);//指定位置，把要读的位置写入0x70位置
	Num = IoRead8(0x71);//通过0x71读数据
	return Num;
}
void ShowTime()
{
	UINT8
		s = 0,//秒
		m = 0,//分钟
		h = 0,//小时
		d = 0,//天
		mm = 0,//月份
		yy = 0,//年份
		WeekDay = 0;//星期
	//设置时间
	Print(L"\n");
	Print(L"time setting...\n");
	TimeWrite(0x00, 11);//设置秒
	TimeWrite(0x02, 11);//设置分钟
	TimeWrite(0x04, 11);//设置小时
	TimeWrite(0x06, 1);//设置星期
	TimeWrite(0x07, 22);//设置天数
	TimeWrite(0x08, 3);//设置月份
	TimeWrite(0x09, 21);//设置年份
	//库函数获取时间
	s = TimeGet(s, 0x00);//获取秒
	m = TimeGet(m, 0x02);//获取分钟
	h = TimeGet(h, 0x04);//获取小时
	d = TimeGet(d, 0x07);//获取天数
	mm = TimeGet(mm, 0x08);//获取月份
	yy = TimeGet(yy, 0x09);//获取年份
	Print(L"Internal code implementation: 20");
	Print(L"%d/%d/%d%3d:%d:%d\n", yy, mm, d, h, m, s);
	//汇编函数获取时间
	s = TimeRead(0x00);//获取秒
	m = TimeRead(0x02);//获取分钟
	h = TimeRead(0x04);//获取小时
	d = TimeRead(0x07);//获取天数
	mm = TimeRead(0x08);//获取月份
	yy = TimeRead(0x09);//获取年份
	WeekDay = TimeRead(0x06);//获取星期
	Print(L"Assembly code implementation: 20");
	Print(L"%d/%d/%d%3d:%d:%d\n", yy, mm, d, h, m, s);
	//周的转化
	switch (WeekDay)
	{
	case 1:
		Print(L"Monday\n");
		break;
	case 2:
		Print(L"Tuesday\n");
		break;
	case 3:
		Print(L"Wednesday\n");
		break;
	case 4:
		Print(L"Tursday\n");
		break;
	case 5:
		Print(L"Friday\n");
		break;
	case 6:
		Print(L"Sturday\n");
		break;
	case 7:
		Print(L"Sunday\n");
	default:
		Print(L"error\n");
	}
}
EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{

	ShowTime();//输出时间
	return EFI_SUCCESS;
}