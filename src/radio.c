#include "radio.h"

static void txCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e);
static void rxCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e);

bool initialize() {
	// Semaphore initialization
	Semaphore_Params semParams;
	Semaphore_Params_init(&semParams);
	Semaphore_construct(&semStruct, 0, &semParams);
	semHandle = Semaphore_handle(&semStruct);

	// LED initialization
	ledPinHandle = PIN_open(&ledPinState, pinTable);
	if (ledPinHandle == NULL)
	{
		while(1);
	}

	// RFQueue initialization
	if( RFQueue_defineQueue(&dataQueue,
			    rxDataEntryBuffer,
			    sizeof(rxDataEntryBuffer),
			    NUM_DATA_ENTRIES,
			    MAX_LENGTH + NUM_APPENDED_BYTES))
	{
		return false;
	}

	// RF Handle initialization
	rfBleHandle = RF_open(&rfBleObject, &RF_prop,
			(RF_RadioSetup*)&RF_cmdBle5RadioSetup, NULL);
	reset();
	return true;
}

void reset() {
	setChannel(12);
}

bool setChannel(int channel) {
	bool rvalue = true;
	if (channel >= 11 && channel <= 26) {
		currentChannel = channel;
		setReceiveMode();
	}
	else rvalue = false;
	return rvalue;
}
int getChannel() {
	return currentChannel;
}

bool send(uint8_t payload[],int size) {
	RF_cancelCmd(rfBleHandle,lastRxHandle,1);
	RF_Params rfBleParams;
	RF_Params_init(&rfBleParams);
	int total_size = 4  + size;
	
	uint8_t buffer[total_size];
	
	for (int i=0;i<4;i++) buffer[i] = 0;
	memcpy(buffer+4,payload,size);

	uint8_t output_buffer[MAX_PACKET_SIZE];
	for (int i=0;i<MAX_PACKET_SIZE;i++) output_buffer[i] = 0;
	
	
	for (int i=0;i<total_size;i++) {
		uint8_t msb = buffer[i] >> 4;
		uint8_t lsb = buffer[i] & 0x0F;

		output_buffer[i*8+0] = (SYMBOL_TO_CHIP_MAPPING[lsb][0]);
		output_buffer[i*8+1] = (SYMBOL_TO_CHIP_MAPPING[lsb][1]);
		output_buffer[i*8+2] = (SYMBOL_TO_CHIP_MAPPING[lsb][2]);
		output_buffer[i*8+3] = (SYMBOL_TO_CHIP_MAPPING[lsb][3]);

		output_buffer[i*8+4] = (SYMBOL_TO_CHIP_MAPPING[msb][0]);
		output_buffer[i*8+5] = (SYMBOL_TO_CHIP_MAPPING[msb][1]);
		output_buffer[i*8+6] = (SYMBOL_TO_CHIP_MAPPING[msb][2]);
		output_buffer[i*8+7] = (SYMBOL_TO_CHIP_MAPPING[msb][3]);


	}
	int length = ((total_size-1)*8+7)+1;
	RF_cmdBle5AdvAux.pParams->pAdvPkt = (uint8_t *)&ble5ExtAdvPacket;
	ble5ExtAdvPacket.extHdrInfo.length = EXTENDED_HEADER_LENGTH;
	ble5ExtAdvPacket.advDataLen = length;
	ble5ExtAdvPacket.pAdvData = output_buffer;
	RF_cmdBle5AdvAux.startTrigger.triggerType = TRIG_ABSTIME;
	RF_cmdBle5AdvAux.startTrigger.pastTrig = 1;
	RF_cmdBle5AdvAux.startTime = 0;

	RF_cmdBle5AdvAux.channel = (0x66 + 3 + 5 * (currentChannel - 11));
	RF_cmdBle5AdvAux.whitening.init = 0;
	RF_cmdBle5AdvAux.phyMode.mainMode = 1;

    	/* Get current time */
    	uint32_t curtime = RF_getCurrentTime();
        RF_cmdBle5AdvAux.startTime = curtime;
        RF_runCmd(rfBleHandle,(RF_Op*)&RF_cmdBle5AdvAux, RF_PriorityNormal, txCallback, 0); 
	setReceiveMode();
	return true;
}
void setReceiveMode() {
	RF_cancelCmd(rfBleHandle,lastRxHandle,1);
	RF_cmdBle5GenericRx.pOutput = &recvStats;
	RF_cmdBle5GenericRx.pParams->pRxQ = &dataQueue;

	RF_cmdBle5GenericRx.channel = (0x66 + 3 + 5 * (currentChannel - 11));
	RF_cmdBle5GenericRx.whitening.init = 0;
	RF_cmdBle5GenericRx.phyMode.mainMode = 1; 

	RF_cmdBle5GenericRx.pParams->accessAddress = 0x9b3af703;
	RF_cmdBle5GenericRx.pParams->crcInit0 = 0;
	RF_cmdBle5GenericRx.pParams->crcInit1 = 0;
	RF_cmdBle5GenericRx.pParams->crcInit2 = 0;
	RF_cmdBle5GenericRx.pParams->bRepeat = 0x01; // receive multiple packets
	RF_cmdBle5GenericRx.pParams->rxConfig.bAutoFlushIgnored = 0;
	RF_cmdBle5GenericRx.pParams->rxConfig.bAutoFlushCrcErr = 0;
	RF_cmdBle5GenericRx.pParams->rxConfig.bAutoFlushEmpty = 1;
	RF_cmdBle5GenericRx.pParams->rxConfig.bIncludeLenByte = 1;
	RF_cmdBle5GenericRx.pParams->rxConfig.bIncludeCrc = 1;
	RF_cmdBle5GenericRx.pParams->rxConfig.bAppendRssi = 0;
	RF_cmdBle5GenericRx.pParams->rxConfig.bAppendStatus = 0;
	RF_cmdBle5GenericRx.pParams->rxConfig.bAppendTimestamp = 0;

	RF_cmdBle5GenericRx.pParams->endTrigger.triggerType = TRIG_NEVER;
	RF_cmdBle5GenericRx.pParams->endTime = 0;

	
	lastRxHandle = RF_postCmd(rfBleHandle, (RF_Op*)&RF_cmdBle5GenericRx, RF_PriorityNormal, &rxCallback, RF_EventRxEntryDone);
}

