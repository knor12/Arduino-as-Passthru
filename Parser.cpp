#include "Parser.h"
//#include <C:\Program Files\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.h>



Parser::Parser() {
}
Parser::~Parser() {
}

void  Parser::parse(Packet * CurrentPacketInput_ , Packet * CurrentPacketOutput_ ) {

  CurrentPacketInput = CurrentPacketInput_ ;
  CurrentPacketOutput = CurrentPacketOutput_;
  ERRORS err =  ERROR_OK;

  //check the itegrity of the input packet
  if ((err = CurrentPacketInput->checkIntegrity()) != ERROR_OK) {
    CurrentPacketOutput->updatePacket(err);
    return ;
  }

  // if packet is ok, start parsing the packet
  switch (CurrentPacketInput->getUInt8(OFFSET_COMMAND)) {

  case COMMAND_DIGITAL :
    parseDigital() ;
    return ;
  case COMMAND_ANALOG :
    parseAnalog();
    return ;
  case COMMAND_PWM :
    parsePWM();
    return ;
  case COMMAND_EPROM:
    parseEPROM();
    return ;
  case COMMAND_FIRMWARE_PROTOCOL:
    parseFrmwareProtocolVer();
    return ;
  case COMMAND_SPI:
    parseSPI();
    return ;
  case COMMAND_I2C:
    parseI2C();
    return ;  
  default :
    CurrentPacketOutput->updatePacket(ERROR_UNKNOWN_COMMAND);



  }

}



void Parser::parseI2C(){
   DebugPrintLn("Now in Parser::parseI2C()")
   switch (CurrentPacketInput->getUInt8(OFFSET_COMMAND + 1)) {
    case COMMAND_WRITE:
      parseI2CWrite();
      return ;
    case COMMAND_READ:
      parseI2CRead();
      return ;
    default :
      CurrentPacketOutput->updatePacket(ERROR_UNKNOWN_COMMAND);
    }
  
}

void Parser::parseI2CWrite(){
  DebugPrintLn("Now in Parser::parseI2CWrite()")
   
  uint8_t I2C_Address ; 
  int length_ ; 
  byte data[100];
  int index ;
  
  I2C_Address=CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2);
  DebugPrint("I2C Address="); DebugPrintLn(I2C_Address);  
  length_=CurrentPacketInput->getUInt8(OFFSET_COMMAND + 3);
  DebugPrint("I2C data length="); DebugPrintLn(length_); 
  
 
  
  for ( index =0 ; index<length_;index++){
    data[index]=CurrentPacketInput->getUInt8(OFFSET_COMMAND + 4+ index );
    DebugPrint("Index="); DebugPrint(index);  DebugPrint("    ,I2C_Data_Sent=");DebugPrintLn( data[index]);
    
  }
  
  //Wire.begin();
  Wire.beginTransmission(I2C_Address);
  //Wire.write(data, length_);
  for(index =0;index <length_;index++){
    DebugPrint("Writing: "); DebugPrint(data[index]);  DebugPrintLn("   over I2C bus");
    Wire.write((uint8_t)data[index]);
  }
  Wire.endTransmission();
  
  byteCommBack[0] = COMMAND_I2C;
  byteCommBack[1] = COMMAND_WRITE;
  byteCommBack[2] = I2C_Address;
  CurrentPacketOutput->updatePacket(ERROR_OK,  byteCommBack , 3);
  
}


