
//===========================================================================================
//                                             FIX VARIBLE
//===========================================================================================

const String DEVICE_TYPEHEX = Tools__StringToHexStr(DEVICE_TYPECODE);
byte _Dvc_Phase = 0;

//byte _Waiter = 0;
void SenarioCommandRun_Key(byte SenId, byte Pol, byte Cmd, int Delay);

void RunsenCheck_CustomCode(String Code);
void RunsenCheck_Virtual(byte Status);
void RunsenCheck_Linked(String Data);
void RunsenCheck_CloudAction(byte Pol, String Type);



//===========================================================================================
//  متغییر وضعیت رله ها که همیشه در آن وضعیت را بصورت سراسری نگه می داریم که از مزیت های آن کاهش اشتباهات در هنگام ارسال داده می باشد
//===========================================================================================
bool DvcRelay[MAX_RELAY] = { 0, 0 };

//===========================================================================================
//                                پایه های رله
//                         نوع رله 4 پایه نرمال اوپن است 
//===========================================================================================
const byte Relay[MAX_RELAY] = { 16, 17 };

//===========================================================================================
//                                    ENABLE AP MODE
//  پایه کلید برای بردن ابزار به حالت تنظیمات است که باید بعد از گرفتن آن به مدت شش ثانیه به تنظیمات می رود
//===========================================================================================
const byte Key = 23;

//===========================================================================================
//       متغییر سراسری برای لاین رله است گه در کدها نحوهاستفاده آن را خواهید دید 
//===========================================================================================
byte Line = 0;


bool _Dvc_IsEnableSetAddress = false;

//===========================================================================================
//                                          DVC Controls
//      یکی از نکات مهم این است پس از هر بار  اتصال به بروکر اصلی وضعیت خود را ارسال کنید
//      توسط این متد ما وضعیت خود را در هنگام اتصال ارسال می  نماییم
//===========================================================================================

void Dvc__CloudSendAllStatus() {
  try {
    DEBUG_SERIAL_PRINTLN("Dvc__CloudSendAllStatus");
    //......................................................................
    for (byte i = 0; i < MAX_RELAY; i++) {
      if (!_MqttObj.connected() || _MqttObj.state() < 0) break;
      if (_Dvc_Relay[i].Type == 0xFF) continue;

      if(i==0&&_Dvc_Relay[i].Status==1)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch1on" , "", true);
      }
      else if(i==0&&_Dvc_Relay[i].Status==0)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch1off" , "", true);
      }else if(i==1&&_Dvc_Relay[i].Status==1)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch2on" , "", true);
      }
      else if(i==2&&_Dvc_Relay[i].Status==0)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch2off" , "", true);
      }

      delay(100);
      switch (_Dvc_Relay[i].Type) {
        case T_CURTAIN:
          i += 1;
          break;
        case T_FANCOIL_SIMPLE:
        case T_COOLER:
        case T_THERMOSTAT_0PIPE:
          i += 2;
          break;
        case T_THERMOSTAT_2PIPE_1VALVE:
          i += 3;
          break;
        case T_THERMOSTAT_4PIPE_2VALVE:
          i += 4;
          break;
      }
    }
  } catch (...) {}
}

//===========================================================================================
//      این متد برای راحتی استفاده طراحی شده تا داده های دریافتی را بتوان در یک نقطه بررسی نماییم
//===========================================================================================
void Dvc__AnalyzeData(String Data) {
  if (Data == "") return;
                  if (Data == "ch1on") {
                      DvcRelay[0] = 1;
                      digitalWrite(Relay[0], HIGH);
                    RunsenCheck_CloudAction(1, "On");
                  } else if (Data == "ch1off") {
                    DvcRelay[0] == 0;
                      DvcRelay[0] = 0;
                      digitalWrite(Relay[0], LOW);
                    RunsenCheck_CloudAction(1, "Off");
                  } else if (Data == "ch2on") {
                      DvcRelay[1] = 1;
                      digitalWrite(Relay[1], HIGH);
                    RunsenCheck_CloudAction(1, "On");
                  } else if (Data == "ch2off") {
                      DvcRelay[1] = 0;
                      digitalWrite(Relay[1], LOW);
                    RunsenCheck_CloudAction(1, "Off");
                  }
        Mqtt__Send("DeviceToServer", "cloud", Data, "", true);
 }


