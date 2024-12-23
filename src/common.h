#ifndef _FILE_H_COMMON_
#define _FILE_H_COMMON_
//===========================================================================================
//                                         Common & Tools
//===========================================================================================

#if SOFTWARE_DEBUGMODE
  #define DEBUG_SERIAL_PRINT(x) Debug_ConsolePrint(x)
#else
  #define DEBUG_SERIAL_PRINT(x)
#endif
#if SOFTWARE_DEBUGMODE
  #define DEBUG_SERIAL_PRINTLN(x) Debug_ConsolePrintln(x)
#else
  #define DEBUG_SERIAL_PRINTLN(x)
#endif

void Debug_ConsolePrint(String p1)
{
#if SOFTWARE_DEBUGMODE
  try
  {
    p1.replace("\r\n", "\n");
    p1.replace('\r', '\n');
    p1.replace("\n", "\r\nDebug("+String(SOFTWARE_VERSION)+")\t     ");
#if HARDWARE_GSM
    if (_Dvc_GSM.Type == T_GSM)
    {
      int cnt = 0;
      while (GSM__IsPrinting && cnt++ < 3000)
        delay(1);
      GSM__PreventPrinting = true;
      p1.replace("\r\n", "\r\n"+String(GSM_MAGICWORD_FOR_PRINT));
      Serial.print(String(GSM_MAGICWORD_FOR_PRINT)+String("Debug("+String(SOFTWARE_VERSION))+")\t     "+ p1);
    }
    else
      Serial.print("Debug("+String(SOFTWARE_VERSION)+")\t     "+p1);
#else
    Serial.print("Debug("+String(SOFTWARE_VERSION)+")\t     "+p1);
#endif
  }
  catch (...)
  {
    Serial.println("\r\nErr Debug_ConsolePrint\r\n");
  }
#if HARDWARE_GSM
  GSM__PreventPrinting = false;
#endif
#endif
}
void Debug_ConsolePrint(int p1) { Debug_ConsolePrint(String(p1)); }
void Debug_ConsolePrint(long p1) { Debug_ConsolePrint(String(p1)); }
void Debug_ConsolePrint(float p1) { Debug_ConsolePrint(String(p1)); }


void Debug_ConsolePrintln(String p1)
{
#if SOFTWARE_DEBUGMODE
  try
  {
    p1.replace("\r\n", "\n");
    p1.replace('\r', '\n');
    p1.replace("\n", "\r\nDebug("+String(SOFTWARE_VERSION)+")\t     ");
#if HARDWARE_GSM
    if (_Dvc_GSM.Type == T_GSM)
    {
      int cnt = 0;
      while (GSM__IsPrinting && cnt++ < 3000)
        delay(1);
      GSM__PreventPrinting = true;
      p1.replace("\r\n", "\r\n"+String(GSM_MAGICWORD_FOR_PRINT));
      Serial.println(String(GSM_MAGICWORD_FOR_PRINT) +"Debug("+String(SOFTWARE_VERSION)+")\t     "+ p1);
    }
    else
      Serial.println("Debug("+String(SOFTWARE_VERSION)+")\t     "+p1);
#else
    Serial.println("Debug("+String(SOFTWARE_VERSION)+")\t     "+p1);
#endif
    Serial.flush();
  }
  catch (...)
  {
    Serial.println("\r\nErr Debug_ConsolePrintln\r\n");
  }
#if HARDWARE_GSM
  GSM__PreventPrinting = false;
#endif
#endif
}
void Debug_ConsolePrintln(int p1) { Debug_ConsolePrint(String(p1)); }
void Debug_ConsolePrintln(long p1) { Debug_ConsolePrint(String(p1)); }
void Debug_ConsolePrintln(float p1) { Debug_ConsolePrint(String(p1)); }


bool Debug_ConsolePrintedCheck(String &tx)
{
#if SOFTWARE_DEBUGMODE
  try
  {
#if HARDWARE_GSM
    if (_Dvc_GSM.Type != T_GSM)
      return false;
    if (tx.length() < GSM_MAGICWORD_FOR_PRINT_LEN)
      return false;
    return (tx.substring(0, GSM_MAGICWORD_FOR_PRINT_LEN) == GSM_MAGICWORD_FOR_PRINT) ? true : false;
#else
    return false;
#endif
  }
  catch (...)
  {
  }
#endif
  return false;
}



void Debug__ConsoleBegin()
{

  if (!_IsSerialStarted)
  {
    Serial.setRxBufferSize(1024); // تنظیم اندازه بافر فقط در صورت عدم راه‌اندازی
    Serial.begin(9600);
    _IsSerialStarted = true; // علامت‌گذاری به عنوان شروع شده
    Serial.flush();
  }
}



int Tools__StringSplitSize(String inp, char splitter)
{
  int j, i = 0, max = inp.length();
  for (j = 0; j < max; j++)
  {
    if (inp.charAt(j) == splitter)
      i++;
  }
  i++;
  return i;
}



void Tools__StringSplit(String inp, char splitter, String outArray[], int sizeOfArray)
{
  int j, j_old = 0, i = 0, max = inp.length();
  for (j = 0; j < max && i < sizeOfArray; j++)
  {
    if (inp.charAt(j) == splitter)
    {
      outArray[i] = inp.substring(j_old, j);
      j_old = j + 1;
      i++;
    }
  }
  if (i < sizeOfArray)
    outArray[i] = inp.substring(j_old);
}



byte Tools__StringToByte(String inp)
{
  byte r = 0;
  try
  {
    r = (byte)((inp).toInt() & 0xFF);
  }
  catch (...)
  {
  }
  return r;
}



int Tools__StringToInt(String inp)
{
  int r = 0;
  try
  {
    inp.trim();
    r = (inp).toInt();
  }
  catch (...)
  {
  }
  return r;
}



String Tools__ByteToHexstring(long b)
{
  String Str = String(b, HEX);
  Str.toUpperCase();
  return (Str.length() % 2 == 0 ? "" : "0") + Str;
}



uint64_t Tools__HexToUInt64(String p)
{
  p.toUpperCase();
  uint64_t accumulator = 0;
  for (size_t i = 0; isxdigit((unsigned char)p[i]); ++i)
  {
    char c = p[i];
    accumulator *= 16;
    if (isdigit(c))
      accumulator += c - '0'; // '0' .. '9'
    else
      accumulator += c - 'A' + 10; //'A'  .. 'F'
  }
  return accumulator;
}



long Tools__HexToDecimal(String value)
{
  int base = 16;
  int length = value.length() + 1;
  char valueAsArray[length];
  value.toCharArray(valueAsArray, length);
  return strtol(valueAsArray, NULL, base);
}



String Tools__HexStrToString(String value)
{
  String res = "";
  value.trim();
  int len = value.length();
  if (len % 2 != 0)
  {
    value = "0" + value;
    len++;
  }
  for (int i = 0; i < len; i += 2)
  {
    res += (char)(Tools__HexToUInt64(value.substring(i, i + 2)) & 0xFF);
  }
  return res;
}



String Tools__StringToHexStr(String value)
{
  String res = "";
  int len = value.length();
  for (int i = 0; i < len; i++)
    res += Tools__ByteToHexstring((byte)value.charAt(i));
  return res;
}



byte Tools__StringToCharVal(String inp, byte index)
{
  if (inp == "")
    return 0;
  char c = inp.charAt(index);
  return (byte)c;
}



String Tools__StringValidChars(String inp, String validChars = "")
{
  String output = "", ch;
  if (validChars == "")
    validChars = String(STRING_ALPHABET) + "_ " + String(STRING_NUMERIC);
  for (int n = 0; n < inp.length(); n++)
  {
    ch = inp.substring(n, n + 1);
    if (validChars.indexOf(ch) >= 0)
      output += ch;
  }
  return output;
}



void CA_ConcateStr(char output[], String str, int max = 0)
{
  int j, k, c, i = strlen(output), len = str.length();
  for (j = 0; j < len; j++)
  {
    k = i + j;
    c = str.charAt(j);
    if (c == '\0' || (max > 0 && k >= max))
      break;
    output[k] = c;
  }
  output[i + j] = '\0';
}



void CA_Concate(char output[], char input[], int max = 0)
{
  int i = strlen(output), j, k, len = strlen(input);
  for (j = 0; j < len; j++)
  {
    k = i + j;
    if (input[j] == '\0' || (max > 0 && k >= max))
      break;
    output[k] = input[j];
  }
  output[i + j] = '\0';
}



void CA_Copy(char output[], char input[], int max = 0)
{
  int i, len = strlen(input);
  for (i = 0; i < len; i++)
  {
    if (max > 0 && i >= max)
      break;
    if (input[i] == '\0')
      break;
    output[i] = input[i];
  }
  output[i] = '\0';
}



void CA_CopyStr(char output[], String str, int max = 0)
{
  int i, c, len = str.length();
  for (i = 0; i < len; i++)
  {
    if (max > 0 && i >= max)
      break;
    c = str.charAt(i);
    if (c == '\0')
      break;
    output[i] = c;
  }
  output[i] = '\0';
}



void CA_Sub(char input[], int start = 0, int len = 0)
{
  int i, j, end = strlen(input);
  if (len == 0)
    len = end - start;
  if (len <= 0)
  {
    input[0] = '\0';
  }
  else
  {
    char tmp[len + 1];
    for (i = start, j = 0; j < len; i++, j++)
    {
      if (input[i] == '\0')
        break;
      tmp[j] = input[i];
    }
    tmp[j] = '\0';
    for (i = 0; i <= j; i++)
      input[i] = tmp[i];
  }
}



int CA_IndexOf(char input[], char srch[])
{
  int i, j, max = strlen(input), len = strlen(srch);
  if (len > max || len == 0)
    return -1;
  max = max - len;
  for (i = 0; i <= max; i++)
  {
    for (j = 0; j < len; j++)
    {
      if (input[i + j] != srch[j])
        break;
    }
    if (j == len)
      return i;
  }
  return -1;
}



int CA_Compare(const char input1[], const char input2[])
{
  int i, v, min, len1 = strlen(input1), len2 = strlen(input2);
  min = len1;
  if (len2 < len1)
    min = len2;
  for (i = 0; i < min; i++)
  {
    v = ((byte)input1[i]) - ((byte)input2[i]);
    if (v != 0)
      break;
  }
  if (v == 0)
    return (len1 - len2);
  else
    return v;
}



bool CA_Equal(const char input1[], const char input2[])
{
  int i, v, min, len1 = strlen(input1), len2 = strlen(input2);
  if (len1 != len2)
    return false;
  min = len1;
  if (len2 < len1)
    min = len2;
  for (i = 0; i < min; i++)
  {
    v = ((byte)input1[i]) - ((byte)input2[i]);
    if (v != 0)
      return false;
  }
  return true;
}



bool CA_StartWith(const char input[], const char srch[])
{
  int i, len = strlen(srch);
  if (len > strlen(input))
    return false;
  for (i = 0; i < len; i++)
  {
    if (input[i] != srch[i])
      return false;
  }
  return true;
}



void CA_UpperCase(char input[])
{
  int i, v, len = strlen(input);
  for (i = 0; i < len; i++)
  {
    v = (byte)input[i];
    if (v >= 97 && v <= 122)
      input[i] = ((char)(v - 32));
  }
}



void CA_LowerCase(char input[])
{
  int i, v, len = strlen(input);
  for (i = 0; i < len; i++)
  {
    v = (byte)input[i];
    if (v >= 65 && v <= 90)
      input[i] = ((char)(v + 32));
  }
}



void CA_RightPad(char input[], int max, char c = ' ')
{
  try
  {
    int i, v, len = strlen(input);
    for (i = len; i < max; i++)
      input[i] = c;
  }
  catch (...)
  {
  }
}



