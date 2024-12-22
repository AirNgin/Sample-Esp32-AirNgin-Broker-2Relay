#ifndef _FILE_H_DIFINITION_
#define _FILE_H_DIFINITION_
//===========================================================================================
//                                         Difinitions
//                                 تعاریف سراسری نرم افزار
//===========================================================================================


#define BYTE_1 1
#define BYTE_2 2
#define BYTE_3 3
#define BYTE_10 10
#define BYTE_11 11
#define BYTE_12 12
#define BYTE_14 14
#define BYTE_15 15
#define BYTE_16 16
#define BYTE_17 17
#define BYTE_20 20
#define BYTE_32 32
#define BYTE_37 37
#define BYTE_38 38
#define BYTE_100 100
#define BYTE_400 400

#define STRING_NUMERIC "0123456789"
#define STRING_HEX "0123456789abcdefABCDEF"
#define STRING_ALPHABET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define STRING_ALPHABETNUMERIC String(STRING_ALPHABET) + String(STRING_NUMERIC)
//===========================================================================================
//                                           EPPROM
//                  مشخص کردن فضاهای ایپرام برای ذخیره سازی ساده تر
//                 
//===========================================================================================
#define EEPROM_SIZE 4000

#define EP_MEMORY_NOT_CLEANABLE 84
#define EP_RGBTYPE (EP_MEMORY_NOT_CLEANABLE + 1) // (85)
#define EP_BANNED (EP_RGBTYPE + 1)              // (87)
#define EP_CONFIGURED (EP_BANNED + 1)            // (88)
#define EP_STARTMODE (EP_CONFIGURED + 1)         // (89)
#define EP_SERIAL_S (EP_STARTMODE + 1)           // (90-99)
#define EP_SERIAL_E (EP_SERIAL_S + BYTE_10 - 1)  // (90-99)
#define EP_MEMORYSTART1 100                      // MEMORY POINT 1 BEGIN :  Cleanable Settings, Clear Begin From Here To End Of EPROM

#define EP_EXTENDER (EP_MEMORYSTART1 + 1) //........................................................... NETWORK
#define EP_CIRCUIT (EP_EXTENDER + 1)
#define EP_PROJECTCODE_S (EP_CIRCUIT + 1)
#define EP_PROJECTCODE_E (EP_PROJECTCODE_S + BYTE_20 - 1)
#define EP_CLIENTID_S (EP_PROJECTCODE_E + 1)
#define EP_CLIENTID_E (EP_CLIENTID_S + BYTE_38 - 1)
#define EP_ENCRYPTIONKEY_S (EP_CLIENTID_E + 1)
#define EP_ENCRYPTIONKEY_E (EP_ENCRYPTIONKEY_S + BYTE_10 - 1)
#define EP_MESHRSSI (EP_ENCRYPTIONKEY_E + 1) //........................................................ NETWORK MESH
#define EP_MESHCHANNEL (EP_MESHRSSI + 1)
#define EP_MESHCHANNEL_OLD (EP_MESHCHANNEL + 1)
#define EP_MESHTYPE (EP_MESHCHANNEL_OLD + 1)
#define EP_MESHNAME_S (EP_MESHTYPE + 1)
#define EP_MESHNAME_E (EP_MESHNAME_S + BYTE_20 - 1)
#define EP_MESHNAME_OLD_S (EP_MESHNAME_E + 1)
#define EP_MESHNAME_OLD_E (EP_MESHNAME_OLD_S + BYTE_20 - 1)
#define EP_MESHPASS_S (EP_MESHNAME_OLD_E + 1)
#define EP_MESHPASS_E (EP_MESHPASS_S + BYTE_20 - 1)
#define EP_MESHPASS_OLD_S (EP_MESHPASS_E + 1)
#define EP_MESHPASS_OLD_E (EP_MESHPASS_OLD_S + BYTE_20 - 1)
#define EP_MQTTUSER_S (EP_MESHPASS_OLD_E + 1) //....................................................... NETWORK MQTT
#define EP_MQTTUSER_E (EP_MQTTUSER_S + BYTE_16 - 1)
#define EP_MQTTPASS_S (EP_MQTTUSER_E + 1)
#define EP_MQTTPASS_E (EP_MQTTPASS_S + BYTE_16 - 1)
#define EP_MQTTBROKER_S (EP_MQTTPASS_E + 1)
#define EP_MQTTBROKER_E (EP_MQTTBROKER_S + BYTE_100 - 1)
#define EP_MODEMCHANNEL (EP_MQTTBROKER_E + 1) //....................................................... NETWORK MODEM
#define EP_MODEMSSID_S (EP_MODEMCHANNEL + 1)
#define EP_MODEMSSID_E (EP_MODEMSSID_S + BYTE_32 - 1)
#define EP_MODEMPASS_S (EP_MODEMSSID_E + 1)
#define EP_MODEMPASS_E (EP_MODEMPASS_S + BYTE_20 - 1)
#define EP_MODEMRSSI (EP_MODEMPASS_E + 1)