//===========================================================================================
//      این متد برای راحتی استفاده طراحی شده تا داده های دریافتی را بتوان در یک نقطه بررسی نماییم
//      اگر دقت نمایید قبل یک آرایه جهت نگهداری وصعیت رله ها داشتیم که به ما کمک می کرد تا راحتتر کنترل نماییم.
//===========================================================================================
void Dvc__RelayControl() {
  for (byte j = 0; j < MAX_RELAY; j++) {
    digitalWrite(Relay[j], _Dvc_Relay[j].Status);
  }  
}


void Dvc__LastStatusSave() {
  // SOON
}



void Dvc__LastStatusLoad() {
  // SOON
}



//===========================================================================================
//                         کنترل ال ای دی WS2812
//===========================================================================================
void Dvc__WarningLEDSetColor(byte R, byte G, byte B) {
  pixels.setPixelColor(0, pixels.Color(R, G, B));
  pixels.show();
}

//===========================================================================================
//  وقتی پین کانفیگ نگه می داریم (با منفی فعال می شود) به این مکان می آید و وارد حالت تنظیمات می شود
//===========================================================================================
void Manual_SwichToConfig() {
  byte Counter = 0;
  while (Counter < 20) {
    delay(50);
    Counter++;
    if (Counter >= 20) {
      Tools__LED_Warning(WARNINGLED_OFF, 300);
      Tools__LED_WarningBlink(10, WARNINGLED_CONFIG, WARNINGLED_CONFIG, 100);
      Tools__SetMode(_StartMode == "config_panel" ? "normal" : "config_panel", true);
      break;
    }
  }
}


#if HARDWARE_SENKEY
void RunsenCheck_Key(byte Pol, byte Status) {  // Check To Do Scenario, For Senkey & Key
  TimerSec_Refresh();
  for (byte i = 0; i < MAX_RUNSENKEY; i++) {
    if (_Runsen_Senkey[i].Dvc == 0xFF) break;
    if (_Runsen_Senkey[i].Dvc == Pol && _Runsen_Senkey[i].Con == Status) Dvc__Senario_DoAction(_Runsen_Senkey[i].Sen, _Runsen_Senkey[i].Act);
  }
}
#endif


void RunsenCheck_Virtual(byte Status) {
  TimerSec_Refresh();
  for (byte i = 0; i < MAX_RUNSENVIRTUAL; i++) {
    if (_Runsen_Virtual[i].Dvc == 0xFF) break;
    if (_Runsen_Virtual[i].Con == Status) Dvc__Senario_DoAction(_Runsen_Virtual[i].Sen, _Runsen_Virtual[i].Act);
  }
}



void RunsenCheck_CustomCode(String Code) {
  TimerSec_Refresh();
  for (byte i = 0; i < MAX_RUNSENCODE; i++) {
    if (_Runsen_Code[i].Dvc == 0xFF) break;
    if (CA_ToString(_Runsen_Code[i].Con, 0, 20) == Code) Dvc__Senario_DoAction(_Runsen_Code[i].Sen, _Runsen_Code[i].Act /*, false*/);
  }
}