void CA_Trim(char inp[])
{
  int start = 0;
  char *buffer = inp;
  while (*inp && *inp++ == ' ')
    ++start;
  while (*inp++)
    ;
  int end = inp - buffer - 1;
  while (end > 0 && buffer[end - 1] == ' ')
    --end;
  buffer[end] = 0;
  if (end <= start || start == 0)
    return;
  inp = buffer + start;
  while ((*buffer++ = *inp++))
    ;
}


// متد تبدیل کاراکتر به رشته
String CA_ToString(char input[], int start = 0, int len = 0)
{
  int i, end = strlen(input);
  String str = "";
  if (len == 0)
    len = end - start;
  end = start + len;
  for (i = start; i < end; i++)
  {
    if (input[i] == '\0')
      break;
    str += input[i];
  }
  return str;
}



bool Tools__File_TextWrite(const String &filename, const String &data)
{
  bool res = false;
  try
  {
    File file = SPIFFS.open(filename.c_str(), FILE_WRITE); // Open the file for writing
    try
    {
      if (!file)
        return false;
      res = file.print(data);
    }
    catch (...)
    {
    }
    file.close();
  }
  catch (...)
  {
  }
  return res;
}



String Tools__File_TextRead(const String &filename)
{
  String data = "";
  try
  {
    File file = SPIFFS.open(filename.c_str(), FILE_READ);
    try
    {
      if (!file)
        return "";
      try
      {
        data.reserve(8100);
      }
      catch (...)
      {
      }
      data = file.readString();
    }
    catch (...)
    {
    }
    file.close();
  }
  catch (...)
  {
  }
  return data;
}



bool Tools__File_Clear(const String &filename)
{
  bool res = false;
  try
  {
    File file = SPIFFS.open(filename.c_str(), FILE_WRITE); // Open the file for writing
    try
    {
      if (!file)
        return false;
      res = file.print("");
    }
    catch (...)
    {
    }
    file.close();
  }
  catch (...)
  {
  }
  return res;
}



void Tools__File_ClearAll()
{
  try
  {
    SPIFFS.format();
  }
  catch (...)
  {
  }
}



void Tools__File_JSONLoad(String filename, JsonVariant &jsn, JsonDocument &doc)
{
  try
  {
    filename.toLowerCase();
    filename.replace('\\', '/');
    if (filename.substring(0, 1) != "/")
      filename = "/" + filename;
    if (filename.indexOf(".json") < 0)
      filename += ".json";
    String data = Tools__File_TextRead(filename);
    DEBUG_SERIAL_PRINTLN( "Tools__File_JSONLoad " + filename + " => " + data);
    if (doc == NULL)
      doc = _JSONDoc;
    if (data != "")
    {
      doc.clear();
      data = "{\"root\":" + data + "}";
      deserializeJson(doc, data.c_str());
      jsn = doc["root"].as<JsonVariant>();
      // doc.clear();
    }
  }
  catch (...)
  {
  }
}



void Tools__File_JSONLoad(String filename, JsonVariant &jsn)
{
  try
  {
    filename.toLowerCase();
    filename.replace('\\', '/');
    if (filename.substring(0, 1) != "/")
      filename = "/" + filename;
    if (filename.indexOf(".json") < 0)
      filename += ".json";
    String data = Tools__File_TextRead(filename);
    DEBUG_SERIAL_PRINTLN( "Tools__File_JSONLoad " + filename + " => " + data );
    if (data != "")
    {
      _JSONDoc.clear();
      data = "{\"root\":" + data + "}";
      deserializeJson(_JSONDoc, data.c_str());
      jsn = _JSONDoc["root"].as<JsonVariant>();
      //_JSONDoc.clear();
    }
  }
  catch (...)
  {
  }
}



bool Tools__File_JSONSave(String filename, JsonVariant &jsn)
{
  try
  {
    String data = "";
    try
    {
      data = jsn.as<String>();
    }
    catch (...)
    {
    }
    if (data == "")
      serializeJson(jsn, data);
    if (data.length() <= 8000)
    {
      DEBUG_SERIAL_PRINTLN( "Tools__File_JSONSave " + filename + " => " + data );
      try
      {
        data.reserve(8100);
      }
      catch (...)
      {
      }
      filename.toLowerCase();
      filename.replace('\\', '/');
      if (filename.substring(0, 1) != "/")
        filename = "/" + filename;
      if (filename.indexOf(".json") < 0)
        filename += ".json";
      Tools__File_TextWrite(filename, data);
      return true;
    }
  }
  catch (...)
  {
  }
  return false;
}



bool Tools__File_JSONSave(String filename, String &data)
{
  try
  {
    if (data.length() <= 8000)
    {
      DEBUG_SERIAL_PRINTLN( "Tools__File_JSONSave " + filename + " => " + data );
      try
      {
        data.reserve(8100);
      }
      catch (...)
      {
      }
      filename.toLowerCase();
      filename.replace('\\', '/');
      if (filename.substring(0, 1) != "/")
        filename = "/" + filename;
      if (filename.indexOf(".json") < 0)
        filename += ".json";
      Tools__File_TextWrite(filename, data);
      return true;
    }
  }
  catch (...)
  {
  }
  return false;
}



void MemoWriteByte(int p_start, byte val)
{
  EEPROM.write(p_start, val);
  EEPROM.commit();
  delay(1);
}



void MemoWriteString(int p_start, int p_end, String val)
{
  int max = val.length();
  byte v[max];
  val.getBytes(v, max + 1);
  for (int i = p_start, c = 0; i <= p_end; i++, c++)
  {
    EEPROM.write(i, c < max ? v[c] : 0);
  }
  EEPROM.commit();
  delay(1);
}



byte MemoReadByte(int p_start)
{
  byte r = 0;
  try
  {
    r = 0 + EEPROM.read(p_start);
  }
  catch (...)
  {
  }
  return r;
}



String MemoReadString(int p_start, int p_end)
{
  String val = "";
  int i, empty = 0;
  char v, cFF = char(0xFF);
  for (i = p_start; i <= p_end; i++)
  {
    try
    {
      v = EEPROM.read(i);
      if (v == '\0')
        break;
      if (v == cFF)
        empty++;
      val += ("" + String(v));
    }
    catch (...)
    {
    }
  }
  if (val.length() == empty)
    val = "";
  return val;
}



bool Tools__Memory_StrSet(String key, String val)
{
  DEBUG_SERIAL_PRINTLN( "Tools__Memory_StrSet > " + key + " : " + val );
  try
  {
    if (key == "_Configured")
    {
      MemoWriteByte(EP_CONFIGURED, val == "1" ? 1 : 0);
    }
    else if (key == "_StartMode")
    {
      MemoWriteByte(EP_STARTMODE,
                    val == "normal"           ? 0
                    : val == "reboot_sendall" ? 7
                    : val == "config_sendall" ? 8
                    : val == "config_panel"   ? 9
                                              : 0);
    }
    else if (key == "_Extender")
    {

      MemoWriteByte(EP_EXTENDER, val == "2" ? 2 : 0);
    }
    else if (key == "_SerialNo")
    {
      val.toUpperCase();
      MemoWriteString(EP_SERIAL_S, EP_SERIAL_E, val);
    }
    else if (key == "_Circuit")
    {
      MemoWriteByte(EP_CIRCUIT, val == "iot"    ? 9
                                               : 9);
    }
    else if (key == "_CloudClientId")
    {
      MemoWriteString(EP_CLIENTID_S, EP_CLIENTID_E, val);
    }
    else if (key == "_ProjectCode")
    {
      val.toUpperCase();
      MemoWriteString(EP_PROJECTCODE_S, EP_PROJECTCODE_E, val);
    }
    else if (key == "_EncryptionKey")
    {
      MemoWriteString(EP_ENCRYPTIONKEY_S, EP_ENCRYPTIONKEY_E, val);
    }
    else if (key == "_MqttUser")
    {
      MemoWriteString(EP_MQTTUSER_S, EP_MQTTUSER_E, val);
    }
    else if (key == "_MqttPass")
    {
      MemoWriteString(EP_MQTTPASS_S, EP_MQTTPASS_E, val);
    }
    else if (key == "_MqttBroker")
    {
      MemoWriteString(EP_MQTTBROKER_S, EP_MQTTBROKER_E, val);
    }
    else if (key == "_MeshRSSI")
    {
      byte b = Tools__StringToByte(val);
      MemoWriteByte(EP_MESHRSSI, (b >= 0 && b < 255) ? b : 255);
    }
    else if (key == "_MeshName" || key == "_MeshNameOld")
    {
      if (key == "_MeshName")
        MemoWriteString(EP_MESHNAME_S, EP_MESHNAME_E, val);
      else
        MemoWriteString(EP_MESHNAME_OLD_S, EP_MESHNAME_OLD_E, val);
    }
    else if (key == "_MeshPass" || key == "_MeshPassOld")
    {
      if (key == "_MeshPass")
        MemoWriteString(EP_MESHPASS_S, EP_MESHPASS_E, val);
      else
        MemoWriteString(EP_MESHPASS_OLD_S, EP_MESHPASS_OLD_E, val);
    }
    else if (key == "_MeshChannel" || key == "_MeshChannelOld")
    {
      byte b = Tools__StringToByte(val);
      b = (b <= 14 && b > 0 ? b : 0);
      MemoWriteByte(key == "_MeshChannel" ? EP_MESHCHANNEL : EP_MESHCHANNEL_OLD, b);
    }
    else if (key == "_MqttBroker")
    {
      MemoWriteString(EP_MQTTBROKER_S, EP_MQTTBROKER_E, val);
    }
    else if (key == "_MqttUser")
    {
      MemoWriteString(EP_MQTTUSER_S, EP_MQTTUSER_E, val);
    }
    else if (key == "_MqttPass")
    {
      MemoWriteString(EP_MQTTPASS_S, EP_MQTTPASS_E, val);
    }
    else if (key == "_ModemChannel")
    {
      byte b = Tools__StringToByte(val);
      b = (b <= 14 && b > 0 ? b : 0);
      MemoWriteByte(EP_MODEMCHANNEL, b);
    }
    else if (key == "_ModemSSID")
    {
      MemoWriteString(EP_MODEMSSID_S, EP_MODEMSSID_E, val);
    }
    else if (key == "_ModemPass")
    {
      MemoWriteString(EP_MODEMPASS_S, EP_MODEMPASS_E, val);
    }
    else if (key == "_ModemRSSI")
    {
      byte b = Tools__StringToByte(val);
      b = (b < 255 && b >= 0 ? b : 255);
      MemoWriteByte(EP_MODEMRSSI, b);
    }
    return true;
  }
  catch (...)
  {
  }
  return false;
}



