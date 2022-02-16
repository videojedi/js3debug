#include "defines.h"
#include "index.h"
#include "css.h"
#include "script.h"
#include "script2.h"
#include "logo.h"


#define WEBSOCKETS_USE_ETHERNET     true
#include <WebSockets2_Generic.h>                      
#include <ArduinoJson.h>                              
#include <EEPROM.h>

// define the number of bytes you want to access
#define EEPROM_SIZE 192

StaticJsonDocument<2000> doc_tx;                 
StaticJsonDocument<2000> doc_rx;

////IP structure
struct netSetup {
  byte mac[6];
  uint32_t ipAddr;
  uint32_t subnet;
  uint32_t gw;
  uint32_t rtrIPaddr;
  uint16_t rtrPort;
  uint8_t protocol;
  uint8_t sn;
  bool dhcp;
};

netSetup setupIP;

using namespace websockets2_generic;

EthernetWebServer server(80);
EthernetClient client; //connection to hardware

const byte maxSocketClients = 4;

WebsocketsClient socketClients[maxSocketClients];
WebsocketsServer webSocket;

unsigned long STATUS_CHECK_INTERVAL = 1000L;

//------------------------------------------------------------------------------------------------------------------------------------
void(* resetFunc) (void) = 0;//declare reset function @ address 0

//------------------------------------------------------------------------------------------------------------------------------------
void handleRoot() {
  server.send(200, ("text/html"), F(webpage));
}
//------------------------------------------------------------------------------------------------------------------------------------
void handleCSS() {
  server.send_P(200, "text/css", css);
}
//------------------------------------------------------------------------------------------------------------------------------------
void handleScript() {
  server.send_P(200, "*/*", script);
}
void handleScript2() {
  server.send_P(200, "*/*", script2);
}
//------------------------------------------------------------------------------------------------------------------------------------
void handleLogo() {
  server.send_P(200, "text/plain", logo);
}
//404 webpage--------------------------------------------------------------------------------------------------------------------------
void handleNotFound() {
  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, F("text/plain"), message);
}
void watchDogCallback() {
  Serial.println("Resetting Soon..");
}
//------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  readConfig();
  Serial.begin(115200);
  //while (!Serial);
  stdPrint = &Serial;
  
  Serial.print(F("\nStarting AdvancedWebServer on ")); Serial.print(F(BOARD_NAME));
  Serial.print(F(" " )); Serial.println(SHIELD_TYPE);
  Serial.println(ETHERNET_WEBSERVER_VERSION);
  uint8_t mac[6];
  Ethernet.macAddress(mac);
  printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n",
         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  // start the ethernet connection and the server:
  // Use DHCP dynamic IP
  if (setupIP.dhcp) {
    Serial.println("DHCP");
    Ethernet.begin();
  }
  else {
    // Use Static IP
    Serial.println("Static");
    Ethernet.begin(setupIP.ipAddr, setupIP.subnet, setupIP.gw);
  }
  Serial.println(F("========================="));

  if (!Ethernet.waitForLocalIP(5000)) {
    Serial.println("Failed to configure Ethernet");
    if (!Ethernet.linkStatus()) {
      Serial.println("Ethernet cable is not connected.");
    }
    // Stay here forever
    while (true) {
      delay(1);
    }
  }
  else {
    Serial.printf("IP Address = ");
    Serial.println(Ethernet.localIP());
  }
  server.on(F("/"), handleRoot);
  server.on(F("/styles.css"), handleCSS);
  server.on(F("/script.js"), handleScript);
  server.on(F("/script2.js"), handleScript2);
  server.on("/logo", handleLogo);
  server.onNotFound(handleNotFound);
  server.begin();
  webSocket.listen(8080);
}
//------------------------------------------------------------------------------------------------------------------------------------
void check_status() {
  static unsigned long checkstatus_timeout = 0;
  // Send status report every STATUS_REPORT_INTERVAL (60) seconds
  if (millis() > checkstatus_timeout) 
  {
    checkstatus_timeout = millis() + STATUS_CHECK_INTERVAL;
    Serial.print(".");
    //comment following line to allow web gui to run again after crash
    PollHardware(); 
  }
}
//------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  server.handleClient();
  listenForSocketClients();
  pollSocketClients();
  check_status();

}


//------------------------------------------------------------------------------------------------------------------------------------
String IpAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") + \
         String(ipAddress[1]) + String(".") + \
         String(ipAddress[2]) + String(".") + \
         String(ipAddress[3]);
}
//------------------------------------------------------------------------------------------------------------------------------------
IPAddress StringAddress2IP(const char * string) {
  IPAddress sendVal;
  if (sendVal.fromString(string)) {
    return sendVal;
  }
    return -1;
}
//------------------------------------------------------------------------------------------------------------------------------------
String sendAllvalues() {
  String jsonString = "";                           // create a JSON string for sending data to the client
  JsonObject object = doc_tx.to<JsonObject>();      // create a JSON Object
  object["protocol"] = "SWP-08";//setupIP.protocol;
  object["rtrIPaddr"] = IpAddress2String(setupIP.rtrIPaddr);
  object["rtrPort"] = setupIP.rtrPort;
  if(!setupIP.dhcp){
  object["ipAddr"] = IpAddress2String(setupIP.ipAddr);
  object["subnet"] = IpAddress2String(setupIP.subnet);
  object["gw"] = IpAddress2String(setupIP.gw);
  }
  else {
  object["ipAddr"] = IpAddress2String(Ethernet.localIP());
  object["subnet"] = IpAddress2String(Ethernet.subnetMask());
  object["gw"] = IpAddress2String(Ethernet.gatewayIP());
  }
  object["dhcp"] = setupIP.dhcp;
  serializeJson(doc_tx, jsonString);                // convert JSON object to string
  Serial.print("Sending ");
  Serial.println(jsonString);
  return jsonString;               // send JSON string to clients
}
//------------------------------------------------------------------------------------------------------------------------------------
int8_t getFreeSocketClientIndex() {
  // If a client in our list is not available, it's connection is closed and we
  // can use it for a new client.
  for (byte i = 0; i < maxSocketClients; i++)
  {
    if (!socketClients[i].available())
      return i;
  }
  return -1;
}