void RunsenCheck_CloudAction(byte Pol, String Type) {
  TimerSec_Refresh();
  String jsn;
  jsn.reserve(500);
  jsn = "";
  bool isSent = false;
  int ch = -1;
  if (Pol >= 1 && Pol <= MAX_RELAY) ch = Pol - 1;
  if (ch >= 0 && ch < MAX_RELAY) {
    //...................... If Status is ON/OFF (for command)
    if (Type == "Open" || Type == "On") {
      if (_Dvc_Relay[ch].CO_On.isValid) jsn = "{'deviceSerial':'" + CA_ToString(_Dvc_Relay[ch].CO_On.deviceSerial) + "','operationName':'" + CA_ToString(_Dvc_Relay[ch].CO_On.operationName) + "','value':'" + CA_ToString(_Dvc_Relay[ch].CO_On.value) + "'}";
    } else if (Type == "Close" || Type == "Off") {
      if (_Dvc_Relay[ch].CO_Off.isValid) jsn = "{'deviceSerial':'" + CA_ToString(_Dvc_Relay[ch].CO_Off.deviceSerial) + "','operationName':'" + CA_ToString(_Dvc_Relay[ch].CO_Off.operationName) + "','value':'" + CA_ToString(_Dvc_Relay[ch].CO_Off.value) + "'}";
    } else if (Type == "Stop") {
      if (_Dvc_Relay[ch].CO_Stop.isValid) jsn = "{'deviceSerial':'" + CA_ToString(_Dvc_Relay[ch].CO_Stop.deviceSerial) + "','operationName':'" + CA_ToString(_Dvc_Relay[ch].CO_Stop.operationName) + "','value':'" + CA_ToString(_Dvc_Relay[ch].CO_Stop.value) + "'}";
    }
    if (jsn != "") {
      Mqtt__Send("AppToServer", jsn, false);
      isSent = true;
    }
    //...................... If Status is ON/OFF (for scenario)
    jsn = "";
    if (Type == "Open" || Type == "On") {
      if (_Dvc_Relay[ch].SO_On.isValid) jsn = "{'deviceSerial':'general','value':" + String(_Dvc_Relay[ch].SO_On.code) + ",'operationName':'" + String(_Dvc_Relay[ch].SO_On.act == 0 ? "disable_scenario" : _Dvc_Relay[ch].SO_On.act == 1 ? "enable_scenario"
                                                : "run_scenario")
                                                + "'}";
    } else if (Type == "Close" || Type == "Off") {
      if (_Dvc_Relay[ch].SO_Off.isValid) jsn = "{'deviceSerial':'general','value':" + String(_Dvc_Relay[ch].SO_Off.code) + ",'operationName':'" + String(_Dvc_Relay[ch].SO_Off.act == 0 ? "disable_scenario" : _Dvc_Relay[ch].SO_Off.act == 1 ? "enable_scenario"
            : "run_scenario")
            + "'}";
    } else if (Type == "Stop") {
      if (_Dvc_Relay[ch].SO_Stop.isValid) jsn = "{'deviceSerial':'general','value':" + String(_Dvc_Relay[ch].SO_Stop.code) + ",'operationName':'" + String(_Dvc_Relay[ch].SO_Stop.act == 0 ? "disable_scenario" : _Dvc_Relay[ch].SO_Stop.act == 1 ? "enable_scenario"
            : "run_scenario")
            + "'}";
    }
    if (jsn != "") {
      if (isSent) delay(200);
      Mqtt__Send("RunScenario", jsn, false);
    }
  }
}