#define EP_RESERVE1_S (EP_MODEMRSSI + 1)           // RESERVE S 100
#define EP_RESERVE1_E (EP_RESERVE1_S + BYTE_1 - 1) // RESERVE E 100
#define EP_MEMORYEND1 (EP_RESERVE1_E)              // MEMORY POINT 1 END
#define EP_MEMORYPOINT1H (EP_MEMORYEND1 + 1)       // MEMORY POINT 1 SAVE : to save high-byte of current EProm-Address (to detect if upper EProm-Addresses changed)
#define EP_MEMORYPOINT1L (EP_MEMORYPOINT1H + 1)    // MEMORY POINT 1 SAVE : to save low-byte of current EProm-Address (to detect if upper EProm-Addresses changed)
#define EP_MEMORYSTART2 (EP_MEMORYPOINT1L + 1)     // MEMORY POINT 2 BEGIN : Cleanable Settings, Clear Begin From Here To End Of EPROM

//................................................................................................. DVC RELAY
#if HARDWARE_RELAY
#define EP_RELAY_TYPE_S (EP_MEMORYSTART2 + 1)
#define EP_RELAY_TYPE_E (EP_RELAY_TYPE_S + (MAX_OPR * BYTE_1) - 1)
#define EP_RELAY_SUBNET_S (EP_RELAY_TYPE_E + 1)
#define EP_RELAY_SUBNET_E (EP_RELAY_SUBNET_S + (MAX_OPR * BYTE_1) - 1)
#define EP_RELAY_ADDRESS_S (EP_RELAY_SUBNET_E + 1)
#define EP_RELAY_ADDRESS_E (EP_RELAY_ADDRESS_S + (MAX_OPR * BYTE_1) - 1)
#define EP_RELAY_CHANNEL_S (EP_RELAY_ADDRESS_E + 1)
#define EP_RELAY_CHANNEL_E (EP_RELAY_CHANNEL_S + (MAX_OPR * BYTE_1) - 1)
#define EP_RELAY_HOLDSEC_S (EP_RELAY_CHANNEL_E + 1)
#define EP_RELAY_HOLDSEC_E (EP_RELAY_HOLDSEC_S + (MAX_OPR * BYTE_1) - 1)
#define EP_RELAY_HOLDMIN_S (EP_RELAY_HOLDSEC_E + 1)
#define EP_RELAY_HOLDMIN_E (EP_RELAY_HOLDMIN_S + (MAX_OPR * BYTE_1) - 1)
#define EP_RELAY_LOCK_S (EP_RELAY_HOLDMIN_E + 1)
#define EP_RELAY_LOCK_E (EP_RELAY_LOCK_S + (MAX_OPR * BYTE_1) - 1)
#define EP_RELAY_MODE_S (EP_RELAY_LOCK_E + 1)
#define EP_RELAY_MODE_E (EP_RELAY_MODE_S + (MAX_OPR * BYTE_1) - 1)
#define EP_LASTDVCMODE_S (EP_RELAY_MODE_E + 1)
#define EP_LASTDVCMODE_E (EP_LASTDVCMODE_S + (MAX_OPR * BYTE_1) - 1)
#else
#define EP_LASTDVCMODE_E (EP_MEMORYSTART2)
#endif

//................................................................................................. SCENARIO
#define EP_SCENARIO_NAME_S (EP_LASTDVCMODE_E + 1)
#define EP_SCENARIO_NAME_E (EP_SCENARIO_NAME_S + (MAX_SCENARIO * BYTE_12) - 1)
#define EP_SCENARIO_ACT_S (EP_SCENARIO_NAME_E + 1)

#define EP_SCENARIO_VIRTUAL_S (EP_SCENARIO_ACT_S + 1)
#define EP_SCENARIO_VIRTUAL_E (EP_SCENARIO_VIRTUAL_S + (MAX_SCENARIO * MAX_SCENARIO_VIRTUAL * BYTE_1) - 1)
#define EP_SCENARIO_VIRTUALMIN_S (EP_SCENARIO_VIRTUAL_E + 1)
#define EP_SCENARIO_VIRTUALMIN_E (EP_SCENARIO_VIRTUALMIN_S + (MAX_SCENARIO * MAX_SCENARIO_VIRTUAL * BYTE_1) - 1)
#define EP_SCENARIO_VIRTUALSEC_S (EP_SCENARIO_VIRTUALMIN_E + 1)
#define EP_SCENARIO_VIRTUALSEC_E (EP_SCENARIO_VIRTUALSEC_S + (MAX_SCENARIO * MAX_SCENARIO_VIRTUAL * BYTE_1) - 1)
#define EP_SCENARIO_CODESTATE_S (EP_SCENARIO_VIRTUALSEC_E + 1) 