String Tools__Memory_StrGet(String key)
{
  bool checkNull = false;
  String res = "";
  int v = 0;
  try
  {
    if (key == "_Configured")
    {
      v = MemoReadByte(EP_CONFIGURED);
      res = (v == 1 ? "1" : "0");
    }
    else if (key == "_StartMode")
    {
      v = MemoReadByte(EP_STARTMODE);
      res = (v == 0   ? "normal"
             : v == 7 ? "reboot_sendall"
             : v == 8 ? "config_sendall"
             : v == 9 ? "config_panel"
                      : "normal");
    }
    else if (key == "_Extender")
    {
      v = MemoReadByte(EP_EXTENDER);

      res = (v == 2 ? "2" : "0");
    }
    else if (key == "_SerialNo")
    {
      res = MemoReadString(EP_SERIAL_S, EP_SERIAL_E);
      if (res == "" || res[0] == char(255))
        res = "0000000000";
    }
    else if (key == "_CloudClientId")
    {
      res = MemoReadString(EP_CLIENTID_S, EP_CLIENTID_E);
      if (res[0] == char(255))
        res = "";
    }
    else if (key == "_Circuit")
    {
      v = MemoReadByte(EP_CIRCUIT);

      res = (v == 9 ? "iot"
                      : "iot");
    }
    else if (key == "_ProjectCode")
    {
      res = MemoReadString(EP_PROJECTCODE_S, EP_PROJECTCODE_E);
      // res.toUpperCase();
      checkNull = true;
    }
    else if (key == "_EncryptionKey")
    {
      res = MemoReadString(EP_ENCRYPTIONKEY_S, EP_ENCRYPTIONKEY_E);
      checkNull = true;
    }
    else if (key == "_MqttUser")
    {
      res = MemoReadString(EP_MQTTUSER_S, EP_MQTTUSER_E);
      checkNull = true;
    }
    else if (key == "_MqttPass")
    {
      res = MemoReadString(EP_MQTTPASS_S, EP_MQTTPASS_E);
      checkNull = true;
    }
    else if (key == "_MqttBroker")
    {
      res = MemoReadString(EP_MQTTBROKER_S, EP_MQTTBROKER_E);
      checkNull = true;
    }
    else if (key == "_MeshRSSI")
    {
      v = MemoReadByte(EP_MESHRSSI);
      res = "" + String(v);
    }
    else if (key == "_MeshName" || key == "_MeshNameOld")
    {
      res = (key == "_MeshName" ? MemoReadString(EP_MESHNAME_S, EP_MESHNAME_E) : MemoReadString(EP_MESHNAME_OLD_S, EP_MESHNAME_OLD_E));
      checkNull = true;
    }
    else if (key == "_MeshPass" || key == "_MeshPassOld")
    {
      res = (key == "_MeshPass" ? MemoReadString(EP_MESHPASS_S, EP_MESHPASS_E) : MemoReadString(EP_MESHPASS_OLD_S, EP_MESHPASS_OLD_E));
      checkNull = true;
    }
    else if (key == "_MeshChannel" || key == "_MeshChannelOld")
    {
      v = MemoReadByte(key == "_MeshChannel" ? EP_MESHCHANNEL : EP_MESHCHANNEL_OLD);
      res = "" + String(v);
    }
    else if (key == "_ModemChannel")
    {
      v = MemoReadByte(EP_MODEMCHANNEL);
      res = "" + String(v);
    }
    else if (key == "_ModemSSID")
    {
      res = MemoReadString(EP_MODEMSSID_S, EP_MODEMSSID_E);
      checkNull = true;
    }
    else if (key == "_ModemPass")
    {
      res = MemoReadString(EP_MODEMPASS_S, EP_MODEMPASS_E);
      checkNull = true;
    }
    else if (key == "_ModemRSSI")
    {
      v = MemoReadByte(EP_MODEMRSSI);
      res = "" + String(v);
    }
  }
  catch (...)
  {
  }
  if (checkNull && res.charAt(0) == char(255))
    res = "";
  return res;
}



void MemoClean(int p_start = EP_MEMORYSTART2, int p_end = EP_MEMORYEND2, bool cleanFiles = true)
{
  DEBUG_SERIAL_PRINTLN( "MemoClean: " + String(p_start) + " To " + String(p_end) + (cleanFiles ? "  And Clean Files" : " "));
  try
  {
    for (int i = p_start; i <= p_end; i++)
    {
      EEPROM.write(i, 0xFF);
      if (i % 100 == 0)
      {
        EEPROM.commit();
        delay(1);
      }
    }
    EEPROM.commit();
    delay(100);
  }
  catch (...)
  {
  }
  if (cleanFiles)
    Tools__File_ClearAll();
}



void MemoCheck()
{
  int sum = 0;
  try
  {
    sum = (MemoReadByte(EP_MEMORYPOINT1H) * 256) + MemoReadByte(EP_MEMORYPOINT1L);
  }
  catch (...)
  {
  }
  if (sum != EP_MEMORYPOINT1L)
  {
    MemoClean(EP_MEMORYSTART1, EP_MEMORYEND1, true);
    Tools__SettingForTest();
    MemoWriteByte(EP_MEMORYPOINT1H, (EP_MEMORYPOINT1L & 0xFF00) >> 8);
    MemoWriteByte(EP_MEMORYPOINT1L, EP_MEMORYPOINT1L & 0xFF);
  }
  //................................................................................
  sum = 0;
  try
  {
    sum = (MemoReadByte(EP_MEMORYPOINT2H) * 256) + MemoReadByte(EP_MEMORYPOINT2L);
  }
  catch (...)
  {
  }
  if (sum != EP_MEMORYPOINT2L)
  {
    MemoClean(EP_MEMORYSTART2, EP_MEMORYEND2, true);
    MemoWriteByte(EP_MEMORYPOINT2H, (EP_MEMORYPOINT2L & 0xFF00) >> 8);
    MemoWriteByte(EP_MEMORYPOINT2L, EP_MEMORYPOINT2L & 0xFF);
  }
}



long Tools__Random(long a, long b)
{
  long d;
  if (a > b)
  {
    d = a;
    a = b;
    b = d;
  }
  d = (b - a) + 1;
  try
  {
    return a + (esp_random() % d);
  }
  catch (...)
  {
    DEBUG_SERIAL_PRINTLN("Random Error");
  }
  return a + (rand() % d);
}



void Tools__Reboot(String Reason = "")
{
  Tools__LED_Warning(WARNINGLED_OFF, 0);
  Dvc__LastStatusSave();
  pixels.clear();
  pixels.show();
  DEBUG_SERIAL_PRINTLN( "\r\n\r\n!!!!!!!! Rquested To REBOOT !!!!!!!!\r\n      Due to : " + Reason + "\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n\r\n" );
  ESP.restart();
}



void Tools__BackgroundWorks()
{
  try
  {
    // ESP.wdtFeed();
    delay(0);
    // ESP.wdtFeed();
  }
  catch (const std::exception &ex)
  {
  }
}



//-----------------------------------------------------------------------------------------------
//-------------------------------- Set/Get Settings From EPPROM  --------------------------------
//-----------------------------------------------------------------------------------------------

void Tools__SerialBarcodeReload()
{
  _SerialNo = Tools__Memory_StrGet("_SerialNo");
  if (_SerialNo.length() != 10)
  {
    _SerialNo = _SerialNo + "0000000000";
    _SerialNo = _SerialNo.substring(0, 10);
  }
  if (_SerialNo.substring(0, 3) == DEVICE_TYPECODE)
    _Dvc_HasBarcode = true;
  //............................................. Build Random Serial
  else
  {
    String chip = (String(Tools__GetChipID()) + "0000000").substring(0, 7);
    if (chip == "0000000")
      chip = (String(Tools__Random(1000000, 9999998)) + "0000000").substring(0, 7);
    _SerialNo = ("000" + chip).substring(0, 10);
    Tools__LED_WarningBlink(10, WARNINGLED_CONFIG_NEED, WARNINGLED_CONFIG_NEED, 100);
  }
  //............................................. Rebuild Some Variables
  _SerialHex = Tools__StringToHexStr(_SerialNo);
  _SerialCloud = "VTRA" + _SerialNo;
  CA_CopyStr(_MySenderId, "dvc_" + _SerialNo);
  DEBUG_SERIAL_PRINTLN("_SerialNo : " + _SerialCloud);
}



void Tools__SettingRead()
{
  DEBUG_SERIAL_PRINTLN("Setting Read... ");
  //...................................... Load Important Data
  _Banned = (MemoReadByte(EP_BANNED) == 1 ? 1 : 0);
  _Configured = (MemoReadByte(EP_CONFIGURED) == 1 ? 1 : 0);
  _StartMode = Tools__Memory_StrGet("_StartMode");
  //...................................... Read Other Settings
  _CloudClientId = Tools__Memory_StrGet("_CloudClientId");
  _CloudClientId.trim();
  byte b = MemoReadByte(EP_EXTENDER);
  _Extender = (b == 3 || b == 1 || b == 2 ? b : 0);
  _CloudSupport = (b == 3 || b == 2 ? true : false);
  _Circuit = Tools__Memory_StrGet("_Circuit");
  _Circuit = "iot";
  _Extender = 0;


  _ProjectCode = Tools__Memory_StrGet("_ProjectCode");
  _EncryptionKey = Tools__Memory_StrGet("_EncryptionKey");

  _ModemRSSI = MemoReadByte(EP_MODEMRSSI);
  if (_ModemRSSI <= 0 || _ModemRSSI > 99)
    _ModemRSSI = 99;
  _ModemSSID = Tools__Memory_StrGet("_ModemSSID");
  _ModemPass = Tools__Memory_StrGet("_ModemPass");
  _ModemChannel = Tools__StringToByte(Tools__Memory_StrGet("_ModemChannel"));
  if (_ModemChannel > 11 || _ModemChannel < 0)
    _ModemChannel = 0;

  _MeshRSSI = MemoReadByte(EP_MESHRSSI);
  if (_MeshRSSI <= 0 || _MeshRSSI > 99)
    _MeshRSSI = 99;
  _MeshChannel = Tools__StringToByte(Tools__Memory_StrGet("_MeshChannel"));
  if (_MeshChannel > 11 || _MeshChannel < 1)
    _MeshChannel = 1;
  _MeshChannelOld = Tools__StringToByte(Tools__Memory_StrGet("_MeshChannelOld"));
  if (_MeshChannelOld > 11 || _MeshChannelOld < 1)
    _MeshChannelOld = 1;
  _MeshName = Tools__Memory_StrGet("_MeshName");
  _MeshPass = Tools__Memory_StrGet("_MeshPass");
  if (_MeshPass == "")
    _MeshPass = DVC_DEFAULTPASS;
  _MeshNameOld = Tools__Memory_StrGet("_MeshNameOld");
  _MeshPassOld = Tools__Memory_StrGet("_MeshPassOld");

  _MqttBroker = Tools__Memory_StrGet("_MqttBroker");
  _MqttUser = Tools__Memory_StrGet("_MqttUser");
  _MqttPass = Tools__Memory_StrGet("_MqttPass");
  //........................................... Local-Broker or Cloud-Broker
  _IOTCloudUse = true;
  if (_MqttBroker.length() > 8)
  {
    if (_MqttBroker.substring(0, 8) == "192.168.")
      _IOTCloudUse = false;
  }
  else
    _IOTCloudUse = false;
}



void Tools__SettingDefault()
{
  DEBUG_SERIAL_PRINTLN("Setting To Dafault >>> ");
  //..................

  _Extender = 0;
  _Circuit = "iot";

  _ProjectCode = "";
  _EncryptionKey = "";

  _ModemSSID = "";
  _ModemPass = "";
  _ModemChannel = 0;

  _MeshChannel = 1;
  _MeshChannelOld = 1;
  _MeshName = "";
  _MeshPass = DVC_DEFAULTPASS;

  _MqttBroker = "";
  _MqttUser = "";
  _MqttPass = "";
#if BRAND_AIRNGIN
  _CloudSupport = true;
#else
  _CloudSupport = false;
#endif
}



