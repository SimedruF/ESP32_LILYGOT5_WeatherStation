// According to the board, cancel the corresponding macro definition
#define LILYGO_T5_V213
// #define LILYGO_T5_V22
// #define LILYGO_T5_V24
// #define LILYGO_T5_V28
// #define LILYGO_T5_V102
// #define LILYGO_T5_V266
// #define LILYGO_EPD_DISPLAY_102
// #define LILYGO_EPD_DISPLAY_154

#include <boards.h>
#define EINKDISPLAY
#include <GxEPD.h>

#if defined(LILYGO_T5_V102) || defined(LILYGO_EPD_DISPLAY_102)
#include <GxGDGDEW0102T4/GxGDGDEW0102T4.h> //1.02" b/w
#elif defined(LILYGO_T5_V266)
#include <GxDEPG0266BN/GxDEPG0266BN.h> // 2.66" b/w   form DKE GROUP
#elif defined(LILYGO_T5_V213)
#include <GxDEPG0213BN/GxDEPG0213BN.h> // 2.13" b/w  form DKE GROUP
// #include <GxGDE0213B1.h>
#else
// #include <GxGDGDEW0102T4/GxGDGDEW0102T4.h> //1.02" b/w
// #include <GxGDEW0154Z04/GxGDEW0154Z04.h>  // 1.54" b/w/r 200x200
// #include <GxGDEW0154Z17/GxGDEW0154Z17.h>  // 1.54" b/w/r 152x152
// #include <GxGDEH0154D67/GxGDEH0154D67.h>  // 1.54" b/w
// #include <GxDEPG0150BN/GxDEPG0150BN.h>    // 1.51" b/w   form DKE GROUP
// #include <GxDEPG0266BN/GxDEPG0266BN.h>    // 2.66" b/w   form DKE GROUP
// #include <GxDEPG0290R/GxDEPG0290R.h>      // 2.9" b/w/r  form DKE GROUP
// #include <GxDEPG0290B/GxDEPG0290B.h>      // 2.9" b/w    form DKE GROUP
// #include <GxGDEW029Z10/GxGDEW029Z10.h>    // 2.9" b/w/r  form GoodDisplay
// #include <GxGDEW0213Z16/GxGDEW0213Z16.h>  // 2.13" b/w/r form GoodDisplay
// #include <GxGDE0213B1/GxGDE0213B1.h>      // 2.13" b/w  old panel , form GoodDisplay
// #include <GxGDEH0213B72/GxGDEH0213B72.h>  // 2.13" b/w  old panel , form GoodDisplay
// #include <GxGDEH0213B73/GxGDEH0213B73.h>  // 2.13" b/w  old panel , form GoodDisplay
// #include <GxGDEM0213B74/GxGDEM0213B74.h>  // 2.13" b/w  form GoodDisplay 4-color
// #include <GxGDEW0213M21/GxGDEW0213M21.h>  // 2.13"  b/w Ultra wide temperature , form GoodDisplay
// #include <GxDEPG0213BN/GxDEPG0213BN.h>    // 2.13" b/w  form DKE GROUP
// #include <GxGDEW027W3/GxGDEW027W3.h>      // 2.7" b/w   form GoodDisplay
// #include <GxGDEW027C44/GxGDEW027C44.h>    // 2.7" b/w/r form GoodDisplay
// #include <GxGDEH029A1/GxGDEH029A1.h>      // 2.9" b/w   form GoodDisplay
// #include <GxDEPG0750BN/GxDEPG0750BN.h>    // 7.5" b/w   form DKE GROUP
#endif

#include GxEPD_BitmapExamples
// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMono9pt7b.h>
// #include <Fonts/Tiny3x3a2pt7b.h>
// #include <Fonts/FreeMonoBold9pt7b.h>
// #include <Fonts/FreeMonoBold12pt7b.h>
// #include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/Picopixel.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include <qrcode.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <Adafruit_INA219.h>
#include <ArduinoJson.h>
#include <Time.h>
#include <ESP32Time.h>
#include <ESPAsyncWebServer.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <EEPROM.h>

#define SERIAL_DEBUG_ON
#define SERIAL_TIME_DATE_OFF
#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 60          /* Time ESP32 will go to sleep (in seconds) */
#define SIBIU 72
#define TG_MURES 39
#define INTORSURA_BUZAULUI 72
#define DEFAULT_CITY_CODE 70
#define CITY_CODE SIBIU

GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);
QRcode qrcode(&display);
void callback(char *topic, byte *payload, unsigned int length);
void printPanel();
String printLocalTime();
void printBatStatus(float battery_proc);
void printHeader();
int getWifiStrenght();
void drawWifiStrenght(int bars);
void batteryRead(void);
void setup_clock();
String CheckString(String name, String str);
void SecureClientRead();
std::string getSerialMessages();
String list_wifi_networks();

