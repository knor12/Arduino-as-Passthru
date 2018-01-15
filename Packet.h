#ifndef PACKET_H
#define PACKET_H

//#include <WProgram.h>
#include <Arduino.h>
#include "enumerations.cpp"
#define MAX_PACKET_CHARACTERS 300

static const  char  Nible_TO_Hex[16] = {'0' , '1', '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , 'A' , 'B' , 'C' , 'D' , 'E' , 'F'};
class Packet
{
  private:
    char  pCharPacket[MAX_PACKET_CHARACTERS]  ;
    byte  pBytePacket[MAX_PACKET_CHARACTERS / 2];
    byte  pByteCommand[MAX_PACKET_CHARACTERS / 2] ;
    int intPacketByte_Size ;
    byte hexToByte(char * pchar); //done
    uint16_t byteToHex(byte byte_);
    uint16_t calculateChecksum();
    ERRORS validCharacters();
    ERRORS validLength();
    ERRORS validKey();
    int getStringLength(char  str[]);

  public:

    Packet ();
    Packet(char packetString[]);
    Packet (ERRORS err);
    Packet (ERRORS err, byte * Command , int CommandLength  );

    void updatePacket(ERRORS err);
    void updatePacket(char packetString[]);
    void  updatePacket(ERRORS err, byte * Command , int CommandLength);

    ~Packet ();
    void setString(char  charPacket_[]); //done
    uint8_t  getUInt8(int ByteOffset);
    uint16_t getUInt16(int ByteOffset);
    void setUint16(uint16_t ui16 , int ByteOffset );
    int getRealByteSize(); //done
    char * getString() ; //done
    byte * getBytes(); //done

    ERRORS checkIntegrity();

};
#endif
