#ifndef _FILE_H_MESSAGE_
#define _FILE_H_MESSAGE_
//===========================================================================================
//                                         Cloud Message
//===========================================================================================

//..................................................................... Mqtt Send Notfication To User
void MessageCloud__ViaMqtt_NotifyTo_User(int messageCode)
{
  Mqtt__Send("SendNotificationToUser", "{\"code\":\"" + String(messageCode) + "\",\"type\":\"30\"}", true); // Reboot Popup-Notify To User of Project
}
void MessageCloud__ViaMqtt_NotifyTo_User(String& messageCode)
{
  Mqtt__Send("SendNotificationToUser", "{\"code\":\"" + messageCode + "\",\"type\":\"30\"}", true); // Reboot Popup-Notify To User of Project
}




//..................................................................... Mqtt Send SMS To User
void MessageCloud__ViaMqtt_SMSTo_User(int messageCode)
{
  Mqtt__Send("SendNotificationToUser", "{\"code\":\"" +  String(messageCode) + "\",\"type\":\"20\"}", true); // Reboot Popup-Notify To User of Project
}
void MessageCloud__ViaMqtt_SMSTo_User(String& messageCode)
{
  Mqtt__Send("SendNotificationToUser", "{\"code\":\"" +  messageCode + "\",\"type\":\"20\"}", true); // Reboot Popup-Notify To User of Project
}



//..................................................................... Mqtt Send SMS To Center
void MessageCloud__ViaMqtt_SMSTo_Center(int messageCode)
{
  Mqtt__Send("SendNotificationToCenter", "{\"code\":\"" +  String(messageCode) + "\",\"type\":\"20\"}", true); // Reboot Popup-Notify To User of Project
}
void MessageCloud__ViaMqtt_SMSTo_Center(String& messageCode)
{
  Mqtt__Send("SendNotificationToCenter", "{\"code\":\"" +  messageCode + "\",\"type\":\"20\"}", true); // Reboot Popup-Notify To User of Project
}



#if HARDWARE_GSM
//..................................................................... GSM Send Notfication To User
void MessageCloud__ViaGSM_NotifyTo_User(int messageCode)
{
  Sms_QueueAdd(CA_ToString(_Dvc_GSM.PhoneOffline), "NU:" +  String(messageCode), false);
}
void MessageCloud__ViaGSM_NotifyTo_User(String& messageCode)
{
  Sms_QueueAdd(CA_ToString(_Dvc_GSM.PhoneOffline), "NU:" +  messageCode, false);
}

//..................................................................... GSM Send SMS To User
void MessageCloud__ViaGSM_SMSTo_User(int messageCode)
{
  Sms_QueueAdd(CA_ToString(_Dvc_GSM.PhoneOffline), "MU:" +  String(messageCode), false);
}
void MessageCloud__ViaGSM_SMSTo_User(String& messageCode)
{
  Sms_QueueAdd(CA_ToString(_Dvc_GSM.PhoneOffline), "MU:" +  messageCode, false);
}


//..................................................................... GSM Send SMS To Center
void MessageCloud__ViaGSM_SMSTo_Center(int messageCode)
{
  Sms_QueueAdd(CA_ToString(_Dvc_GSM.PhoneOffline), "MC:" +  String(messageCode), false);
}
void MessageCloud__ViaGSM_SMSTo_Center(String& messageCode)
{
  Sms_QueueAdd(CA_ToString(_Dvc_GSM.PhoneOffline), "MC:" +  messageCode, false);
}
#endif





//===========================================================================================
//                                          End
//===========================================================================================
#endif