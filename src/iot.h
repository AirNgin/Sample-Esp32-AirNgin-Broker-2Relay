#ifndef _FILE_H_IOT_
#define _FILE_H_IOT_
//===========================================================================================
//                                          IOT (Mqtt)
//===========================================================================================

#include <ESP32Ping.h>
#include <HTTPClient.h>
#include <time.h>



bool _Reboot = true;
bool _IOTCloudUse = false;
bool _MqttRcv_IsWorking = false;

// certificate : لازم برای دریافت فایل آپدیت از سرور OTA
// ***** نکته *******
// آپدیت ابزارها از راه دور در حال تکمیل است
const String rootCACertificate = "-----BEGIN CERTIFICATE-----\n" \
"MIIDuzCCAqOgAwIBAgIDBETAMA0GCSqGSIb3DQEBBQUAMH4xCzAJBgNVBAYTAlBM\n" \
"MSIwIAYDVQQKExlVbml6ZXRvIFRlY2hub2xvZ2llcyBTLkEuMScwJQYDVQQLEx5D\n" \
"ZXJ0dW0gQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkxIjAgBgNVBAMTGUNlcnR1bSBU\n" \
"cnVzdGVkIE5ldHdvcmsgQ0EwHhcNMDgxMDIyMTIwNzM3WhcNMjkxMjMxMTIwNzM3\n" \
"WjB+MQswCQYDVQQGEwJQTDEiMCAGA1UEChMZVW5pemV0byBUZWNobm9sb2dpZXMg\n" \
"Uy5BLjEnMCUGA1UECxMeQ2VydHVtIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MSIw\n" \
"IAYDVQQDExlDZXJ0dW0gVHJ1c3RlZCBOZXR3b3JrIENBMIIBIjANBgkqhkiG9w0B\n" \
"AQEFAAOCAQ8AMIIBCgKCAQEA4/t9o3K6wvDJFIf1awFO4W5AB7ptJ11/91sts1rH\n" \
"UV+rpDKmYYe2bg+G0jACl/jXaVehGDldamR5xgFZrDwxSjh80gTSSyjoIF87B6LM\n" \
"TXPb865Px1bVWqeWifrzq2jUI4ZZJ88JJ7ysbnKDHDBy3+Ci6dLhdHUZvSqeexVU\n" \
"BBvXQzmtVSjF4hq79MDkrjhJM8x2hZ85RdKknvISjFH4fOQtf/WsX+sWn7Et0brM\n" \
"kUJ3TCXJkDhv2/DM+44el1k+1WBO5gUo7Ul5E0u6SNsv+XLTOcr+H9g0cvW0QM8x\n" \
"AcPs3hEtF10fuFDRXhmnad4HMyjKUJX5p1TLVIZQRan5SQIDAQABo0IwQDAPBgNV\n" \
"HRMBAf8EBTADAQH/MB0GA1UdDgQWBBQIds3LB/8k9sXN7buQvOKEN0Z19zAOBgNV\n" \
"HQ8BAf8EBAMCAQYwDQYJKoZIhvcNAQEFBQADggEBAKaorSLOAT2mo/9i0Eidi15y\n" \
"sHhE49wcrwn9I0j6vSrEuVUEtRCjjSfeC4Jj0O7eDDd5QVsisrCaQVymcODU0HfL\n" \
"I9MA4GxWL+FpDQ3Zqr8hgVDZBqWo/5U30Kr+4rP1mS1FhIrlQgnXdAIv94nYmem8\n" \
"J9RHjboNRhx3zxSkHLmkMcScKHQDNP8zGSal6Q10tz6XxnboJ5ajZt3hrvJBW8qY\n" \
"VoNzcOSGGtIxQbovvi0TWnZvTuhOgQ4/WwMioBK+ZlgRSssDxLQqKi2WF+A5VLxI\n" \
"03YnnZotBqbJ7DnSq9ufmgsnAjUpsUCV5/nonFWIGUbWtzT1fs45mtk48VH3Tyw=\n" \
"-----END CERTIFICATE-----\n";

bool _Update_Trying = false;

// تنظیم زمان جهت دریافت آپدیت از سرور می باشد
// SSL : برای آنکه در خواست ما مورد قبول باشد باید ساعت ابزار درخواست دهنده درست باشد. در غیر اینصورت درخواست شما ریجکت می باشد
void SetClock(String UTC="") {
  struct tm timeinfo = {};
  if(UTC!=""){
    DEBUG_SERIAL_PRINTLN("SetClock : Try to sync the time from Mqtt...");
    const char* dateTimeStr = UTC.c_str();
    if (strptime(dateTimeStr, "%Y-%m-%dT%H:%M:%S", &timeinfo) != NULL) {
      timeinfo.tm_isdst = 0;
      time_t t = mktime(&timeinfo);
      if (t != -1) {
        struct timeval tv = { .tv_sec = t, .tv_usec = 0 };
        settimeofday(&tv, NULL); 
        DEBUG_SERIAL_PRINTLN("SetClock : Time set : "+String(asctime(&timeinfo)));
        return;
      } else {
        DEBUG_SERIAL_PRINTLN("SetClock : Error setting time");
      }
    } else {
      DEBUG_SERIAL_PRINTLN("SetClock : Invalid date format. Use YYYY-MM-DDTHH:MM:SS");
    }
  }
  DEBUG_SERIAL_PRINTLN("SetClock : Try to sync the time from web...");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");  // UTC
  DEBUG_SERIAL_PRINTLN("SetClock : Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    yield();
    delay(500);
    DEBUG_SERIAL_PRINTLN(".");
    now = time(nullptr);
  }
  timeinfo={};
  gmtime_r(&now, &timeinfo);
  DEBUG_SERIAL_PRINTLN("SetClock : Current time: "+String(asctime(&timeinfo)));
}