void handleConnect(AsyncWebServerRequest *request);
void saveCredentials(String ssid, String password);
void loadCredentials(String &ssid, String &password, int &length_ssid, int &length_pass, int &end_ssid, int &end_pass);

/* Put your SSID & Password */
const char *ssid = "AC0571";              // Enter SSID here
const char *password = "BPH!tnE@UNhV4dH"; // Enter Password here

const char *apSSID = "E32_AP_WS";
const char *apPassword = "password";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
String wifi_networks;
struct tm timeinfo;
// MQTT Broker
const char *mqtt_broker = "192.168.0.235";
const char *topic_general = "emqx/esp32";
const char *topic_temperatura = "workgroup/c4ca4238a0b923820dcc509a6f75849b/air/temperature";
const char *mqtt_username = "Florin";
const char *mqtt_password = "120479@homeassistant!";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);
char mesage_mqtt[200];
float busvoltage_f;
String busvoltage_s;
Adafruit_INA219 ina219;
ESP32Time rtc(3600); // offset in seconds GMT+1
wl_status_t wifi_connection;

String weather_server = "https://www.meteoromania.ro/wp-json/meteoapi/v2/starea-vremii";
HTTPClient http;

const char *test_root_ca = "-----BEGIN CERTIFICATE-----\n"
                           "MIIF3jCCA8agAwIBAgIQAf1tMPyjylGoG7xkDjUDLTANBgkqhkiG9w0BAQwFADCB\n"
                           "iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n"
                           "cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n"
                           "BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAw\n"
                           "MjAxMDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNV\n"
                           "BAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVU\n"
                           "aGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBSU0EgQ2Vy\n"
                           "dGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK\n"
                           "AoICAQCAEmUXNg7D2wiz0KxXDXbtzSfTTK1Qg2HiqiBNCS1kCdzOiZ/MPans9s/B\n"
                           "3PHTsdZ7NygRK0faOca8Ohm0X6a9fZ2jY0K2dvKpOyuR+OJv0OwWIJAJPuLodMkY\n"
                           "tJHUYmTbf6MG8YgYapAiPLz+E/CHFHv25B+O1ORRxhFnRghRy4YUVD+8M/5+bJz/\n"
                           "Fp0YvVGONaanZshyZ9shZrHUm3gDwFA66Mzw3LyeTP6vBZY1H1dat//O+T23LLb2\n"
                           "VN3I5xI6Ta5MirdcmrS3ID3KfyI0rn47aGYBROcBTkZTmzNg95S+UzeQc0PzMsNT\n"
                           "79uq/nROacdrjGCT3sTHDN/hMq7MkztReJVni+49Vv4M0GkPGw/zJSZrM233bkf6\n"
                           "c0Plfg6lZrEpfDKEY1WJxA3Bk1QwGROs0303p+tdOmw1XNtB1xLaqUkL39iAigmT\n"
                           "Yo61Zs8liM2EuLE/pDkP2QKe6xJMlXzzawWpXhaDzLhn4ugTncxbgtNMs+1b/97l\n"
                           "c6wjOy0AvzVVdAlJ2ElYGn+SNuZRkg7zJn0cTRe8yexDJtC/QV9AqURE9JnnV4ee\n"
                           "UB9XVKg+/XRjL7FQZQnmWEIuQxpMtPAlR1n6BB6T1CZGSlCBst6+eLf8ZxXhyVeE\n"
                           "Hg9j1uliutZfVS7qXMYoCAQlObgOK6nyTJccBz8NUvXt7y+CDwIDAQABo0IwQDAd\n"
                           "BgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/BAQDAgEGMA8G\n"
                           "A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAFzUfA3P9wF9QZllDHPF\n"
                           "Up/L+M+ZBn8b2kMVn54CVVeWFPFSPCeHlCjtHzoBN6J2/FNQwISbxmtOuowhT6KO\n"
                           "VWKR82kV2LyI48SqC/3vqOlLVSoGIG1VeCkZ7l8wXEskEVX/JJpuXior7gtNn3/3\n"
                           "ATiUFJVDBwn7YKnuHKsSjKCaXqeYalltiz8I+8jRRa8YFWSQEg9zKC7F4iRO/Fjs\n"
                           "8PRF/iKz6y+O0tlFYQXBl2+odnKPi4w2r78NBc5xjeambx9spnFixdjQg3IM8WcR\n"
                           "iQycE0xyNN+81XHfqnHd4blsjDwSXWXavVcStkNr/+XeTWYRUc+ZruwXtuhxkYze\n"
                           "Sf7dNXGiFSeUHM9h4ya7b6NnJSFd5t0dCy5oGzuCr+yDZ4XUmFF0sbmZgIn/f3gZ\n"
                           "XHlKYC6SQK5MNyosycdiyA5d9zZbyuAlJQG03RoHnHcAP9Dc1ew91Pq7P8yF1m9/\n"
                           "qS3fuQL39ZeatTXaw2ewh0qpKJ4jjv9cJ2vhsE/zB+4ALtRZh8tSQZXq9EfX7mRB\n"
                           "VXyNWQKV3WKdwrnuWih0hKWbt5DHDAff9Yk2dDLWKMGwsAvgnEzDHNb842m1R0aB\n"
                           "L6KCq9NjRHDEjf8tM7qtj3u1cIiuPhnPQCjY/MiQu12ZIvVS5ljFH4gxQ+6IHdfG\n"
                           "jjxDah2nGN59PRbxYvnKkKj9\n"
                           "-----END CERTIFICATE-----\n";
