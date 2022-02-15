/****************************************************************************************************************************
  defines.h
  EthernetWebServer is a library for the Ethernet shields to run WebServer
  
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/EthernetWebServer
  Licensed under MIT license
 ***************************************************************************************************************************************/

#ifndef defines_h
#define defines_h

#define DEBUG_ETHERNET_WEBSERVER_PORT       Serial

// Debug Level from 0 to 4
#define _ETHERNET_WEBSERVER_LOGLEVEL_       2
#define BOARD_TYPE                          "TEENSY 4.1"
#define USE_QN_ETHERNET                     true
#ifndef BOARD_NAME
  #define BOARD_NAME                        BOARD_TYPE
#endif

#include "QNEthernet.h"       // https://github.com/ssilverman/QNEthernet
using namespace qindesign::network;
#define SHIELD_TYPE                         "using QNEthernet"  
#include <EthernetWebServer.h>

#define SENDCONTENT_P_BUFFER_SZ             4096

#endif    //defines_h