// متد آپدیت فریمور که در حال تکمیل روند می باشیم

void IOT__FirmwareUpdate(String DownloadLink, String Certificate = "", String UTC=""){
  if(_Update_Trying) return;
  _Update_Trying = true;
  try{
    DEBUG_SERIAL_PRINTLN("IOT__FirmwareUpdate : Start");
    if (WiFi.isConnected()==true)
    {
      DEBUG_SERIAL_PRINTLN("IOT__FirmwareUpdate : Wifi ok");
      SetClock(UTC);
      DEBUG_SERIAL_PRINTLN("IOT__FirmwareUpdate : clock ok");
      WiFiClientSecure client;
      client.setCACert( (Certificate == "" ? rootCACertificate : Certificate).c_str());
      client.setTimeout(20); // timeout argument is defined in seconds for setTimeout
      t_httpUpdate_return ret = httpUpdate.update(
          client, DownloadLink, "",
          [](HTTPClient *client)
          {
          }
      );
      switch (ret)
      {
        case HTTP_UPDATE_FAILED:
           DEBUG_SERIAL_PRINTLN("IOT__FirmwareUpdate : UPDATE_FAILED Error ("+String(httpUpdate.getLastError())+"): "+String(httpUpdate.getLastErrorString().c_str()));
            break;

        case HTTP_UPDATE_NO_UPDATES:
             DEBUG_SERIAL_PRINTLN("IOT__FirmwareUpdate : NO_UPDATES");
            break;

        case HTTP_UPDATE_OK:
             DEBUG_SERIAL_PRINTLN("IOT__FirmwareUpdate : UPDATE_OK");
            break;
      }
      DEBUG_SERIAL_PRINTLN("IOT__FirmwareUpdate : IOT__FirmwareUpdate : End");
    }
  }
  catch(...){
      DEBUG_SERIAL_PRINTLN("IOT__FirmwareUpdate : Err");
  }
  _Update_Trying = false;
}


// حلقه اصلی اتصال و حفظ اتصال به وای فای 
void Network_RSSI() 
{
  try{
    //...................................................................... Modem Out Of Reach
    if (!WiFi.isConnected())
    {
      DEBUG_SERIAL_PRINTLN("Network_RSSI No Modem");
      #if HARDWARE_GSM
          MessageCloud__ViaGSM_NotifyTo_User(MESSAGE_CLOUD_MODEM_OUT);
      #endif
    }
    //...................................................................... Modem Is Valid
    else
    {
      // دریافت کیفیت سیگنال اتصال ابزار به مودم یا روتر
      int rssi = WiFi.RSSI();
      DEBUG_SERIAL_PRINTLN("Network_RSSI of Modem : "+String(rssi));پ
      // اگر کیفیت سیگنال ضعیف باشد نیاز است به کاربر جهت اطلاع پیام دهیم
      if(rssi<-80) {
        if (_MqttCon_IsConnected)
        {
          // MESSAGE_CLOUD_MODEM_WAKE : مقدار کد پیام می باشد که شما در پنل کاربری  خود تعریف کرده اید و باید مقدار آن را جایگزین کنید

          // پیام از طریق نوتفیکیشن به کاربر مستر ارسال می شود
          if(_IOTCloudUse) MessageCloud__ViaMqtt_NotifyTo_User(MESSAGE_CLOUD_MODEM_WAKE);
         
          // اگر ابزار شما حساس می باشد می توانید از طریق پیامک نیز به کاربر اطلاع دهید که شامل هزینه برای کاربر است این ارسال
          // if(_IOTCloudUse) MessageCloud__ViaMqtt_SMSTo_User(MESSAGE_CLOUD_MODEM_WAKE);
        }
        else{
      #if HARDWARE_GSM // این برای کارکردن ابزار توسط ماژول سیم کارت می باشد که به زودی تکمیل می شود
                       // اگر ابزار شما ماژول پیامک ندارد از این کد چشم پوشی نمایید.
          if(_IOTCloudUse) MessageCloud__ViaGSM_NotifyTo_User(MESSAGE_CLOUD_MODEM_WAKE);
          // else for local_broker
      #endif
        }
      }
    }
  }
  catch(...){}
}



// متد پینگ کرفتن که جهت کنترل و مدیریت اتصال به سرور بروکر استفاده می شود.
bool Network_Ping(String remote_host)
{
  DEBUG_SERIAL_PRINTLN("Wifi_Ping To " + remote_host);
  if (!WiFi.isConnected())
  {
    DEBUG_SERIAL_PRINTLN("Wifi_Ping No Modem");
    return false;
  }
  if (Ping.ping(remote_host.c_str()))
  {
    DEBUG_SERIAL_PRINTLN("Wifi_Ping Success!!");
    return true;
  }
  else
  {
    DEBUG_SERIAL_PRINTLN("Wifi_Ping Error :");
    return false;
  }
}



