
#ifndef GAMECONTROLLER_SSD1306DISPLAY_H
#define GAMECONTROLLER_SSD1306DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Axis.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

class SSD1306Display {

private:
    Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

public:
    SSD1306Display(Axis &xAxis);

    void updateDisplay();

    int setup();

    String state = "";
    String task = "";
    bool showTaskParam = false;
    float taskParam = 0;
    Axis &xAxis;
};


#endif //GAMECONTROLLER_SSD1306DISPLAY_H