void Dvc__Senario_DoAction(byte SenId, byte Act) {
  //....................................................................... ***** Reject Invalid *****
  if (SenId >= MAX_SCENARIO) return;
  // DEBUG_SERIAL_PRINTLN ("\r\n\r\nSenId:" + String(SenId) + "  Act:" + String(Act));

  if (_Scenario_List[SenId].Act == 0xFF) return;
  //....................................................................... ***** Enable / Disable *****
  if (Act == 0x00 || Act == 0x01) {
    _Scenario_List[SenId].Act = Act;
    MemoWriteByte(EP_SCENARIO_ACT_S + SenId, Act);  //Tools__ScenarioSave(SenId);
    //Tools__ScenarioSave(SenId);
    if (Act == 0x00) {  // Clear Commands Of Disabled Scenario
      byte r, max;
#if HARDWARE_SENKEY
      max = (SenId * MAX_RELAY);
      for (r = 0; r < max; r++) {
        if (_QueueAction_Opr[r].SenId == SenId) _QueueAction_Opr[r].Valid = false;
      }
#endif
      max = (SenId * MAX_SCENARIO_VIRTUAL);
      for (r = 0; r < max; r++) {
        if (_QueueAction_Virtual[r].SenId == SenId) _QueueAction_Virtual[r].Valid = false;
      }
      max = (SenId * MAX_SCENARIO_CODE);
      for (r = 0; r < max; r++) {
        if (_QueueAction_Code[r].SenId == SenId) _QueueAction_Code[r].Valid = false;
      }
    }
    return;
  }
  //....................................................................... ***** Run a Scenario *****
  //........................................ Check Scenario To Reject Invalid
  if (Act != 0x02) return;
  if (_Scenario_List[SenId].Act != 0x01) return;
  byte z = 0;

  //........................................ Relay Check Commands & Send
  for (z = 0; z < MAX_SCENARIO_OPR; z++) {
    if (_Scenario_List[SenId].RelayCmd[z] != 0xFF) {
      SenarioCommandRun_Key(SenId, _Scenario_List[SenId].RelayNo[z], _Scenario_List[SenId].RelayCmd[z], _Scenario_List[SenId].RelayDelay[z]);
    }
  }

}



void SenarioCommandRun_Key(byte SenId, byte Pol, byte Cmd, int Delay = 0) {
  DEBUG_SERIAL_PRINTLN("SenarioCommandRun_Key  SenId:" + String(SenId) + "  Pol:" + String(Pol) + "  Delay:" + String(Delay));
  if (Pol >= MAX_RELAY) return;
  if (Delay == 0) {

      if(Pol==0&&Cmd==1)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch1on" , "", true);
      }
      else if(Pol==0&&Cmd==0)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch1off" , "", true);
      }else if(Pol==1&&Cmd==1)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch2on" , "", true);
      }
      else if(Pol==2&&Cmd==0)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch2off" , "", true);
      }
  } 
#if HARDWARE_SENKEY
  else {
    _QueueAction_Opr[Pol].DelayLeft = Delay + _TimerForActions;  //+ (isFromSenkey ? 3 : 0);
    _QueueAction_Opr[Pol].Pol = Pol;
    _QueueAction_Opr[Pol].Cmd = Cmd;
    _QueueAction_Opr[Pol].Valid = true;
    _QueueAction_Opr[Pol].SenId = SenId;
  }
#endif
}




void _Dvc_SenarioCommand_TimerCheck() {
  if (_TimerForActions > 0) {
    byte max, r;
    //................................................... Other Devices Timer
    for (r = 0; r < MAX_RELAY; r++) {
      if (_Dvc_Relay[r].HoldTimeout > 0) {
        DEBUG_SERIAL_PRINTLN("\r\n* HoldTimeout:" + String(_Dvc_Relay[r].HoldTimeout) + " - " + String(_TimerForActions) + " = ");
        _Dvc_Relay[r].HoldTimeout -= _TimerForActions;
        DEBUG_SERIAL_PRINTLN(String(_Dvc_Relay[r].HoldTimeout));
        if (_Dvc_Relay[r].HoldTimeout <= 0) {
          _Dvc_Relay[r].Status = 0;
          Dvc__RelayControl();
      if(r==0&&_Dvc_Relay[r].Status==1)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch1on" , "", true);
      }
      else if(r==0&&_Dvc_Relay[r].Status==0)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch1off" , "", true);
      }else if(r==1&&_Dvc_Relay[r].Status==1)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch2on" , "", true);
      }
      else if(r==2&&_Dvc_Relay[r].Status==0)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch2off" , "", true);
      }

        }
      }
    }
    //................................................... reset timer
    _TimerForActions = 0;
  }
}