// از آنجایی که ممکن است مودم شما دسترسی یک ابزار را بلاک کرده باشد 
// جهت رفع این مشکل یک بار وای فای داخلی میکرو کنترلر را ریست می نماییم
void Network_Reset()
{
  DEBUG_SERIAL_PRINTLN("\r\n************* Network_Reset!! ***************\r\n");
  //....................................................... Prevent IOT_Checker
  _MqttCon_Steps = 99;
  //.......................................................
  _IOT_PingTimeout = MIN_15;

  if (WiFi.isConnected())
  {
    WiFi.disconnect(true);
    delay(500);
  }
  Tools__WifiPower(false);
  delay(500);
  Tools__WifiPower(true);

  _MqttCon_IsConnected = false;
  _WiFi_IsConnected = false;
  // WiFi.onEvent(IOT__WiFiEvent);
  //....................................................... Activate IOT_Checker from start
  _MqttCon_Steps = 0;
}



// فرآینده نگهداشت اتصال به سرور بروکر . اینترنت
bool Network_HelthCheck()
{
  // اگر mqtt تا 15 دقیقه اگر وصل نبود یک بار وی فای ریست شود
  // اگر کمتر از 10 ثانیه هست که ریکانکت میشه نیازی به پینگ نیست
  DEBUG_SERIAL_PRINTLN("Network_HelthCheck (after " + String(MIN_15 - _IOT_PingTimeout) + " sec)");
  if (MIN_15 - _IOT_PingTimeout < SEC_30) return true;
  //................................................................
  if (Network_Ping(WiFi.gatewayIP().toString()))
  {
    if (Network_Ping(_MqttBroker))
    {
      // اگر به بروکر وصل بود تلاش کنیم برای وصل
      _IOT_MqttTimeout = TIMER_NEED_RUN;
      return true;
    }
    else if (_IOTCloudUse)
    {
      //  may no internet
      //  یعنی در کل اینترنت نیست ، ولی ممکن است اینترانت شده باشد
      if (MIN_15 - _IOT_PingTimeout > MIN_10)
      {
        if (_MqttBroker == "mqtt.airngin.ir")
        {
          if (Network_Ping("mqtt.airngin.com"))
          {
            _MqttBroker = "mqtt.airngin.com";
            return true;
          }
        }
        else if (Network_Ping("mqtt.airngin.ir"))
        {
          _MqttBroker = "mqtt.airngin.ir";
          return true;
        }
        return false;
      }
      else
        return true;
    }
    else
    {
      return (MIN_15 - _IOT_PingTimeout > MIN_10 ? false : true);
    }
  }
  else
  {
    // اگر پینگ gateway نداریم نیاز است یک بار از مودم قطع شویم و دوباره وصل شویم
    Network_Reset();
    return false;
  }
  return false;
}



// جهت آگاهی کاربر مستر بعد از هر بار روشن شدن به کاربر مستر یک نوتفیکیشن ارسال می نماییم
void Send_RebootAndStatus()
{
  if (_Reboot)
  {
    //............................................ CLOUD BROKER
    if (_IOTCloudUse)
    {
      if (_MqttCon_IsConnected)
      {
#if HARDWARE_GSM
        _GSM_AllStatusTimeout = TIMER_JOB_DONE;
#endif 
        _Reboot = false;
        Network_RSSI();//  Check Modem RSSI
        MessageCloud__ViaMqtt_NotifyTo_User(MESSAGE_CLOUD_REBOOT);
      }
      else
      {
#if HARDWARE_GSM
        _Reboot = false;
        Network_RSSI();//  Check Modem RSSI
        _GSM_AllStatusTimeout = SEC_5;
        MessageCloud__ViaGSM_SMSTo_User(MESSAGE_CLOUD_REBOOT);
#endif
      }
    }
  }
}



