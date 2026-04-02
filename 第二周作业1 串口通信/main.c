#include <STC89C5xRC.H>
#include "I2C.h"
#include "AT24C02.h"
#include "UART.h"
#include "Timer0.h"

#define SAVE_DATA_MAX     60      //最多能够存60个字节
#define SAVE_TIME         1000    //1000ms无新数据，自动保存

unsigned char Receive_Data;                      //最近一次收到的串口字节
bit ReceiveFlag;                                 //接收完成标志位 1 接收到字节 0 处理完成

unsigned char ReceiveBuffer[SAVE_DATA_MAX];      //字符串缓冲区
unsigned char ReceiveCount = 0;                  //已经收到多少个字符

unsigned int ReceiveTime = 0;                    //距离上次接收已经过去多少ms
bit SaveFlag = 0;                                //超时保存标志位

void SaveToEEPROM(void);                         //把数据保存到EEPROM中
void ShowFromEEPROM(void);                       //显示EEPROM中的数据

void main()
{
    UART_Init();      //串口初始化
    Timer0Init();     //定时器0初始化

    UART_SendString("\r\nSystem Start\r\n");
    ShowFromEEPROM();
    UART_SendString("Send data and wait to save\r\n");

    while(1)
    {
        if(ReceiveFlag==1)
        {
            ReceiveFlag=0;

            //如果接收到的是普通字符，先存进缓冲区
            if(ReceiveCount<SAVE_DATA_MAX)  //判断是否溢出
            {
                ReceiveBuffer[ReceiveCount]=Receive_Data;
                ReceiveCount++;
            }

            //每收到一个新字节，就把空闲时间和保存标志位清零
            ReceiveTime=0;
            SaveFlag=0;
        }

        //判断如果超时就自动保存
        if(SaveFlag==1)
        {
            SaveFlag=0;

            if(ReceiveCount>0)       //如果不是空数据
            {
                SaveToEEPROM();      //调用保存函数，把数据保存到EEPROM中
                ReceiveCount=0;

                UART_SendString("\r\nSAVE OK\r\n");
                ShowFromEEPROM();
                UART_SendString("Send data and wait to save\r\n");
            }
        }
    }
}

void SaveToEEPROM(void)  //把缓冲区内容写入EEPROM，地址0：长度，地址1~n：字符串内容
{
    unsigned char i;

    AT24C02_WriteByte(0x00,ReceiveCount);   // 先存长度

    for(i=0;i<ReceiveCount;i++)
    {
        AT24C02_WriteByte(i+1,ReceiveBuffer[i]);
    }

}

void ShowFromEEPROM(void)  //上电后从EEPROM读出并显示
{
    unsigned char i;
    unsigned char len;

    len = AT24C02_ReadByte(0x00);  //获得长度

    UART_SendString("EEPROM:");

    if(len==0xFF || len==0 || len>SAVE_DATA_MAX)
    {
        UART_SendString("NO DATA\r\n");
        return;
    }

    for(i=0;i<len;i++)      //依次读取字符
    {
        UART_SendByte(AT24C02_ReadByte(i+1));
    }

    UART_SendString("\r\n");
}

void Timer0_Routine() interrupt 1  //定时器0中断，1ms进入一次
{
    TL0=0x66;
    TH0=0xFC;

    if(ReceiveCount>0 && SaveFlag==0)
    {
        ReceiveTime++;

        if(ReceiveTime>=SAVE_TIME)
        {
            ReceiveTime=0;
            SaveFlag=1;
        }
    }
}

void UART_Routine() interrupt 4  //串口中断
{
    if(RI==1)
    {
        Receive_Data=SBUF;  //把SBUF中的数据存入Receive_Data
        RI=0;
        ReceiveFlag=1;      //1 接收到字节，主循环开始处理
    }
}