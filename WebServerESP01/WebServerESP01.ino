#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <uri/UriBraces.h>
#include <uri/UriRegex.h>


const char *ssid = "SSID_NAME";
const char *password = "PASSWORD";

const char *json = "{\"Hello\": \"World!\", \"from\": \"ESP-01\"}";
const char *jsonContent = "application/json";

const String list[] = {
  "{\"id\": 1, \"msg\": \"Olá\"}",
  "{\"id\": 2, \"msg\": \"Massa\"}",
  "{\"id\": 3, \"msg\": \"Abacaxi\"}",
  "{\"id\": 4, \"msg\": \"Suco de Laranja\"}"
};

ESP8266WebServer server(80);

void setup(void) {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleIndex);

  server.on(UriBraces("/hello/{}"), []() {
    String msg = server.pathArg(0);
    server.send(200, jsonContent, "{\"Hello\": \"" + msg + "\"}");
  });

  server.on(UriRegex("^\\/api\\/([0-9]+)$"), handleApi);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}

void handleIndex() {
  server.send(200, jsonContent, json);
}

void handleApi() {
  String id = server.pathArg(0);

  String response;

  if (!(id.length() > 0) || (id.toInt() > 4) || (id.toInt() <= 0)) {
    response = "{\"msg\": \"Not Found!\"}";
  }
  else {
    for (int i = 1; i <= 4; i++) {
      if (i == id.toInt()) {
        response = list[i - 1];
      }
    }
  }
  server.send(200, jsonContent, response);
}