void Tools__SettingSave()
{
  DEBUG_SERIAL_PRINTLN("Setting Save... ");
  try
  {
    _CloudClientId.trim();
    Tools__Memory_StrSet("_CloudClientId", _CloudClientId);
    Tools__Memory_StrSet("_Extender", _Extender == 3 || _Extender == 1 || _Extender == 2 ? String(_Extender) : "0");
    Tools__Memory_StrSet("_Circuit", _Circuit);
    Tools__Memory_StrSet("_ProjectCode", _ProjectCode);
    Tools__Memory_StrSet("_EncryptionKey", _EncryptionKey);

    Tools__Memory_StrSet("_ModemChannel", String(_ModemChannel));
    Tools__Memory_StrSet("_ModemSSID", _ModemSSID);
    Tools__Memory_StrSet("_ModemPass", _ModemPass);

    Tools__Memory_StrSet("_MeshChannel", String(_MeshChannel));
    Tools__Memory_StrSet("_MeshName", _MeshName);
    Tools__Memory_StrSet("_MeshPass", _MeshPass);

    Tools__Memory_StrSet("_MqttBroker", _MqttBroker);
    Tools__Memory_StrSet("_MqttUser", _MqttUser);
    Tools__Memory_StrSet("_MqttPass", _MqttPass);

    // Tools__Memory_StrSet("_CloudSupport", _CloudSupport ? "1" : "0");
  }
  catch (...)
  {
  }
}



void Tools__SettingForTest()
{
  //------------- change for test only --------------------

  _Circuit = "iot";
  _ProjectCode = "ZSLS00010001";

  _EncryptionKey = "";

  _ModemSSID = "smarthome"; 
  _ModemPass = "00000000";
  _ModemChannel = 0;

  _MeshChannel = 0;
  _MeshChannelOld = 1;
  _MeshName = "m1";
  _MeshPass = "mpass0000";

  _MqttBroker = "mqtt.airngin.com"; 
  _MqttUser = "smarthome";
  _MqttPass = "00000000";
                                
  _IOTCloudUse = false;
  Tools__SettingSave();
}



void Tools__SettingFromJSON(String json)
{
  DEBUG_SERIAL_PRINTLN("Setting From JSON ...");
  Class_Json obj;
  const char *tmp;
  JsonDocument doc;
  doc.clear();
  deserializeJson(doc, json.c_str());
  tmp = doc["circuit"];
  _Circuit = String(tmp);
  tmp = doc["projectCode"];
  _ProjectCode = String(tmp);
  tmp = doc["encryptionKey"];
  _EncryptionKey = String(tmp);

  tmp = doc["modemSsid"];
  _ModemSSID = String(tmp);
  tmp = doc["modemPass"];
  _ModemPass = String(tmp);
  _ModemChannel = (byte)doc["modemChannel"];

  _MeshChannel = (byte)doc["meshChannel"];
  tmp = doc["meshtype"];
  tmp = doc["meshName"];
  _MeshName = String(tmp);
  tmp = doc["meshPass"];
  _MeshPass = String(tmp);

  tmp = doc["mqttBroker"];
  _MqttBroker = String(tmp);
  tmp = doc["mqttUser"];
  _MqttUser = String(tmp);
  tmp = doc["mqttPass"];
  _MqttPass = String(tmp);
  DEBUG_SERIAL_PRINTLN("********************* JSON OK");
}



void Tools__SettingShowInfo()
{
#if SOFTWARE_DEBUGMODE
    DEBUG_SERIAL_PRINTLN("\r\n -----------------------\r\nSetting Show Info\r\n-----------------------");
    DEBUG_SERIAL_PRINTLN("_SerialNo: " + _SerialCloud);
    DEBUG_SERIAL_PRINTLN("_StartMode: " + _StartMode);
    DEBUG_SERIAL_PRINTLN("Token: " + _Token);

    DEBUG_SERIAL_PRINTLN("\r\n _Circuit: " + _Circuit);
    DEBUG_SERIAL_PRINTLN("_Extender: " + String(_Extender) + (_Extender == 1 || _Extender == 3 ? " / Yes" : " / No"));
    DEBUG_SERIAL_PRINTLN("_ProjectCode: " + _ProjectCode);
    DEBUG_SERIAL_PRINTLN("_EncryptionKey: " + _EncryptionKey);

    DEBUG_SERIAL_PRINTLN("\r\n _ModemChannel: " + String(_ModemChannel));
    DEBUG_SERIAL_PRINTLN("_ModemSSID: " + _ModemSSID);
    DEBUG_SERIAL_PRINTLN("_ModemPass: " + _ModemPass);
    DEBUG_SERIAL_PRINTLN("_ModemRSSI: " + String(_ModemRSSI));

    DEBUG_SERIAL_PRINTLN("\r\n _MeshChannel: " + String(_MeshChannel));
    DEBUG_SERIAL_PRINTLN("_MeshName: " + _MeshName);
    DEBUG_SERIAL_PRINTLN("_MeshPass: " + _MeshPass);
    DEBUG_SERIAL_PRINTLN("_MeshRSSI: " + String(_MeshRSSI));
    DEBUG_SERIAL_PRINTLN("_MeshNameOld: " + _MeshNameOld);
    DEBUG_SERIAL_PRINTLN("_MeshPassOld: " + _MeshPassOld);
    DEBUG_SERIAL_PRINTLN("_MeshChannelOld: " + String(_MeshChannelOld));

    DEBUG_SERIAL_PRINTLN("\r\n _MqttBroker: " + _MqttBroker);
    DEBUG_SERIAL_PRINTLN("_MqttUser: " + _MqttUser);
    DEBUG_SERIAL_PRINTLN("_MqttPass: " + _MqttPass);
    DEBUG_SERIAL_PRINTLN("_CloudClientId: " + _CloudClientId);
    DEBUG_SERIAL_PRINTLN("");
#endif
}



//===========================================================================================
//                                       GENERAL SETUP
//===========================================================================================
void Tools__TaskSetup()
{
  // Tools__Reboot_Task.disable();
  //_TaskRunner.addTask(Tools__Reboot_Task);
  // Tools__Reboot_Task.disable();

  //...................................
  //_TaskRunner.addTask(Tools__FreeMemory_Task);
  // Tools__FreeMemory_Task.enableIfNot();
}



// String Tools__DataToString(char scope[], const char type[], const char data[], const char sender[] = "") {
String Tools__DataToString(char scope[], const char type[], const char data[], const char sender[] = "")
{
  CA_LowerCase(scope);
  String JsonText = "";
  JsonDocument doc;
  doc["Scope"] = scope;
  doc["Type"] = type;
  doc["Data"] = data;
  doc["Sender"] = (strlen(sender) == 0 ? _MySenderId : sender);
  serializeJson(doc, JsonText);
  // String JsonText = "{\"Scope\":\"" + scope + "\",\"Type\":\"" + type + "\",\"Data\":\"" + data + "\",\"DataVersion\":1,\"Sender\":\"" + sender + "\"}";
  return JsonText;
}



void Tools__DataToJSON(Class_Json &obj, String str)
{
  char Kscope[] = "\"Scope\""; // KeyLen= 7 & Memory= 10 Bytes
#define JSONPACK_SCOPE_L 7
#define JSONPACK_SCOPE_B 10
  //..................
  char Ktype[] = "\"Type\""; // KeyLen= 6 & Memory= 10 Bytes
#define JSONPACK_TYPE_L 6
#define JSONPACK_TYPE_B 10
  //..................
  char Kdata[] = "\"Data\""; // KeyLen= 6 & Memory= 1024 Bytes
#define JSONPACK_DATA_L 6
#define JSONPACK_DATA_B 1024
  //..................
  char Ksender[] = "\"Sender\""; // KeyLen= 8 & Memory= 23 Bytes
#define JSONPACK_SENDER_L 8
#define JSONPACK_SENDER_B 23
  //..................
  obj.Scope[0] = '\0';
  obj.Scope[JSONPACK_SCOPE_B] = '\0';
  obj.Type[0] = '\0';
  obj.Type[JSONPACK_TYPE_B] = '\0';
  obj.Data[0] = '\0';
  obj.Data[JSONPACK_DATA_B] = '\0';
  obj.Sender[0] = '\0';
  obj.Sender[JSONPACK_SENDER_B] = '\0';
  char c = str.charAt(0);
  int i, j, k, l, m, len = str.length() - 1;
  //........................................ Start & End Of JSON
  while (c != '{' && len > 0)
  {
    str = str.substring(1);
    len = str.length() - 1;
    c = str.charAt(0);
  }
  c = str.charAt(len);
  while (c != '}' && len > 0)
  {
    str = str.substring(0, len);
    len = str.length() - 1;
    c = str.charAt(len);
  }
  int max = len - (JSONPACK_SENDER_L + 1); // longest key = 8char(JSONPACK_SENDER_L) & 1char(endSign'}')
  //........................................
  for (i = 1; i <= max; i++)
  {
    c = str.charAt(i);
    if (c != '\"')
      continue;
    //........................... Scope
    if (obj.Scope[0] == '\0')
    {
      l = JSONPACK_SCOPE_L;
      m = JSONPACK_SCOPE_B;
      for (j = 0; j < l; j++)
      {
        k = i + j;
        if (str.charAt(k) != Kscope[j])
          break;
      }
      if (j == l)
      {
        c = str.charAt(++k);
        while (c == ' ' && k < max)
          c = str.charAt(++k); // jump space
        if (c == ':')
        {
          c = str.charAt(++k);
          while (c == ' ' && k < max)
            c = str.charAt(++k); // jump space
          for (++k, j = 0; k < len && j < m; j++, k++)
          {
            c = str.charAt(k);
            if (c == '\"')
              break;
            obj.Scope[j] = c;
          }
          obj.Scope[j] = '\0';
        }
        i = k;
        continue;
      }
    }
    //........................... Type
    if (obj.Type[0] == '\0')
    {
      l = JSONPACK_TYPE_L;
      m = JSONPACK_TYPE_B;
      for (j = 0; j < l; j++)
      {
        k = i + j;
        if (str.charAt(k) != Ktype[j])
          break;
      }
      if (j == l)
      {
        c = str.charAt(++k);
        while (c == ' ' && k < max)
          c = str.charAt(++k); // jump space
        if (c == ':')
        {
          c = str.charAt(++k);
          while (c == ' ' && k < max)
            c = str.charAt(++k); // jump space
          for (++k, j = 0; k < len && j < m; j++, k++)
          {
            c = str.charAt(k);
            if (c == '\"')
              break;
            obj.Type[j] = c;
          }
          obj.Type[j] = '\0';
        }
        i = k;
        continue;
      }
    }
    //........................... Data
    if (obj.Data[0] == '\0')
    {
      l = JSONPACK_DATA_L;
      m = JSONPACK_DATA_B;
      for (j = 0; j < l; j++)
      {
        k = i + j;
        if (str.charAt(k) != Kdata[j])
          break;
      }
      if (j == l)
      {
        c = str.charAt(++k);
        while (c == ' ' && k < max)
          c = str.charAt(++k); // jump space
        if (c == ':')
        {
          c = str.charAt(++k);
          while (c == ' ' && k < max)
            c = str.charAt(++k); // jump space
          for (++k, j = 0; k < len && j < m; j++, k++)
          {
            c = str.charAt(k);
            if (c == '\"')
              break;
            obj.Data[j] = c;
          }
          obj.Data[j] = '\0';
        }
        i = k;
        continue;
      }
    }
    //........................... Sender
    if (obj.Sender[0] == '\0')
    {
      l = JSONPACK_SENDER_L;
      m = JSONPACK_SENDER_B;
      for (j = 0; j < l; j++)
      {
        k = i + j;
        if (str.charAt(k) != Ksender[j])
          break;
      }
      if (j == l)
      {
        c = str.charAt(++k);
        while (c == ' ' && k < max)
          c = str.charAt(++k); // jump space
        if (c == ':')
        {
          c = str.charAt(++k);
          while (c == ' ' && k < max)
            c = str.charAt(++k); // jump space
          for (++k, j = 0; k < len && j < m; j++, k++)
          {
            c = str.charAt(k);
            if (c == '\"')
              break;
            obj.Sender[j] = c;
          }
          obj.Sender[j] = '\0';
        }
        i = k;
        continue;
      }
    }
  }
}


