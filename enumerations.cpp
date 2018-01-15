#ifndef NULL_0
#define NULL_0
#define null 0
#endif

#ifndef ENUMERATION_CPP
#define ENUMERATION_CPP
//#include <wiring_analog.h>

//#define DEBUG_BUILD 1 // comment out this lign to disable the debug comment reporting function

#ifdef DEBUG_BUILD
#  define DebugPrintLn(x) Serial.print("Debug: ") ; Serial.println(x);
#  define DebugPrint(x) Serial.print("Debug: ") ; Serial.print(x);
#else
#   define DebugPrintLn(x) do {} while (0);
#   define DebugPrint(x) do {} while (0);
#endif


#define FIRMWARE_VERSION_MINOR 0x01
#define FIRMWARE_VERSION_MAJOR 0x01

#define PROTOCOL_VERSION_MINOR 0X01
#define PROTOCOL_VERSION_MAJOR 0x01

enum ERRORS {ERROR_OK = 0x0 , ERROR_CHECKSUM_WRONG = 0X1 , ERROR_WRONG_CHARACTER_FOUND = 0X2 ,
             ERROR_WRONG_PACKET_LENGTH = 0X3 , ERROR_WRONG_KEY = 0x4 , ERROR_UNKNOWN_COMMAND = 0x5
            };

#define KEY 0X13
enum OFFSETS {OFFSET_KEY = 0 , OFFSET_CHECKSUM = 1 , OFFSET_LENGTH = 3 , OFFSET_ERROR = 5 , OFFSET_COMMAND = 7 };

enum COMMAND {COMMAND_DIGITAL = 0x1 , COMMAND_ANALOG = 0x2 , COMMAND_SPI = 0x3 , COMMAND_EPROM = 0X4 , COMMAND_WRITE = 0X5 ,
              COMMAND_READ = 0x6 , COMMAND_WRITE_READ = 0X7 , COMMAND_CONFIGURE = 0X8 , COMMAND_PWM = 0x9 ,  COMMAND_FIRMWARE_PROTOCOL = 0xA ,
             COMMAND_I2C=0xB};


enum ANALOG_REF {  ANALOG_REF_DEFAULT = 0X1 , ANALOG_REF_EXTERNAL = 0X3};
//enum ANALOG_RESOLUTION {TEN_BITS = 0XA , TWELVE_BITS};


enum SPI_CLOCK_DIVIDER {SPI_CLOCK_DIV02 = 0x2 , SPI_CLOCK_DIV04 = 0x4 , SPI_CLOCK_DIV08 = 0x8 ,
                        SPI_CLOCK_DIV016 = 0x10 , SPI_CLOCK_DIV032 = 0x20 , SPI_CLOCK_DIV064 = 0x40 , SPI_CLOCK_DIV0128 = 0x80 ,
                       };
                       
enum SPI_BIT_ORDER {LEAST_SIGNIFICANT_BIT_FIRST = 0x0 , MOST_SIGNIFICANT_BIT_FIRST = 0x1  };

enum SPI_DATA_MODE {SPI_MODE_0 = 0x0 , SPI_MODE_1 = 0x1 , SPI_MODE_2 = 0x2 , SPI_MODE_3 = 0x3 , };

/*

INPUT = 0x0
OUTPUT = 0x1
HIGH= 0x1
LOW 0x0

TRUE = 0x1 ;

FALSE 0x0
*/



#endif
