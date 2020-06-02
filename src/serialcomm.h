#ifndef SERIALCOMM_H
#define SERIALCOMM_H
#include <ti/drivers/UART.h>
#include <string.h>
#include "ti_drivers_config.h"

#define PREAMBLE_START		0xBE
#define PREAMBLE_END		0xEE

#define FRAME_TYPE_COMMAND 	0x00
#define FRAME_TYPE_RESPONSE 	0x01
#define FRAME_TYPE_PACKET 	0x02

typedef enum CommandType {
	None = -1,
	GetFirmwareVersion = 0,
	GetFCSMode = 1,
	SetFCSMode = 2,
	GetChannel = 3,
	SetChannel = 4,
	SendPacket = 5,
	Reset = 6,
	Unknown = 42
} CommandType;

typedef enum {
	UNKNOWN = -1,
	ALL = 0,
	VALID_FCS_ONLY = 1
} FCSMode;

static UART_Handle uart;

void SerialComm_init(int baudrate);

void SerialComm_sendPacketToHost(uint8_t *frame, uint8_t size,int valid_fcs);
void SerialComm_sendResponseToHost(CommandType type, uint8_t *parameter, uint8_t parameter_length);

uint8_t SerialComm_read();

CommandType SerialComm_waitCommandFromHost();
FCSMode SerialComm_getFCSModeFromHost();
uint8_t SerialComm_getChannelFromHost();
bool SerialComm_getCalcFCSFromHost();
uint8_t SerialComm_getPacketSizeFromHost();
void SerialComm_getPacketFromHost(uint8_t *packet, uint8_t size);

#endif
