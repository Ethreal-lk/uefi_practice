
#include <Library/UefiLib.h>
#include <library/IoLib.h>
/*
函数：获取时间
参数：UINT8 Num:保存数据，UINT8 port:数据读取位置
返回值：UINT8 Num:
*/
UINT8 TimeGet(UINT8 Num, UINT8 port);
//展示时间
void ShowTime();
/*
函数：时间的写
参数：UNINT8 port:写入数据的入口，UNINT8 num:写入的数据
返回值：void
*/
void TimeWrite(UINT8 port, UINT8 num);
/*
函数：读取数据
参数：UINT8 port:读取数据位置
返回值：返回UINT8型的读取数据
*/
UINT8 TimeRead(UINT8 port);