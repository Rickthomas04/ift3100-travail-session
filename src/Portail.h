#pragma once
#include "ofMain.h"

class Portail : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

private:
    ofVideoGrabber cam;
};