void Parser::parseI2CRead(){
  DebugPrintLn("Now in Parser::parseI2CRead()")
  uint8_t I2C_Address ; 
  uint8_t length_ ; 
 // byte data[100];
  int data_index ;
  

  
  I2C_Address=CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2);
  DebugPrint("I2C Address="); DebugPrintLn(I2C_Address);  
  length_=CurrentPacketInput->getUInt8(OFFSET_COMMAND + 3);
  DebugPrint("I2C data length="); DebugPrintLn(length_);
 
  
  if (length_==0){// if the length of bytes needed set to 0, this means send back as many as available  
    length_ =0 ; //initialize data counter 
    while(Wire.available()){
      byteCommBack[length_ + 4]=Wire.read();
      length_ ++;
     
    }
  
  }else {
    
    Wire.requestFrom(I2C_Address, length_);
    length_ =0 ; //initialize data counter 
    while(Wire.available()){
      byteCommBack[length_ + 4]=Wire.read();
      DebugPrint("Index="); DebugPrint(length_);  DebugPrint("    ,I2C_Data_Received=") ; DebugPrintLn( byteCommBack[length_ + 4]);
      length_ ++;
     
    }
    
  }
  byteCommBack[0] = COMMAND_I2C;
  byteCommBack[1] = COMMAND_READ;
  byteCommBack[2] = I2C_Address;
  byteCommBack[3] = length_;
  CurrentPacketOutput->updatePacket(ERROR_OK,  byteCommBack , 4+length_);
}

void Parser::parseSPI() {
  DebugPrintLn("Now in Parser::parseSPI()")
    switch (CurrentPacketInput->getUInt8(OFFSET_COMMAND + 1)) {
    case COMMAND_CONFIGURE:
      parseSPIConfigure();
      return ;
    case COMMAND_WRITE_READ:
      parseSPIWriteRead();
      return ;
    default :
      CurrentPacketOutput->updatePacket(ERROR_UNKNOWN_COMMAND);
    }
}


void Parser::parseSPIWriteRead() {
  uint16_t uint16_SPI_Data_length = CurrentPacketInput->getUInt16(OFFSET_COMMAND + 2);
  int i ;
  DebugPrintLn("Now in Parser::parseSPIWriteRead()");

  DebugPrint("SPI Write/Read length in bytes= ");
  DebugPrintLn(uint16_SPI_Data_length);

  byteCommBack[0] = COMMAND_SPI ;
  byteCommBack[1] = COMMAND_WRITE_READ ;
  byteCommBack[2] = uint16_SPI_Data_length & 0xFF ; //lower 8 bits of the value read
  byteCommBack[3] = (uint16_SPI_Data_length >> 8) & 0xFF ; //higher 8 bits

  //pinMode(SPI_nCS_Slave_Pin, OUTPUT);
  digitalWrite(SPI_nCS_Slave_Pin, LOW);
  for (i = 0 ; i < uint16_SPI_Data_length ; i++) {
    byteCommBack[4 + i] = SPI.transfer(CurrentPacketInput->getUInt8(i + 4 + OFFSET_COMMAND));
    DebugPrint("SPI Wrote= ");
    DebugPrintLn(CurrentPacketInput->getUInt8(i + 4 + OFFSET_COMMAND));
    DebugPrint("SPI Read= ");
    DebugPrintLn(byteCommBack[4 + i] );
  }
  digitalWrite(SPI_nCS_Slave_Pin, HIGH);
  CurrentPacketOutput->updatePacket(ERROR_OK,  byteCommBack , uint16_SPI_Data_length + 4 );
}

