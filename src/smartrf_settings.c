//*********************************************************************************
// Generated by SmartRF Studio version 2.12.0 (build#147)
// The applied template is compatible with CC26x2 SDK 2.40.xx.xx
// Device: CC2652R Rev. 2.1
//
//*********************************************************************************


//*********************************************************************************
// Parameter summary
// Adv. Address: 010203040506
// Adv. Data: dummy
// BLE Channel: 17
// Extended Header: 09 09 010203040506 babe
// Frequency: 2440 MHz
// PDU Payload length:: 30
// TX Power: 5 dBm
// Whitening: true

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_common_cmd.h)
#include DeviceFamily_constructPath(driverlib/rf_ble_cmd.h)
#include <ti/drivers/rf/RF.h>
#include DeviceFamily_constructPath(rf_patches/rf_patch_cpe_bt5.h)

#include "smartrf_settings.h"


// TI-RTOS RF Mode Object
RF_Mode RF_prop =
{
    .rfMode = RF_MODE_AUTO,
    .cpePatchFxn = &rf_patch_cpe_bt5,
    .mcePatchFxn = 0,
    .rfePatchFxn = 0
};


// Overrides for CMD_BLE5_RADIO_SETUP
uint32_t pOverridesCommon[] =
{
    // override_ble5_setup_override_common.xml
    // Bluetooth 5: Default to no CTE.
    HW_REG_OVERRIDE(0x5328,0x0000),
    // Increases max RX packet length from 37 to 255
    // Sets one byte firmware parameter at offset 0xA5 to 0xFF
    (uint32_t)0x00FF8A53,
    (uint32_t)0xFFFFFFFF
};

// CMD_BLE5_RADIO_SETUP
// Bluetooth 5 Radio Setup Command for all PHYs
rfc_CMD_BLE5_RADIO_SETUP_t RF_cmdBle5RadioSetup =
{
    .commandNo = 0x1820,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .defaultPhy.mainMode = 0x0,
    .defaultPhy.coding = 0x0,
    .loDivider = 0x00,
    .config.frontEndMode = 0x0,
    .config.biasMode = 0x0,
    .config.analogCfgMode = 0x0,
    .config.bNoFsPowerUp = 0x0,
    .txPower = 0x941E,
    .pRegOverrideCommon = pOverridesCommon,
    .pRegOverride1Mbps = 0,
    .pRegOverride2Mbps = 0,
    .pRegOverrideCoded = 0,
};

// CMD_FS
// Frequency Synthesizer Programming Command
rfc_CMD_FS_t RF_cmdFs =
{
    .commandNo = 0x0803,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .frequency = 0x0988,
    .fractFreq = 0x0000,
    .synthConf.bTxMode = 0x0,
    .synthConf.refFreq = 0x0,
    .__dummy0 = 0x00,
    .__dummy1 = 0x00,
    .__dummy2 = 0x00,
    .__dummy3 = 0x0000,
};

// Structure for CMD_BLE_ADV_NC.pParams
rfc_bleAdvPar_t bleAdvPar =
{
    .pRxQ = 0, // INSERT APPLICABLE POINTER: (dataQueue_t*)&xxx
    .rxConfig.bAutoFlushIgnored = 0x0,
    .rxConfig.bAutoFlushCrcErr = 0x0,
    .rxConfig.bAutoFlushEmpty = 0x0,
    .rxConfig.bIncludeLenByte = 0x0,
    .rxConfig.bIncludeCrc = 0x0,
    .rxConfig.bAppendRssi = 0x0,
    .rxConfig.bAppendStatus = 0x0,
    .rxConfig.bAppendTimestamp = 0x0,
    .advConfig.advFilterPolicy = 0x0,
    .advConfig.deviceAddrType = 0x0,
    .advConfig.peerAddrType = 0x0,
    .advConfig.bStrictLenFilter = 0x0,
    .advConfig.chSel = 0x0,
    .advConfig.privIgnMode = 0x0,
    .advConfig.rpaMode = 0x0,
    .advLen = 0x18,
    .scanRspLen = 0x00,
    .pAdvData = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .pScanRspData = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .pDeviceAddress = 0, // INSERT APPLICABLE POINTER: (uint16_t*)&xxx
    .pWhiteList = 0, // INSERT APPLICABLE POINTER: (uint32_t*)&xxx
    .__dummy0 = 0x0000,
    .__dummy1 = 0x00,
    .endTrigger.triggerType = 0x1,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .endTime = 0x00000000,
};