String data_meteo;
String temperatura_locala;
String temp_int;
String presiunea_locala;
String viteza_vantului_locala;
String nebulozitate;
String umezeala;
String fenomene_precippitatii;
String zapada;
String actualizat;
String oras;

int page = 0;
String page_str;
const String configured_city_str = "SIBIU";
int city_code_cfg = CITY_CODE;
unsigned long ClockSec = 0; 
unsigned long Clock30Sec = 0;
unsigned long Clock1Sec = 0;
AsyncWebServer server(80);
std::string messages = "";
std::string msg;
bool new_serial_data = false;
bool access_point_configured = false;
bool trigger_reconnect = false;
String List_of_wifi_networks_HTML;
String newSSID;
String newPass;
IPAddress apIP(192, 168, 1, 2);     // Setează adresa IP a punctului de acces WiFi
IPAddress netMsk(255, 255, 255, 0); // Setează masca de subrețea
IPAddress gateway(192, 168, 1, 1);  // Setează gateway-ul

void saveCredentials(String ssid, String password)
{
  String ssid_local = ssid;
  String password_local = password;
  int length_ssid = ssid.length();
  int length_pass = password.length();

  ssid_local = String(length_ssid) + char(2)  + ssid_local + char(3);
  password_local = String(length_pass) + char(2) + password_local + char(3);
  EEPROM.begin(512); // Inițializează EEPROM
  
  // Erase 64 de bytesM
  for (int i = 0; i < 64; ++i)
  {
    EEPROM.write(i, ssid_local[i]);
  }

  // Salvează SSID-ul în primele 32 de bytes ale EEPROM
  for (int i = 0; i < ssid_local.length(); ++i)
  {
    EEPROM.write(i, ssid_local[i]);
  }

  // Salvează parola în următoarele 64 de bytes ale EEPROM
  for (int i = 0; i < password_local.length(); ++i)
  {
    EEPROM.write(32 + i, password_local[i]);
  }

  EEPROM.commit(); // Salvează datele în EEPROM
  EEPROM.end();    // Termină utilizarea EEPROM
}
void loadCredentials(String &ssid, String &password, int &length_ssid, int &length_pass,int &end_ssid, int &end_pass)
{
  String ssid_local = "";
  String password_local = "";
  int loc_end_length_ssid =0;
  int loc_end_length_pass = 0;
  int loc_end_ssid = 0;
  int loc_end_pass = 0;
  EEPROM.begin(512); // Inițializează EEPROM

  // Încarcă SSID-ul din primele 32 de bytes ale EEPROM
  ssid_local = "";
  for (int i = 0; i < 32; ++i)
  {
    char c = EEPROM.read(i);
    if (c == '\0')
      break;
    if (c == '\2')
    {
      loc_end_length_ssid = i;
    }
    if (c == '\3')
    {
      loc_end_ssid = i;
    }

    ssid_local += c;
  }

  // Încarcă parola din următoarele 64 de bytes ale EEPROM
  password_local = "";
  for (int i = 32; i < 96; ++i)
  {
    char c = EEPROM.read(i);
    if (c == '\0')
      break;
    if (c == '\2')
    {
      loc_end_length_pass = i-32;
    }
    if (c == '\3')
    {
      loc_end_pass = i-32;
    }
    password_local += c;
  }

  ssid = ssid_local;
  password = password_local;
  length_ssid = loc_end_length_ssid;
  length_pass = loc_end_length_pass;
  end_ssid = loc_end_ssid;
  end_pass = loc_end_pass;
  EEPROM.end(); // Termină utilizarea EEPROM
}
String list_wifi_networks()
{
  String responseHTML = "<select id='ssid' name=\"ssid\">"; //<input type=\"text\" name=\"ssid\">
  // Scanarea rețelelor WiFi disponibile
  Serial.println("Scanning for networks...");
  // Scanarea rețelelor WiFi disponibile
  int numNetworks = WiFi.scanNetworks();
  if (numNetworks == 0)
  {
    responseHTML += "<option value='no wifi'>No networks found</option>";
  }
  else
  {
    for (int i = 0; i < numNetworks; ++i)
    {
      responseHTML += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
    }
  }

  responseHTML += "</select>";
  return responseHTML;
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

std::string getSerialMessages()
{
  std::string messages;
  // Implementează logica pentru a citi mesajele de pe portul serial și le adaugă la sirul "messages"
  while (Serial.available())
  {
    char c = Serial.read();
    messages += c;
  }
  return messages;
}

void Setup_and_Start_Server()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
  String html_script = R"(  <!-- Alte elemente head... -->
  <script>
    function getSerialMessages() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) 
        {
          // Actualizeaza lista de mesaje cu raspunsul primit
          document.getElementById("messages").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/getMessages", true);
      xhttp.send();
    }
    function clearMessages() {
      // Sterge continutul text box-ului
      document.getElementById("messages").value = '';
    }
    // Actualizează mesajele la fiecare 2 secunde (sau în intervalul dorit)
    setInterval(getSerialMessages, 2000);
  </script>)";
    String html = "<html><head>";
    html += html_script;
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; text-align: center; }";
    html += "button { padding: 10px 20px; font-size: 16px; margin: 10px; }";
    html += "</style>";
    html += "</head><body>";
    html += "<h1>Weather station configuration </h1>";

    html += "<form action=\"/connect\" method=\"post\">";
    html += "SSID: " + List_of_wifi_networks_HTML + "<br>"; //<input type=\"text\" name=\"ssid\">
    html += "Password: <input type=\"password\" name=\"password\"><br>";
    html += "<input type=\"submit\" value=\"Connect\"></form><br> <hr>";
    html += "<br><label for='Wifi network'>Select Wifi network:</label>";
    html += "<br>" + List_of_wifi_networks_HTML;

    //html += "<label for='SSID'>SSID</label>";
    //html += "<input type='string' id='SSID' placeholder='Input SSID'><br>";

    html += "<br><label for='password'>password</label>";
    html += "<input type='string' id='password' placeholder='Input password'><br>";
    html += "<button id='reconnect'>Reconect Wifi</button>";

    html += "<br><br><button id='saveButton'>Save Credentials</button>";
    html += "<button id='stopButton'>Stop</button>";
    html += "<button id='backwardButton'>Backward</button>";
   // html += "<br><button onclick = 'clearMessages()' id='clearButton'> Clear serial messages</button>";
    html += "<!--Lista pentru afisarea mesajelor-->";
    // html += "<br><ul id=\"messages\"> </ul>";
    html += "<br><textarea id = 'messages' rows = '20' cols = '50'></textarea>";
    html += "<br><button onclick = 'clearMessages()' id='clearButton'> Clear serial messages</button>";
    html += "<script>";
    html += "document.getElementById('startRotationButton').onclick = function() {";
    html += "   var rotations = document.getElementById('rotationsInput').value;";
    html += "   var direction = document.getElementById('directionSelect').value;";
    html += "   sendCommand('/startRotation?rotations=' + rotations + '&direction=' + direction);";
    html += "};";
    html += "document.getElementById('saveButton').onclick = function() {";
    html += "   sendCommand('/save');";
    html += "};";
    html += "document.getElementById('stopButton').onclick = function() {";
    html += "   sendCommand('/stop');";
    html += "};";
    html += "document.getElementById('backwardButton').onclick = function() {";
    html += "   sendCommand('/backward');";
    html += "};";
    html += "document.getElementById('clearButton').onclick = function() {";
    html += "   document.getElementById(\"messages\").value = '';";
    html += "};";
    html += "function sendCommand(command) {";
    html += "   var xhr = new XMLHttpRequest();";
    html += "   xhr.open('GET', command, true);";
    html += "   xhr.send();";
    html += "}";
    html += "</script>";
    html += "</body></html>";

    request->send(200, "text/html", html); });

  server.on("/connect", HTTP_POST, handleConnect);

  server.on("/success", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("Reconnection successfully done !");
    request->send(200, "text/plain", "success"); 
  });
  server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", "forward"); 
  });

  server.on("/backward", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", "backward"); 
  });

  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("Stop spinning !");
    request->send(200, "text/plain", "stop"); 
  });

  server.on("/startRotation", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    String rotationsParam = request->getParam("rotations")->value();
    //directionParam = request->getParam("direction")->value();
    request->send(200, "text/plain", "Free Rotation"); 
  });

  // Configurare rută pentru obținerea mesajelor de pe portul serial
  server.on("/getMessages", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    // Obtinerea mesajelor de pe portul serial și trimiterea către client
    msg = getSerialMessages();
    if (msg.empty() == false)
    {
      messages = messages + '\n' + ">>" + msg;
      new_serial_data = true;
    }
    request->send(200, "text/plain", String(messages.c_str())); 
  });

  server.onNotFound(notFound);
  server.begin();
  Serial.println("HTTP server started");  
}

