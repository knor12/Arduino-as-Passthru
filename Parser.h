#ifndef PARSER_H
#define PARSER_H

//#include <WProgram.h>
#include <Arduino.h>
#include "enumerations.cpp"
#include "Packet.h"
//#include "C:\Program Files\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.h"
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
class Parser {
  private:

    Packet * CurrentPacketInput;
    Packet * CurrentPacketOutput;
    byte byteCommBack[300] ;
    ERRORS CurrentError ;
    uint8_t SPI_nCS_Slave_Pin  ; 
  public :

    Parser();
    ~Parser();


    void parse(Packet * CurrentPacketInput_ , Packet * CurrentPacketOutput_);


    //Digital operations functions
    void parseDigital();
    void parseDigitaWrite();
    void parseDigitaRead();
    void parseDigitaConfigure();

    //Analog operation functions
    void parseAnalog() ;
    void parseAnalogConfigure();
    void parseAnalogRead();
    
    //PWM Operation
    void parsePWM() ;


    //EPROM Operations
    void parseEPROM();
    void parseEPROMWrite();
    void parseEPROMRead();
    
    
    //Firmware Protocol version operations
    void parseFrmwareProtocolVer();
    
    
    //SPI Communication operations
    void parseSPI();
    void parseSPIConfigure();
    void parseSPIWriteRead();
    
    //I2C Communication operations
    void parseI2C();
    void parseI2CWrite();
    void parseI2CRead();
    

};


#endif
