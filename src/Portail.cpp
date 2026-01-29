#include "Portail.h"

void Portail::setup() {
    ofSetWindowShape(400, 300);
    ofSetWindowTitle("Portail");
    vector<ofVideoDevice> devices = cam.listDevices();
    for (size_t i = 0; i < devices.size(); i++) {
        ofLogNotice() << "Caméra " << devices[i].id << ": " << devices[i].deviceName;
    }

    cam.setDeviceID(0); // Essaye un autre ID si 0 ne fonctionne pas
    bool success = cam.setup(400, 300);

    if (!success) {
        ofLogError() << "Erreur : Impossible d'initialiser la caméra !";
    }
}

void Portail::update() {
    cam.update();
}

void Portail::draw() {
    ofBackground(0);

    if (cam.isFrameNew()) {
        cam.draw(0, 0); // Afficher la vidéo de la caméra
    }

     
    float width = ofGetWidth();
    float height = ofGetHeight();
    float barWidth = width / 256.0f;
}