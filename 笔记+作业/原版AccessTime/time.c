#include <Uefi.h>
#include "time.h"
UINT8 TimeGet(UINT8 Num, UINT8 port)
{
	IoWrite8(0x70, port);//ָ��λ�ã���Ҫ����λ��д��0x70λ��
	Num = IoRead8(0x71);//ͨ��0x71������
	return Num;
}
void ShowTime()
{
	UINT8
		s = 0,//��
		m = 0,//����
		h = 0,//Сʱ
		d = 0,//��
		mm = 0,//�·�
		yy = 0,//���
		WeekDay = 0;//����
	//����ʱ��
	Print(L"\n");
	Print(L"time setting...\n");
	TimeWrite(0x00, 11);//������
	TimeWrite(0x02, 11);//���÷���
	TimeWrite(0x04, 11);//����Сʱ
	TimeWrite(0x06, 1);//��������
	TimeWrite(0x07, 22);//��������
	TimeWrite(0x08, 3);//�����·�
	TimeWrite(0x09, 21);//�������
	//�⺯����ȡʱ��
	s = TimeGet(s, 0x00);//��ȡ��
	m = TimeGet(m, 0x02);//��ȡ����
	h = TimeGet(h, 0x04);//��ȡСʱ
	d = TimeGet(d, 0x07);//��ȡ����
	mm = TimeGet(mm, 0x08);//��ȡ�·�
	yy = TimeGet(yy, 0x09);//��ȡ���
	Print(L"Internal code implementation: 20");
	Print(L"%d/%d/%d%3d:%d:%d\n", yy, mm, d, h, m, s);
	//��ຯ����ȡʱ��
	s = TimeRead(0x00);//��ȡ��
	m = TimeRead(0x02);//��ȡ����
	h = TimeRead(0x04);//��ȡСʱ
	d = TimeRead(0x07);//��ȡ����
	mm = TimeRead(0x08);//��ȡ�·�
	yy = TimeRead(0x09);//��ȡ���
	WeekDay = TimeRead(0x06);//��ȡ����
	Print(L"Assembly code implementation: 20");
	Print(L"%d/%d/%d%3d:%d:%d\n", yy, mm, d, h, m, s);
	//�ܵ�ת��
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

	ShowTime();//���ʱ��
	return EFI_SUCCESS;
}