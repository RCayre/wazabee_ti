#ifndef RADIO_H
#define RADIO_H

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/drivers/rf/RF.h>
#include "declarations.h"
#include "helpers.h"


static RF_CmdHandle lastRxHandle;

static int currentChannel = 12;
static uint8_t packet[MAX_LENGTH + NUM_APPENDED_BYTES - 1];

bool initialize();
void reset();

void setReceiveMode();
bool setChannel(int channel);
int getChannel();

bool send(uint8_t payload[],int size);
void receive(uint8_t *payload, int *length);
#endif