void handleConnect(AsyncWebServerRequest *request)
{
  String network = newSSID;
  String password = newPass;
  Serial.println("handleConnect...");

  if (request->method() == HTTP_POST)
  {
    if (request->hasParam("ssid", true) && request->hasParam("password", true))
    {
      AsyncWebParameter *ssidParam = request->getParam("ssid", true);
      AsyncWebParameter *passwordParam = request->getParam("password", true);

      newSSID = ssidParam->value();
      newPass = passwordParam->value();
      Serial.println(newSSID);
      Serial.println(newPass);
      trigger_reconnect = true;
      // Redirecționează utilizatorul către o pagină care indică succesul
      request->redirect("/");
      return;
    }
  }
}

void handleSaveCredentials(AsyncWebServerRequest *request)
{
  String network = newSSID;
  String password = newPass;
  Serial.println("handleSaveCredentials...");

  if (request->method() == HTTP_POST)
  {
    if (request->hasParam("ssid", true) && request->hasParam("password", true))
    {
      AsyncWebParameter *ssidParam = request->getParam("ssid", true);
      AsyncWebParameter *passwordParam = request->getParam("password", true);

      newSSID = ssidParam->value();
      newPass = passwordParam->value();
      Serial.println(newSSID);
      Serial.println(newPass);
      trigger_reconnect = true;
      // Redirecționează utilizatorul către o pagină care indică succesul
      request->redirect("/");
      return;
    }
  }
}