// متد اصلی اتصال به سرور بروکر می باشد
void Mqtt__Connect()
{
  if (_Mqtt_TryConnecting)
    return;
  try
  {
    if (!_MqttObj.connected() || _MqttObj.state() < 0)
    {
      _Mqtt_TryConnecting = true;
      if (!Network_HelthCheck())
      {
        _Mqtt_TryConnecting = false;
        return;
      }
      if (_IOT_MqttTimeout > 0)
      {
        _Mqtt_TryConnecting = false;
        return;
      }
#if HARDWARE_GSM
      if (_Dvc_GSM.Type != T_GSM)
        DEBUG_SERIAL_PRINTLN("Mqtt_Connecting...");
#elif SOFTWARE_DEBUGMODE
      DEBUG_SERIAL_PRINTLN("Mqtt_Connecting...");
#endif

      delay(1500);
      if (_MqttObj.connect(_MqttClientId.c_str(), _MqttUser.c_str(), _MqttPass.c_str()))
      {
        //........................................................... Reconnected
        delay(10);
        DEBUG_SERIAL_PRINTLN("Mqtt_Connect Success Status :: " + String(_MqttObj.state()));
        if (_IOTCloudUse)
        { 
          // تپیک هایی که ابزار به آنها گوش می دهد
          _MqttObj.subscribe((_ProjectCode + "/DeviceToDevice").c_str()); // تاپیک ارسال داده بین ابزارها که برای اجرای سناریو و یا تاچ پنل ها استفاده می شود و سرور بروکر هیچگونه پردازی روی این متد انجام نمی دهد
          _MqttObj.subscribe((_ProjectCode + "/ServerToDevice").c_str()); // تاپیک ارسال داده به سرور بروکتر
          _MqttObj.subscribe((_ProjectCode + "/DeviceSetting").c_str());  // تاپیک اصلی ارسال تنظیمات اختصاصی به ابزار
          _MqttObj.subscribe((_ProjectCode + "/Share").c_str()); // تاپیک اشتراکی بین همه که ممکن است شما متناسب با نیاز خود از آن استفاده نمایید
          _MqttObj.subscribe("Time/Tehran"); // متد گوش دادن به ساعت
        }
        _MqttCon_IsConnected = true; // متغییر سراسری باری آنکه متوجه اتصال به سرور بروکر در نقاط مختلف کد نویسی شویم
        if (_IOT_ModemTimeout > 0)  _IOT_ModemTimeout = TIMER_JOB_DONE; // تایم _IOT_ModemTimeOut برای بررسی مدام اتصال بر اساس زمان های  مختلف است
        Dvc__CloudSendAllStatus(); // ارسال آخرین وضعیت تمام operationName
                                  //  های ابزار به سرور ما اینجا صرفا 2 عدد operationName داریم که 2 عدد رله است
        Tools__LED_Warning(_Need_Extender ? WARNINGLED_CONNECT_IOTEXTENDER : WARNINGLED_CONNECT_IOT);
      }
      //........................................................... Can't Reconnect
      else
      {
        _MqttCon_IsConnected = false; // تغییر متغییر سراسری 
        if (_IOT_ModemTimeout > 0) _IOT_ModemTimeout = TIMER_JOB_DONE;
        Tools__LED_Warning(WARNINGLED_AUTO);
#if HARDWARE_GSM
        if (_Dvc_GSM.Type != T_GSM)
        {
          DEBUG_SERIAL_PRINTLN("Mqtt_Connect Fail Status :: " + String(_MqttObj.state()));
        }
#elif SOFTWARE_DEBUGMODE
        DEBUG_SERIAL_PRINTLN("Mqtt_Connect Fail Status :: " + String(_MqttObj.state()));
#endif
        _IOT_MqttTimeout = MIN_5; //  5 Min (300sec) Delay If Retry Failed (To Prevent Hanging Manual Touch)
      }
      delay(50);
    }
    //......................................... Mqtt Has Been Connected (Without Trying)
    else
    {
      // DEBUG_SERIAL_PRINTLN("Mqtt_Connect : OK");
    }
  }
  catch (...)
  {
    DEBUG_SERIAL_PRINTLN("Mqtt_Connect Error");
  }
  _Mqtt_TryConnecting = false;
}



