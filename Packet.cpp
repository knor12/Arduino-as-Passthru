#include "Packet.h"

Packet::Packet() {


}


Packet::Packet(char * packetString) {
  //Serial.println("Packet::Packet(char * packetString)");

  setString(packetString);
}

void Packet::updatePacket(char packetString[]) {
  setString(packetString);
}

void Packet::updatePacket(ERRORS err) {
  int CommandLength = 0 ;

  //assembling the packet in bytes
  intPacketByte_Size = OFFSET_COMMAND;
  //pBytePacket = new byte[intPacketByte_Size];
  pBytePacket[OFFSET_KEY] = KEY ;
  setUint16(OFFSET_COMMAND + CommandLength , OFFSET_LENGTH);
  setUint16(err , OFFSET_ERROR);
  int i ;

  uint16_t sum = calculateChecksum();

  setUint16(sum , OFFSET_CHECKSUM);


  //creating a hex string of the packet
  //pCharPacket = new char[intPacketByte_Size * 2 +1];
  for (i = 0 ; i < intPacketByte_Size * 2 ; i += 2) {
    pCharPacket[i] = (char)Nible_TO_Hex[pBytePacket[i / 2] >> 4 ] ;
    pCharPacket[i + 1] = (char)Nible_TO_Hex[pBytePacket[i / 2] & 0xF] ;
  }
  pCharPacket[intPacketByte_Size * 2 ] = '\0' ; //terminate the string with a null
}
Packet::Packet (ERRORS err) {

  //Serial.println("Packet::Packet (ERRORS err)");
  int CommandLength = 0 ;

  //assembling the packet in bytes
  intPacketByte_Size = OFFSET_COMMAND;
  //pBytePacket = new byte[intPacketByte_Size];
  pBytePacket[OFFSET_KEY] = KEY ;
  setUint16(OFFSET_COMMAND + CommandLength , OFFSET_LENGTH);
  setUint16(err , OFFSET_ERROR);
  int i ;

  uint16_t sum = calculateChecksum();
  //Serial.print("checksum ") ; Serial.print(i ); Serial.print(" =   "); Serial.println( sum, HEX);
  setUint16(sum , OFFSET_CHECKSUM);

  /*
  for (i =0 ; i < CommandLength+ OFFSET_COMMAND; i++){
  	Serial.print("byte ") ; Serial.print(i ); Serial.print(" =   "); Serial.println( (byte)pBytePacket[i], HEX);
  }*/


  //creating a hex string of the packet
  //pCharPacket = new char[intPacketByte_Size * 2 +1];
  for (i = 0 ; i < intPacketByte_Size * 2 ; i += 2) {
    pCharPacket[i] = (char)Nible_TO_Hex[pBytePacket[i / 2] >> 4 ] ;
    pCharPacket[i + 1] = (char)Nible_TO_Hex[pBytePacket[i / 2] & 0xF] ;
  }
  pCharPacket[intPacketByte_Size * 2 ] = '\0' ; //terminate the string with a null
}

int Packet::getStringLength(char * str) {
  //Serial.println("Packet::getStringLength(char * str)");
  int i = 0 ;
  char * temp  ;
  temp = str ;
  while ((char)(*temp) != '\0') {
    i++ ;
    temp++;
  }
  return i ;
}