String CheckString(String name, String str)
{
  if ((str == "null") || (str.isEmpty() == true))
  {
    return "NA";
  }
  else
  {
    return name;
  }
}
void SecureClientRead()
{
  WiFiClientSecure *clientSec = new WiFiClientSecure;
  if (clientSec)
  {
    clientSec->setCACert(test_root_ca);
    {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
      HTTPClient https;
      https.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);

      Serial.print("[HTTPS] begin...\n");
      if (https.begin(*clientSec, weather_server))
      { // HTTPS
        Serial.print("[HTTPS] GET...\n");
        // start connection and send HTTP header
        int httpCode = https.GET();

        // httpCode will be negative on error
        if (httpCode > 0)
        {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
          {
#ifdef SERIAL_DEBUG_ON
            String payload = https.getString();
            Serial.println(payload);
#endif
            // Parse response
            DynamicJsonDocument doc(65000);
            DynamicJsonDocument doc_city(2000);
            deserializeJson(doc, https.getStream());
#ifdef SERIAL_DEBUG_ON
            Serial.println(doc["features"][CITY_CODE].as<String>());
#endif
            if (city_code_cfg == 0)
            {
              for (int i = 0; i < 161; i++)
              {
                doc_city = doc["features"][i];
                deserializeJson(doc_city, doc["features"][i].as<String>());
                oras = doc_city["properties"]["nume"].as<String>();
                if (oras == configured_city_str)
                {
                  city_code_cfg = i;
#ifdef SERIAL_DEBUG_ON
                  Serial.printf("\nCity Code CFG: %d\n", city_code_cfg);
#endif
                }
              }
            }
            else
            {
              doc_city = doc["features"][city_code_cfg];
              deserializeJson(doc_city, doc["features"][city_code_cfg].as<String>());
              // Read values
              data_meteo = doc["date"].as<String>();
#ifdef SERIAL_DEBUG_ON
              Serial.println(doc["features"][city_code_cfg].as<String>());
              Serial.println(doc_city["properties"]["tempe"].as<String>());
              Serial.println(doc["date"].as<String>());
#endif
              oras = doc_city["properties"]["nume"].as<String>();
              temperatura_locala = doc_city["properties"]["tempe"].as<String>();
              presiunea_locala = doc_city["properties"]["presiunetext"].as<String>();
              viteza_vantului_locala = doc_city["properties"]["vant"].as<String>();
              nebulozitate = doc_city["properties"]["nebulozitate"].as<String>();
              umezeala = doc_city["properties"]["umezeala"].as<String>();
              fenomene_precippitatii = doc_city["properties"]["fenomen_e"].as<String>();
              zapada = doc_city["properties"]["zapada"].as<String>();
              actualizat = doc_city["properties"]["actualizat"].as<String>();
            }
          }
        }
        else
        {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }

        https.end();
      }
      else
      {
        Serial.printf("[HTTPS] Unable to connect\n");
      }

      // End extra scoping block
    }
    delete clientSec;
  }
  else
  {
    Serial.println("Unable to create client");
  }
}