// متد اصلی دریافت داده از تاپیک های مختلف
void Mqtt__OnRecieve(char *topic, uint8_t *payload, unsigned int length)
{

  // topic : مشخص می نماید از کدام تاپیک این پیام دریافت شده است
  // ما توسط این مقدار و شرطهایی  که بر روی آن می گذاریم عملیات های خود را دسته بندی می نماییم.
  // در ادامه راه بیشتر با این روند آشنا خواهید شد

  // payload : پیام اصلی می باشد
  try
  {
    // چون فرآیند OnRecive
    // می تواند سنگین باشد و بیشتر از زمان keepalive
    // زمان ببرد یک بار آن را صدا میزنیم
    // درون همین متد کنترل کرده ایم که اگر زیر 10 ثانیه می باشد پس نیاز به ارسال پینگ به سرور نمی باشد
    // اگر زیر زمان تعیین شده در keepalive 
    // پینگ مورد نیاز به سمت سرور ارسال نشود سرور این اتصال را از دست رفته خواهد دید و اتصال شما را قطع خواهد کرد
    IOT__Call__Loop__MQTT();
#if HARDWARE_GSM
    if (_Dvc_GSM.Type != T_GSM) DEBUG_SERIAL_PRINTLN("\r\nMqtt Rcv...\r\n    length : " + String(length) + "\r\n    Project/Topic: " + topic + (_IOTCloudUse ? "\r\n    _IOTCloudUse True" : "\r\n    _IOTCloudUse False"));
#elif SOFTWARE_DEBUGMODE
    DEBUG_SERIAL_PRINTLN("\r\nMqtt Rcv...\r\n    length : " + String(length) + "\r\n    Project/Topic: " + topic + (_IOTCloudUse ? "\r\n    _IOTCloudUse True" : "\r\n    _IOTCloudUse False"));
#endif
//----------------------------------------------------------------------------- Cloud MqttBroker
//.................................... Check Project
    //
    String projectTopic = CA_ToString(topic); // تبدیل کارکتر تاپیک به رشته و آماده کردن جهت شروع بررسی
    int p = projectTopic.indexOf('/');        //  پیدا کردن مکان / 
                                              // در تاپیک زیر تاپیک بصورت روبر است : projectCode/ServerToDevice
                                              // با این روش کد پروژه و مسیر دوم استخراج می نماییم
    // اولین مرحله بسیار مهم وامنیتی این است که شما مطین شوید این پیام برای پروژه شما است
    // ***************** نکته *********************************
    // در سمت سرور چک ها و روت بندی ها به خوبی رعایت شده است و احتمال ارسال یک پیام از یک گروژه دیگر به پروژه شما تقریبا برابر 0 است
    // اما احتیاط شرط است
    if (projectTopic.substring(0, p) != _ProjectCode) 
      return; // اگر پروژه کد دریافتی از تاپیک با پروژه کدی که هنگام اضافه کردن ابزار به یک مکان برایش ارسال می شود و ذخیره می کند یکی نباشد بر می گردد و عملیات انجام نمی شود
    projectTopic = projectTopic.substring(p + 1); // پیدا کردن آدرس دوم بعد از / که کد پروژه بود اول
    if (_IOTCloudUse) // در توسعه های بعدی این مورد نیاز است چون قابلیت استفاده از گیت وی نیز اضافه می شود
    {
      // شروع فرآیند شکستن JSON
      // زیرا داده های خود را بصورت JSON
      // ارسال می کند
      JsonDocument doc;
      doc.clear();
      deserializeJson(doc, payload);
      //.................................... Check Topic
      // دقیقا از این مکان است که شروع به چک کردن تاپیک می نماییم و متناسب با هر تاپیک ادامه آنالیز خود را انجام می دهیم
      if (projectTopic == "DeviceSetting") // تاپیکی که تنظیمات اختصاصی و سناریو داخلی به آن ارسال می شود
      {
        // 
        // payload تاپیک DeviceSetting 
        // دارای مقادیر زیر باید باشد که از سمت سرور ارسال می شود.
        String opr = doc["operationName"].as<String>();

        // بخش سناریو داخلی در حال توسعه و تمیل است        
        if (opr == "save_scenario" || opr == "delete_scenario")
        {
          Config__ReceiveJson_ScenarioEdit(doc, (opr == "delete_scenario"));
        }
        else if (opr == "save_setting") // اگر operationName == save_setting
                                        // بود یعنی در حال تنظیم تنظیمات اختصاصی شما می باشد
        {
          if (doc["deviceSerial"].as<String>() == _SerialCloud) // اگر سریال ابزار با سریال ابزاری که تنظیمات برای آن می باشد یکی بود
          {
            String cmd, d = doc["value"].as<String>();
            DEBUG_SERIAL_PRINTLN("json value :: " + d);
            if (d != "")
            {
              deserializeJson(doc, d);
              if (doc["request"]["commandName"] && doc["request"]["commandData"])
              {
                cmd = doc["request"]["commandName"].as<String>();
                if (cmd == "saveScenarioOperation")
                {
                  JsonVariant inp = doc["scenarioOperation"].as<JsonVariant>();
                  Config__ReceiveJson_ScenarioOperation(inp, true, false);
                  Tools__CloudJSON_Reload("scenarioOperation");
                }
                else if (cmd == "saveCommandOperation")
                {
                  JsonVariant inp = doc["commandOperation"].as<JsonVariant>();
                  Config__ReceiveJson_CommandOperation(inp, true, false);
                  Tools__CloudJSON_Reload("commandOperation");
                }
                else
                {
                  d = doc["request"]["commandData"].as<String>();
                  Config__ReceiveJson_Commands(cmd, d);
                }
              }
            }
          }
        }
      }
      else if (projectTopic == "ServerToDevice") // تاپیک ارسال دستورات به ابزار از طریق کاربر که در این تاپیک دریافت می کند
      {
        // از آنجایی که وقتی سرور برای یک پروژه دستوری ارسال می کند، همه ابزارها آن را دریافت می کنند.
        // نیاز است سریال ابزار را با سریال دستوری که ارسال شده است بررسی شوند
        String deviceSerial = doc["deviceSerial"].as<String>(); // استخراج سریال از دستور باری آنکه بدانیم آیا این دستور ارسالی برای این ابزار است یا خیر
        String cmd = "";
        try
        {
          if (doc["operationName"]) cmd = doc["operationName"].as<String>(); // پرکردن operationName
        }
        catch (...) {}
        if (cmd != "")
        {
          if (cmd == "firmware_update"){ // اگر فریمور آپدیت بود مسیر زیر ادامه گیدا می کند
            String value = doc["value"].as<String>();
            if(value!=""){
              doc.clear();
              deserializeJson(doc, value);
              IOT__FirmwareUpdate(doc["link"].as<String>(), doc["cert"].as<String>(), doc["utc"].as<String>());
            }
          }
          else if (cmd == "firmware_version"){ // اگر سرور ورژن فریمور پرسیده باشد مسیر زیر دنبال می شود
              Mqtt__Send("FirmwareVersion", "{'deviceSerial':'" + _SerialCloud + "','operationName':'firmware_version','value':'" + String(SOFTWARE_VERSION) + "'}", false);
          }
          else if (cmd == "disable_scenario" || cmd == "enable_scenario" || cmd == "run_scenario") // اگر سرور درخوات فعال یا غیر فعال سازی یا اجرای یک سناریو داخلی داده باشد مسیر زیر دنبال می شود
          {
            String nameCode = doc["value"].as<String>() + "_"; // To Find Scenario By Name-StartWith
            for (int i = MAX_SCENARIO - 1; i >= 0; i--)
            {
              String sceneCode = CA_ToString(_Scenario_List[i].Name);
              // DEBUG_SERIAL_PRINTLN(String(i) + " : " + sceneCode);
              if (sceneCode.indexOf(nameCode) == 0)
              { // Valid Scenario
                Dvc__Senario_DoAction(i, cmd == "disable_scenario" ? 0x00 : cmd == "enable_scenario" ? 0x01
                                                                                                     : 0x02);
              }
            }
          }
          else if (cmd == "debugger") // اگر سرور دستور دیباگر اراسل کرده باشد که بیشتر در بخش پشتیبانی از راه دور مورد نیاز است مسیر زیر دنبال می شود
          {
            if (doc["value"])
            {
              String value = doc["value"].as<String>();
              if (value == "reboot")
                Tools__Reboot("Mqtt__OnRecieve reboot");
              else if (value == "clean")
              {
                MemoClean(EP_MEMORYSTART2);
                Tools__Reboot("Mqtt__OnRecieve clean");
              }
              else if (value == "reset_factory")
              {
                MemoClean(EP_MEMORYSTART1);
                Tools__Reboot("Mqtt__OnRecieve reset_factory");
              }
              else if (value == "config")
              {
                Tools__LED_WarningBlink(10, WARNINGLED_CONFIG, WARNINGLED_CONFIG, 100);
                Tools__SetMode("config_panel", true);
              }
              else if (value == "special")
                ;
            }
          }
        }
        else if (doc["data"]) // این بخش اصلی جهت کنترل یک ابزار است که مسیر زیر دنبال می شود
        {
          cmd = doc["data"].as<String>(); // دستور شما که در فایل js
                                          // { type: 'command', value: outPut };
                                      
          if (cmd != "")
          {
            doc.clear();
            deserializeJson(doc, cmd);
            String type = doc["type"].as<String>();   // مثلا می شود : command
            String value = doc["value"].as<String>(); // مثلا می شود : ch1on
            //.................................... Extarct Data
            if (type == "command") // اگر Command باشد یعنی دستور مستقیم برای شما است
            {
              if (deviceSerial == _SerialCloud) // اگر سریال برابر با سریال همین ابزار باشد
              {
                // DEBUG_SERIAL_PRINTLN("command by Own-Serial > Dvc__AnalyzeData");
                Dvc__AnalyzeData(value); // Cloud Command Check 
                delay(70);
              }
            }
          }
        }
      }
      else if (projectTopic == "DeviceToDevice") // اگر داده ارسالی بین ابزاری بود دقت نمایید هر داده دریافتی از سمت ابزار توسط سرور در این تاپیک منتشر می شود
                                                // دلیل اینکار برای امنکان کنترل ابزار شرکت های مختلف توسط هم می باشد که در آینده در هنگام تنظیمات تخصصی و سناریو داخلی با آن روبرو می شویم
      {
        //.................................... Extarct Data
        String type = doc["type"].as<String>();
        String data = doc["data"].as<String>();
        Dvc__AnalyzeData(data);
      }
      doc.clear();
    }
  }
  catch (...)
  {
  }
  Tools__LED_Warning(WARNINGLED_AUTO, 30);
  //_MqttRcv_IsWorking = false;
}



