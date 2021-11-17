#include <SoftwareSerial.h>
#include "RS485_Wind_Speed_Transmitter.h"

SoftwareSerial mySerial(2, 3); //�������ڣ�3�Ŷ˿�ΪTX��2�Ŷ˿�ΪRX��

RS485_Wind_Speed_Transmitter windSpeed;
uint8_t  Address = 0xFF;


void setup()
{
    Serial.begin(115200);
//    mySerial.begin(9600);
//    windSpeed.begin(mySerial);
    //windSpeed.ModifyAddress(0x00, Address);//�豸��ַ�޸�,�޸ĵ�ַ����ע�͵���������ϵ硣
    
}

void loop()
{
  Serial.println(windSpeed.readWindSpeed(Address)); //��ȡ����
  delay(1000);
}