#define EP_SCENARIO_ACT_E (EP_SCENARIO_CODESTATE_S + (MAX_SCENARIO * BYTE_1) - 1)
#if HARDWARE_RELAY
#define EP_SCENARIO_RELAYNO_S (EP_SCENARIO_ACT_E + 1) //------------------------------------------ SCENARIO RELAY
#define EP_SCENARIO_RELAYNO_E (EP_SCENARIO_RELAYNO_S + (MAX_SCENARIO * MAX_SCENARIO_OPR * BYTE_1) - 1)
#define EP_SCENARIO_RELAYCMD_S (EP_SCENARIO_RELAYNO_E + 1)
#define EP_SCENARIO_RELAYCMD_E (EP_SCENARIO_RELAYCMD_S + (MAX_SCENARIO * MAX_SCENARIO_OPR * BYTE_1) - 1)
#define EP_SCENARIO_RELAYMIN_S (EP_SCENARIO_RELAYCMD_E + 1)
#define EP_SCENARIO_RELAYMIN_E (EP_SCENARIO_RELAYMIN_S + (MAX_SCENARIO * MAX_SCENARIO_OPR * BYTE_1) - 1)
#define EP_SCENARIO_RELAYSEC_S (EP_SCENARIO_RELAYMIN_E + 1)
#define EP_SCENARIO_RELAYSEC_E (EP_SCENARIO_RELAYSEC_S + (MAX_SCENARIO * MAX_SCENARIO_OPR * BYTE_1) - 1)
#else
#define EP_SCENARIO_RELAYSEC_E (EP_SCENARIO_ACT_E)
#endif

//................................................................................................. RUNSEN VIRTUAL
#define EP_RUNSENVIRTUAL_CON_S (EP_SCENARIO_RELAYSEC_E + 1)
#define EP_RUNSENVIRTUAL_CON_E (EP_RUNSENVIRTUAL_CON_S + (MAX_RUNSENVIRTUAL * BYTE_1) - 1)
#define EP_RUNSENVIRTUAL_SEN_S (EP_RUNSENVIRTUAL_CON_E + 1)
#define EP_RUNSENVIRTUAL_SEN_E (EP_RUNSENVIRTUAL_SEN_S + (MAX_RUNSENVIRTUAL * BYTE_1) - 1)
#define EP_RUNSENVIRTUAL_ACT_S (EP_RUNSENVIRTUAL_SEN_E + 1)
#define EP_RUNSENVIRTUAL_ACT_E (EP_RUNSENVIRTUAL_ACT_S + (MAX_RUNSENVIRTUAL * BYTE_1) - 1)
//................................................................................................. DVC GSM
#if HARDWARE_GSM
#define EP_GSM_OPERATOR (EP_RUNSENVIRTUAL_ACT_E + 1)
#define EP_GSM_TYPE (EP_GSM_OPERATOR + 1)
#define EP_GSM_SUBNET (EP_GSM_TYPE + 1)
#define EP_GSM_ADDRESS (EP_GSM_SUBNET + 1)
#define EP_GSM_CHANNEL (EP_GSM_ADDRESS + 1)
#define EP_GSM_PHONEOFFLINE_S (EP_GSM_CHANNEL + 1)
#define EP_GSM_PHONEOFFLINE_E (EP_GSM_PHONEOFFLINE_S + BYTE_15 - 1)
#define EP_GSM_PHONE1_S (EP_GSM_PHONEOFFLINE_E + 1)
#define EP_GSM_PHONE1_E (EP_GSM_PHONE1_S + BYTE_15 - 1)
#define EP_GSM_PHONE2_S (EP_GSM_PHONE1_E + 1)
#define EP_GSM_PHONE2_E (EP_GSM_PHONE2_S + BYTE_15 - 1)
#define EP_GSM_PHONE3_S (EP_GSM_PHONE2_E + 1)
#define EP_GSM_PHONE3_E (EP_GSM_PHONE3_S + BYTE_15 - 1)
#else
#define EP_GSM_PHONE3_E (EP_RUNSENVIRTUAL_ACT_E)
#endif
//................................................................................................. END
#define EP_MEMORYEND2 (EP_GSM_PHONE3_E)  // MEMORY POINT 2 END : Cleanable Settings, ClearFrom Start To Here (End Of EPROM)
#define EP_MEMORYPOINT2H (EP_MEMORYEND2 + 1)    // MEMORY POINT 2 SAVE : To save high-byte of current EProm-Address (to detect if upper EProm-Addresses changed)
#define EP_MEMORYPOINT2L (EP_MEMORYPOINT2H + 1) // MEMORY POINT 2 SAVE : To save low-byte of current EProm-Address (to detect if upper EProm-Addresses changed)

//===========================================================================================
//                                      PREFERENCES MEMORY
//===========================================================================================
#define FILE_WRITE "w"
#define FILE_APPEND "a"
#define FILE_READ "r"
#define FILE_RW "w+"
#define FILE_RA "r+"

// Preferences _MemoFile;
JsonDocument _JSONDoc;
// String Tools__File_TextRead(String filename);
// bool Tools__File_TextWrite(String filename, const String& data);

void Tools__File_JSONLoad(String filename, JsonVariant &jsn, JsonDocument &doc);
void Tools__File_JSONLoad(String filename, JsonVariant &jsn);
bool Tools__File_JSONSave(String filename, JsonVariant &jsn);
bool Tools__File_JSONSave(String filename, String &data);

