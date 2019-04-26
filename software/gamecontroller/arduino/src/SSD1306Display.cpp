//
// Created by Andreas Rosnes Bade on 2019-04-24.
//

#include "SSD1306Display.h"


SSD1306Display::SSD1306Display(Axis &_xAxis): xAxis(_xAxis) {

}

int SSD1306Display::setup() {
    Serial1.println("init SSD1306");
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        Serial1.println("init SSD1306 - ERROR: SSD1306 allocation failed");
        return 1;
    }

    Serial1.println("init SSD1306 - OK");
    return 0;
}


void SSD1306Display::updateDisplay() {

    int xPosBno = 38;
    int xPosSpeed = 80;
    int xPosSet = 5;
    display.clearDisplay();
    display.setTextSize(1);
    display.cp437(true);
    display.setTextColor(WHITE);


    display.setCursor(0, 0);
    display.println(state);

    display.setCursor(0, 8);
    display.println(task);
    if (showTaskParam) {
        display.setCursor(75, 8);
        display.println(taskParam);
    }


    display.setCursor(xPosSet, 8 * 2);
    display.println(xAxis.setpointAngle);
    display.setCursor(xPosBno, 8 * 2);
    display.println(xAxis.bnoAngle);
    display.setCursor(xPosSpeed, 8 * 2);
    display.println(xAxis.xSpeedAdjusted);

    display.setCursor(xPosSet, 8 * 3);
    // display.println(setYAngle);
    display.setCursor(xPosBno, 8 * 3);
    // display.println(bnoY);

    display.display();

}