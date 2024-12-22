
#ifndef _FILE_IMPORTALL_
#define _FILE_IMPORTALL_
//===========================================================================================
//                                         Import All
//===========================================================================================

#ifdef __cplusplus
extern "C" {
#endif
// #include "FreeRTOS.h"
// #include "timers.h"
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

#include <Arduino.h>
#include <EEPROM.h>
#include <SPIFFS.h>
#include <WString.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <HTTPUpdate.h>
#include <ESPmDNS.h>
#include "dvcSetting.h"
#include <Adafruit_NeoPixel.h>

#include "definitions.h"
#include "common.h"
#include "message_cloud.h"
#include "config.h"
#include "iot.h"
#include "dvc.h"


//===========================================================================================
//                                          End
//===========================================================================================
#endif