void Tools__FreeMemory()
{
  try
  {
    ESP.getFreeHeap();
    // heap_caps_free(NULL);
  }
  catch (...)
  {
  }
}



void Tools__WifiPower(bool SetToOn)
{
  if (SetToOn)
  {
    //................................. Sets WiFi Power-Sleep to None
    WiFi.setSleep(WIFI_PS_NONE);
    //................................. Sets WiFi RF power output to highest level
    WiFi.setTxPower(WIFI_POWER_19_5dBm);
    // WiFi.setTxPower(WIFI_POWER_2dBm);  // Sets WiFi RF power output to low level
  }
  else
  {
    WiFi.mode(WIFI_OFF);
  }
}



void Tools__SetMode(String mode, bool restartNeed)
{
  DEBUG_SERIAL_PRINTLN("Set Next Mode To : " + mode);
  //.............................................
  if (mode == "normal")
  {
  }
  //.............................................
  Tools__Memory_StrSet("_StartMode", mode);
  if (restartNeed)
  {
    Tools__Reboot("Tools__SetMode");
  }
}



uint32_t Tools__GetChipID()
{
  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8)
  {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  return chipId;
}



String Tools__ESPGetTemprature()
{
  try
  {
    uint8_t f = temprature_sens_read();
    int c = (f - 32) * 0.555555;
    return (String(f) + " F / " + String(c) + " C");
  }
  catch (...)
  {
  }
  return "? F / ? C";
}



void Tools__ColorRGBPercent(const byte r, const byte g, const byte b, byte output[], int percent = 1000, const bool correctiotn = true)
{
  if (percent > 1000)
    percent = 1000;
  //.................................................. Intensity Of Colors
  byte max = (r > g ? r : g);
  if (max < b)
    max = b;
  if (max < 1)
    max = 1;
  float n, level, def = ((max - r) + (max - g) + (max - b)) / max;
  level = (((float)percent) / 1000) * (1 + (def * 0.28));

  //.................................................. Percentage & Correction(G,R/B)
  n = (r * level * (correctiotn ? 0.9 : 1));
  output[0] = (byte)(n > 255 ? 255 : n);
  n = (g * level * (correctiotn ? 0.72 : 1));
  output[1] = (byte)(n > 255 ? 255 : n);
  n = (b * level * (correctiotn ? 1.05 : 1));
  output[2] = (byte)(n > 255 ? 255 : n);
 
}



void Tools__ColorRGBLevel(byte r, byte g, byte b, byte output[], int level = 3, bool correctiotn = true)
{
  byte p = 1;
  switch (level)
  {
  case 1:
    p = 7;
    break; // 10
  case 2:
    p = 15;
    break; // 30
  case 3:
    p = 35;
    break; // 70
  case 4:
    p = 75;
    break; // 160
  case 5:
    p = 200;
    break; // 360
  default:
    p = 35;
    break; // 70
  }
  Tools__ColorRGBPercent(r, g, b, output, p, correctiotn);
}






void Tools__SetRGB_Warning(byte R, byte G, byte B)
{
#if NEOPIXEL_LEDWARNING_INDEX
  pixels.setPixelColor(NEOPIXEL_LEDWARNING_INDEX, pixels.Color(R, G, B));
#else
  pixels.setPixelColor(NEOPIXEL_NUM - 1, pixels.Color(R, G, B));
#endif
  pixels.show();
}



void Tools__SetRGB_ReadWrite(byte R, byte G, byte B)
{
#if NEOPIXEL_LEDRW_INDEX
  pixels.setPixelColor(NEOPIXEL_LEDRW_INDEX, pixels.Color(R, G, B));
#else
  pixels.setPixelColor(NEOPIXEL_NUM - 1, pixels.Color(R, G, B));
#endif
  pixels.show();
}

void Tools__LED_Warning(byte mode, int dly = 0)
{
  try
  {
    if (mode == WARNINGLED_AUTO)
    {
      //............................ Config
      if (_Started_ConfigPanel)
        mode = WARNINGLED_CONFIG;
      //............................ SendAll
      else if (_Started_ConfigSendall || _Started_RebootSendall)
        mode = WARNINGLED_SENDALL;
      else
      {
        //............................ WireOnly
        if (_Started_WiredOnly)
          mode = WARNINGLED_WIREONLY;
        //............................ ConfigNeed
        else if (_Need_ConfigIsBad)
          mode = WARNINGLED_CONFIG_NEED;
        //........................ Mesh
        else if (_Started_MeshRun)
        {
          // if (_Need_ConfigIsBad) mode = WARNINGLED_CONFIG_NEED;
          // else
          // mode = (!_Mesh_IsConnected ? WARNINGLED_DIS_MESH : _Need_Extender ? WARNINGLED_CONNECT_MESHEXTENDER
          //                                                                   : WARNINGLED_CONNECT_MESH);
        }
        //........................ IOT
        else if (_Started_IotRun)
        {
          // if (_Need_ConfigIsBad) mode = WARNINGLED_CONFIG_NEED;
          // else
          mode = (!_WiFi_IsConnected ? WARNINGLED_DIS_WIFI : !_MqttCon_IsConnected ? WARNINGLED_DIS_IOT
                                                         : _Need_Extender          ? WARNINGLED_CONNECT_IOTEXTENDER
                                                                                   : WARNINGLED_CONNECT_IOT);
        }
        //........................ Other
        else
        {
          // mode = WARNINGLED_OFF;
          return;
        }
      }
    }
    //.............................................................................................
    switch (mode)
    {
    case WARNINGLED_CONNECT_MESHEXTENDER: // light blue  2288FF > 020814
    Tools__SetRGB_Warning(0x02, 0x08, 0x14);
      break;
    case WARNINGLED_CONFIG_NEED: //               white  FFFFFF > 10111B
      Tools__SetRGB_Warning(0x10, 0x15, 0x1A);
      break;
    case WARNINGLED_CONFIG: //                   yellow  EECC00 > 111001
      Tools__SetRGB_Warning(0x11, 0x10, 0x01);
      break;
    case WARNINGLED_DIS_IOT: //                  orange  FF5500 > 090400
        Tools__SetRGB_Warning(0x0B, 0x04, 0x00);
      break;
    case WARNINGLED_DIS_WIFI: //                    red  FF0000 > 160001
        Tools__SetRGB_Warning(0x16, 0x00, 0x01);
      break;
    case WARNINGLED_DIS_MESH: //                 purple  FF0066 > 100014
        Tools__SetRGB_Warning(0x10, 0x00, 0x14);
      break;
    case WARNINGLED_DATA: //                     violet  6600FF > 060016
        Tools__SetRGB_Warning(0x06, 0x00, 0x16);
      break;
    case WARNINGLED_CONNECT_MESH: //               blue  0022FF > 010014
        Tools__SetRGB_Warning(0x01, 0x00, 0x14);
      break;
    case WARNINGLED_CONNECT_IOT: //               green  00FF22 > 000904
        Tools__SetRGB_Warning(0x00, 0x09, 0x04);
      break;
    case WARNINGLED_CONNECT_IOTEXTENDER: //       grass  FF5500 > 051400
        Tools__SetRGB_Warning(0x05, 0x14, 0x00);
      break;
    case WARNINGLED_WIREONLY:
    case WARNINGLED_OFF: //                       black  000000 > 000000
        Tools__SetRGB_Warning(0x00, 0x00, 0x00);
      break;
    case WARNINGLED_REBOOT: //                  darkRed  880103 > 070000
      Tools__SetRGB_Warning(0x07, 0x00, 0x00);
      break;
    case WARNINGLED_LEARN: //                  darkRed  880103 > 070000
      Tools__SetRGB_Warning(0x00, 0x14, 0x00);
      break;
    default:
      break;
    }
  }
  catch (...)
  {
  }
  if (dly > 0)
    delay(dly);
}



void Tools__LED_WarningBlink(byte counter = 4, byte mode1 = WARNINGLED_AUTO, byte mode2 = WARNINGLED_AUTO, int milisec = 100)
{
  do
  {
    Tools__LED_Warning(mode1, milisec);
    Tools__LED_Warning(WARNINGLED_OFF, milisec);
  } while (--counter > 0);
  Tools__LED_Warning(mode2);
}


void Tools__LED_AllBlink(byte counter = 6, byte color = WARNINGLED_COLOR_WHITE, int dly = 200)
{
  do
  {
    switch (color)
    {
    case WARNINGLED_COLOR_RED:
      pixels.fill(pixels.Color(0xFF, 0x00, 0x00));
      break;
    case WARNINGLED_COLOR_GREEN:
      pixels.fill(pixels.Color(0x00, 0xFF, 0x00));
      break;
    case WARNINGLED_COLOR_BLUE:
      pixels.fill(pixels.Color(0x00, 0x00, 0xFF));
      break;
    case WARNINGLED_COLOR_YELLOW:
      pixels.fill(pixels.Color(0xFF, 0xF0, 0x00));
      break;
    case WARNINGLED_COLOR_VIOLET:
      pixels.fill(pixels.Color(0xE0, 0x00, 0xFF));
      break;
    case WARNINGLED_COLOR_WHITE:
    default:
      pixels.fill(pixels.Color(0xFF, 0xFF, 0xFF));
      break;
    }
    pixels.show();
    delay(dly);
    pixels.fill(pixels.Color(0x00, 0x00, 0x00));
    pixels.show();
    delay(dly >> 1);
  } while (--counter > 0);
  Tools__LED_Warning(WARNINGLED_AUTO);
}



void Tools__CloudJSON_Reload(String typ = "all")
{
  DEBUG_SERIAL_PRINTLN("Tools__CloudJSON_Reload ...");
  typ.toLowerCase();
  JsonVariant inp;
  //.........................................................  Scenario Operation Reload
  if (typ == "all" || typ == "scenariooperation")
  {
    Tools__File_JSONLoad("scenarioOperation", inp);
    inp = inp["scenarioOperation"].as<JsonVariant>();
    Config__ReceiveJson_ScenarioOperation(inp, false, true);
  }
  //.........................................................  Command Operation Reload
  if (typ == "all" || typ == "commandoperation")
  {
    Tools__File_JSONLoad("commandOperation", inp);
    inp = inp["commandOperation"].as<JsonVariant>();
    Config__ReceiveJson_CommandOperation(inp, false, true);
  }
}



void Tools__AllDefaults()
{
  //....................................... Network Setting
  Tools__SettingDefault();

  //....................................... GSM Cloud-Phone-Number
#if HARDWARE_GSM
  CA_CopyStr(_Dvc_GSM.PhoneOffline, String(GSM_PHONEOFFLINE));
#endif
//....................................... Relays HoldTimeout
#if HARDWARE_RELAY
  for (byte r = 0; r < MAX_OPR; r++)
  {
    _Dvc_Relay[r].HoldTimeout = 0;
  }
#endif
  //....................................... _Scenario_List
  for (byte s = 0; s < MAX_SCENARIO; s++)
  {
    _Scenario_List[s].Act = 0xFF;
    for (byte r = 0; r < 12; r++)
      _Scenario_List[s].Name[r] = 0x20;
#if HARDWARE_RELAY
    for (byte r = 0; r < MAX_SCENARIO_OPR; r++)
      _Scenario_List[s].RelayNo[r] = 0xFF;
    for (byte r = 0; r < MAX_SCENARIO_OPR; r++)
      _Scenario_List[s].RelayCmd[r] = 0xFF;
    for (byte r = 0; r < MAX_SCENARIO_OPR; r++)
      _Scenario_List[s].RelayDelay[r] = 0;
#endif
    for (byte r = 0; r < MAX_SCENARIO_VIRTUAL; r++)
      _Scenario_List[s].Virtual[r] = 0xFF;
    for (byte r = 0; r < MAX_SCENARIO_VIRTUAL; r++)
      _Scenario_List[s].VirtualDelay[r] = 0;
    for (byte r = 0; r < MAX_SCENARIO_CODE; r++)
    {
      _Scenario_List[s].CodeCmd[r] = 0xFF;
      for (byte rr = 0; rr < 21; rr++)
        _Scenario_List[s].CodeVal[r][rr] = 0;
      _Scenario_List[s].CodeDelay[r] = 0;
    }
#if HARDWARE_GSM
    byte max = MAX_SCENARIO * MAX_SCENARIO_GSM;
    for (byte r = 0; r < max; r++)
      DVC__GSMValid[r] = 0;
#endif
  }
  
}



