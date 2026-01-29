#pragma once
#include "ofMain.h"

class HistogramWindow : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void computeHistogram(ofImage& image); 

    std::vector<int> histogramR, histogramG, histogramB;
    bool hasData = false;
};