// متد اصلی ارسال داده به سرور می باشد که همین داده به فایل js
// جهت تجزیه و تحیلیل تحویل خواهیم داد
void Mqtt__Send(String scope, String type, String data, String sender, bool offlineSupport = true)
{
  try
  {
    Tools__LED_Warning(WARNINGLED_AUTO);
    //................................. Reject If Mesh Not Connected
    if (data == "") return;
    if(data[0]=='{'){
      if( data.indexOf('"')<0 && data.indexOf('\'')>=0 ) data.replace('\'', '"');
    }
    //----------------------------------------------------------------------------- Cloud SMS
    if (!_MqttCon_IsConnected || !_MqttObj.connected() || _MqttObj.state() < 0)
    {
#if HARDWARE_GSM
      if (_IOTCloudUse && offlineSupport)
      {
        if (_Dvc_GSM.Type == T_GSM)
        {
          _GSM_AllStatusTimeout = SEC_5; // Sms_QueueAdd(CA_ToString(_Dvc_GSM.PhoneOffline), "CC:" + data, false);
        }
        else {
          DEBUG_SERIAL_PRINTLN("SMS Send...   >  CC:" + data);
        }
      }
#endif
    }
    //----------------------------------------------------------------------------- Cloud MqttBroker
    else
    {

      if (_IOTCloudUse)
      {
        String topic = _ProjectCode + "/" + scope;
        if (type == "cloud")
        {
          data = "{\"data\":\"" + data + "\"}";
        }
        else
        {
          ;
        }
        DEBUG_SERIAL_PRINTLN("Mqtt Send... " + topic + "  >  " + data);
        if (_MqttObj.connected())
          _MqttObj.publish(topic.c_str(), data.c_str());
      }
    }
  }
  catch (...)
  {
    DEBUG_SERIAL_PRINTLN("MqttSend Error ");
  }
}