void Tools__ScenarioCheckDeleted(byte s)
{
  DEBUG_SERIAL_PRINTLN("Scenario Check Deleted ...");
  byte i, num;
  bool find;
  //........................................................ Reject If Scenario Exist
  num = _Scenario_List[s].Act;
  if (num != 0xFF)
    return;
    //........................................................ Find Deleted Scenario In RunSen_GSM
#if HARDWARE_GSM
  find = false;
  for (i = 0; i < MAX_RUNSENGSM; i++)
    try
    {
      if (_Runsen_GSM[i].Dvc != 0xFF && _Runsen_GSM[i].Sen == s)
      {
        _Runsen_GSM[i].Dvc = 0xFF;
        find = true;
      }
    }
    catch (...)
    {
    }
  if (find)
    Tools__RunsenSave("gsm");
#endif
  //........................................................ Find Deleted Scenario In RunSen_Virtual
  find = false;
  for (i = 0; i < MAX_RUNSENVIRTUAL; i++)
    try
    {
      if (_Runsen_Virtual[i].Dvc != 0xFF && _Runsen_Virtual[i].Sen == s)
      {
        _Runsen_Virtual[i].Dvc = 0xFF;
        find = true;
      }
    }
    catch (...)
    {
    }
  if (find)
    Tools__RunsenSave("virtual");
  //........................................................ Find Deleted Scenario In RunSen_Code
  find = false;
  for (i = 0; i < MAX_RUNSENCODE; i++)
    try
    {
      if (_Runsen_Code[i].Dvc != 0xFF && _Runsen_Code[i].Sen == s)
      {
        _Runsen_Code[i].Dvc = 0xFF;
        find = true;
      }
    }
    catch (...)
    {
    }
  if (find)
    Tools__RunsenSave("code");
}



void Tools__ScenarioLoad(byte s = 0xFF)
{
  DEBUG_SERIAL_PRINTLN(s == 0xFF ? "Scenario All Load..." : "Scenario [" + String(s) + "] Load...");
  byte i, j, jj, n, istart = 0, iend = MAX_SCENARIO - 1;
  int st;
  String tmp;
  if (s != 0xFF)
  {
    istart = s;
    iend = s;
  }
  for (i = istart; i <= iend; i++)
    try
    {
      Class_Scenario sen;
      sen.Id = i + 1; // can be simple (i+1) according to Security deleted
      n = MemoReadByte(EP_SCENARIO_ACT_S + i);
      //................................................... Act-Scenario Load
      sen.Act = (n == 0 || n == 1 ? n : 0xFF);
      if (sen.Act != 0xFF)
      {
        //................................................. Name-Scenario Load
        st = EP_SCENARIO_NAME_S + (BYTE_12 * i);
        String tmp = MemoReadString(st, st + BYTE_12 - 1);
        tmp.trim();
        CA_CopyStr(sen.Name, tmp, BYTE_12);
//................................................. Relay-Scenario Load
#if HARDWARE_RELAY
        for (j = 0; j < MAX_SCENARIO_OPR; j++)
        {
          st = (MAX_SCENARIO_OPR * i) + j;
          n = MemoReadByte(EP_SCENARIO_RELAYNO_S + st);
          if (n == 0xFF)
          {
            sen.RelayNo[j] = sen.RelayCmd[j] = 0xFF;
            sen.RelayDelay[j] = 0;
          }
          else
          {
            sen.RelayNo[j] = n;
            n = MemoReadByte(EP_SCENARIO_RELAYCMD_S + st);
            if (n == 0xFF)
            {
              sen.RelayNo[j] = sen.RelayCmd[j] = 0xFF;
              sen.RelayDelay[j] = 0;
            }
            else
            {
              sen.RelayCmd[j] = n;
              sen.RelayDelay[j] = MemoReadByte(EP_SCENARIO_RELAYMIN_S + st) * 60 + MemoReadByte(EP_SCENARIO_RELAYSEC_S + st);
            }
          }
        }
#endif
        //................................................. Virtual-Scenario Load
        for (j = 0; j < MAX_SCENARIO_VIRTUAL; j++)
        {
          st = (MAX_SCENARIO_VIRTUAL * i) + j;
          sen.Virtual[j] = MemoReadByte(EP_SCENARIO_VIRTUAL_S + st);
          sen.VirtualDelay[j] = (sen.Virtual[j] == 0xFF ? 0 : MemoReadByte(EP_SCENARIO_VIRTUALMIN_S + st) * 60 + MemoReadByte(EP_SCENARIO_VIRTUALSEC_S + st));
        }
#if HARDWARE_GSM
        //................................................. Gsm-Scenario Load
        for (j = 0; j < MAX_SCENARIO_GSM; j++)
        {
          st = EP_SCENARIO_GSMSTATE_S + (3 * i) + j;
          n = MemoReadByte(st);
          sen.GsmCmd[j] = (n == 0xFF || n == 0 ? n : 1);
          sen.GsmVal[j] = n;
          st = EP_SCENARIO_GSMPHONE_S + (45 * i) + (j * 15);
          tmp = MemoReadString(st, st + 14);
          CA_CopyStr(sen.GsmPhone[j], tmp);
        }
#endif
      }
      //................................................... Undefined Scenario
      else
      {
#if HARDWARE_RELAY
        CA_CopyStr(sen.Name, "", BYTE_12);
        for (j = 0; j < MAX_SCENARIO_OPR; j++)
        {
          sen.RelayNo[j] = sen.RelayCmd[j] = 0xFF;
          sen.RelayDelay[j] = 0;
        }
#endif
        //................................................. Undefined Virtual-Scenario
        for (j = 0; j < MAX_SCENARIO_VIRTUAL; j++)
        {
          sen.Virtual[j] = 0xFF;
          sen.VirtualDelay[j] = 0;
        }
        //................................................. Undefined Code-Scenario
        for (j = 0; j < MAX_SCENARIO_CODE; j++)
        {
          sen.CodeCmd[j] = 0xFF;
          sen.CodeDelay[j] = 0;
          CA_CopyStr(sen.CodeVal[j], "");
        }
      }
      _Scenario_List[i] = sen;
      // if (sen.Act == 0xFF) Tools__ScenarioCheckDeleted(i);
    }
    catch (...)
    {
    }
}



void Tools__ScenarioSave(byte s = 0xFF)
{
  DEBUG_SERIAL_PRINTLN(s == 0xFF ? "Scenario All Save..." : "Scenario [" + String(s) + "] Save...");
  byte i, j, jj, k, n, istart = 0, iend = MAX_SCENARIO - 1;
  int st;
  String tmp;
  if (s != 0xFF)
  {
    istart = s;
    iend = s;
  }
  //......................................................... Name-Scenario Save
  for (i = istart; i <= iend; i++)
    try
    {
      Class_Scenario sen = _Scenario_List[i];
      MemoWriteByte(EP_SCENARIO_ACT_S + i, sen.Act);
      tmp = CA_ToString(sen.Name, 0, BYTE_12);
      tmp = tmp.substring(0, BYTE_12);
      tmp.trim();
      if (tmp.length() == 0)
        tmp = "Scenario " + String(i + 1);
      st = EP_SCENARIO_NAME_S + (BYTE_12 * i);
      MemoWriteString(st, st + 12 - 1, tmp);
      //..................................................... Relay-Scenario Save
#if HARDWARE_RELAY
      for (j = 0; j < MAX_SCENARIO_OPR; j++)
      {
        st = (MAX_SCENARIO_OPR * i) + j;
        MemoWriteByte(EP_SCENARIO_RELAYNO_S + st, sen.RelayNo[j]);
        MemoWriteByte(EP_SCENARIO_RELAYCMD_S + st, sen.RelayCmd[j]);
        MemoWriteByte(EP_SCENARIO_RELAYMIN_S + st, sen.RelayDelay[j] / 60);
        MemoWriteByte(EP_SCENARIO_RELAYSEC_S + st, sen.RelayDelay[j] % 60);
      }
#endif
      //..................................................... Virtual-Scenario Save
      for (j = 0; j < MAX_SCENARIO_VIRTUAL; j++)
      {
        st = (MAX_SCENARIO_VIRTUAL * i) + j;
        MemoWriteByte(EP_SCENARIO_VIRTUAL_S + st, sen.Virtual[j]);
        MemoWriteByte(EP_SCENARIO_VIRTUALMIN_S + st, sen.VirtualDelay[j] / 60);
        MemoWriteByte(EP_SCENARIO_VIRTUALSEC_S + st, sen.VirtualDelay[j] % 60);
      }
//..................................................... Gsm-Scenario Save
#if HARDWARE_GSM
      for (j = 0; j < MAX_SCENARIO_GSM; j++)
      {
        st = EP_SCENARIO_GSMSTATE_S + (MAX_SCENARIO_GSM * i) + j;
        MemoWriteByte(st, sen.GsmVal[j]);
        tmp = CA_ToString(sen.GsmPhone[j], 0, 15);
        tmp = tmp.substring(0, 15);
        st = EP_SCENARIO_GSMPHONE_S + (45 * i) + (j * 15);
        MemoWriteString(st, st + 15, tmp);
      }
#endif
      // if (sen.Act == 0xFF) Tools__ScenarioCheckDeleted(i);
    }
    catch (...)
    {
    }
}



void Tools__DvcAddressSave(String typ = "all", byte index = 0xFF)
{
  DEBUG_SERIAL_PRINTLN("Dvc Address Save ...");
  typ.toLowerCase();
  byte i, num, max;
  int ep;
  String tmp = "";
  //......................................................... Relay  Address-Save
#if HARDWARE_RELAY
  if (typ == "all" || typ == "relay")
  {
    if (index == 0xFF || index == 0xFE)
    {
      //      MemoWriteByte(EP_RELAY_VARIANT, _Dvc_Variant);
      if (index == 0xFE)
        return;
      max = MAX_OPR - 1;
      i = 0;
    }
    else
      i = max = index;
    for (; i <= max; i++)
    {
      MemoWriteByte(EP_RELAY_TYPE_S + i, _Dvc_Relay[i].Type);
      MemoWriteByte(EP_RELAY_SUBNET_S + i, _Dvc_Relay[i].Subnet);
      MemoWriteByte(EP_RELAY_ADDRESS_S + i, _Dvc_Relay[i].Address);
      MemoWriteByte(EP_RELAY_CHANNEL_S + i, _Dvc_Relay[i].Channel);
      MemoWriteByte(EP_RELAY_HOLDSEC_S + i, _Dvc_Relay[i].Hold % 60);
      MemoWriteByte(EP_RELAY_HOLDMIN_S + i, _Dvc_Relay[i].Hold / 60);
      MemoWriteByte(EP_RELAY_LOCK_S + i, (_Dvc_Relay[i].Lock ? 1 : 0) & (_Dvc_Relay[i].BabyLock ? 2 : 0));
      _Dvc_Relay[i].HoldTimeout = 0;
    }
  }
#endif
  //......................................................... GSM  Address-Save
#if HARDWARE_GSM
  if (typ == "all" || typ == "gsm")
  {
    MemoWriteByte(EP_GSM_OPERATOR, _Dvc_GSM.Operator);
    MemoWriteByte(EP_GSM_TYPE, _Dvc_GSM.Type);
    MemoWriteByte(EP_GSM_SUBNET, _Dvc_GSM.Subnet);
    MemoWriteByte(EP_GSM_ADDRESS, _Dvc_GSM.Address);
    MemoWriteByte(EP_GSM_CHANNEL, _Dvc_GSM.Channel);
    tmp = CA_ToString(_Dvc_GSM.Phone1);
    tmp.trim();
    MemoWriteString(EP_GSM_PHONE1_S, EP_GSM_PHONE1_E, tmp);
    tmp = CA_ToString(_Dvc_GSM.Phone2);
    tmp.trim();
    MemoWriteString(EP_GSM_PHONE2_S, EP_GSM_PHONE2_E, tmp);
    tmp = CA_ToString(_Dvc_GSM.Phone3);
    tmp.trim();
    MemoWriteString(EP_GSM_PHONE3_S, EP_GSM_PHONE3_E, tmp);
    tmp = CA_ToString(_Dvc_GSM.PhoneOffline);
    tmp.trim();
    MemoWriteString(EP_GSM_PHONEOFFLINE_S, EP_GSM_PHONEOFFLINE_E, tmp);
  }
#endif
}