//===========================================================================================
//                                           CONSTANTS
//===========================================================================================
#define BRAND_NAME "AirNgin"
#define BRAND_URL "https://AirNgin.com"
#define DVC_DEFAULTNAME "AirNgin"

#define DVC_DEFAULTPASS "00000000"   // *** default pass word for AP Mode : این پسوورد باید 00000000 باشید زیرا پسوورد دیفالت حالت وای فای ابزار جهت اتصال به ابزار از طریق نرم افزار و اضافه کردن آن به پروژه می باشد


#define HARDWARE_RELAY_AMPER_10 true

#define MODEM_RSSI_ACCEPTABLE 95
#define CIRCUIT_AUTO_0 0
#define CIRCUIT_WIRED_1 1
#define CIRCUIT_WIRELESS_2 2
#define CIRCUIT_IOTCLOUD_3 3

// -------------------------------------------- TASK PRIORITY 

#define TASK_PRIORITY_HIGH 1       // 5
#define TASK_PRIORITY_MID 2        // 3
#define TASK_PRIORITY_LOW 3        // 2
#define TASK_STACKSIZE_SMALL 2048  // 1024
#define TASK_STACKSIZE_MEDIUM 4096 // 4096
#define TASK_STACKSIZE_LARGE 8192  // 8192
#define TASK_CORE_0 0
#define TASK_CORE_1 1
#define TASK_LOOP_DELAY 20

// -------------------------------------------- SETTING WANING LED : NEO PIXEL -> WS2812
// این قسمتا را می توانید با ال ای دی های خود جایگزین نمایید

#define WARNINGLED_OFF 0
#define WARNINGLED_REBOOT 1
#define WARNINGLED_DATA 2
#define WARNINGLED_SENDALL 10
#define WARNINGLED_RANKING 20
#define WARNINGLED_CONFIG 30
#define WARNINGLED_CONFIG_NEED 31
#define WARNINGLED_DIS_WIFI 41
#define WARNINGLED_CONNECT_MESHEXTENDER 51
#define WARNINGLED_CONNECT_MESH 52
#define WARNINGLED_DIS_MESH 53
#define WARNINGLED_CONNECT_IOTEXTENDER 61
#define WARNINGLED_CONNECT_IOT 62
#define WARNINGLED_DIS_IOT 63
#define WARNINGLED_LEARN 70
#define WARNINGLED_WIREONLY 99
#define WARNINGLED_COLOR_RED 101
#define WARNINGLED_COLOR_GREEN 102
#define WARNINGLED_COLOR_BLUE 103
#define WARNINGLED_COLOR_WHITE 104
#define WARNINGLED_COLOR_YELLOW 105
#define WARNINGLED_COLOR_VIOLET 106
#define WARNINGLED_AUTO 255


// -------------------------------------------- SETTING FOR GSM
#define SIMCARD_OPERATOR_MCI 1
#define SIMCARD_OPERATOR_IRANCELL 2
#define SIMCARD_OPERATOR_RIGHTEL 3
#define SIMCARD_OPERATOR_UNKNOWN 0
#define SIMCARD_OPERATOR_END 3
#define SIMCARD_UUSD_NONE 0
#define SIMCARD_UUSD_MONEY 1
#define SIMCARD_UUSD_LANGUAGE 2


// ------------------------------------------ CONSTANTS FOR SEND NOTFICATION TO USER OR PRODUCER
#define MESSAGE_CLOUD_REBOOT 1001
#define MESSAGE_CLOUD_MODEM_WAKE 9000
#define MESSAGE_CLOUD_MODEM_OUT 9001
#define MESSAGE_CLOUD_GSM_SIGNAL_UNKNOWN 8001
#define MESSAGE_CLOUD_GSM_SIGNAL_OUT 8002
#define MESSAGE_CLOUD_GSM_SIGNAL_WAKE 8003
#define MESSAGE_CLOUD_GSM_SIGNAL_MID 8004
#define MESSAGE_CLOUD_GSM_SIGNAL_GOOD 8005
#define MESSAGE_CLOUD_GSM_SIGNAL_BEST 8006
#define MESSAGE_CLOUD_GSM_QUALITY_UNKNOWN 8010
#define MESSAGE_CLOUD_GSM_QUALITY_GOOD 8011
#define MESSAGE_CLOUD_GSM_QUALITY_MID 8012
#define MESSAGE_CLOUD_GSM_QUALITY_WAKE 8013
#define MESSAGE_CLOUD_GSM_QUALITY_BAD 8014
#define MESSAGE_CLOUD_GSM_WALET_0_TO_5 8020
#define MESSAGE_CLOUD_GSM_WALET_5_TO_10 8021
#define MESSAGE_CLOUD_GSM_WALET_10_TO_20 8022
#define MESSAGE_CLOUD_GSM_WALET_20_TO_MORE 8023


