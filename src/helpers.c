#include <stdint.h>
#include "helpers.h"

int hamming(uint8_t *demod_buffer,uint8_t *pattern) {
	int count = 0;
	for (int i=0;i<4;i++) {
		for (int j=0;j<8;j++) {
			if (((pattern[i] >> (7-j)) & 0x01) != (((demod_buffer[i] & (i==0 && j==0 ? 0x7F : 0xFF)) >> (7-j)) & 0x01)) {
				count++;
			}
		}
	}
	return count;
}

void shift_buffer(uint8_t *demod_buffer,int size) {
	for (int i=0;i<size;i++) {
		if (i != 0) {
			demod_buffer[i-1]=((demod_buffer[i] & 0x80) >> 7) | demod_buffer[i-1];
		}
		demod_buffer[i] = demod_buffer[i] << 1;
	}
}

int match_pattern(uint8_t *demod_buffer, uint8_t *pattern) {
	return ((demod_buffer[0] & 0x7F) == pattern[0]) &&
	 	((demod_buffer[1] & 0xFF) == pattern[1]) &&
		((demod_buffer[2] & 0xFF) == pattern[2]) &&
		((demod_buffer[3] & 0xFF) == pattern[3]);
	
}

void decode(uint8_t *output_buffer, int *length, uint8_t * packet) {

	for (int i=0;i<50;i++) output_buffer[i] = 0;
	// index of the current Zigbee symbol
	int index = 0;
	// indicator of current 4 bits position (1 = 4 MSB, 0 = 4 LSB)
	int part = 1;
	// indicator of start frame delimiter
	int sfd = 0;
	// Hamming distance
	int hamming_dist = 0;
	// Thresold Hamming distance
	int hamming_thresold = 8;

	// Align the buffer with the SFD
	output_buffer[0] |= (0x0F & 0x07);
	hamming_dist = 32;
	while (hamming_dist > hamming_thresold) {
		hamming_dist = hamming(packet,SYMBOL_TO_CHIP_MAPPING[0]);
		if (hamming_dist > hamming_thresold) {
			shift_buffer(packet,255);
		}
	}

	hamming_dist = 0;
	while (hamming_dist <= hamming_thresold) {
		int symbol = -1;
		// Compute the hamming distance for every zigbee symbol
		for (int i=0;i<16;i++) {
			hamming_dist = hamming(packet,SYMBOL_TO_CHIP_MAPPING[i]);
			if (hamming_dist <= hamming_thresold) {
				symbol = i;
				break;
			}
		}

		// If a zigbee symbol has been found ...
		if (symbol != -1) {
			// If it matches the SFD next symbol, start the frame decoding
			if (sfd == 0 && symbol == 10) {
				sfd = 1;
			}
			
			// If we are in the frame decoding state ...
			if (sfd == 1) {
				// Fill the output buffer with the selected symbol
				output_buffer[index] |= (symbol & 0x0F) << 4*part;

				// Select the next 4 bits free space in the output buffer
				part = part == 1 ? 0 : 1;
				if (part == 0) index++;
			}
			// Shift the buffer (31 bits shift)
			for (int i=0;i<32;i++) shift_buffer(packet,255);
		}
	}

	// Export of the effective length of the Zigbee frame
	*length = index;
}


uint16_t update_fcs_dot15d4(uint8_t byte, uint16_t fcs) {
	uint16_t q = (fcs ^ byte) & 15;
	fcs = (fcs / 16) ^ (q * 4225);
	q = (fcs ^ (byte / 16)) & 15;
	fcs = (fcs / 16) ^ (q * 4225);	
	return fcs;	
}

uint16_t calculate_fcs_dot15d4(uint8_t *data, int size) {
	uint16_t fcs = 0;
	for (int i=2; i<size-2;i++) {
		fcs = update_fcs_dot15d4(data[i],fcs);
	}
	return fcs;
}

int check_fcs_dot15d4(uint8_t *data,int size) {
	uint16_t fcs = calculate_fcs_dot15d4(data,size);
	return ((fcs & 0xFF) == data[size-2]) && (((fcs >> 8) & 0xFF) == data[size-1]);
}

