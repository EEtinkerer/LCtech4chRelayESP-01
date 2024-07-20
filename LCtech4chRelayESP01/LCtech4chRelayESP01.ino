#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

WiFiServer server(80);

void setup() {
  Serial.begin(115200); // Start serial communication with the relay module

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Parse request and send the appropriate command
    if (request.indexOf("/relay1/on") != -1) {
      sendCommand("A0 01 01 A2");
    } else if (request.indexOf("/relay1/off") != -1) {
      sendCommand("A0 01 00 A1");
    } else if (request.indexOf("/relay2/on") != -1) {
      sendCommand("A0 02 01 A3");
    } else if (request.indexOf("/relay2/off") != -1) {
      sendCommand("A0 02 00 A2");
    } else if (request.indexOf("/relay3/on") != -1) {
      sendCommand("A0 03 01 A4");
    } else if (request.indexOf("/relay3/off") != -1) {
      sendCommand("A0 03 00 A3");
    } else if (request.indexOf("/relay4/on") != -1) {
      sendCommand("A0 04 01 A5");
    } else if (request.indexOf("/relay4/off") != -1) {
      sendCommand("A0 04 00 A4");
    }
    
    client.stop();
  }
}

void sendCommand(String command) {
  Serial.write(parseHex(command));
}

uint8_t parseHex(String hexString) {
  uint8_t byteArray[hexString.length() / 2];
  for (int i = 0; i < hexString.length(); i += 2) {
    byteArray[i / 2] = (hexCharToInt(hexString.charAt(i)) << 4) | hexCharToInt(hexString.charAt(i + 1));
  }
  return byteArray;
}

uint8_t hexCharToInt(char ch) {
  if (ch >= '0' && ch <= '9') {
    return ch - '0';
  } else if (ch >= 'A' && ch <= 'F') {
    return ch - 'A' + 10;
  } else if (ch >= 'a' && ch <= 'f') {
    return ch - 'a' + 10;
  }
  return 0;
}
