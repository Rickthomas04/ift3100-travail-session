#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"

class PortailWindow : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void renderScene();

	bool reloadShader();

	void keyPressed(int key);

	// Méthode callback pour le bouton reload shader
	void reloadShaderButtonPressed();

	ofEasyCam camera;
	ofCubeMap cubeMap;
	ofMaterial matFloor;
	ofMaterial matPlywood;
	ofMaterial matSphere;

	ofVboMesh meshPlySphere;

	float metallicStrength = 1.0f;
	float roughnessStrength = 0.05f;
	float texCoordScale1 = 1.0f;
	float texCoordScale2 = 0.3f;
	float clearCoatStrength = 1.0f;
	float clearCoatRoughness = 0.0f;
	float texCoordScale1Floor = 1.0f;
	float texCoordScale2Floor = 0.3f;
	float clearCoatStrengthFloor = 1.0f;
	float clearCoatRoughnessFloor = 0.0f;

	void updateValue(int index, float value);

	int mode = 0;
	int cptSky = 0;

	ofLight light;
	ofLight spotLight;
	bool bDebug = false;
	bool rotate = true;
	bool skyboxActive = true;

	void changeSky();
};