// ------------------------------------------ CONSTANTS FOR SEND SMS TO USER OR PRODUCER : برای پیامک باید تکست باشد پس به شکل زیر تعریف شده است. 
// ------------------------------------------ دقت نمایید باید کدهای زیر را با کدهای شخصی خود جایگزین نمایید . این کدها در پنل کاربری خود در بخش پیام ابزار قابل تعریف است
#define TX_MESSAGE_CLOUD_REBOOT "1001"
#define TX_MESSAGE_CLOUD_MODEM_WAKE "9000"
#define TX_MESSAGE_CLOUD_MODEM_OUT "9001"
#define TX_MESSAGE_CLOUD_GSM_SIGNAL_UNKNOWN "8001"
#define TX_MESSAGE_CLOUD_GSM_SIGNAL_OUT "8002"
#define TX_MESSAGE_CLOUD_GSM_SIGNAL_WAKE "8003"
#define TX_MESSAGE_CLOUD_GSM_SIGNAL_MID "8004"
#define TX_MESSAGE_CLOUD_GSM_SIGNAL_GOOD "8005"
#define TX_MESSAGE_CLOUD_GSM_SIGNAL_BEST "8006"
#define TX_MESSAGE_CLOUD_GSM_QUALITY_UNKNOWN "8010"
#define TX_MESSAGE_CLOUD_GSM_QUALITY_GOOD "8011"
#define TX_MESSAGE_CLOUD_GSM_QUALITY_MID "8012"
#define TX_MESSAGE_CLOUD_GSM_QUALITY_WAKE "8013"
#define TX_MESSAGE_CLOUD_GSM_QUALITY_BAD "8014"
#define TX_MESSAGE_CLOUD_GSM_WALET_0_TO_5 "8020"
#define TX_MESSAGE_CLOUD_GSM_WALET_5_TO_10 "8021"
#define TX_MESSAGE_CLOUD_GSM_WALET_10_TO_20 "8022"
#define TX_MESSAGE_CLOUD_GSM_WALET_20_TO_MORE "8023"

// ------------------------------------------ CONSTANTS FOR SEND NOTFICATION TO USER OR PRODUCER

#define MIN_20 1200
#define MIN_15 900
#define MIN_10 600
#define MIN_5 300
#define MIN_4 240
#define MIN_3 180
#define MIN_2 120
#define MIN_1 60
#define SEC_90 90
#define SEC_30 30
#define SEC_20 20
#define SEC_10 10
#define SEC_5 5
#define SEC_4 4
#define SEC_3 3
#define SEC_2 2
#define SEC_1 1
#define TIMER_NEED_RUN 0
#define TIMER_JOB_DONE -1
#define TIMER_EXPIRED_FOREVER -2

//===========================================================================================
//                                        GENERAL Tools
//===========================================================================================

void Debug_ConsolePrint(String p1);
void Debug_ConsolePrintln(String p1);
void Debug__ConsoleBegin();

void CA_CopyStr(char output[], String str, int max);
void MemoCheck();
void Tools__SetMode(String mode, bool restartNeed);
void Tools__SettingForTest();
void Tools__SettingRead();
void Tools__SettingDefault();
void Tools__SettingSave(String key);
void Tools__LED_ReadWrite(byte mode, int dly, bool needClean);
void Tools__LED_Warning(byte mode, int dly);
void Tools__LED_WarningBlink(byte counter, byte mode1, byte mode2, int milisec);
void Tools__AllDefaults();
void Tools__ScenarioLoad(byte s);
void Tools__ScenarioSave(byte s);
void Tools__RnnsenLoad(String typ);
void Tools__RunsenSave(String typ);
void Tools__DvcAddressLoad(String typ);
void Tools__DvcAddressSave(String typ, byte index);

// Scheduler _TaskRunner;
void TaskTimerLoop(void *param);
void Tools__Reboot(String Reason);
// Task Tools__Reboot_Task(TASK_SECOND * 2, TASK_FOREVER, &Tools__Reboot);
void Tools__TaskSetup();
void Tools__BackgroundWorks();
void Tools__FreeMemory();
// Task Tools__FreeMemory_Task(TASK_MINUTE * 3, TASK_FOREVER, &Tools__FreeMemory);
void Tools__WifiPower(bool SetToOn);
uint32_t Tools__GetChipID();
long Tools__Random(long a, long b);

//===========================================================================================
//                                        GENERAL Vars
//===========================================================================================

byte _Banned = 0;                              // در قسمت هایی از برنامه نیاز داریم که قسمتی از عملیات ها متوقف شود 
byte _Fail = 0;
byte _TestRebootCount = 0;
bool _Dvc_HasBarcode = false;

char _MySenderId[38];
String _CloudClientId = "";
String _Token = "";
String _StartMode = "";
String _SerialNo = "";
String _SerialHex = "";
String _SerialCloud = "";

int _ConfigTimeout = 0;

byte _Configured = 0;
byte _Extender = 0;
String _Circuit = "";
String _ProjectCode = "";
String _EncryptionKey = "";

String _ModemSSID = "";
String _ModemPass = "";
byte _ModemRSSI = 99;
byte _ModemChannel = 0;

String _MqttBroker = "";
String _MqttUser = "";
String _MqttPass = "";