void Parser::parseSPIConfigure() {

  DebugPrintLn("Now in Parser::parseSPIConfigure()");
  uint8_t uint8_bit_order = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2);
  uint8_t uint8_clock_divider = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 3);
  uint8_t uint8_data_mode = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 4);
  SPI_nCS_Slave_Pin = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 5);

  //set
  // if debug enabled print out SPI configs
  SPI.begin() ;

  DebugPrint("SPI_BIT_ORDER: ");
  if (uint8_bit_order == LEAST_SIGNIFICANT_BIT_FIRST) {
    SPI.setBitOrder(LSBFIRST ) ;
    DebugPrintLn(" LEAST_SIGNIFICANT_BIT_FIRST");
  } 
  else if (uint8_bit_order == MOST_SIGNIFICANT_BIT_FIRST) {
    SPI.setBitOrder(MSBFIRST ) ;
    DebugPrintLn(" MOST_SIGNIFICANT_BIT_FIRST");
  } 
  else {
    CurrentPacketOutput->updatePacket(ERROR_UNKNOWN_COMMAND);
    return ;
  }

  DebugPrint("SPI_CLOCK_DIVIDER=  ")
    DebugPrintLn(uint8_clock_divider)
      switch (uint8_clock_divider) {
      case SPI_CLOCK_DIV02:
        SPI.setClockDivider(SPI_CLOCK_DIV2);
        break;
      case SPI_CLOCK_DIV04:
        SPI.setClockDivider(SPI_CLOCK_DIV4);
        break ;
      case SPI_CLOCK_DIV08:
        SPI.setClockDivider(SPI_CLOCK_DIV8);
        break ;
      case SPI_CLOCK_DIV016:
        SPI.setClockDivider(SPI_CLOCK_DIV16);
        break ;
      case SPI_CLOCK_DIV032:
        SPI.setClockDivider(SPI_CLOCK_DIV32);
        break ;
      case SPI_CLOCK_DIV064:
        SPI.setClockDivider(SPI_CLOCK_DIV64);
        break ;
      case SPI_CLOCK_DIV0128:
        SPI.setClockDivider(SPI_CLOCK_DIV128);
        break ;

      default :
        CurrentPacketOutput->updatePacket(ERROR_UNKNOWN_COMMAND);
        return ;
      }



  DebugPrint("SPI_MODE=  ")
    DebugPrintLn(uint8_data_mode)
      switch (uint8_data_mode) {
      case SPI_MODE_0:
        SPI.setDataMode(SPI_MODE0) ;
        break ;
      case SPI_MODE_1:
        SPI.setDataMode(SPI_MODE1) ;
        break ;
      case SPI_MODE_2:
        SPI.setDataMode(SPI_MODE2) ;
        break ;
      case SPI_MODE_3:
        SPI.setDataMode(SPI_MODE3) ;
        break ;
      default :
        CurrentPacketOutput->updatePacket(ERROR_UNKNOWN_COMMAND);
        return ;
      }

  //set the nCS High
  pinMode(SPI_nCS_Slave_Pin, OUTPUT);
  digitalWrite(SPI_nCS_Slave_Pin, HIGH);

  DebugPrint("SPI_nCS_SLAVE_PIN=  ")
    DebugPrintLn(SPI_nCS_Slave_Pin)

      //SET digital pin 53 on arduino Mega 2560 ,  as output so the devoce doesn't act as slave
      pinMode(53, OUTPUT);

  CurrentPacketOutput->updatePacket(ERROR_OK);



}



void Parser::parseFrmwareProtocolVer() {

  byteCommBack[0] = COMMAND_FIRMWARE_PROTOCOL;
  byteCommBack[1] = FIRMWARE_VERSION_MAJOR;
  byteCommBack[2] = FIRMWARE_VERSION_MINOR;
  byteCommBack[3] = PROTOCOL_VERSION_MAJOR ;
  byteCommBack[4] = PROTOCOL_VERSION_MINOR ;

  DebugPrint("FIRMWARE_VERSION_MAJOR = ");
  DebugPrintLn(FIRMWARE_VERSION_MAJOR );

  DebugPrint("FIRMWARE_VERSION_MINOR = ");
  DebugPrintLn(FIRMWARE_VERSION_MINOR);

  DebugPrint("PROTOCOL_VERSION_MAJOR = ");
  DebugPrintLn(PROTOCOL_VERSION_MAJOR);

  DebugPrint("PROTOCOL_VERSION_MINOR = ");
  DebugPrintLn(PROTOCOL_VERSION_MINOR);

  CurrentPacketOutput->updatePacket(ERROR_OK,  byteCommBack , 5 );

}
void Parser::parseEPROM() {
  switch (CurrentPacketInput->getUInt8(OFFSET_COMMAND + 1)) {
  case COMMAND_WRITE:
    parseEPROMWrite();
    return ;
  case COMMAND_READ:
    parseEPROMRead();
    return ;
  default :
    CurrentPacketOutput->updatePacket(ERROR_UNKNOWN_COMMAND);
  }
}

