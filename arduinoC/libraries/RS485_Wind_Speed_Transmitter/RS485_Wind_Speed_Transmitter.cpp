#include "RS485_Wind_Speed_Transmitter.h"



RS485_Wind_Speed_Transmitter::RS485_Wind_Speed_Transmitter()
{
    _hardSerial = NULL;
    _softSerial = NULL;

}

RS485_Wind_Speed_Transmitter::~RS485_Wind_Speed_Transmitter()
{
    
}

void RS485_Wind_Speed_Transmitter::begin(HardwareSerial &myhardSerial)
{
    serialMode = 0;
    _hardSerial = &myhardSerial;
}

void RS485_Wind_Speed_Transmitter::begin(SoftwareSerial &mysoftSerial)
{
    serialMode = 1;
    _softSerial = &mysoftSerial;
}

// void RS485_Wind_Speed_Transmitter::begin(uart &myuartSerial)
// {
//     serialMode = 2;
//     _uartSerial = &myuartSerial;
// }
// int x = 1;
float RS485_Wind_Speed_Transmitter::readWindSpeed(uint8_t Address)
{
    // if(x) _uartSerial->write(x);x=0;
    uint8_t Data[7] = {0};   //储存传感器返回的原始数据包
    uint8_t COM[8] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};   //读取风速的指令
    boolean ret = false;   //风速获取成功标志位
    float WindSpeed = 0;
    long curr = millis();
    long curr1 = curr;
    uint8_t ch = 0;
    COM[0] = Address;    //参考通讯协议将指令包添加完整。
    addedCRC(COM , 6);   //为读取风速指令包添加CRC_16校验
    // for(uint8_t i=0; i<8; i++)
    //     Serial.println(COM[i]);

    if(serialMode == 1){
        _softSerial->write(COM, 8);  //发送读取风速的指令
    // }else if(serialMode == 2){
        
    //     _uartSerial->write(COM, 8);  //发送读取风速的指令
    }else if(serialMode == 0){
        _hardSerial->write(COM, 8);  //发送读取风速的指令
    }
  
    while (!ret) {
        if (millis() - curr > 1000) {
            WindSpeed = -1;    //如果整个超过1000毫秒还未读取到风速将视为超时，并返回—1.
            break;
        }

        if (millis() - curr1 > 100) {

            if(serialMode == 1){
                _softSerial->write(COM, 8);  //如果上一条读取风速的指令发出超过100毫秒还未收到返回指令将从新发送读取风速的指令
            // }else if(serialMode == 2){
            //     _uartSerial->write(COM, 8);  //如果上一条读取风速的指令发出超过100毫秒还未收到返回指令将从新发送读取风速的指令
            }else if(serialMode == 0){
                _hardSerial->write(COM, 8);  //如果上一条读取风速的指令发出超过100毫秒还未收到返回指令将从新发送读取风速的指令
            }          
            curr1 = millis();
        }

        if (readN(&ch, 1) == 1) {
            if (ch == Address) {          //读取并判断包头。
                Data[0] = ch;
                if (readN(&ch, 1) == 1) {
                    if (ch == 0x03) {         //读取并判断包头。
                        Data[1] = ch;
                        if (readN(&ch, 1) == 1) {
                            if (ch == 0x02) {       //读取并判断包头。
                                Data[2] = ch;
                                if (readN(&Data[3], 4) == 4) {
                                    if (CRC16_2(Data, 5) == (Data[5] * 256 + Data[6])) {  //校验数据包
                                        ret = true;
                                        WindSpeed = (Data[3] * 256 + Data[4]) / 10.00;  //计算风速
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
  }
  return WindSpeed;

}

boolean RS485_Wind_Speed_Transmitter::ModifyAddress(uint8_t Address1, uint8_t Address2)
{
    uint8_t ModifyAddressCOM[11] = {0x00, 0x10, 0x10, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00};
    boolean ret = false;
    long curr = millis();
    long curr1 = curr;
    uint8_t ch = 0;
    ModifyAddressCOM[0] = Address1;
    ModifyAddressCOM[8] = Address2;
    addedCRC(ModifyAddressCOM , 9);
    if(serialMode == 1){
        _softSerial->write(ModifyAddressCOM, 11);
    // }else if(serialMode == 2){
    //     // _uartSerial->write(ModifyAddressCOM, 11);     //microbit,串口bug
    //     _uartSerial->write(ModifyAddressCOM, 11);
    }else if(serialMode == 0){
        _hardSerial->write(ModifyAddressCOM, 11);
    }

    while (!ret) {
    if (millis() - curr > 1000) {
      break;
    }

    if (millis() - curr1 > 100) {
      if(serialMode == 1){
            _softSerial->write(ModifyAddressCOM, 11);
        // }else if(serialMode == 2){
        //     _uartSerial->write(ModifyAddressCOM, 11);
        }else if(serialMode == 0){
            _hardSerial->write(ModifyAddressCOM, 11);
        }
      curr1 = millis();
    }

    if (readN(&ch, 1) == 1) {
      if (ch == Address1) {
        if (readN(&ch, 1) == 1) {
          if (ch == 0x10 ) {
            if (readN(&ch, 1) == 1) {
              if (ch == 0x10) {
                if (readN(&ch, 1) == 1) {
                  if (ch == 0x00) {
                    if (readN(&ch, 1) == 1) {
                      if (ch == 0x00) {
                        if (readN(&ch, 1) == 1) {
                          if (ch == 0x01) {
                            //                            while (1) {
                            // Serial.println("请给传感器重新上电。");
                            //                              delay(1000);
                            //                            }
                            ret = true ;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return ret;
}

void RS485_Wind_Speed_Transmitter::addedCRC(uint8_t *buf, int len)
{
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++)
    {
        crc ^= (uint16_t)buf[pos];
        for (int i = 8; i != 0; i--)
        {
            if ((crc & 0x0001) != 0)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    buf[len] = crc % 0x100;
    buf[len + 1] = crc / 0x100;
}

size_t RS485_Wind_Speed_Transmitter::readN(uint8_t *buf, size_t len)
{
    size_t offset = 0, left = len;
    int16_t Tineout = 1500;
    uint8_t  *buffer = buf;
    long curr = millis();
    bool val;
    
    while (left) {
        if(serialMode == 1){
            val = _softSerial->available();
        // }else if(serialMode == 2){
        //     val = _uartSerial->available();
        }else if(serialMode == 0){
            val = _hardSerial->available();
        }
        
        if (val) {
            if(serialMode == 1){
                buffer[offset] = _softSerial->read();
            // }else if(serialMode == 2){
            //     buffer[offset] = _uartSerial->read();
            }else if(serialMode == 0){
                buffer[offset] = _hardSerial->read();
            }
          
            offset++;
            left--;
        }
        if (millis() - curr > Tineout) {
        break;
        }
    }
    return offset;
}

uint16_t RS485_Wind_Speed_Transmitter::CRC16_2(uint8_t *buf, int16_t len)
{
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++)
    {
        crc ^= (uint16_t)buf[pos];
        for (int i = 8; i != 0; i--)
        {
        if ((crc & 0x0001) != 0)
        {
            crc >>= 1;
            crc ^= 0xA001;
        }
        else
        {
            crc >>= 1;
        }
        }
    }

    crc = ((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8);
    return crc;
}