void Tools__DvcAddressLoad(String typ = "all")
{
  DEBUG_SERIAL_PRINTLN("Dvc Address Load ...");
  typ.toLowerCase();
  byte i, j, num, lock;
  int max, sum, ep;
  String tmp = "";
  //......................................................... Virtual  Address-Load
  if (typ == "all" || typ == "virtual")
  {
    _Dvc_Virtual.Subnet = 0;
    _Dvc_Virtual.Address = 254;
    _Dvc_Virtual.Channel = 252;
  }
  //......................................................... Relay  Address-Load
#if HARDWARE_RELAY
  if (typ == "all" || typ == "relay")
  {
    //_Dvc_Variant = MemoReadByte(EP_RELAY_VARIANT);
    // if (_Dvc_Variant == 0xFF) _Dvc_Variant = DEVICE_VARIANTDEFAULT;
    for (i = 0; i < MAX_OPR; i++)
    {
      _Dvc_Relay[i].Type = MemoReadByte(EP_RELAY_TYPE_S + i);
      _Dvc_Relay[i].Subnet = MemoReadByte(EP_RELAY_SUBNET_S + i);
      _Dvc_Relay[i].Address = MemoReadByte(EP_RELAY_ADDRESS_S + i);
      _Dvc_Relay[i].Channel = MemoReadByte(EP_RELAY_CHANNEL_S + i);
      _Dvc_Relay[i].Hold = (MemoReadByte(EP_RELAY_HOLDMIN_S + i) * 60) + MemoReadByte(EP_RELAY_HOLDSEC_S + i);
      lock = MemoReadByte(EP_RELAY_LOCK_S + i);
      if (_Dvc_Relay[i].Type == 0xFF && _Dvc_Relay[i].Subnet == 0xFF && _Dvc_Relay[i].Address == 0xFF)
      {
        _Dvc_Relay[i].Type = T_LIGHT;
        _Dvc_Relay[i].Subnet = 0;
        _Dvc_Relay[i].Address = 1;
        _Dvc_Relay[i].Channel = i + RELAY_CH_START;
        _Dvc_Relay[i].Hold = 0;
        lock = 0;
      }
      _Dvc_Relay[i].Lock = (lock & 1 == 1 ? true : false);
      _Dvc_Relay[i].BabyLock = (lock & 2 == 2 ? true : false);
      _Dvc_Relay[i].HoldTimeout = 0;
      DEBUG_SERIAL_PRINTLN("Dvc Address Load ...  " + String(_Dvc_Relay[i].Type) + "  " + String(_Dvc_Relay[i].Subnet) + "  " + String(_Dvc_Relay[i].Address) + "  " + String(_Dvc_Relay[i].Channel));
    }
  }
#endif
  //......................................................... GSM  Address-Load
#if HARDWARE_GSM
  if (typ == "all" || typ == "gsm")
  {
    _Dvc_GSM.Operator = MemoReadByte(EP_GSM_OPERATOR);
    if (_Dvc_GSM.Operator < 1 || _Dvc_GSM.Operator == 255)
      _Dvc_GSM.Operator = SIMCARD_OPERATOR_UNKNOWN;
    _Dvc_GSM.Type = MemoReadByte(EP_GSM_TYPE);
    _Dvc_GSM.Subnet = MemoReadByte(EP_GSM_SUBNET);
    _Dvc_GSM.Address = MemoReadByte(EP_GSM_ADDRESS);
    _Dvc_GSM.Channel = MemoReadByte(EP_GSM_CHANNEL);
    if (_Dvc_GSM.Type == 0xFF && _Dvc_GSM.Subnet == 0xFF && _Dvc_GSM.Address == 0xFF)
    {
      _Dvc_GSM.Type = T_GSM;
      _Dvc_GSM.Subnet = 0;
      _Dvc_GSM.Address = 1;
      _Dvc_GSM.Channel = CH_START_GSM;
      Tools__DvcAddressSave("gsm");
    }
    for (byte i = 0; i < BYTE_15; i++)
    { // check and correct phone-numbers
      byte EE = MemoReadByte(EP_GSM_PHONE1_S + i);
      if (EE > 0x39 || EE < 0x30)
        MemoWriteByte(EP_GSM_PHONE1_S + i, 0);

      EE = MemoReadByte(EP_GSM_PHONE2_S + i);
      if (EE > 0x39 || EE < 0x30)
        MemoWriteByte(EP_GSM_PHONE2_S + i, 0);

      EE = MemoReadByte(EP_GSM_PHONE3_S + i);
      if (EE > 0x39 || EE < 0x30)
        MemoWriteByte(EP_GSM_PHONE3_S + i, 0);
    }
    tmp = MemoReadString(EP_GSM_PHONE1_S, EP_GSM_PHONE1_E);
    tmp.trim();
    DEBUG_SERIAL_PRINTLN("phone1: '" + tmp + "'");
    CA_CopyStr(_Dvc_GSM.Phone1, tmp);
    tmp = MemoReadString(EP_GSM_PHONE2_S, EP_GSM_PHONE2_E);
    tmp.trim();
    DEBUG_SERIAL_PRINTLN("phone2: '" + tmp + "'");
    CA_CopyStr(_Dvc_GSM.Phone2, tmp);
    tmp = MemoReadString(EP_GSM_PHONE3_S, EP_GSM_PHONE3_E);
    tmp.trim();
    DEBUG_SERIAL_PRINTLN("phone3: '" + tmp + "'");
    CA_CopyStr(_Dvc_GSM.Phone3, tmp);
    tmp = MemoReadString(EP_GSM_PHONEOFFLINE_S, EP_GSM_PHONEOFFLINE_E);
    tmp.trim();
    if (tmp == "")
      tmp = GSM_PHONEOFFLINE;
    DEBUG_SERIAL_PRINTLN("phoneOffline: '" + tmp + "'");
    CA_CopyStr(_Dvc_GSM.PhoneOffline, tmp);
  }
#endif
}






void Tools__RunsenLoad(String typ = "all")
{
  DEBUG_SERIAL_PRINTLN("Runsen Load ...");
  typ.toLowerCase();
  byte b, i, j, k, num;
  int st, min, max;
  String tmp = "";
  //......................................................... GSM  Runsen-Load
#if HARDWARE_GSM
  if (typ == "all" || typ == "gsm")
  {
    for (i = 0; i < MAX_RUNSENGSM; i++)
    {
      num = MemoReadByte(EP_RUNSENGSM_CON_S + i);
      if (num == 0xFF)
      {
        _Runsen_GSM[i].Dvc = 0xFF;
        _Runsen_GSM[i].Con = 0xFF;
        _Runsen_GSM[i].Act = 0xFF;
        _Runsen_GSM[i].Sen = 0xFF;
      }
      else
      {
        _Runsen_GSM[i].Dvc = (num == 0 ? 0x00 : 0x01);
        _Runsen_GSM[i].Con = num;
        _Runsen_GSM[i].Act = MemoReadByte(EP_RUNSENGSM_ACT_S + i);
        _Runsen_GSM[i].Sen = MemoReadByte(EP_RUNSENGSM_SEN_S + i);
      }
    }
  }
#endif
  //......................................................... Virtual  Runsen-Load
  if (typ == "all" || typ == "virtual")
  {
    for (i = 0; i < MAX_RUNSENVIRTUAL; i++)
    {
      num = MemoReadByte(EP_RUNSENVIRTUAL_CON_S + i);
      if (num == 0xFF || num == 0)
      {
        _Runsen_Virtual[i].Dvc = 0xFF;
        _Runsen_Virtual[i].Con = 0xFF;
        _Runsen_Virtual[i].Act = 0xFF;
        _Runsen_Virtual[i].Sen = 0xFF;
      }
      else
      {
        _Runsen_Virtual[i].Dvc = 0x01;
        _Runsen_Virtual[i].Con = num;
        _Runsen_Virtual[i].Act = MemoReadByte(EP_RUNSENVIRTUAL_ACT_S + i);
        _Runsen_Virtual[i].Sen = MemoReadByte(EP_RUNSENVIRTUAL_SEN_S + i);
      }
    }
  }
}



void Tools__RunsenSave(String typ = "all")
{
  DEBUG_SERIAL_PRINTLN("Runsen Save ...");
  typ.toLowerCase();
  byte i, j, k, num;
  int st, min, max;
  String tmp = "";
  //......................................................... GSM  Runsen-Save
#if HARDWARE_GSM
  if (typ == "all" || typ == "gsm")
  {
    for (i = 0; i < MAX_RUNSENGSM; i++)
    {
      MemoWriteByte(EP_RUNSENGSM_CON_S + i, _Runsen_GSM[i].Con);
      MemoWriteByte(EP_RUNSENGSM_ACT_S + i, _Runsen_GSM[i].Act);
      MemoWriteByte(EP_RUNSENGSM_SEN_S + i, _Runsen_GSM[i].Sen);
    }
  }
#endif
  //......................................................... Virtual  Runsen-Save
  if (typ == "all" || typ == "virtual")
  {
    for (i = 0; i < MAX_RUNSENVIRTUAL; i++)
    {
      MemoWriteByte(EP_RUNSENVIRTUAL_CON_S + i, _Runsen_Virtual[i].Con);
      MemoWriteByte(EP_RUNSENVIRTUAL_ACT_S + i, _Runsen_Virtual[i].Act);
      MemoWriteByte(EP_RUNSENVIRTUAL_SEN_S + i, _Runsen_Virtual[i].Sen);
    }
  }
}



