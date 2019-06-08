#ifndef __ESP8266_H
#define __ESP8266_H

#include "stm32f10x.h"

#define SOCKET_ERROR 1
#define INVALID_SOCKET 1

#define ESP8266RXBufferMaxLength 128

///////////////////////////////
// Command Response Timeouts //
///////////////////////////////
#define COMMAND_RESPONSE_TIMEOUT 10000
#define COMMAND_PING_TIMEOUT 3000
#define WIFI_CONNECT_TIMEOUT 30000
#define COMMAND_RESET_TIMEOUT 5000
#define CLIENT_CONNECT_TIMEOUT 5000

#define ESP8266_MAX_SOCK_NUM 5
#define ESP8266_SOCK_NOT_AVAIL 255

#define ESP8266_USART USART2	//如要更改串口，还需更改串口配置及中断

typedef enum
{
	FALSE = 0,
	TRUE = 1
} bool;

typedef enum {
	ESP8266_CMD_BAD = -5,
	ESP8266_RSP_MEMORY_ERR = -4,
	ESP8266_RSP_FAIL = -3,
	ESP8266_RSP_UNKNOWN = -2,
	ESP8266_RSP_TIMEOUT = -1,
	ESP8266_RSP_SUCCESS = 0
} esp8266_cmd_rsp;

typedef enum {
	ESP8266_MODE_STA = 1,
	ESP8266_MODE_AP = 2,
	ESP8266_MODE_STAAP = 3
} esp8266_wifi_mode;

typedef enum {
	ESP8266_TRANSMISSION_NORMAL = 0,
	ESP8266_TRANSMISSION_TRANSPARENT = 1
} esp8266_transmission_mode;

typedef enum {
	ESP8266_CMD_QUERY,
	ESP8266_CMD_SETUP,
	ESP8266_CMD_EXECUTE
} esp8266_command_type;

typedef enum  {
	ESP8266_ECN_OPEN,
	ESP8266_ECN_WPA_PSK,
	ESP8266_ECN_WPA2_PSK,
	ESP8266_ECN_WPA_WPA2_PSK
} esp8266_encryption;

typedef enum  {
	ESP8266_STATUS_GOTIP = 2,
	ESP8266_STATUS_CONNECTED = 3,
	ESP8266_STATUS_DISCONNECTED = 4,
	ESP8266_STATUS_NOWIFI = 5	
} esp8266_connect_status;

typedef enum  {
	AVAILABLE = 0,
	TAKEN = 1,
} esp8266_socket_state;

typedef enum  {
	ESP8266_TCP,
	ESP8266_UDP,
	ESP8266_TYPE_UNDEFINED
} esp8266_connection_type;

typedef enum  {
	ESP8266_CLIENT,
	ESP8266_SERVER
} esp8266_tetype;

////////////////////
// Initialization //
////////////////////
bool esp8266Begin(void);

///////////////////////
// Basic AT Commands //
///////////////////////
bool esp8266Test(void);

////////////////////
// WiFi Functions //
////////////////////
int16_t esp8266GetMode(void);
bool esp8266SetMode(esp8266_wifi_mode mode);
bool esp8266ConnectAP(const char* ssid, const char* pwd);
bool esp8266DisconnectAP(void);

/////////////////////
// TCP/IP Commands //
/////////////////////
bool esp8266TCPConnect(uint8_t* destination, uint8_t* port);
bool esp8266TCPSend(uint8_t* buf);
bool esp8266TCPClose(void);
bool esp8266SetMux(uint8_t mux);

//////////////////////////////////////////////////
// Private, Low-Level, Ugly, Hardware Functions //
//////////////////////////////////////////////////
void esp8266SendCommand(const char* cmd, esp8266_command_type type, const char* params);
bool esp8266ReadForResponse(const char* rsp, unsigned int timeout);
bool esp8266ReadForResponses(const char* pass, const char* fail, unsigned int timeout);

//////////////////
// Buffer Stuff //
//////////////////
void esp8266ClearBuffer(void);
bool esp8266RxBufferAvailable(void);
bool esp8266TLINKBufferAvailable(void);
bool esp8266SearchBuffer(const char* test);

#endif