unsigned long _TimerMqttLoopPrev = 0;
const unsigned long _MttLoopInterval = 5000; // فاصله زمانی (10 ثانیه)

byte _MeshRSSI = 99;
byte _MeshChannel = 1;
byte _MeshChannelOld = 1;
String _MeshName = DVC_DEFAULTNAME;
String _MeshPass = DVC_DEFAULTPASS;
String _MeshNameOld = DVC_DEFAULTNAME;
String _MeshPassOld = DVC_DEFAULTPASS;
bool _CloudSupport = false;

//.......................
bool _Need_IOT = false;
bool _Need_Mesh = false;
bool _Need_Wire = false;
bool _Need_Extender = false;
bool _Need_Taskloop = false;
bool _Need_ConfigIsBad = false;

bool _Started_WiredOnly = false;
bool _Started_IotRun = false;
bool _Started_MeshRun = false;
bool _Started_ConfigPanel = false;
bool _Started_ConfigSendall = false;
bool _Started_ConfigRanking = false;
bool _Started_RebootSendall = false;

// SimpleList<uint32_t> _Mesh_NodesList;

class Class_Json
{
public:
  char Scope[11];
  char Type[11];
  char Data[2048];
  // int DataVersion = 1;
  char Sender[38];
};
Class_Json _DataRcv_JSON;