void receive(uint8_t *payload, int *length) {
	// The semaphore wants to be catched
	Semaphore_pend(semHandle,BIOS_WAIT_FOREVER);
	// output_buffer is a 50-bytes buffer, allowing to store the decoded Zigbee frame.
	// This variable length indicates the length of the decoded Zigbee frame.
	*length = 0;
	// This call to 'decode()' converts the frame to its MSK representation to its equivalent Zigbee frame.
	decode(payload, length, packet);
}

void txCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    if (e & RF_EventLastCmdDone)
    {
	// Toggle RED Led
        PIN_setOutputValue(ledPinHandle, CONFIG_PIN_RLED,!PIN_getOutputValue(CONFIG_PIN_RLED));
    }
    else
    {
	// Turn Off RED Led
        PIN_setOutputValue(ledPinHandle, CONFIG_PIN_RLED, 0);
    }
}

void rxCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    if (e & RF_EventRxEntryDone)
    {
	// Toggle RED Led
        PIN_setOutputValue(ledPinHandle, CONFIG_PIN_RLED,!PIN_getOutputValue(CONFIG_PIN_RLED));

        // Get current unhandled data entry
        currentDataEntry = RFQueue_getDataEntry();
        packetDataPointer = (uint8_t*)(&currentDataEntry->data);

        // Copy the payload in the packet buffer
        memcpy(packet, packetDataPointer, 0xF7);

        RFQueue_nextEntry();
	// Release the semaphore
        Semaphore_post(semHandle);

    }
}