void Parser::parseEPROMWrite() {
  int num_bytes = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 3);
  int start_address = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2);
  int i = 0 ;
  DebugPrint(" Parser::parseEPROMWrite(), Start address =");
  DebugPrint(start_address);
  DebugPrint(" Data length = ");
  DebugPrintLn(num_bytes);

  for (i = 0 ; i < num_bytes ; i++) {
    DebugPrint("Writing value = ");
    DebugPrint(CurrentPacketInput->getUInt8(OFFSET_COMMAND + 4 + i));
    DebugPrint(" , to address = ");
    DebugPrintLn(start_address + i);
    //EEPROM.write(1 ,2 );
    EEPROM.write(start_address + i , CurrentPacketInput->getUInt8(OFFSET_COMMAND + 4 + i) );
  }
  CurrentPacketOutput->updatePacket(ERROR_OK);
}


void Parser::parseEPROMRead() {
  int num_bytes = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 3);
  int start_address = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2);
  int i = 0 ;
  DebugPrint(" Parser::parseEPROMRead(), Start address =");
  DebugPrint(start_address);
  DebugPrint(" Data length = ");
  DebugPrintLn(num_bytes);
  byteCommBack[0] = COMMAND_EPROM;
  byteCommBack[1] = COMMAND_READ;
  byteCommBack[2] = start_address;
  byteCommBack[3] = num_bytes ;
  for (i = 0 ; i < num_bytes ; i++) {
    byteCommBack[i + 4] = EEPROM.read(start_address + i  );
    DebugPrint("read value = ");
    DebugPrint( byteCommBack[i + 4]);
    DebugPrint(" , from address = ");
    DebugPrintLn(start_address + i);
  }
  CurrentPacketOutput->updatePacket(ERROR_OK,  byteCommBack , 4 + num_bytes);
  // CurrentPacketOutput->updatePacket(ERROR_OK);
}




void  Parser::parsePWM() {
  int numPins = (CurrentPacketInput->getUInt8(OFFSET_LENGTH) - OFFSET_COMMAND - 1)/2 ;
  for (int i = 0; i < numPins*2; i+=2){
      int pin = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 1 + i) ;
      int value = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2 + i);
    if (pin >= 2 && pin <= 13){
      DebugPrint("Parsing PWM for pin ") DebugPrint(pin) DebugPrint(", value: ") DebugPrintLn(value)
      analogWrite(pin, value);
    }
    else {
      DebugPrint("Error writing to pin number ") DebugPrint(pin) DebugPrintLn("Enter a pin number between 2 and 13.")
    }
    
  }  
  
  CurrentPacketOutput->updatePacket(ERROR_OK);
}

void  Parser::parseDigital( ) {
  switch (CurrentPacketInput->getUInt8(OFFSET_COMMAND + 1)) {
  case COMMAND_CONFIGURE:
    parseDigitaConfigure();
    return ;
  case COMMAND_WRITE:
    parseDigitaWrite();
    return ;
  case COMMAND_READ:
    parseDigitaRead();
    return ;
  default :
    CurrentPacketOutput->updatePacket(ERROR_UNKNOWN_COMMAND);
  }

}


void  Parser::parseDigitaConfigure() {
  DebugPrintLn("In Parser::parseDigitaConfigure()")
  int numPins = ( CurrentPacketInput->getUInt8(OFFSET_LENGTH) - OFFSET_COMMAND -2)/2 ; 
  int pin_ ;
  int inOrOut ;
  DebugPrint("Num of Pins to Configure= ")
  DebugPrintLn(numPins);
  for (int i = 0; i < numPins*2 ; i+=2){
    pin_ =CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2 + i) ;
    inOrOut = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 3 + i);
    pinMode(pin_, inOrOut);  
     DebugPrint("Pin number =  ")
      DebugPrint(pin_)
      DebugPrint(" set as = ")
      if (inOrOut==INPUT){
         DebugPrintLn("INPUT")
      }else if (inOrOut==OUTPUT){
         DebugPrintLn("OUTPUT")
      }else {
         DebugPrintLn("unspecified INPUT or OUTPUT ")
      }
  }
  
  CurrentPacketOutput->updatePacket(ERROR_OK);


}

