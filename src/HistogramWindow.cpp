#include "HistogramWindow.h"

void HistogramWindow::setup() {
    ofSetWindowShape(400, 300); 
    ofSetWindowTitle("Histogramme");
}

void HistogramWindow::update() {
}

void HistogramWindow::draw() {
    if (!hasData) return;

    ofBackground(0);
    float width = ofGetWidth();
    float height = ofGetHeight();
    float barWidth = width / 256.0f;


    int maxR = *std::max_element(histogramR.begin(), histogramR.end());
    int maxG = *std::max_element(histogramG.begin(), histogramG.end());
    int maxB = *std::max_element(histogramB.begin(), histogramB.end());

    if (maxR == 0) maxR = 1;
    if (maxG == 0) maxG = 1;
    if (maxB == 0) maxB = 1;

    ofSetColor(255, 0, 0); 
    for (int i = 0; i < 256; i++) {
        float normalizedValue = (float)histogramR[i] / maxR;
        ofDrawRectangle(i * barWidth, height, barWidth, -normalizedValue * height);
    }

    ofSetColor(0, 255, 0);
    for (int i = 0; i < 256; i++) {
        float normalizedValue = (float)histogramG[i] / maxG;
        ofDrawRectangle(i * barWidth, height, barWidth, -normalizedValue * height);
    }

    ofSetColor(0, 0, 255);
    for (int i = 0; i < 256; i++) {
        float normalizedValue = (float)histogramB[i] / maxB;
        ofDrawRectangle(i * barWidth, height, barWidth, -normalizedValue * height);
    }
}

void HistogramWindow::computeHistogram(ofImage& image) {
    histogramR.assign(256, 0);
    histogramG.assign(256, 0);
    histogramB.assign(256, 0);

    ofPixels& pixels = image.getPixels();
    int totalPixels = pixels.size() / 3;

    for (int i = 0; i < totalPixels; i++) {
        int index = i * 3;
        histogramR[pixels[index]]++;
        histogramG[pixels[index + 1]]++;
        histogramB[pixels[index + 2]]++;
    }

    hasData = true;

    ofLog() << "Histogram R (0) : " << histogramR[0] << ", Max : " << *std::max_element(histogramR.begin(), histogramR.end());
    ofLog() << "Histogram G (0) : " << histogramG[0] << ", Max : " << *std::max_element(histogramG.begin(), histogramG.end());
    ofLog() << "Histogram B (0) : " << histogramB[0] << ", Max : " << *std::max_element(histogramB.begin(), histogramB.end());
}
