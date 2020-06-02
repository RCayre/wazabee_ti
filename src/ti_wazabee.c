/* Wazabee header files */
#include "radio.h"
#include "serialcomm.h"

#define VERSION_MAJOR		0x01
#define VERSION_MINOR		0x00

#define NRF_DEVICE		0x0A
#define TI_DEVICE		0x0B

#define DEVICE_TYPE		TI_DEVICE

static FCSMode fcsMode = VALID_FCS_ONLY;

void *recvThread(void *arg0)
{
	uint8_t output_buffer[50];
	while (1) {
		// Reset buffer and length
		for (int i=0;i<50;i++) output_buffer[i] = 0;
		int length = 0;

		// Wait for an incoming frame and decode it
		receive(output_buffer, &length);
		
		// Check the FCS
		int valid_fcs = check_fcs_dot15d4(output_buffer, length);
		
		// Transmit the decoded frame to host
		if (length > 0 && (fcsMode == ALL || (fcsMode == VALID_FCS_ONLY && valid_fcs))) {
			SerialComm_sendPacketToHost(output_buffer,length, valid_fcs);
		}
	}
}


void *mainThread(void *arg0)
{
	SerialComm_init(115200);	
	initialize();
	while (1) {
		CommandType command = SerialComm_waitCommandFromHost();
		if (command == GetFirmwareVersion) {
			uint8_t version[3];
			version[0] = VERSION_MAJOR;
			version[1] = VERSION_MINOR;
			version[2] = DEVICE_TYPE;
			SerialComm_sendResponseToHost(GetFirmwareVersion,version,3);
		}
		else if (command == GetFCSMode) {
			uint8_t mode[1];
			mode[0] = (uint8_t)fcsMode;
			SerialComm_sendResponseToHost(GetFCSMode,mode,1);
		}
		else if (command == SetFCSMode) {
			FCSMode newFcsMode = SerialComm_getFCSModeFromHost();
			uint8_t status[1];
			if (newFcsMode != UNKNOWN) {
				fcsMode = newFcsMode;
				status[0] = 0;
			}
			else status[0] = 1;
			SerialComm_sendResponseToHost(SetFCSMode,status,1);
		}
		else if (command == GetChannel) {
			uint8_t channel[1];
			channel[0] = (uint8_t)getChannel();
			SerialComm_sendResponseToHost(GetChannel,channel,1);
		}
		else if (command == SetChannel) {
			uint8_t newChannel = SerialComm_getChannelFromHost();
			uint8_t status[1];
			bool success = setChannel(newChannel);
			status[0] = success ? 0x00 : 0x01;
			SerialComm_sendResponseToHost(SetChannel,status,1);
		}
		else if (command == SendPacket) {
			bool calcFcs = SerialComm_getCalcFCSFromHost();
			uint8_t size = SerialComm_getPacketSizeFromHost();

			if (calcFcs) {
				uint8_t packet[size+2];
				SerialComm_getPacketFromHost(packet, size);
				uint16_t fcs = calculate_fcs_dot15d4(packet,size+2);
				packet[size] = fcs & 0xFF;
				packet[size+1] = (fcs >> 8) & 0xFF;
				send(packet,size+2);
			}
			else {
				uint8_t packet[size];
				SerialComm_getPacketFromHost(packet, size);
				send(packet,size);
			}
			SerialComm_sendResponseToHost(SendPacket,NULL,0);
		}
		else if (command == Reset) {
			fcsMode = VALID_FCS_ONLY;
			reset();
			SerialComm_sendResponseToHost(Reset,NULL,0);
		}
	}
	
}

