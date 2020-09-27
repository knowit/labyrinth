//
// Created by Andreas Rosnes Bade on 2019-04-24.
//

#include "SSD1306Display.h"


SSD1306Display::SSD1306Display(Axis &_xAxis, Axis &_yAxis) :
        xAxis(_xAxis),
        yAxis(_yAxis) {
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

    if( state.equals("Running")) {
        displayAxis(8 * 2, xAxis.setpointAngle, xAxis.bnoAngle, xAxis.speedAdjusted);
        displayAxis(8 * 3, yAxis.setpointAngle, yAxis.bnoAngle, yAxis.speedAdjusted);

    }

    display.display();
}

void SSD1306Display::displayAxis(int y, double setpointAngle, double bnoAngle, double speedAdjusted) {
    display.setCursor(5, y);
    display.println(setpointAngle);
    display.setCursor(38, y);
    display.println(bnoAngle);
    display.setCursor(80, y);
    display.println(speedAdjusted);
}