void  Packet::updatePacket(ERRORS err, byte * Command , int CommandLength) {
  //assembling the packet in bytes
  //pByteCommand = Command;
  intPacketByte_Size = OFFSET_COMMAND + CommandLength;
  //pBytePacket = new byte[intPacketByte_Size];
  pBytePacket[OFFSET_KEY] = KEY ;
  setUint16(intPacketByte_Size , OFFSET_LENGTH);
  //(uint16_t) (* (pBytePacket + OFFSET_LENGTH)) = (OFFSET_COMMAND + CommandLength) ;
  //pBytePacket[OFFSET_LENGTH ] = ((OFFSET_COMMAND + CommandLength) & 0xFFFF)>>8;
  setUint16(err , OFFSET_ERROR);
  //pBytePacket[OFFSET_ERROR+1] = 0xFF & err ;
  // pBytePacket[OFFSET_ERROR] = (0xFF00 & err) >> 8;
  int i ;
  for (i = 0 ; i < CommandLength; i++) {
    pBytePacket[OFFSET_COMMAND + i] = Command[i];
  }

  uint16_t sum = calculateChecksum();
  //Serial.print("checksum ") ; Serial.print(i ); Serial.print(" =   "); Serial.println( sum, HEX);
  setUint16(sum , OFFSET_CHECKSUM);


  //for (i =0 ; i < CommandLength+ OFFSET_COMMAND; i++){
  //Serial.print("byte ") ; Serial.print(i ); Serial.print(" =   "); Serial.println( (byte)pBytePacket[i], HEX);
  //}


  //creating a hex string of the packet
  //pCharPacket = new char[intPacketByte_Size * 2 +1];
  for (i = 0 ; i < intPacketByte_Size * 2 ; i += 2) {
    pCharPacket[i] = (char)Nible_TO_Hex[pBytePacket[i / 2] >> 4 ] ;
    pCharPacket[i + 1] = (char)Nible_TO_Hex[pBytePacket[i / 2] & 0xF] ;
  }
  pCharPacket[intPacketByte_Size * 2 ] = '\0' ; //terminate the string with a null
}
Packet::Packet (ERRORS err, byte * Command , int CommandLength  ) {

  //assembling the packet in bytes
  //pByteCommand = Command;
  intPacketByte_Size = OFFSET_COMMAND + CommandLength;
  //pBytePacket = new byte[intPacketByte_Size];
  pBytePacket[OFFSET_KEY] = KEY ;
  setUint16(intPacketByte_Size , OFFSET_LENGTH);
  //(uint16_t) (* (pBytePacket + OFFSET_LENGTH)) = (OFFSET_COMMAND + CommandLength) ;
  //pBytePacket[OFFSET_LENGTH ] = ((OFFSET_COMMAND + CommandLength) & 0xFFFF)>>8;
  setUint16(err , OFFSET_ERROR);
  //pBytePacket[OFFSET_ERROR+1] = 0xFF & err ;
  // pBytePacket[OFFSET_ERROR] = (0xFF00 & err) >> 8;
  int i ;
  for (i = 0 ; i < CommandLength; i++) {
    pBytePacket[OFFSET_COMMAND + i] = Command[i];
  }

  uint16_t sum = calculateChecksum();
  //Serial.print("checksum ") ; Serial.print(i ); Serial.print(" =   "); Serial.println( sum, HEX);
  setUint16(sum , OFFSET_CHECKSUM);


  //for (i =0 ; i < CommandLength+ OFFSET_COMMAND; i++){
  //Serial.print("byte ") ; Serial.print(i ); Serial.print(" =   "); Serial.println( (byte)pBytePacket[i], HEX);
  //}


  //creating a hex string of the packet
  //pCharPacket = new char[intPacketByte_Size * 2 +1];
  for (i = 0 ; i < intPacketByte_Size * 2 ; i += 2) {
    pCharPacket[i] = (char)Nible_TO_Hex[pBytePacket[i / 2] >> 4 ] ;
    pCharPacket[i + 1] = (char)Nible_TO_Hex[pBytePacket[i / 2] & 0xF] ;
  }
  pCharPacket[intPacketByte_Size * 2 ] = '\0' ; //terminate the string with a null
}
void Packet::setUint16(uint16_t ui16_ , int ByteOffset ) {
  *(ByteOffset + pBytePacket) =  (byte)(ui16_ & 0xFF) ;
  *(ByteOffset + pBytePacket + 1 ) = (byte)( (ui16_ & 0xFF00) >> 8 ) ;

}
uint16_t Packet::calculateChecksum() {
  int i = 0 ;
  int sum = 0 ;
  for (i = 0 ; i < intPacketByte_Size ; i++) {
    sum += pBytePacket[i];
  }

  sum -= pBytePacket[OFFSET_CHECKSUM] ;
  sum -= pBytePacket[OFFSET_CHECKSUM + 1] ;
  return sum & 0xFFFF ;
}


ERRORS Packet::validCharacters() {
  int i = intPacketByte_Size * 2 ;
  for (i = 0 ; i < intPacketByte_Size * 2; i++ ) {

    if ((pCharPacket[i] >= '0' && pCharPacket[i] <= '9') || (pCharPacket[i] >= 'A' && pCharPacket[i] <= 'F') || (pCharPacket[i] >= 'a' && pCharPacket[i] <= 'f')) {

    } else {
      return ERROR_WRONG_CHARACTER_FOUND ;
    }

  }
  return ERROR_OK ;

}

