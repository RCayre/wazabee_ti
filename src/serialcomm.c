#include "serialcomm.h"


void SerialComm_init(int baudrate) {
	/* UART initialization */
	UART_Params uartParams;
	/* Call driver init functions */
	UART_init();


	/* Create a UART with data processing off. */
	UART_Params_init(&uartParams);
	uartParams.writeDataMode = UART_DATA_BINARY;
	uartParams.readDataMode = UART_DATA_BINARY;
	uartParams.readReturnMode = UART_RETURN_FULL;
	uartParams.readEcho = UART_ECHO_OFF;
	uartParams.baudRate = baudrate;

	uart = UART_open(CONFIG_UART_0, &uartParams);

	if (uart == NULL) {
		/* UART_open() failed */
		while (1);
	}

}

void SerialComm_sendPacketToHost(uint8_t *frame, uint8_t size,int valid_fcs) {
	uint8_t packet[2+1+1+size+1];
	packet[0] = PREAMBLE_START;
	packet[1] = PREAMBLE_END;
	packet[2] = FRAME_TYPE_PACKET;
	packet[3] = size;
	memcpy(&packet[4],frame,size);
	packet[4+size] = valid_fcs ? 0x01 : 0x00;
	UART_write(uart,&packet, size+5);
}

void SerialComm_sendResponseToHost(CommandType type, uint8_t *parameter, uint8_t parameter_length) {
	uint8_t response[2+1+1+parameter_length];
	response[0] = PREAMBLE_START;
	response[1] = PREAMBLE_END;
	response[2] = FRAME_TYPE_RESPONSE;
	response[3] = (uint8_t)type;
	if (parameter_length > 0) {
		memcpy(&response[4],parameter,parameter_length);
	}
	UART_write(uart,&response, parameter_length+4);
}

uint8_t SerialComm_read() {
	uint8_t input[1];
        UART_read(uart, &input, 1);
	return input[0];
}

CommandType SerialComm_waitCommandFromHost() {
	CommandType command = None;
	while (command == None) {
		uint8_t next = SerialComm_read();
		if (next == PREAMBLE_START) {
			if (SerialComm_read() == PREAMBLE_END) {
				uint8_t type = SerialComm_read();
				if (type == 0) {
					uint8_t commandField = SerialComm_read();
					if (commandField == 0x00) command = GetFirmwareVersion;
					else if (commandField == 0x01) command = GetFCSMode;
					else if (commandField == 0x02) command = SetFCSMode;
					else if (commandField == 0x03) command = GetChannel;
					else if (commandField == 0x04) command = SetChannel;
					else if (commandField == 0x05) command = SendPacket;
					else if (commandField == 0x06) command = Reset;
					else command = Unknown;
				}
			}
			
		}
		
	}
	return command;
}

FCSMode SerialComm_getFCSModeFromHost() {
	uint8_t mode = SerialComm_read();
	FCSMode fcsMode;
	if (mode == 0x00) fcsMode = ALL;
	else if (mode == 0x01) fcsMode = VALID_FCS_ONLY;
	else fcsMode = UNKNOWN;
	return fcsMode;
}


uint8_t SerialComm_getChannelFromHost() {
	uint8_t channel = SerialComm_read();
	return channel;
}


bool SerialComm_getCalcFCSFromHost() {
	uint8_t calc_fcs = SerialComm_read();
	return (calc_fcs == 0x01);
}
uint8_t SerialComm_getPacketSizeFromHost() {
	uint8_t size = SerialComm_read();
	return size;
}

void SerialComm_getPacketFromHost(uint8_t *packet, uint8_t size) {
	for (int i=0;i<size;i++) {
		packet[i] = SerialComm_read();
	}
}