//------------------------------------------------------------------------------------------------------------------------------------
void handleMessage(WebsocketsClient &client, WebsocketsMessage message) {
  auto data = message.data();

  // Log message
  Serial.print(F("Got Message: "));
  Serial.println(data);

  //try to decipher the JSON string received
  DeserializationError error = deserializeJson(doc_rx, data);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  else {
    if (!doc_rx["protocol"].isNull()) {
      setupIP.dhcp = doc_rx["dhcp"];
      setupIP.protocol = 0;
      setupIP.rtrIPaddr = StringAddress2IP(doc_rx["rtrIPaddr"]);
      setupIP.rtrPort = doc_rx["rtrPort"];
      setupIP.ipAddr = StringAddress2IP(doc_rx["ipAddr"]);
      setupIP.subnet = StringAddress2IP(doc_rx["subnet"]);
      setupIP.gw = StringAddress2IP(doc_rx["gw"]);
      sendAll();
    }
    // JSON string was received correctly, so information can be retrieved:
    const bool saveBTN = doc_rx["save"];
    if (saveBTN == true) {
      saveConfig();
    }
    const bool resetBTN = doc_rx["reset"];
    if (resetBTN == true) {
      resetFunc();  //call reset
    }
    if (!doc_rx["tally"].isNull()) {
      const uint8_t tally = doc_rx["tally"];
      const bool pushed = doc_rx["pushed"];
      pushState();
    }
    if (!doc_rx["mode"].isNull()) {
      const uint8_t mode_index = doc_rx["mode"];
      pushMode();
    }
    const bool dhcpBTN = doc_rx["dhcp"];
    if (dhcpBTN) {
      setupIP.dhcp = dhcpBTN;
    }
  }
}
//------------------------------------------------------------------------------------------------------------------------------------
void handleEvent(WebsocketsClient &client, WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connection closed");
  }
}
//------------------------------------------------------------------------------------------------------------------------------------
void listenForSocketClients() {
  if (webSocket.poll()) {
    int8_t freeIndex = getFreeSocketClientIndex();

    if (freeIndex >= 0) {
      WebsocketsClient newClient = webSocket.accept();
      Serial.printf("Accepted new websockets client at index %d\n", freeIndex);
      newClient.onMessage(handleMessage);
      newClient.onEvent(handleEvent);
      newClient.send(sendAllvalues());
      socketClients[freeIndex] = newClient;
    }
  }
}
//------------------------------------------------------------------------------------------------------------------------------------
void sendAll() {
  for (int i = 0; i < maxSocketClients; i++)
  {
    if (socketClients[i].available()) {
      socketClients[i].send(sendAllvalues());
    }
  }
}
//------------------------------------------------------------------------------------------------------------------------------------
void pushState() {
  String jsonString = "";
  JsonObject object = doc_tx.to<JsonObject>();
  JsonArray gpis = object.createNestedArray("gpis");
  serializeJson(doc_tx, jsonString);                // convert JSON object to string
  Serial.println(jsonString);
  for (int i = 0; i < maxSocketClients; i++)
  {
    if (socketClients[i].available()) {
      socketClients[i].send(jsonString);
    }
  }
}

//------------------------------------------------------------------------------------------------------------------------------------
void pushMode() {
  String jsonString = "";
  JsonObject object = doc_tx.to<JsonObject>();
  JsonArray gpis = object.createNestedArray("gpis");
  serializeJson(doc_tx, jsonString);                // convert JSON object to string
  Serial.println(jsonString);
  for (int i = 0; i < maxSocketClients; i++) {
    if (socketClients[i].available()) {
      socketClients[i].send(jsonString);
    }
  }
}

//------------------------------------------------------------------------------------------------------------------------------------
void pollSocketClients() {
  for (byte i = 0; i < maxSocketClients; i++) {
    socketClients[i].poll();
  }
}

//------------------------------------------------------------------------------------------------------------------------------------
void saveConfig() {
  EEPROM.put(0, setupIP);
  Serial.println("Values Saved");
}

//------------------------------------------------------------------------------------------------------------------------------------
void readConfig() {
  EEPROM.get(0, setupIP);
  Serial.println("Values Loaded");
}

//------------------------------------------------------------------------------------------------------------------------------------
void PollHardware() { 
  Serial.print("get ");
//client.setConnectionTimeout(100);
  if (client.connect(setupIP.rtrIPaddr, setupIP.rtrPort)) {
    Serial.println("Connected");
  } else {
    // if you didn't get a connection to the server:
    Serial.println("Connection failed!");
    client.close();

  }
  return;
}
