#include <Arduino.h>
#include <Modbus.h>
#include <ModbusSerial.h>

#include "hardware.h"

// just test code to validate hardware for rs485 bus
// code snippets from library https://github.com/vermut/arduino-ModbusSerial
// examples used: "TempSensor"
// 
// ATTENTION: library was modified - every time the lib sets the tx pin, 
//            the VdMot_Controller Pin RS485_RXENA_PIN is set to the same state (HIGH/LOW)
//            this avoids echos which are not handled correctly by modbus library

//#define RS485_DBG				Serial3		// serial port for debugging
#define RS485_DBG				Serial6		// serial port for debugging

//Modbus Registers Offsets (0-9999)
const int SENSOR_IREG = 0; 

HardwareSerial Serial2(USART2);

// ModbusSerial object
ModbusSerial mb;

long ts;


void rs485_setup() {
    // enable receive pin
    pinMode(RS485_RXENA_PIN, OUTPUT);
    digitalWrite(RS485_RXENA_PIN, LOW);
    
    // Config Modbus Serial (port, speed, byte format) 
    mb.config(&Serial2, 9600, SERIAL_8N1, RS485_TXENA_PIN);

    // Set the Slave ID (1-247)
    mb.setSlaveId(10);  

    // Add SENSOR_IREG register - Use addIreg() for analog Inputs
    mb.addIreg(SENSOR_IREG);
    
    ts = millis();
}

void rs485_loop() {

    static int cntx = 0;

    //Call once inside loop() - all magic here
    mb.task();

    //Read each two seconds
    if (millis() > ts + 1000) {   
        ts = millis();

        if(cntx<22) cntx++;
        else cntx = 0;

        //Setting raw value (0-1024)
        mb.Ireg(SENSOR_IREG, cntx);

        //digitalWrite(RS485_TXENA_PIN, HIGH);
        //Serial2.write("A");
    } 
}
