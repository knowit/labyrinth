
#include <Arduino.h>

#include <Preferences.h>
#include <wificonfigmode.h>
#include <ESPmDNS.h>
//
// Created by Andreas Rosnes Bade on 27/09/2020.
//

#ifndef ARDUINO_ESP2_NETWORK_H
#define ARDUINO_ESP2_NETWORK_H

#endif //ARDUINO_ESP2_NETWORK_H

Preferences preferences;

void setupWIFI() {

    delay(1000);

    preferences.begin("dings01", false);
    String name = preferences.getString("name", String("not set"));
    String ssid = preferences.getString("ssid", String("not set"));
    String pwd = preferences.getString("pwd", String("not set"));
    preferences.end();

    Serial.print("pwd=");
    Serial.print(pwd);

    Serial.print("name=" + name + " , ");
    Serial.print("ssid=" + ssid + " , ");
    Serial.print("pwd="+pwd+" , ");
    Serial.println();

    Serial.print("Connecting to WiFi. ssid=");
    Serial.println(ssid);

    if (pwd == "none") {
        Serial.println("Using no password");
        WiFi.begin(
                ssid.c_str(), ""
        );
    } else {
        WiFi.begin(
                ssid.c_str(),
                pwd.c_str()
        );
    }

    int timeout = 0;

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(250);
        delay(250);
        timeout++;
        if (timeout > 20) {
            Serial.println("Unable to connect to network (SSID=" + ssid + ").");
            serve();
        }
    }


    // serve();

    Serial.print("Connecting to WiFi. IP=");
    Serial.println(WiFi.localIP());

    if (!MDNS.begin(name.c_str())) {
        Serial.println("Unable to set up MSDN");
        while (1) {
            delay(1000);
        }
    }

    Serial.print("MSDN setup OK. name=");
    Serial.print(name);
    Serial.println(".local");
}