void  Parser::parseDigitaWrite() {
  DebugPrintLn("In Parser::parseDigitaWrite()")
  int numPins = ( CurrentPacketInput->getUInt8(OFFSET_LENGTH) - OFFSET_COMMAND -2)/2 ;
  DebugPrint("Number of pins to write to:  ") 
  DebugPrintLn(numPins)
  for (int i = 0; i < numPins*2; i +=2){
    int pin = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2 + i);
    int value = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 3 + i);
    digitalWrite(pin, value);
    DebugPrint("Pin = ")
    DebugPrint(pin)
    DebugPrint("Value = ")
    DebugPrintLn(value)
  }

  
 // digitalWrite(CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2) , CurrentPacketInput->getUInt8(OFFSET_COMMAND + 3)) ;
  CurrentPacketOutput->updatePacket(ERROR_OK) ;
}

void  Parser::parseDigitaRead() {
  DebugPrintLn("Now in DigitalRead")
  int numPins = (CurrentPacketInput->getUInt8(OFFSET_LENGTH) - OFFSET_COMMAND - 2);
  DebugPrint("Number of pins to read from:  ")
  DebugPrintLn(numPins)
  for (int i = 0; i < numPins; i++){
    int pin = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2 + i);
    byteCommBack[i] = digitalRead(pin);
    DebugPrint("Pin:  ") DebugPrint(pin) DebugPrint(byteCommBack[i] + " ")
    
    if (digitalRead(pin)==HIGH){
         DebugPrintLn("HIGH")
      }else if (digitalRead(pin)==LOW){
         DebugPrintLn("LOW")
      }else {
         DebugPrint("Error reading from pin") DebugPrintLn(pin)
      }

  }
    
    
  CurrentPacketOutput->updatePacket(ERROR_OK,  byteCommBack , numPins ); //changed 1 to numPins+2

}


void  Parser::parseAnalog() {
  //DebugPrintLn("Parser::parseAnalog()");
  switch (CurrentPacketInput->getUInt8(OFFSET_COMMAND + 1)) {
  case COMMAND_CONFIGURE:
    parseAnalogConfigure();
    return ;
  case COMMAND_READ:
    parseAnalogRead(); //changed parseAnalogRead to parseAnalogReadRequest
    //parseAnalogReadReply();
    return ;
  default :
    CurrentPacketOutput->updatePacket(ERROR_UNKNOWN_COMMAND);
  }
}


void Parser::parseAnalogRead() { 
  DebugPrintLn("Parsing Analog Read Request");
  
  int numPins = (CurrentPacketInput->getUInt8(OFFSET_LENGTH) - OFFSET_COMMAND - 2);
  DebugPrint("Number of pins to read from:  ")
  DebugPrintLn(numPins)
  
  for (int i = 0; i < numPins ; i++){
    DebugPrint(" Pin ") DebugPrintLn(CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2 + i))
    uint16_t val = analogRead(CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2 + i));
    byteCommBack[0] = COMMAND_ANALOG ;
    byteCommBack[1+(3*i)] = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2 + i);
    byteCommBack[2+(3*i)] = val & 0xFF ; //lower 8 bits of the value read
    byteCommBack[3+(3*i)] = (val >> 8) & 0xFF ; //higher 8 bits
    DebugPrint(" Lower 8 bit value: ") DebugPrintLn(byteCommBack[2+(3*i)])
    DebugPrint(" Higher 8 bit value: ") DebugPrintLn(byteCommBack[3+(3*i)])
  } 
  CurrentPacketOutput->updatePacket(ERROR_OK,  byteCommBack , numPins*3+1 );
}


void  Parser::parseAnalogConfigure() {
  byte Voltage_Ref = CurrentPacketInput->getUInt8(OFFSET_COMMAND + 2);

  //configure the voltage reference of the ADC
  if (Voltage_Ref == ANALOG_REF_DEFAULT) {
    analogReference(DEFAULT);
    DebugPrintLn(" analogReference(DEFAULT)");
  }

  else if (Voltage_Ref == ANALOG_REF_EXTERNAL) {
    analogReference(EXTERNAL);
    DebugPrintLn("analogReference(EXTERNAL);");
  }
  else {
    DebugPrintLn("UNKNOWN VOLTAGE REFERENCE");
    CurrentPacketOutput->updatePacket(ERROR_UNKNOWN_COMMAND);
    return ;
  }

  CurrentPacketOutput->updatePacket(ERROR_OK);

}