wl_status_t wifi_connect(String ssid, String password,bool new_connection)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  List_of_wifi_networks_HTML = list_wifi_networks();
  Serial.println(List_of_wifi_networks_HTML);
  Serial.println(ssid);
  Serial.println(password);
  if (new_connection == true)
  {
    WiFi.begin(ssid.c_str(), password.c_str());
    int new_connection_in_progress = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      new_connection_in_progress++;
#ifdef SERIAL_DEBUG_ON
      Serial.print(".");
#endif
      if (new_connection_in_progress > 30)
      {
        break;
      }
      Serial.printf(" \nConnection attempt %d ..", new_connection_in_progress);
    }
    Serial.println("");
    Serial.printf("Connected to WiFi %s network with IP Address: %s", newSSID.c_str(),WiFi.localIP().toString());
    trigger_reconnect = false;
    saveCredentials(newSSID, newPass);
    return WiFi.status();
  }
  else
  {
    WiFi.begin(ssid.c_str(), password.c_str());
  }
#ifdef SERIAL_DEBUG_ON
  Serial.println("Connecting");
#endif
  int connection_in_progress=0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    connection_in_progress++;
#ifdef SERIAL_DEBUG_ON
    Serial.print(".");
#endif
    if (connection_in_progress > 30)
    {
      break;
    }
    Serial.printf(" \nConnection attempt %d ..", connection_in_progress);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    // Creează un hotspot WiFi
    WiFi.softAPConfig(apIP, gateway, netMsk); // Configurează adresa IP a punctului de acces WiFi
    WiFi.softAP(apSSID, apPassword);
    Serial.println("");
    Serial.println("Access point configured !");
    Serial.println(WiFi.softAPIP());
    access_point_configured = true;
  }
  else
  {

  #ifdef SERIAL_DEBUG_ON
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
  #endif
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    saveCredentials(ssid, password);
    access_point_configured = false;
  }
  return WiFi.status();
}

void setup(void)
{
  String saved_SSID;
  String saved_password;
#ifdef SERIAL_DEBUG_ON
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
#endif
  int length_ssid = 0;
  int length_pass = 0;
  int end_ssid = 0;
  int end_pass = 0;
  loadCredentials(saved_SSID, saved_password, length_ssid, length_pass, end_ssid, end_pass);
  Serial.printf("\nLength ssid %d - end ssid %d", length_ssid, end_ssid);
  Serial.printf("\nLength pass %d - end pass %d\n", length_pass, end_pass);
  Serial.println(saved_SSID.substring(length_ssid + 1, end_ssid));
  Serial.println(saved_password.substring(length_pass + 1, end_pass));
  saved_SSID = saved_SSID.substring(length_ssid + 1, end_ssid);
  saved_password = saved_password.substring(length_pass + 1, end_pass);

  if (saved_SSID.isEmpty() && saved_password.isEmpty())
  {
    wifi_connect(String(ssid), String(password), trigger_reconnect);
  }
  else
  {
    wifi_connect(saved_SSID.c_str(),saved_password.c_str(),trigger_reconnect);
  }


  if (WiFi.status() != WL_CONNECTED)
  {
     // Creează un hotspot WiFi
     //WiFi.softAP(apSSID, apPassword);
  }
  else
  {
#ifdef SERIAL_DEBUG_ON
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
  #endif
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    setup_clock();
  #ifdef SERIAL_DEBUG_ON
    Serial.println(printLocalTime());
  #endif
  }
  Setup_and_Start_Server();

#if defined(LILYGO_EPD_DISPLAY_102)
  pinMode(EPD_POWER_ENABLE, OUTPUT);
  digitalWrite(EPD_POWER_ENABLE, HIGH);
#endif /*LILYGO_EPD_DISPLAY_102*/
#if defined(LILYGO_T5_V102)
  pinMode(POWER_ENABLE, OUTPUT);
  digitalWrite(POWER_ENABLE, HIGH);
#endif /*LILYGO_T5_V102*/
  

  SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
  display.init(); // enable diagnostic output on Serial


#ifdef SERIAL_DEBUG_ON
  Serial.println("setup done");
#endif
  printPanel();
  qrcode.init(255, 122);
  display.setCursor(2, 60);
  qrcode.debug();
  if (WiFi.status() == WL_CONNECTED)
  {
    // connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    int mqtt_connection_attempts = 0;
    while (!client.connected())
    {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
        {
          Serial.println("Public EMQX MQTT broker connected");
          // Publish and subscribe
          client.publish(topic_general, "Hi, I'm ESP32 LilyGo T5 ");
          client.subscribe(topic_general);
          client.subscribe(topic_temperatura);
        }
        else
        {
          Serial.println("failed with state ");
          Serial.print(client.state());
          if (mqtt_connection_attempts > 10)
          {
            break;
          }
          delay(2000);
        }
        mqtt_connection_attempts++;
     }
   }
   display.update();
}

