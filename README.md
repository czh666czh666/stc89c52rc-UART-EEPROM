# stc89c52rc-UART-EEPROM
Keil5单片机作业：基于STC89C52RC的串口通信与EEPROM掉电存储

## 开发环境
Keil5  
C语言  
单片机：STC89C52RC

## 实现功能
1 串口接收电脑发送的数据  
2 通过IIC总线将数据写入EEPROM  
3 断电后数据不丢失  
4 单片机重新上电后自动读取EEPROM中的数据  
5 通过串口将读取到的数据发送回电脑显示  

## 工程文件
本工程文件位于：  
第一周作业2 串口通信与EEPROM存储

主要文件包括：  
main.c：主程序  
UART.c / UART.h：串口通信相关程序  
AT24C02.c / AT24C02.h：EEPROM读写相关程序  
I2C.c / I2C.h：IIC通信相关程序  
Delay.c / Delay.h：延时函数  
Project.uvproj：Keil工程文件  

## 运行方法

使用STC-ISP打开Project.hex文件  
下载到STC89C52RC开发板  
打开电脑串口助手并连接单片机串口  
发送数据后，单片机通过串口接收数据并写入EEPROM  
断电重启后，单片机自动读取EEPROM中的数据并通过串口发送到电脑显示