ERRORS Packet::validKey() {
 
  if (getUInt8(OFFSET_KEY) != KEY) {
    return ERROR_WRONG_KEY ;
  }
  return ERROR_OK ;
}
ERRORS Packet::validLength() {
  //Serial.print("UI16_Length") ; Serial.println((int)getUInt16(OFFSET_LENGTH));
  if ((int)getUInt16(OFFSET_LENGTH) != intPacketByte_Size) {
    return ERROR_WRONG_PACKET_LENGTH ;
  } else {
    return ERROR_OK ;
  }

}


uint8_t  Packet::getUInt8(int ByteOffset) {
  return (uint8_t) * (ByteOffset + pBytePacket);
}
uint16_t Packet::getUInt16(int ByteOffset) {
  uint16_t   ui = 0   ;
  ui = *(pBytePacket + ByteOffset)  ;
  ui |= (*(pBytePacket + ByteOffset + 1)) << 8 ;

  // ui = (uint16_t)*(pBytePacket+ByteOffset);

  return ui ;
  //return (uint16_t)*(ByteOffset + pBytePacket);
}

void  Packet::setString(char * pcharPacket_) {
  int i = 0 ;

  //pCharPacket = pcharPacket_ ;

  while (pcharPacket_[i] != '\0') {
    pCharPacket[i] = pcharPacket_[i];
   
    i++;
  }
  pCharPacket[i++] = '\0' ;
  intPacketByte_Size = getStringLength(pcharPacket_) / 2 ;

  //Serial.print("length: ") ; Serial.println(intPacketByte_Size);
  //converting hex strings to bytes
  //pBytePacket =  new byte[intPacketByte_Size];
  i = 0 ;
  for (i = 0 ; i < intPacketByte_Size * 2; i += 2) {
    
    pBytePacket[i / 2] = (hexToByte((pCharPacket + i)) << 4) | hexToByte((pCharPacket + i + 1));
    //Serial.print("in hex to byte : byte: "); Serial.print(i/2) ;  Serial.print("=") ;  Serial.println((int)pBytePacket[i/2] );
     
    
  }

}

Packet::~Packet () {
  //Serial.println("deleting pBytePacket");

  //if (  pBytePacket!=null ){	delete (pBytePacket);}
  //Serial.println("deleting pCharPacket");
  //if (  pCharPacket!=null ){	delete (pCharPacket);}
  //Serial.println("deleting pByteCommand");
  //if (  pByteCommand!=null ){	delete (pByteCommand);}
  //Serial.println("returning from Packet::~Packet ()");

}

int Packet::getRealByteSize() {

  return  intPacketByte_Size;
}


char * Packet::getString() {
  //Serial.print("String address::["); Serial.print((int)pCharPacket , HEX); Serial.print("] data ::"); Serial.println(pCharPacket);
  return pCharPacket ;
}






byte Packet::hexToByte(char * pchar) {
  //Serial.print("hex to byte got: "); Serial.print(char(* pchar));
  if (*pchar >= '0' && *pchar <= '9') {
    //Serial.print("returned");Serial.println((*pchar) - '0');
    return (*pchar) - '0' ;
  }
  if (*pchar >= 'A' && *pchar <= 'F') {
    // Serial.print("returned");Serial.println((*pchar) - 'A' + 10);
    return (*pchar) - 'A' + 10 ;
  }
  if (*pchar >= 'a' && *pchar <= 'f') {
    //Serial.print("returned");Serial.println((*pchar) - 'a' + 10);
    return (*pchar) - 'a' + 10 ;
  }
}

byte * Packet::getBytes() {
  return pBytePacket ;
}

ERRORS Packet::checkIntegrity() {
  ERRORS err ;
  //Serial.println("entered Packet::checkIntegrity()");
  if ((err = validKey()) != ERROR_OK) {
    return err ;
  }
  if ((err = validCharacters()) != ERROR_OK) {
    return err ;
  }
  if ((err = validLength()) != ERROR_OK) {
    return err ;
  }
  if ((calculateChecksum()) != getUInt16(OFFSET_CHECKSUM)) {
    return ERROR_CHECKSUM_WRONG ;
  }
  return ERROR_OK ;
}