void TimerSec_Refresh()
{
  //-------------------------------------------- TIMER
  try
  {
    _TimerSecCurrent = millis();
    _TimerSecDef = ((_TimerSecCurrent - _TimerSecOld) / 1000) & 0xFF;
    if (_TimerSecDef < 0)
      _TimerSecDef = 1;
    if (_TimerSecDef >= 1)
    {
      _TimerSecOld = _TimerSecCurrent;
      _TimerForActions += _TimerSecDef;
      _Dvc_SenarioCommand_TimerCheck();
      if (_Started_IotRun)
      {
        if (_IOT_PingTimeout > 0)
        {
          _IOT_PingTimeout -= _TimerSecDef;
          if (_IOT_PingTimeout <= 0) _IOT_PingTimeout = TIMER_NEED_RUN;
        }
        if (_IOT_MqttTimeout > 0)
        {
          _IOT_MqttTimeout -= _TimerSecDef;
          if (_IOT_MqttTimeout <= 0) _IOT_MqttTimeout = TIMER_NEED_RUN;
        }
        if (_IOT_ModemTimeout > 0)
        {
          _IOT_ModemTimeout -= _TimerSecDef;
          if (_IOT_ModemTimeout <= 0) _IOT_ModemTimeout = TIMER_NEED_RUN;
        }
      }
#if HARDWARE_GSM
      //.................................... Timer For GSM_AllStatus
      if (_GSM_AllStatusTimeout > 0)
      {
        _GSM_AllStatusTimeout -= _TimerSecDef;
        if (_GSM_AllStatusTimeout <= 0) _GSM_AllStatusTimeout = TIMER_NEED_RUN;
      }
      //.................................... Timer For GSM_RSSI
      if (_GSM_RSSI_CheckupTimeout > 0)
      {
        _GSM_RSSI_CheckupTimeout -= _TimerSecDef;
        if (_GSM_RSSI_CheckupTimeout <= 0) _GSM_RSSI_CheckupTimeout = TIMER_NEED_RUN;
      }
      //.................................... Timer For GSM_Money_Request
      if (_GSM_Money_CheckupTimeout > 0)
      {
        _GSM_Money_CheckupTimeout -= _TimerSecDef;
        if (_GSM_Money_CheckupTimeout <= 0)  _GSM_Money_CheckupTimeout = TIMER_NEED_RUN;
      }
      //.................................... Timer For GSM_Money_Response
      if (_GSM_Money_ResponseTimeout > 0)
      {
        _GSM_Money_ResponseTimeout -= _TimerSecDef;
        if (_GSM_Money_ResponseTimeout <= 0)  _GSM_Money_ResponseTimeout = TIMER_NEED_RUN;
      }
      //.................................... Timer For GSM_Operator_Autofind
      if (_GSM_Operator_AutofindTimeout > 0)
      {
        _GSM_Operator_AutofindTimeout -= _TimerSecDef;
        if (_GSM_Operator_AutofindTimeout <= 0)  _GSM_Operator_AutofindTimeout = TIMER_NEED_RUN;
      }
#endif
    }
  }
  catch (...)
  {
  }
}



void TaskTimerLoop(void* param) {
  while (true) {
    try {
      // _TaskRunner.execute();
    } catch (...) {
      DEBUG_SERIAL_PRINTLN("Error TaskTimerLoop ");
    }
    delay(TASK_LOOP_DELAY);
  }
}



void EEPROM_Start(){
  if (!EEPROM.begin(EEPROM_SIZE)) {
    byte fail = 0;
    while (!EEPROM.begin(EEPROM_SIZE)) {
      DEBUG_SERIAL_PRINTLN("EEPROM Failed to init: " + String(fail));
      delay(2000);
      if (++fail > 5) {
#if SOFTWARE_DEBUGMODE
#else
 Debug__ConsoleBegin();
#endif
        Serial.println("EEPROM Failed to init Many Times");
        delay(500);
        Tools__Reboot("EEPROM.begin");
        return;
      }
    }
  }
  DEBUG_SERIAL_PRINTLN("EEPROM Size OK (" + String((byte)(100 * EP_MEMORYEND2 / EEPROM_SIZE)) + "\% Used / " + String(EP_MEMORYEND2 + 2) + " of " + String(EEPROM_SIZE) + " Bytes)");
  if (EP_MEMORYEND2 + 2 >= EEPROM_SIZE) {
#if SOFTWARE_DEBUGMODE
#else
    Debug__ConsoleBegin();
#endif
    while (true) {
      Serial.println("\r\n!!!!!!!!!!!!!!!!!\r\nEPROM Overflow Address (at " + String(EP_MEMORYEND2 + 2) + ") MAX is " + String(EEPROM_SIZE));
      Serial.flush();
      Tools__LED_WarningBlink(10, WARNINGLED_REBOOT, WARNINGLED_OFF);
      delay(100);
    }
  }
}



void SPIFFS_Start(){
  byte fail = 0;
  while (!SPIFFS.begin(true) && fail < 5) {  // Set the true parameter to format the file system if it fails to mount
#if SOFTWARE_DEBUGMODE
#else
    Debug__ConsoleBegin();
#endif
    Serial.println("SPIFFS Mount Failed");
    delay(1000);
  }
  fail = (SPIFFS.totalBytes() - SPIFFS.usedBytes()) * 100 / SPIFFS.totalBytes();
  if (fail < 3) {
#if SOFTWARE_DEBUGMODE
#else
    Debug__ConsoleBegin();
#endif
    while (true) {
      Serial.println("\r\n!!!!!!!!!!!!!!!!!\r\nSPIFFS Is Full (" + String(fail) + "%) " + String(SPIFFS.usedBytes()) + " of " + String(SPIFFS.totalBytes()) + " Bytes");
      Serial.flush();
      Tools__LED_WarningBlink(10, WARNINGLED_REBOOT, WARNINGLED_OFF);
      delay(100);
    }
  }
  DEBUG_SERIAL_PRINTLN("FILES Space OK (" + String((byte)(100 * SPIFFS.usedBytes() / SPIFFS.totalBytes())) + "\% Used / " + String(SPIFFS.usedBytes()/1024) + " of " + String(SPIFFS.totalBytes()/1024) + " Kb)\r\n");
}



void Public_Main(){
   //...................................... 3 Sec Delay to Stable Power
  delay(500);
#if HARDWARE_GSM
  GSM__PowerOff();
#endif
  //-------------------------------------------------------- Preparing Serial & DebugPrint
#if SOFTWARE_DEBUGMODE
    Debug__ConsoleBegin();
    DEBUG_SERIAL_PRINTLN("\r\n=========================\r\n======= Restarted =======\r\n=========================");
#endif
  //-------------------------------------------------------- Preparing Device
  Dvc__SetPinout(true);
  delay(500);
  Tools__LED_WarningBlink(10, WARNINGLED_REBOOT, WARNINGLED_OFF, 100);

  //-------------------------------------------------------- Preparing EEPROM & File
  EEPROM_Start();
  MemoCheck();
  SPIFFS_Start();

  //-------------------------------------------------------- Create Token For This Session
  if (_Token == "") _Token = (String(Tools__GetChipID()) + "0000000000").substring(0, 10) + String(Tools__Random(100000, 999998));  // 16 Digit Token

  //-------------------------------------------------------- Loading Data
  Tools__LED_Warning(WARNINGLED_DATA);

  Tools__AllDefaults();
  Tools__SettingRead();
  Tools__SerialBarcodeReload();
  Tools__SettingShowInfo();

  Tools__DvcAddressLoad();
  

  Tools__RunsenLoad();
  Tools__ScenarioLoad();
  if (_IOTCloudUse) Tools__CloudJSON_Reload();
  Dvc__LastStatusLoad();

  //-------------------------------------------------------- Preparing
  Tools__TaskSetup();
  //........................
  if (_Circuit == "wired") {
    _Need_Wire = true;
    _Started_WiredOnly = true;
  }
  //........................
 else if (_Circuit == "wirefi") {
    _Need_Wire = true;
    _Need_Mesh = true;
    if (_Extender == 1 || _Extender == 3) _Need_Extender = true;
  }
  //........................
  else if (_Circuit == "iot") {
    _Need_IOT = true;
  } else if (_Circuit == "wiot") {
    _Need_IOT = true;
    _Need_Wire = true;
    if (_Extender == 1 || _Extender == 3) _Need_Extender = true;
  }
  //........................
  if (_StartMode == "config_panel") {
    _Started_ConfigPanel = true;
    _Need_Taskloop = true;
  } else if (_StartMode == "reboot_sendall") {
    //if (_ModemSSID == "" ) _Need_ConfigIsBad = true;
    _Started_RebootSendall = true;
  } else if (_StartMode == "config_sendall") {
    //if (_ModemSSID == "" ) _Need_ConfigIsBad = true;
    _Started_ConfigSendall = true;
  } else if (_StartMode == "config_ranking") {
    //if (_ModemSSID == "" ) _Need_ConfigIsBad = true;
    _Started_ConfigRanking = true;
  } else {  // Started_Normal
    if (!_Started_WiredOnly) {
      if (_Need_IOT) {
        if (_ProjectCode == "" || _ModemSSID == "" || _MqttBroker == "") _Need_ConfigIsBad = true;
        else _Started_IotRun = true;
      } else if (_Need_Mesh) {
        if (_ProjectCode == "") _Need_ConfigIsBad = true;
        else _Started_MeshRun = true;
      }
    }
    if (!_Started_MeshRun) _Need_Taskloop = true;
  }

  //...................................... Warning LED
  // if(_Need_ConfigIsBad) Tools__LED_Warning(WARNINGLED_CONFIG_NEED, 2000);
  Tools__LED_Warning(WARNINGLED_DATA);

  //...................................... Set WiFi-Power
  Tools__WifiPower(_Started_WiredOnly ? false : true);

  //.............................................. To Do : Check Banned Any Where ??????????????????????????
  if (_Banned == 1)
  {
    while (true)
    {
      Tools__LED_WarningBlink(10, WARNINGLED_REBOOT, WARNINGLED_OFF);
      delay(1000);
    }
  }



  //...................................... Setup LED & TouchKey (To Use Manualy)
  Manual__Setup();

  //----------------------------------------------------------------- Running By Mode (On Main Thread)
  //...................................... Setup On Mode-Config
  if (_Started_ConfigPanel)
  {
    Config__Setup(NULL);
  }

  //............................................. Setup On Mode IOT / Mqtt
  else if (_Started_IotRun)
  {
    IOT__Setup(NULL);
  }
  //............................................. Setup On Mode Wifi-Mesh
  else if (_Started_MeshRun)
  {
    // Mesh__Setup(NULL);
  }
  //.............................................
  Tools__LED_Warning(WARNINGLED_AUTO);
}



void Public_Loop(){
  while (true)
  {
  
  #if HARDWARE_GSM
  try {
    if (_Dvc_GSM.Type == T_GSM) GSM__SmsRead();
  } catch (...) {}
  #endif
  /*
  try {
	//----------------------
	Tools__BackgroundWorks();
	//............................................. Loop Manual Button
	try {
	  Manual__Loop();
	} catch (const std::exception& ex) {}
	//----------------------
	Tools__BackgroundWorks();
	//............................................. Loop On Config
	try {
	  if (_Started_ConfigPanel) {
		Config__Loop();
	  }
	  //......................................... Loop On Send Setting/Send RootRank/Send Reboot To All (On Old Mesh)
	  else if (_Started_ConfigSendall || _Started_RebootSendall) {
		//    Mesh__Loop();
	  }
	  //......................................... Loop On Mode IOT / Mqtt
	  else if (_Started_IotRun) {
		IOT__Loop();
	  }
	  //......................................... Loop On Mode Wifi-Mesh
	  else if (_Started_MeshRun) {
		//    Mesh__Loop();
	  }
	} catch (const std::exception& ex) {}
	//----------------------
	Tools__BackgroundWorks();
	//............................................. Timer Simple
	try {
	  _SecondCur = millis();
	  _Def = ((_SecondCur - _SecondOld) / 1000) & 0xFF;
	  if (_Def < 0) _Def = 1;
	  if (_Def >= 1) {
		_SecondOld = _SecondCur;
	  }
	} catch (const std::exception& ex) {}
	//----------------------
	Tools__BackgroundWorks();
	//............................................. Timer TaskRunner
	if (_Need_Taskloop)) {
	  try {
		_TaskRunner.execute();
	  } catch (const std::exception& ex) {
		DEBUG_SERIAL_PRINTLN("Error TaskTimerLoop ");
	  }
	}
  } catch (...) {}
  */
  delay(100);
  }
}

//===========================================================================================
//                                          End
//===========================================================================================
#endif