void Dvc__SetPinout(bool refresh) {
  DEBUG_SERIAL_PRINTLN("Dvc__SetPinout");
  pinMode(Key, INPUT);
  for (byte i = 0; i < MAX_RELAY; i++) {
    pinMode(Relay[i], OUTPUT);
    if (refresh) digitalWrite(Relay[i], LOW);
  }
  if (refresh) {
    pixels.begin();
  }
}



void Manual__Loop(void* param) {
  delay(3000);
  Dvc__SetPinout(false);
  while (true) {
    try {
      TimerSec_Refresh();
      byte Time = 0;
      if (digitalRead(Key) == LOW) {
        while (digitalRead(Key) == LOW && Time < 120) {
          if (Time == 60) {
            Manual_SwichToConfig();
          }
          Time++;
          delay(50);
        }
        {
          DEBUG_SERIAL_PRINTLN("Time= " + String(Time));
          DEBUG_SERIAL_PRINTLN("DvcRelay[0] = " + String(DvcRelay[0]));
          DEBUG_SERIAL_PRINTLN("DvcRelay[1] = " + String(DvcRelay[1]));
        }
        if (Time < 30) {
          byte TC = _Dvc_Relay[0].Type;
          switch (TC) {
            case T_LIGHT:
              if (DvcRelay[0] == 0 || DvcRelay[1] == 0) {
                DvcRelay[0] = 1;
                DvcRelay[1] = 1;
              } else {
                DvcRelay[0] = 0;
                DvcRelay[1] = 0;
              }
              digitalWrite(Relay[0], DvcRelay[0]);
              digitalWrite(Relay[1], DvcRelay[1]);
      if(DvcRelay[0]==1)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch1on" , "", true);
      }
      else if(DvcRelay[0]==0)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch1off" , "", true);
      } 
      if(DvcRelay[1]==1)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch2on" , "", true);
      }
      else if(DvcRelay[1]==1)
      {
        Mqtt__Send("DeviceToServer", "cloud", "ch2off" , "", true);
      }

        
              break;
  
          }
        }
      }

    } catch (...) {
      DEBUG_SERIAL_PRINTLN("Manual__Loop Error ");
    }
    //-------------------------------------------- TIMER
    TimerSec_Refresh();
    //-------------------------------------------- Loop-Delay
    delay(TASK_LOOP_DELAY);
  }
}





void Manual__Setup() {
  DEBUG_SERIAL_PRINTLN(".... Start Manual Setup ....");
  Dvc__SetPinout(true);
  Tools__LED_Warning(WARNINGLED_DATA);
  //  Dvc__RelayControl();
  //........................................... Stay In Loop
  xTaskCreatePinnedToCore(
    Manual__Loop,          // Function that should be called
    "Manual__Loop",        // Name of the task (for debugging)
    TASK_STACKSIZE_LARGE,  // Stack size (bytes)
    NULL,                  // Parameter to pass
    TASK_PRIORITY_HIGH,    // Task priority (Max:1)
    NULL,                  // Task handle
    TASK_CORE_1);          // Core (0/1)

  //........................................... Check Scenario "Start"
  String sName = "";
  for (byte i = 0; i < MAX_SCENARIO; i++) {
    if (_Scenario_List[i].Act == 0x01) {
      sName = CA_ToString(_Scenario_List[i].Name);
      sName.toLowerCase();
      std::replace(sName.begin(), sName.end(), '1', ' ');
      std::replace(sName.begin(), sName.end(), '2', ' ');
      std::replace(sName.begin(), sName.end(), '3', ' ');
      sName.trim();
      if (sName == "start") Dvc__Senario_DoAction(i, 0x02);
    }
  }
  DEBUG_SERIAL_PRINTLN(".... End Manual Setup ....");
  Tools__LED_Warning(WARNINGLED_AUTO);
}
