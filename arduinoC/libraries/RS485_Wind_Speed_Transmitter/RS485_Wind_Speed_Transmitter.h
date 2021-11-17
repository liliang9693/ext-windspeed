#ifndef __RS485_Wind_Speed_Transmitter_H__
#define __RS485_Wind_Speed_Transmitter_H__

// #include <stdio.h>
// #include <unistd.h>
// #include <errno.h>
// #include <string.h>
#include "Arduino.h"
#include <SoftwareSerial.h>


class RS485_Wind_Speed_Transmitter
{

public:

  RS485_Wind_Speed_Transmitter();
  ~RS485_Wind_Speed_Transmitter();


  void begin(HardwareSerial &myhardSerial);
  void begin(SoftwareSerial &mysoftSerial);
  // void begin(uart &myuartSerial);


  /**
  @brief  读取风速
  @param  Address 读取的设备地址。
  @return  风速单位m/s，读取超时返回—1.
  */

  float readWindSpeed(uint8_t Address);

  /**
  @brief  修改传感器设备地址
  @param  Address1 设备修改前的地址。使用0x00地址可以设置任何地址，设置后需要重新上电重新启动模块.
  @param  Address2 设备修改后的地址，范围0x00~0xFF,
  @return  返回true表示修改成功，返回false表示修改失败。
  */

  boolean ModifyAddress(uint8_t Address1, uint8_t Address2);


private:
  
  uint8_t serialMode;

  SoftwareSerial *_softSerial;
  HardwareSerial *_hardSerial;
  // uart *_uartSerial;
  /**
  @brief  为数据包末尾添加CRC_16校验
  @param  buf 需要添加校验值的数据包
  @param  len 需要添加校验的数据长度。
  @return  无
  */
  void addedCRC(uint8_t *buf, int len);

  size_t readN(uint8_t *buf, size_t len);

  /**
  @brief  计算CRC16_2检验值
  @param  buf 需要计算校验值的数据包
  @param  len 需要校验的数据长度。
  @return  返回一个16位的校验结果。
  */
  uint16_t CRC16_2(uint8_t *buf, int16_t len);
};


#endif