void loop()
{
  if(trigger_reconnect == true)
  {
    wl_status_t newCon;
    newCon = wifi_connect(newSSID, newPass, trigger_reconnect);
    if(newCon == WL_CONNECTED)
    {
      saveCredentials(newSSID, newPass);
      trigger_reconnect = false;
    }
  }
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMono9pt7b);
  display.setCursor(2, 20);
  display.println(printLocalTime().c_str());
  
  if (millis() - ClockSec >= 500)
  { // 500 milisec = 1/2 sec
    display.updateWindow(0, 250, 450, 40, true); // update clock window
    // update 500ms clock 
    ClockSec = millis();
  }
  display.setCursor(2, 40);
  display.setCursor(103, 20);
  if (WiFi.status() == WL_CONNECTED)
  {
    int bars = getWifiStrenght();
    if (access_point_configured == false)
    {
      String wifi_name = WiFi.SSID();
      int length = wifi_name.length();
      if(length > 6)
      {
        wifi_name = wifi_name.substring(0, 6) + ".(" + String(getWifiStrenght()) + ")";
      }
      else
      {
        wifi_name = WiFi.SSID() + "(" + String(getWifiStrenght()) + ")";
      }
      display.printf("%s", wifi_name.c_str()); // WiFi.SSID().c_str(), getWifiStrenght());
    }
    drawWifiStrenght(bars);
    
    client.loop();
    if (millis() - Clock30Sec >= 30000)
    { // 30000 milisec = 30 sec
      SecureClientRead();
      // update clock variable
      Clock30Sec = millis();
    }
  }
  else
  {
    if (access_point_configured == true)
    {
      display.printf("%s", WiFi.softAPIP().toString());
    }
    else
    {
      display.println("NC");
    }
  }
  printPanel();

  /*Title */
  display.setFont(&FreeMono9pt7b);
  display.setCursor(2, 40);
  display.println(&timeinfo, "%d-%b-%Y");
  display.println("T ext " + temperatura_locala + "C");
  display.setCursor(2, 73);
  display.println("T int " + temp_int.substring(1,6)+"C");
  display.setCursor(2, 88);
  display.println("Hum "+ umezeala+ "%");
  int virgula = viteza_vantului_locala.indexOf(',');
  String doar_viteza = viteza_vantului_locala.substring(0, virgula);
  display.setCursor(2, 103);
  display.println("Wind " + doar_viteza);

  /* Content*/
  display.setCursor(2, 60);
  if(access_point_configured == true)
  {
    // create qrcode for access point address
    qrcode.create("http://192.168.2.1:80");
  }
  else
  {
    // create qrcode
    qrcode.create("https://www.meteoromania.ro/vremea/starea-vremii-romania/");
  }
  /* Footer*/
  display.setFont(&FreeMono9pt7b);
  display.setCursor(2, 123);
  display.print("");
  int virgula_pres = presiunea_locala.indexOf(',');
  String doar_presiunea = presiunea_locala.substring(0, virgula_pres);
  display.print("Atm " + doar_presiunea);

  if (millis() - Clock1Sec >= 1000)
  {
    // display.update();
    display.updateWindow(100, 6, 170, 20, true); // update wifi name
    display.updateWindow(0, 6, 250, 40, true);   // update date window
    display.updateWindow(0, 45, 250, 65, true);  // update content window
    display.updateWindow(0, 108, 250, 20, true); // update footer
    // Actualizăm momentul ultimului apel
    Clock1Sec = millis();
  }
#ifdef INA219_CHECK
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (!ina219.begin())
  {
    Serial.println("Failed to find INA219 chip");
    while (1)
    {
      delay(10);
    }
  }
  else
  {
    // display.updateWindow(230, 6, 20, 20, false);
  }
#endif    
  
}

/*----------------------------------------------------*/
void printPanel()
{

  printHeader();
  /*               x  y width height           */
  /* Title*/
  display.drawRect(0, 25, 150, 20, GxEPD_BLACK); // clock

  /*Content*/
  display.drawRect(0, 45, 150, 65, GxEPD_BLACK);  // content window which contain 

  /*footer*/
  display.drawRect(0, 108, 150, 20, GxEPD_BLACK); // footer panel
}

void printHeader()
{
  /*               x  y width height           */
  display.drawRect(0, 6, 100, 20, GxEPD_BLACK);   // date
  display.drawRect(100, 6, 170, 20, GxEPD_BLACK); // wifi name 
  display.drawRect(230, 6, 20, 20, GxEPD_BLACK);  // battery panel
  printBatStatus(busvoltage_f / 6.0 * 100.0);
}

