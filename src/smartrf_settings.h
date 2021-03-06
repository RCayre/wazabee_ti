#ifndef _SMARTRF_SETTINGS_H_
#define _SMARTRF_SETTINGS_H_

//*********************************************************************************
//// Generated by SmartRF Studio version 2.12.0 (build#147)
//// The applied template is compatible with CC26x2 SDK 2.40.xx.xx
//// Device: CC2652R Rev. 2.1
////
////*********************************************************************************
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_common_cmd.h)
#include DeviceFamily_constructPath(driverlib/rf_ble_cmd.h)
#include <ti/drivers/rf/RF.h>


// TI-RTOS RF Mode Object
extern RF_Mode RF_prop;

// RF Core API commands
extern rfc_CMD_BLE5_RADIO_SETUP_t RF_cmdBle5RadioSetup;
extern rfc_CMD_FS_t RF_cmdFs;
extern rfc_CMD_BLE_ADV_NC_t RF_cmdBleAdvNc;
extern rfc_CMD_BLE5_GENERIC_RX_t RF_cmdBle5GenericRx;
extern rfc_CMD_BLE5_ADV_AUX_t RF_cmdBle5AdvAux;

// RF Core API Overrides
extern uint32_t pOverridesCommon[];

#endif // _SMARTRF_SETTINGS_H_
