#include <WiFi.h>
#include <WebServer.h>

#ifndef FIRMWARE_WIFICONFIGMODE_H
#define FIRMWARE_WIFICONFIGMODE_H

static const char *const preferences_name = "labyrinth";
#endif //FIRMWARE_WIFICONFIGMODE_H

/* IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer configServer(80);

Preferences configPrefences;

void resetPreferences();

void setPreferences(const String &name, const String &ssid, const String &pwd, long rport, long sport);

String SendHTML() {

    configPrefences.begin(preferences_name, false);
    String name = configPrefences.getString("name", String("not set"));
    String ssid = configPrefences.getString("ssid", String("not set"));
    String pwd = configPrefences.getString("pwd", String("not set"));
    int32_t rport = configPrefences.getInt("rport", -1);
    int32_t sport = configPrefences.getInt("sport", -1);
    configPrefences.end();

    String html = "<!DOCTYPE html>";
    html += "<html lang=\"en\">";
    html += "<head>";
    html += "<meta charset=\"UTF-8\">";
    html += "<title>Labyrinth configuration</title>";
    html += "</head>";
    html += "<body>";
    html += "<p>";
    html += "<h1>Configuration data</h1>";
    html += "</p>";
    html += "";
    html += "<form action=\"http://192.168.1.1\" method=\"post\"  >";
    html += "Name of labyrinth:<br>";
    html += "<input type=\"text\" id=\"name\" name=\"name\" value=\"" + name + "\"><br>";
    html += "    ";
    html += "<br>WIFI name (SSID):<br>";
    html += "<input type=\"text\" id=\"ssid\" name=\"ssid\" value=\"" + ssid + "\">";
    html += "    ";
    html += "<br><br>WIFI password:<br>";
    html += "<input type=\"text\" id=\"pwd\" name=\"pwd\" value=\"" + pwd + "\">";
    html += "    ";
    html += "<br><br>OSC receive port (e.g. 10000):<br>";
    html += "<input type=\"text\" id=\"rport\" name=\"rport\" value=\"" + String(rport) + "\">";
    html += "    ";
    html += "<br><br>OSC send port (e.g. 12000):<br>";
    html += "<input type=\"text\" id=\"sport\" name=\"sport\" value=\"" + String(sport) + "\">";
    html += "    ";
    html += "<br><br><br>";
    html += "<input type=\"hidden\" id=\"hiddenfield\" name=\"hiddenfield\" value=\"imhere\">";
    html += "    ";
    html += "<input type=\"submit\" value=\"Submit\" >";
    html += "</form>";
    html += "";
    html += "</body>";
    html += "</html>";

    return html;
}

void handle_OnConnect() {

    // if hiddenfield is set, config data has been sent
    if (configServer.arg("hiddenfield").equals("imhere")) {
        Serial.println("Recieved form data");

        setPreferences(configServer.arg("name"),
                       configServer.arg("ssid"),
                       configServer.arg("pwd"),
                       configServer.arg("rport").toInt(),
                       configServer.arg("sport").toInt());

    } else {
        Serial.println("Did not recieve form data");
    }

    configServer.send(200, "text/html", SendHTML());
}

void setPreferences(const String &name, const String &ssid, const String &pwd, long rport, long sport) {
    configPrefences.begin(preferences_name, false);
    configPrefences.clear();
    configPrefences.putString("name", name);
    configPrefences.putString("ssid", ssid);
    configPrefences.putString("pwd", pwd);
    configPrefences.putInt("rport", rport);
    configPrefences.putInt("sport", sport);

    Serial.print("Preferences : ");
    Serial.print("name=");
    Serial.print(configPrefences.getString("name", String("not set")));
    Serial.print(" , ");
    Serial.print("ssid=");
    Serial.print(configPrefences.getString("ssid", String("not set")));
    Serial.print(" , ");
    Serial.print("pwd=");
    Serial.print(configPrefences.getString("pwd", String("not set")));
    Serial.print(" , ");
    Serial.print("rport=");
    Serial.print(configPrefences.getInt("rport", -1));
    Serial.print(" , ");
    Serial.print("sport=");
    Serial.print(configPrefences.getInt("sport", -1));
    Serial.print(" , ");
    Serial.println();

    configPrefences.end();
}

void resetPreferences() {
    setPreferences("<not set>","<not set>","<not set>",0,0 );
}

void handle_NotFound() {
    configServer.send(404, "text/plain", "Not found");
}

void serve() {
    Serial.println("Entering configuration mode");



    String apssid = "labyrinth_" + String(random(999));
    Serial.println("Setting up AP with SSID=" + apssid);
    // WiFi.softAP(ssid, password);
    WiFi.softAP(apssid.c_str());
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);

    configServer.on("/", handle_OnConnect);
    configServer.onNotFound(handle_NotFound);

    configServer.begin();
    Serial.println("HTTP configServer started");
    Serial.println(
            "Please connect to wifinetwork " + apssid + " and point you browser to: http://" + local_ip.toString());

    while (true) {
        configServer.handleClient();
    }

}