void Mqtt__Send(String topic, String data, bool offlineSupport = true)
{
  try
  {
    Tools__LED_Warning(WARNINGLED_AUTO);
    //................................. Reject If data is empty
    if (data == "")  return;
    if(data[0]=='{'){
      if( data.indexOf('"')<0 && data.indexOf('\'')>=0 ) data.replace('\'', '"');
    }
    topic = _ProjectCode + "/" + topic;
    //----------------------------------------------------------------------------- Cloud SMS
    if (!_MqttCon_IsConnected || !_MqttObj.connected() || _MqttObj.state() < 0)
    {
#if HARDWARE_GSM
      if (_IOTCloudUse && offlineSupport)
      {
        if (_Dvc_GSM.Type == T_GSM)
        {
          _GSM_AllStatusTimeout = SEC_5;
        }
        else {
          DEBUG_SERIAL_PRINTLN("SMS Send...   >  CC:" + data);
        }
      }
#endif
    }
    //----------------------------------------------------------------------------- Cloud MqttBroker
    else
    {

#if HARDWARE_GSM
      if (_Dvc_GSM.Type != T_GSM)
        DEBUG_SERIAL_PRINTLN("Mqtt Send... " + topic + "  >  " + data);
#elif SOFTWARE_DEBUGMODE
      DEBUG_SERIAL_PRINTLN("Mqtt Send... " + topic + "  >  " + data);
#endif
      if (_MqttObj.connected())
        _MqttObj.publish(topic.c_str(), data.c_str());
    }
  }
  catch (...)
  {
    DEBUG_SERIAL_PRINTLN("MqttSend Error ");
  }
}




void IOT__WiFiStart()
{
  _WiFi_ConnectWorking = true;
  Tools__LED_Warning(WARNINGLED_DIS_WIFI, 200);
  try
  {
#if HARDWARE_GSM
    if (_Dvc_GSM.Type != T_GSM)
      DEBUG_SERIAL_PRINTLN("Connecting to Wi-Fi...");
#elif SOFTWARE_DEBUGMODE
    DEBUG_SERIAL_PRINTLN("Connecting to Wi-Fi...");
#endif
    Tools__WifiPower(true);
    const char *ssid = _ModemSSID.c_str();
    const char *password = _ModemPass.c_str();
    WiFi.begin(ssid, password);
  }
  catch (...)
  {
  }
  Dvc__SetPinout(false);
  delay(100);
  Tools__LED_Warning(WARNINGLED_AUTO);
  _WiFi_ConnectWorking = false;
}




void IOT__WiFiEvent(WiFiEvent_t event)
{
  //.............................................................. Prevent Any Action During Network_Reset()
  if (_MqttCon_Steps == 99)
    return;
    //..............................................................
#if HARDWARE_GSM
  if (_Dvc_GSM.Type != T_GSM)
    DEBUG_SERIAL_PRINTLN("[WiFi-event] event: " + String(event));
#elif SOFTWARE_DEBUGMODE
  DEBUG_SERIAL_PRINTLN("[WiFi-event] event: " + String(event));
#endif
  switch (event)
  {
  case SYSTEM_EVENT_STA_GOT_IP:
#if HARDWARE_GSM
    if (_Dvc_GSM.Type != T_GSM)
      DEBUG_SERIAL_PRINTLN("WiFi connected, IP: " + WiFi.localIP().toString());
#elif SOFTWARE_DEBUGMODE
    DEBUG_SERIAL_PRINTLN("WiFi connected, IP: " + WiFi.localIP().toString());
#endif
    _WiFi_IsConnected = true;
    _IOT_MqttTimeout = TIMER_NEED_RUN;
    _MqttCon_Steps = 2;
    break;

  case SYSTEM_EVENT_STA_DISCONNECTED:
    _WiFi_IsConnected = false;
    _MqttCon_IsConnected = false;
    _MqttCon_Steps = 0;
#if HARDWARE_GSM
    if (_Dvc_GSM.Type != T_GSM)
      DEBUG_SERIAL_PRINTLN("WiFi lost connection");
#elif SOFTWARE_DEBUGMODE
    DEBUG_SERIAL_PRINTLN("WiFi lost connection");
#endif
    break;
  }
  delay(100);
  Tools__LED_Warning(WARNINGLED_AUTO);
}




void IOT__Checker(void *param)
{
  while (true)
  {
    delay(1000);
    try
    {
      //.............................................................. Prevent Any Action During Network_Reset()
      while (_MqttCon_Steps == 99)
        delay(1000);
      //..............................................................
      if (_MqttCon_Steps == 0)
      {
        if (_IOT_ModemTimeout == TIMER_JOB_DONE) _IOT_ModemTimeout = MIN_3;
        IOT__WiFiStart();
#if HARDWARE_GSM
        if (_Dvc_GSM.Type != T_GSM)
          DEBUG_SERIAL_PRINTLN("############ _MqttCon_Steps 0 :: WiFiStart()");
#elif SOFTWARE_DEBUGMODE
        DEBUG_SERIAL_PRINTLN("############ _MqttCon_Steps 0 :: WiFiStart()");
#endif
        if (_MqttCon_Steps == 0)
          _MqttCon_Steps = 1;
      }
      else if (_MqttCon_Steps == 2)
      {
        const char *mqtt_broker = _MqttBroker.c_str();
        const int mqtt_port = 1883; // 5000
        _MqttObj.setCallback(Mqtt__OnRecieve);
        _MqttObj.setServer(mqtt_broker, mqtt_port);
        _MqttObj.setBufferSize(11000);
        _MqttObj.setSocketTimeout(15);
        _MqttObj.setKeepAlive(15);
        _MqttCon_Steps = 3;
#if HARDWARE_GSM
        if (_Dvc_GSM.Type != T_GSM)
          DEBUG_SERIAL_PRINTLN("############ _MqttCon_Steps 1 :: MqttObj.setServer()");
#elif SOFTWARE_DEBUGMODE
        DEBUG_SERIAL_PRINTLN("############ _MqttCon_Steps 1 :: MqttObj.setServer()");
#endif
        //......................................
      }
      else if (_MqttCon_Steps == 3)
      {
        delay(1000);
        Mqtt__Connect();
      }
      else
      {
        DEBUG_SERIAL_PRINTLN("############ _MqttCon_Steps " + String(_MqttCon_Steps) + " :: No Action");
      }
    }
    catch (...)
    {
    }
  }
}