void printBatStatus(float battery_proc)
{
  int number_of_lines_to_draw = (int)round((battery_proc / 100.0 * 12.0));
#ifdef SERIAL_DEBUG_DISPLAY_ON
  Serial.printf("Battery procentage: %f\n", battery_proc);
  Serial.printf("Nr de linii: %d", number_of_lines_to_draw);
#endif
  if (number_of_lines_to_draw > 14)
  {
    number_of_lines_to_draw = 14;
  }
  /* Draw empty battery*/
  /*               x  y width height           */
  display.drawRect(235, 10, 10, 14, GxEPD_BLACK);
  display.drawFastHLine(238, 8, 4, GxEPD_BLACK);
  display.drawFastHLine(238, 9, 4, GxEPD_BLACK);

  /* Fill battery based on battery_proc parameter */
  for (int i = 0; i < number_of_lines_to_draw; i++)
  {
    display.drawFastHLine(235, 22 - i, 10, GxEPD_BLACK);
  }

  if (number_of_lines_to_draw < 3)
  {
    display.drawLine(230, 6, 250, 26, GxEPD_BLACK);
  }
  else
  {
    display.setCursor(236, 20);
    display.setFont(&Picopixel);
    display.setTextColor(GxEPD_WHITE);
    display.printf("%1.1f", busvoltage_f);
  }
  display.setTextColor(GxEPD_BLACK);
}

void drawWifiStrenght(int bars)
{
  /*               x  y width height           */
  for (int i = 1; i <= bars; i++)
  {
    if (i == 5)
    {
      display.fillRect(204 + (i * 4), 26 - (i * 4) + 2, 3, (i * 4) - 5, GxEPD_BLACK);
#ifdef SERIAL_DEBUG_DISPLAY_ON
      Serial.printf("\nHeight max: %d", (i * 4) - 5);
#endif
    }
    else
    {
      display.fillRect(204 + (i * 4), 26 - (i * 4), 3, (i * 4) - 3, GxEPD_BLACK);
#ifdef SERIAL_DEBUG_DISPLAY_ON
      Serial.printf("\nHeight: %d", (i * 4) - 3);
#endif
    }
  }
}

String printLocalTime()
{
  if (!getLocalTime(&timeinfo))
  {
#ifdef SERIAL_TIME_DATE_ON
    Serial.println("Failed to obtain time");
#endif
    return String("NC");
  }
#ifdef SERIAL_TIME_DATE_ON
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  char timeHMS[12];
  strftime(timeHour, 3, "%H", &timeinfo);
  strftime(timeHMS, 11, "%H:%M:%S", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay, 10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
#endif
  char timeDate[36];
  strftime(timeDate, 36, "%H:%M:%S", &timeinfo);
  return String(timeDate);
}

int getWifiStrenght()
{
  int bars;
  int RSSI_strgh = 0;
  // simple if then to set the number of bars
  RSSI_strgh = WiFi.RSSI();
  if (RSSI_strgh > -55)
  {
    bars = 5;
  }
  else if ((RSSI_strgh < -55) && (RSSI_strgh > -65))
  {
    bars = 4;
  }
  else if ((RSSI_strgh < -65) && (RSSI_strgh > -70))
  {
    bars = 3;
  }
  else if ((RSSI_strgh < -70) && (RSSI_strgh > -78))
  {
    bars = 2;
  }
  else if ((RSSI_strgh < -78) & (RSSI_strgh > -82))
  {
    bars = 1;
  }
  else
  {
    bars = 0;
  }
  return bars;
}

void callback(char *topic, byte *payload, unsigned int length)
{
  
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  snprintf(mesage_mqtt, length+1, "%s", payload);
  String msg(mesage_mqtt);
  int dpct = msg.indexOf(':');
  int br = msg.indexOf('}');
  temp_int = msg.substring(dpct, br);
  Serial.printf("%s", mesage_mqtt);
  Serial.println();
  Serial.println("-----------------------");
}
#ifdef INA219_CHECK
void batteryRead(void)
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  Serial.print("Bus Voltage:   ");
  Serial.print(busvoltage);
  busvoltage_s = String(loadvoltage);
  busvoltage_f = loadvoltage;
  Serial.println(" V");
  Serial.print("Shunt Voltage: ");
  Serial.print(shuntvoltage);
  Serial.println(" mV");
  Serial.print("Load Voltage:  ");
  Serial.print(loadvoltage);
  Serial.println(" V");
  Serial.print("Current:       ");
  Serial.print(current_mA);
  Serial.println(" mA");
  Serial.print("Power:         ");
  Serial.print(power_mW);
  Serial.println(" mW");
  Serial.println("");

  delay(2000);
}
#endif
void setup_clock()
{
  rtc.setTime(00, 29, 19, 10, 3, 2024); // 3th March 2024 20:18:30
  if (WiFi.status() == WL_CONNECTED)
  {
    /*---------set with NTP---------------*/
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
      rtc.setTimeStruct(timeinfo);
    }
  }
  else
  {  
    //wifi_connection = wifi_connect();
  }

}