// CMD_BLE_ADV_NC
// BLE Non-Connectable Advertiser Command
rfc_CMD_BLE_ADV_NC_t RF_cmdBleAdvNc =
{
    .commandNo = 0x1805,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .channel = 0x8C,
    .whitening.init = 0x51,
    .whitening.bOverride = 0x1,
    .pParams = &bleAdvPar,
    .pOutput = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
};

// Structure for CMD_BLE5_GENERIC_RX.pParams
rfc_bleGenericRxPar_t bleGenericRxPar =
{
    .pRxQ = 0, // INSERT APPLICABLE POINTER: (dataQueue_t*)&xxx
    .rxConfig.bAutoFlushIgnored = 0x0,
    .rxConfig.bAutoFlushCrcErr = 0x0,
    .rxConfig.bAutoFlushEmpty = 0x0,
    .rxConfig.bIncludeLenByte = 0x1,
    .rxConfig.bIncludeCrc = 0x1,
    .rxConfig.bAppendRssi = 0x1,
    .rxConfig.bAppendStatus = 0x1,
    .rxConfig.bAppendTimestamp = 0x0,
    .bRepeat = 0x01,
    .__dummy0 = 0x0000,
    .accessAddress = 0xd9d7b81a,
    .crcInit0 = 0x55,
    .crcInit1 = 0x55,
    .crcInit2 = 0x55,
    .endTrigger.triggerType = 0x1,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .endTime = 0x00000001,
};

// CMD_BLE5_GENERIC_RX
// Bluetooth 5 Generic Receiver Command
rfc_CMD_BLE5_GENERIC_RX_t RF_cmdBle5GenericRx =
{
    .commandNo = 0x1829,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .channel = 0x8C,
    .whitening.init = 0x01,
    .whitening.bOverride = 0x1,
    .phyMode.mainMode = 0x0,
    .phyMode.coding = 0x0,
    .rangeDelay = 0x00,
    .txPower = 0x0000,
    .pParams = &bleGenericRxPar,
    .pOutput = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .tx20Power = 0x00000000,
};


// Structure for CMD_BLE5_ADV_AUX.pParam
rfc_ble5AdvAuxPar_t RF_cmdble5AdvAuxPar =
{
    .pRxQ = 0,
    .rxConfig.bAutoFlushIgnored = 0x0,
    .rxConfig.bAutoFlushCrcErr = 0x0,
    .rxConfig.bAutoFlushEmpty = 0x0,
    .rxConfig.bIncludeLenByte = 0x0,
    .rxConfig.bIncludeCrc = 0x0,
    .rxConfig.bAppendRssi = 0x0,
    .rxConfig.bAppendStatus = 0x0,
    .rxConfig.bAppendTimestamp = 0x0,
    .advConfig.advFilterPolicy = 0x0,
    .advConfig.deviceAddrType = 0x0,
    .advConfig.targetAddrType = 0x0,
    .advConfig.bStrictLenFilter = 0x0,
    .advConfig.bDirected = 0x0,
    .advConfig.privIgnMode = 0x0,
    .advConfig.rpaMode = 0x0,
    .behConfig.scanRspEndType = 0x0,
    .auxPtrTargetType = 0x00,
    .auxPtrTargetTime = 0x00000000,
    .pAdvPkt = 0,
    .pRspPkt = 0,
    .pDeviceAddress = 0,
    .pWhiteList = 0
};

// CMD_BLE5_ADV_AUX
// Bluetooth 5 Secondary Channel Advertiser Command
rfc_CMD_BLE5_ADV_AUX_t RF_cmdBle5AdvAux =
{
    .commandNo = 0x1824,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .channel = 0x69,
    .whitening.init = 0x51,
    .whitening.bOverride = 0x1,
    .phyMode.mainMode = 0x0,
    .phyMode.coding = 0x0,
    .rangeDelay = 0x00,
    .txPower = 0x0000,
    .pParams = &RF_cmdble5AdvAuxPar,
    .pOutput = 0,
    .tx20Power = 0x00000000
};