void IOT__Loop()
{
  while (true)
  {
    try
    {
      if (_MqttCon_IsConnected)
      {
        _IOT_PingTimeout = MIN_15;
        _MqttObj.loop(); // اجرای mqtt.loop
        delay(30);
      }
      else
      {
        if (_IOT_PingTimeout == TIMER_NEED_RUN)
        {
          Network_Reset();
        }
        delay(30);
      }
      if (_IOT_ModemTimeout == TIMER_NEED_RUN)
      {
        _IOT_ModemTimeout = TIMER_EXPIRED_FOREVER;
#if HARDWARE_GSM
        // Tell User Of Project That Modem is Out Of Reach
        if (_IOTCloudUse)
        {
          MessageCloud__ViaGSM_SMSTo_User(MESSAGE_CLOUD_MODEM_OUT);
        }
        else
        {
          GSM__SmsToMgr("This Device Cant Connect To The Modem, Please Check It.");
        }
#endif
        if (_Reboot) Send_RebootAndStatus();
      }
      else if (_Reboot)
      {
        if (_IOT_ModemTimeout == TIMER_JOB_DONE) Send_RebootAndStatus();
      }
      // DEBUG_CALL(Debug_ConsolePrint("."));
    }
    catch (...)
    {
      DEBUG_SERIAL_PRINTLN("IOT__Loop Error ");
    }
#if HARDWARE_GSM
    if (_Dvc_GSM.Type == T_GSM)
      GSM__SmsRead();
#endif
    delay(TASK_LOOP_DELAY);
  }
}




void IOT__Setup(void *param)
{
  Tools__LED_Warning(WARNINGLED_DIS_IOT);
  DEBUG_SERIAL_PRINTLN(".... IOT Mqtt Setup ....");
  //....................................... Reject if Wifi-SSID empty
  // if (_Need_ConfigIsBad) {
  // 	Tools__LED_Warning(WARNINGLED_CONFIG_NEED, 1000);
  // 	// Tools__SetMode("normal", true);
  // 	return;
  // }
  //........................................... WiFi Settings
  _WifiClient.setTimeout(15);
  _WiFi_ConnectWorking = true;
  if (WiFi.isConnected())
  {
    WiFi.disconnect(true);
    delay(100);
  }
  _WiFi_IsConnected = false;
  WiFi.onEvent(IOT__WiFiEvent);
  //........................................... Local-Broker or Cloud-Broker
  _IOTCloudUse = true;
  if (_MqttBroker.length() > 8)
  {
    if (_MqttBroker.substring(0, 8) == "192.168.")
      _IOTCloudUse = false;
  }
  else
    _IOTCloudUse = false;
  //............................................ Mqtt Settings
  if (_IOTCloudUse)
    _MqttClientId = _CloudClientId;
  else
    _MqttClientId = (_ProjectCode + "d" + _SerialNo + "0000" + String(Tools__Random(100000, 999999)) + "xxxxxxxxxxxxxxxxxxxxxxx").substring(0, 23);
  _MqttCon_IsConnected = false;
  _MqttCon_Steps = 0;
  //........................................... IOT Checker
  xTaskCreatePinnedToCore(
      IOT__Checker,         // Function that should be called
      "IOT__Checker",       // Name of the task (for debugging)
      TASK_STACKSIZE_LARGE, // Stack size (bytes)
      NULL,                 // Parameter to pass
      TASK_PRIORITY_HIGH,   // Task priority (Max:1)
      NULL,                 // Task handle
      TASK_CORE_1);         // Core (0/1)

  DEBUG_SERIAL_PRINTLN(".... IOT Setup OK ...");
  //........................................... Stay In Loop
  Tools__LED_Warning(WARNINGLED_AUTO);
  IOT__Loop();
}




void IOT__Call__Loop__MQTT()
{
  if (_MqttCon_IsConnected)
  {
    unsigned long currentMillis = millis();
    // بررسی فاصله زمانی 10 ثانیه
    if (currentMillis - _TimerMqttLoopPrev >= _MttLoopInterval)
    {
      _TimerMqttLoopPrev = currentMillis; // به‌روزرسانی زمان آخرین اجرا
      _MqttObj.loop();                    // اجرای mqtt.loop
    }
    delay(30);
  }
}



//===========================================================================================
//                                          End
//===========================================================================================
#endif