class Class_Scenario
{
public:
  char Name[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  byte Id = 0;
  byte Act = 0xFF;

#if HARDWARE_RELAY
  byte RelayNo[MAX_SCENARIO_OPR] = {};
  byte RelayCmd[MAX_SCENARIO_OPR] = {};
  int RelayDelay[MAX_SCENARIO_OPR] = {};
#endif

  byte Virtual[MAX_SCENARIO_VIRTUAL] = {};
  int VirtualDelay[MAX_SCENARIO_VIRTUAL] = {};

  byte CodeCmd[MAX_SCENARIO_CODE] = {};
  char CodeVal[MAX_SCENARIO_CODE][21] = {};
  int CodeDelay[MAX_SCENARIO_CODE] = {};

#if HARDWARE_GSM
  byte GsmCmd[3] = {0xFF, 0xFF, 0xFF};
  byte GsmVal[3] = {0xFF, 0xFF, 0xFF};
  char GsmPhone[3][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
#endif
};
Class_Scenario _Scenario_List[MAX_SCENARIO]; // Tools__AllDefaults() : need set default before use

class Class_SenActionVirtual
{
public:
  bool Valid = false;
  int DelayLeft = 0;
  byte Cmd = 0;
  byte SenId = 0;
};
class Class_SenActionDigital : public Class_SenActionVirtual
{
public:
  byte Pol = 0;
};
class Class_SenActionKey : public Class_SenActionVirtual
{
public:
  byte Pol = 0;
};
class Class_SenActionCode
{
public:
  bool Valid = false;
  int DelayLeft = 0;
  char Cmd[21] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  byte SenId = 0;
};

#if HARDWARE_RELAY
Class_SenActionDigital _QueueAction_Relay[MAX_OPR];
#endif
Class_SenActionVirtual _QueueAction_Virtual[MAX_SCENARIO * MAX_SCENARIO_VIRTUAL];
Class_SenActionCode _QueueAction_Code[MAX_SCENARIO * MAX_SCENARIO_CODE];

class Class_RunsenCode
{
public:
  byte Dvc = 0xFF;                                                                // 255:NotUse  1:Hex  2:Code
  char Con[21] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Value Condition
  byte Act = 0xFF;                                                                // Senario Action (0:Disable  1:Enable  2:Run)                                                                // Senario Action (0:Deactive  1:Active)
  byte Sen = 0xFF;                                                                // Senario ID
};
class Class_Runsen
{
public:
  byte Dvc = 0xFF; // 255:NotUse   Number(1-20):Index Of DI
  byte Con = 0xFF; // Value Condition
  byte Act = 0xFF; // Senario Action (0:Disable  1:Enable  2:Run)
  byte Sen = 0xFF; // Senario ID
};
#if HARDWARE_GSM
Class_Runsen _Runsen_GSM[MAX_RUNSENGSM];
#endif
Class_Runsen _Runsen_Virtual[MAX_RUNSENVIRTUAL];
Class_RunsenCode _Runsen_Code[MAX_RUNSENCODE];

class Class_CommandOperation
{
public:
  char deviceSerial[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char operationName[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char value[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  bool isValid = false;
  void reset()
  {
    CA_CopyStr(deviceSerial, "", 14);
    CA_CopyStr(operationName, "", 14);
    CA_CopyStr(value, "", 14);
    isValid = false;
  }
};
class Class_ScenarioOperation
{
public:
  byte act = 0xFF;
  byte code = 0xFF;
  bool isValid = false;
  void reset()
  {
    act = 0xFF;
    code = 0xFF;
    isValid = false;
  }
};
class Class_DvcBase
{
public:
  byte Type = 0xFF;
  byte Subnet = 0xFF;
  byte Address = 0xFF;
  byte Channel = 0xFF;
  bool Lock = false;
  bool BabyLock = false;
};
class Class_DvcDigital : public Class_DvcBase
{
public:
  bool Status = false;
  bool StatusChanged = true;
  byte UseAsRelay = 0xFE;
};

class Class_DvcRelay : public Class_DvcBase
{
public:
  // byte StartMode = STARTMODE_OFF;
  bool Status = false;
  bool StatusChanged = true;
  int Hold = 0;
  int HoldTimeout = 0;
  byte CurrentTemp = 0;
  byte ThermoLink = 0xFF;
  Class_ScenarioOperation SO_On;
  Class_ScenarioOperation SO_Off;
  Class_ScenarioOperation SO_Stop;
  Class_CommandOperation CO_On;
  Class_CommandOperation CO_Off;
  Class_CommandOperation CO_Stop;
};
#if HARDWARE_GSM
class Class_DvcGSM : public Class_DvcBase
{
public:
  byte Operator = 0;
  char Phone1[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char Phone2[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char Phone3[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char PhoneOffline[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};
Class_DvcGSM _Dvc_GSM;
#endif

class Class_RGBColor
{
public:
  byte R = 0;
  byte G = 0;
  byte B = 0;
  byte Level = 0;
};
class Class_Theme
{
public:
  Class_RGBColor On;
  Class_RGBColor Off;
};
Class_RGBColor RGB_OriginalDefault;
Class_RGBColor RGB_Original[6];


#if HARDWARE_RELAY
Class_DvcRelay _Dvc_Relay[MAX_OPR];
#endif

Class_DvcBase _Dvc_Virtual;


//===========================================================================================
//                                           Mesh
//===========================================================================================
// painlessMesh _MeshObj;
// String _Mesh_SendData;  // Public WifiMesh Data
// String _Mesh_SendType;  // Public WifiMesh Type
// bool _Mesh_IsConnected = false;
// void Mesh__Setup(void* param);
// void Mesh__PrepareWithTasks(String mname, String mpass, byte channel, bool isroot);
// void Mesh__Connect();
// Task Mesh__Connect_Task(TASK_SECOND * 5, TASK_FOREVER, &Mesh__Connect);
// void Mesh__StopToReconnect();
// void Mesh__OnReceive(const uint32_t& from, const String& msg);  // call back from mesh node in mesh network
// void Mesh__Send(String type, String data);
// void Mesh__SendByTask();
// Task Mesh__SendTask(TASK_SECOND * 2, TASK_FOREVER, &Mesh__SendByTask);
// void Mesh__CheckRootAlive();

//===========================================================================================
//                                         Mqtt / IOT
//===========================================================================================
bool _WiFi_IsConnected = false;
bool _WiFi_ConnectWorking = false;
WiFiClient _WifiClient;
PubSubClient _MqttObj(_WifiClient);
String _CurrentIP = "0.0.0.0";
String _MqttTopic = "";
String _MqttClientId = "";
String _Mqtt_RootAliveRank = "";
String _Mqtt_RootAliveData = "";
byte _Mqtt_RootAliveCount = 0;
bool _Mqtt_RootReady = false;
byte _MqttCon_Steps = 0;
bool _MqttCon_IsConnected = false;
bool _Mqtt_TryConnecting = false;
uint16_t _MqttPort = 1883;
void Mqtt__Send(String scope, String type, String data, String sender, bool offlineSupport);
void Mqtt__Send(String topic, String data, bool offlineSupport);

extern bool _IOTCloudUse;
int _IOT_ModemTimeout = MIN_3;
int _IOT_MqttTimeout = TIMER_NEED_RUN;
int _IOT_PingTimeout = MIN_15;
void IOT__Setup(void *param);
void IOT__Call__Loop__MQTT();
void MessageCloud__ViaMqtt_NotifyTo_User(int messageCode);
void MessageCloud__ViaMqtt_NotifyTo_User(String &messageCode);
void MessageCloud__ViaMqtt_SMSTo_User(int messageCode);
void MessageCloud__ViaMqtt_SMSTo_User(String &messageCode);
void MessageCloud__ViaMqtt_SMSTo_Center(int messageCode);
void MessageCloud__ViaMqtt_SMSTo_Center(String &messageCode);



//===========================================================================================
//                                     SendAll (On Old Mesh)
//===========================================================================================
void Sendall__Setup(void *param);
void Sendall__Loop();

//===========================================================================================
//                                       Config / Update
//===========================================================================================
WebServer _Server(80);
void Config__Setup(void *param);
void Config__Loop();
bool Config__ReceiveJson_ScenarioEdit(JsonDocument doc, bool deleteOnly);
void Config__ReceiveJson_CommandOperation(JsonVariant &inp, bool needSave, bool needReload);
void Config__ReceiveJson_ScenarioOperation(JsonVariant &inp, bool needSave, bool needReload);

//===========================================================================================
//                                        ADM (Wired)
//===========================================================================================

bool _IsSerialStarted = false;

#if HARDWARE_ADM
extern String _ADM_ReceiveData;
extern void ADM__Setup(void *param);
extern String ADM__LoopRead();
extern void ADM__RS485Send(String DataHex);
extern void ADM__SendData_2(String s, byte b);
HardwareSerial _ADMSerial(2); // active uart2 for ADM
#endif

//===========================================================================================
//                                            Dvc
//===========================================================================================
#define T_UNUSE 0xFF                   // 255
#define T_LIGHT 0x00                   // 0
#define T_SENKEY 0x01                  // 1
#define T_CURTAIN 0x02                 // 2
#define T_FANCOIL_SIMPLE 0x03          // 3
#define T_COOLER 0x04                  // 4
#define T_RGB 0x05                     // 5
#define T_IRREMOTE 0x06                // 6
#define T_GSM 0x07                     // 7
#define T_SIREN 0x08                   // 8
#define T_RELAY_REMOTE 0x09            // 10
#define T_SENSOR_TEMP 0x0B             // 11
#define T_SENSOR_LUX 0x0C              // 12
#define T_SENSOR_GAZ 0x0D              // 13
#define T_SENSOR_MOTION 0x0E           // 14
#define T_SENSOR_MAGNET 0x0F           // 15
#define T_SENSOR_HUMIDITY_AIR 0x10     // 16
#define T_SENSOR_HUMIDITY_SOUL 0x11    // 17
#define T_KEYPAD 0x12                  // 18
#define T_RF433_SIMPLE 0x13            // 19
#define T_RF433_TOGGLE 0x14            // 20
#define T_DIGITAL_TOGGLE 0x15          // 21
#define T_DIGITAL_DOUBLE 0x16          // 22
#define T_THERMOSTAT_2PIPE_1VALVE 0x17 // 23
#define T_THERMOSTAT_4PIPE_2VALVE 0x18 // 24
#define T_THERMOSTAT_0PIPE 0x19        // 25
#define T_THERMOSTAT_COOLER 0x1A       // 26
#define T_PLAYER_MINI 0x1E             // 30


Adafruit_NeoPixel pixels(NEOPIXEL_NUM, NEOPIXEL_PIN, NEO_RGB + NEO_KHZ800);

void Dvc__CloudSendAllStatus();
void Dvc__LastStatusSave();
void Dvc__LastStatusLoad();
void Dvc__AnalyzeData(String Data);
void Dvc__Senario_DoAction(byte Sen, byte Act);



void Dvc__SetPinout(bool refresh);
void Manual__Setup();
void Manual__Loop(void *param);

void _Dvc_SenarioCommand_TimerCheck();

unsigned long _TimerSecCurrent, _TimerSecOld = 0;
int _TimerSecExtra = 0;
byte _TimerSecDef = 0;
int _TimerForActions = 0;

#if SOFTWARE_DEBUGMODE
bool GSM__PreventPrinting = false;
bool GSM__IsPrinting = false;
#endif

void GSM__PowerOff();
#if HARDWARE_GSM
#define GSM_PHONEOFFLINE "90003497"
#define GSM_PHONEODEBUG "9035577653"
void MessageCloud__ViaGSM_NotifyTo_User(int messageCode);
void MessageCloud__ViaGSM_NotifyTo_User(String& messageCode);
void MessageCloud__ViaGSM_SMSTo_User(int messageCode);
void MessageCloud__ViaGSM_SMSTo_User(String& messageCode);
void MessageCloud__ViaGSM_SMSTo_Center(int messageCode);
void MessageCloud__ViaGSM_SMSTo_Center(String& messageCode);

#define GSM_MAGICWORD_FOR_PRINT "\a\a\a"
#define GSM_MAGICWORD_FOR_PRINT_LEN String(GSM_MAGICWORD_FOR_PRINT).length()
bool _Dvc_GSM_Alive = false;

int _GSM_RSSI_CheckupTimeout = SEC_30;
int _GSM_Money_CheckupTimeout = MIN_1;
int _GSM_Money_ResponseTimeout = TIMER_JOB_DONE;
int _GSM_Operator_AutofindTimeout = TIMER_JOB_DONE;
int _GSM_AllStatusTimeout = TIMER_JOB_DONE;

bool DVC__GSMValid[MAX_SCENARIO * MAX_SCENARIO_GSM] = {0};
void GSM__ChangeOfflineNumber(String newNumber);
void GSM__RunCommand(String Value, bool num_chang);
void Sms_QueueAdd(String PhoneNumber, String Message, bool IsUnicode);

void MessageCloud__ViaGSM_NotifyTo_User(int messageCode);
void MessageCloud__ViaGSM_NotifyTo_User(String& messageCode);
void MessageCloud__ViaGSM_SMSTo_User(int messageCode);
void MessageCloud__ViaGSM_SMSTo_User(String& messageCode);
void MessageCloud__ViaGSM_SMSTo_Center(int messageCode);
void MessageCloud__ViaGSM_SMSTo_Center(String& messageCode);
void GSM__SmsRead();
void GSM__RSSI(String res);
void GSM__MoneyCharged(String res);
void GSM__SmsToMgr(String sms);
#endif

//===========================================================================================
//                                          End
//===========================================================================================
#endif
