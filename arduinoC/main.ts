
//% color="#77278D" iconWidth=50 iconHeight=40
namespace dfr0845_sen0483 {
    
    //% board="arduino, microbit, esp32"
    //% block="初始化设备地址[ADDRESS]" blockType="command"
    //% ADDRESS.shadow="range"  ADDRESS.params.min=0 ADDRESS.params.max=255 ADDRESS.defl=2
    export function sen0483_modifyAddress(parameter: any, block: any) {
        
        let add = parameter.ADDRESS.code
        
        Generator.addInclude("RS485_Wind_Speed_Transmitter", "#include <RS485_Wind_Speed_Transmitter.h>");
        Generator.addObject(`sen0483add`, `uint8_t`, ` sen0483Add = ${add};`);
        
    }

    //% board="arduino, esp32"
    //% block="读取风速（m/s）接口 [HARDSER] RX:[HARDRX] TX:[HARDTX]" blockType="reporter"
    //% HARDSER.shadow="dropdown" HARDSER.options="HARDSER" 
    //% HARDRX.shadow="dropdown" HARDRX.options="HARDRX"
    //% HARDTX.shadow="dropdown" HARDTX.options="HARDTX"
    export function sen0483_getWindSpeed1(parameter: any, block: any) {
        
        let ser =  parameter.HARDSER.code;
        let rx = parameter.HARDRX.code;
        let tx = parameter.HARDTX.code;

        Generator.addObject(`windSpeed`, `RS485_Wind_Speed_Transmitter`, ` windSpeed;`);
        if(Generator.board === 'arduino'){
            Generator.addSetup("sen0483mySerialbegin",`${ser}.begin(9600);`);
        }else if(Generator.board === 'microbit'){
            Generator.addSetup("sen0483mySerialbegin",`Serial.begin(9600);\n\tint receive = g_PinID[${rx}];\n\tint send = g_PinID[${tx}];\n\tMicroBitPin *rxpin = getMicroBitPin(receive);\n\tMicroBitPin *txpin = getMicroBitPin(send);\n\tuBit.serial.redirect(txpin->name, rxpin->name);\n\tuBit.serial.baud((int)9600);`);
        }else if(Generator.board === 'esp32'){
            Generator.addSetup("sen0483mySerialbegin",`${ser}.begin(9600, ${rx}, ${tx});`);
        }
        Generator.addSetup("windSpeedbegin",`windSpeed.begin(${ser});`);
        if(Generator.board === 'microbit'){
            Generator.addSetup("sen0483modify11",`windSpeed.ModifyAddress(0, sen0483Add);\n\tdelay(100);`);
        }
        Generator.addSetup("sen0483modify",`windSpeed.ModifyAddress(0, sen0483Add);`);
        Generator.addCode(`windSpeed.readWindSpeed(sen0483Add)`);
    }

    //% board="arduino, microbit, esp32"
    //% block="读取风速（m/s）接口 [SOTFSER] RX:[SOFTRX] TX:[SOFTTX]" blockType="reporter"
    //% SOTFSER.shadow="dropdown" SOTFSER.options="SOTFSER"
    //% SOFTRX.shadow="dropdown" SOFTRX.options="SOFTRX" 
    //% SOFTTX.shadow="dropdown" SOFTTX.options="SOFTTX" 
    export function sen0483_getWindSpeed2(parameter: any, block: any) {

        let ser =  parameter.SOTFSER.code;
        let rx = parameter.SOFTRX.code;
        let tx = parameter.SOFTTX.code;
        
        Generator.addInclude('SoftwareSerial', '#include <SoftwareSerial.h>');
        Generator.addObject(`windSpeed`, `RS485_Wind_Speed_Transmitter`, ` windSpeed;`);
        Generator.addObject(`sen0483mySerial`, `${ser}`, ` mySerial(${rx},${tx});`);
        Generator.addSetup("sen0483mySerialbegin",`mySerial.begin(9600);`);
        Generator.addSetup("windSpeedbegin",`windSpeed.begin(mySerial);`);
        Generator.addSetup("sen0483modify",`windSpeed.ModifyAddress(0, sen0483Add);`);
        Generator.addCode(`windSpeed.readWindSpeed(sen0483Add)`);
}

