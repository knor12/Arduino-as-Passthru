
typedef unsigned char byte ;
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include "Packet.h"
#include "Parser.h"

Parser  parser ;
Packet   packet_in ;
Packet   packet_out ;
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(19200);
  Serial.flush();
  Wire.begin();
}

void loop() {

  //Serial.println("Start loop");
  char * strpack = readLine() ;
  //Serial.print("[");  Serial.print(strpack); Serial.println("]");
  packet_in.setString(strpack) ;

  // Serial.print("packet in:  ") ;   Serial.print("["); Serial.print(packet_in.getString());   Serial.println("]");
  parser.parse(&packet_in , &packet_out);
  Serial.println(packet_out.getString());




  //'Serial.println("End loop");
}


char * readLine() {

  int i = 0 ;
  static   char    pchar[MAX_PACKET_CHARACTERS] ;
  while (Serial.available() <= 0) {}
  
  pchar[i] = Serial.read();


  while (pchar[i] != 13 )  {// 13 is ascii value of carriage return && pchar[i]!= '\n'
    
    if (pchar[i] != '\n')
      i++;
    while (Serial.available() <= 0) {  }
    pchar[i] = Serial.read();
  
    
      }

      pchar[i++] = '\0' ;

  Serial.flush();
  return pchar ;
}



