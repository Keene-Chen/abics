#ifndef __SERIAL_PORT_H__
#define __SERIAL_PORT_H__

//串口相关的头文件
#include <errno.h> /*错误号定义*/
#include <fcntl.h> /*文件控制定义*/
#include <stdio.h> /*标准输入输出定义*/
#include <stdlib.h> /*标准函数库定义*/
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h> /*PPSIX 终端控制定义*/
#include <time.h>
#include <unistd.h> /*Unix 标准函数定义*/

//宏定义
#define FALSE -1
#define TRUE 0

/**
 * @brief  打开串口并返回串口设备文件描述
 * @param  fd   文件描述符
 * @param  port 串口号(ttyS0,ttyS1,ttyS2)
 * @return int 成功返回为1，错误返回为0
 */
int serial_port_open(int fd, char* port);

/**
 * @brief  关闭串口并返回串口设备文件描述
 * @param  fd 文件描述符
 * @return void
 */
void serial_port_close(int fd);

/**
 * @brief  设置串口数据位，停止位和效验位
 * @param  fd        串口文件描述符
 * @param  speed     串口速度
 * @param  flow_ctrl 数据流控制
 * @param  databits  数据位   取值为 7 或者8
 * @param  stopbits  停止位   取值为 1 或者2
 * @param  parity    效验类型 取值为N,E,O,S
 * @return int 成功返回为1，错误返回为0
 */
int serial_port_init(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity);

/**
 * @brief  串口接收数据
 * @param  fd 文件描述符
 * @param  recv_buf 接收串口中数据存入rcv_buf缓冲区中
 * @param  data_len 一帧数据的长度
 * @return int 成功返回为1，错误返回为0
 */
int serial_port_recv(int fd, char* recv_buf, int data_len);

/**
 * @brief  串口发送数据
 * @param  fd 文件描述符
 * @param  send_buf 存放串口发送数据
 * @param  data_len 一帧数据的个数
 * @return int 成功返回为1，错误返回为0
 */
int serial_port_send(int fd, char* send_buf, int data_len);

#endif // __SERIAL_PORT_H__