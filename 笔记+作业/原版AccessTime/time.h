
#include <Library/UefiLib.h>
#include <library/IoLib.h>
/*
��������ȡʱ��
������UINT8 Num:�������ݣ�UINT8 port:���ݶ�ȡλ��
����ֵ��UINT8 Num:
*/
UINT8 TimeGet(UINT8 Num, UINT8 port);
//չʾʱ��
void ShowTime();
/*
������ʱ���д
������UNINT8 port:д�����ݵ���ڣ�UNINT8 num:д�������
����ֵ��void
*/
void TimeWrite(UINT8 port, UINT8 num);
/*
��������ȡ����
������UINT8 port:��ȡ����λ��
����ֵ������UINT8�͵Ķ�ȡ����
*/
UINT8 TimeRead(UINT8 port);