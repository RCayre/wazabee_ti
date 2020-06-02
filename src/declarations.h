#include "smartrf_settings.h"
#include "RFQueue.h"

/* Driver configuration */
#include "ti_drivers_config.h"

/**************** Packet RX Configuration ****************/

#define DATA_ENTRY_HEADER_SIZE 8    /* Constant header size of a Generic Data Entry */
#define MAX_LENGTH             257  /* Max 8-bit length + two byte BLE header */
#define NUM_DATA_ENTRIES       2    /* NOTE: Only two data entries supported at the moment */
#define NUM_APPENDED_BYTES     1    /* Prepended length byte */


/**************** Packet TX Configuration ****************/
#define EXTENDED_HEADER_LENGTH    9
#define MAX_PACKET_SIZE 	200


/***** RF declarations *****/

/* Receive dataQueue for RF Core to fill in data */
static dataQueue_t dataQueue;

/* Buffer which contains all Data Entries for receiving data.
 * Pragmas are needed to make sure this buffer is 4 byte aligned (requirement from the RF Core) */
static uint8_t rxDataEntryBuffer [RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(NUM_DATA_ENTRIES,
            MAX_LENGTH, NUM_APPENDED_BYTES)] __attribute__ ((aligned (4)));

/* Receive dataQueue for RF Core to fill in data */
static rfc_bleGenericRxOutput_t recvStats;
static rfc_dataEntryGeneral_t* currentDataEntry;
static dataQueue_t dataQueue;


static uint8_t* packetDataPointer;

static rfc_bleGenericRxOutput_t recvStats;


static rfc_ble5ExtAdvEntry_t ble5ExtAdvPacket;


static RF_Object rfBleObject;
static RF_Handle rfBleHandle;

/*** PIN driver declarations ***/
static PIN_Handle ledPinHandle;
static PIN_State ledPinState;


/***************** LED *********************/
// * Application LED pin configuration table:
// *   - All LEDs board LEDs are off.

static PIN_Config pinTable[] =
{
    CONFIG_PIN_RLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	PIN_TERMINATE
};

/************** SEMAPHORE ******************/
/* semaphore object definitions */
static Semaphore_Struct semStruct;
static Semaphore_Handle semHandle;

