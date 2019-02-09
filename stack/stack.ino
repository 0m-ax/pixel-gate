#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "Labitat (free)";
const char* password = "labitatisawesome";
Adafruit_NeoPixel strip = Adafruit_NeoPixel(170, D11, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(170, D12, NEO_GRB + NEO_KHZ800);
void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setHostname("pixelgate");

  // No authentication by default
  // ArduinoOTA.setPasswor1d("admin");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
}

void loop() {
    for (int a = 150; a > 0; a--) {
      int red = random(0,255);
      int green = random(0,255);
      int blue = random(0,255);
      for (int j = 0; j < a; j++) {
        set(161+j , red, green, blue);
        set(161-j , red, green, blue);
        strip.show();
        strip2.show();
        yield();
        set(161+j, 0, 0, 0);
        set(161-j, 0, 0, 0);
      }
      set(161+a-1 , red, green, blue);
      set(161-a+1 , red, green, blue);
    }
}
void set(int led, uint8_t red, uint8_t green, uint8_t blue) {
  if (led % 2 == 0) {
    strip2.setPixelColor(led / 2, strip2.Color(red, green, blue));
  } else {
    strip.setPixelColor((led / 2) + 1, strip.Color(red, green, blue));
  }
}
