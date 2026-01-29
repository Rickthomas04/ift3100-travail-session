#include "PortailWindow.h"

void PortailWindow::setup() {
	ofSetFrameRate(30);

	ofDisableArbTex();

	spotLight.setSpotlight();
	spotLight.setPosition(100.1, 400, 600);  // Position en (x, y, z)
	float center_x = ofGetWidth() / 2.0f;
	float center_y = ofGetHeight() / 2.0f;
	float center_z = 0; // ou une valeur adaptée à votre scène
	spotLight.lookAt(glm::vec3(center_x, center_y, center_z));
	spotLight.setSpotlightCutOff(45);      // Angle de 45 degrés pour le cône du spot
	spotLight.setSpotConcentration(128);   // Valeur de concentration (plus la valeur est élevée, plus le faisceau est concentré)
	spotLight.enable();
	spotLight.setDiffuseColor(ofFloatColor(0.0, 0.0, 1.0)); // Bleu pur
	spotLight.setAmbientColor(ofFloatColor(0.0, 0.0, 0.5));  // Une ambiance bleutée (optionnel)
	spotLight.setSpecularColor(ofFloatColor(0.0, 0.0, 1.0)); // Optionnel, si vous utilisez la spéculation
	spotLight.getShadow().setEnabled(true);
	spotLight.getShadow().setGlCullingEnabled(true);
	spotLight.getShadow().setDirectionalBounds(2000, 1000);
	spotLight.getShadow().setNearClip(200);
	spotLight.getShadow().setFarClip(2000);
	spotLight.getShadow().setStrength(1.0);
	spotLight.getShadow().setShadowType(OF_SHADOW_TYPE_PCF_LOW);

	light.setDirectional();
	light.enable();
	light.setPosition(100.1, 400, 600);
	light.lookAt(glm::vec3(0, 0, 0));
	light.getShadow().setEnabled(true);
	light.getShadow().setGlCullingEnabled(true);
	light.getShadow().setDirectionalBounds(2000, 1000);
	light.getShadow().setNearClip(200);
	light.getShadow().setFarClip(2000);
	light.getShadow().setStrength(1.0);
	// increase alpha value to increase strength of light
	light.setDiffuseColor(ofFloatColor(1.0, 2.0));
	light.getShadow().setShadowType(OF_SHADOW_TYPE_PCF_LOW);
	
	matFloor.setPBR(true);

	changeSky();

	matPlywood.setPBR(true);
	matPlywood.loadTexture(OF_MATERIAL_TEXTURE_DIFFUSE, "iron/albedo.png");
	matPlywood.loadTexture(OF_MATERIAL_TEXTURE_NORMAL, "iron/normal.png");
	matPlywood.loadTexture(OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC, "iron/combine.png");
	matPlywood.setTexCoordScale(texCoordScale1, texCoordScale2);
	matPlywood.setClearCoatEnabled(true);
	matPlywood.setClearCoatStrength(clearCoatStrength);
	matPlywood.setClearCoatRoughness(clearCoatRoughness);


	matFloor.setPBR(true);
	matFloor.loadTexture(OF_MATERIAL_TEXTURE_DIFFUSE, "floor/albedo.png");
	matFloor.loadTexture(OF_MATERIAL_TEXTURE_NORMAL, "floor/normal.png");
	matFloor.loadTexture(OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC, "floor/combine.png");
	matFloor.setTexCoordScale(texCoordScale1Floor, texCoordScale2Floor);
	matFloor.setClearCoatEnabled(true);
	matFloor.setClearCoatStrength(clearCoatStrengthFloor);
	matFloor.setClearCoatRoughness(clearCoatRoughnessFloor);



	// higher resolution sphere
	meshPlySphere = ofMesh::icosphere(1.0, 5);

	matSphere.setPBR(true);
	matSphere.setDiffuseColor(ofFloatColor(0.25, 1.0));
	matSphere.setMetallic(metallicStrength);
	matSphere.setRoughness(roughnessStrength);

	reloadShader();
}

void PortailWindow::update() {
	// Appliquez les valeurs aux matériaux
	matSphere.setMetallic(metallicStrength);
	matSphere.setRoughness(roughnessStrength);

	matPlywood.setTexCoordScale(texCoordScale1, texCoordScale2);
	matPlywood.setClearCoatStrength(clearCoatStrength);
	matPlywood.setClearCoatRoughness(clearCoatRoughness);

	matFloor.setTexCoordScale(texCoordScale1Floor, texCoordScale2Floor);
	matFloor.setClearCoatStrength(clearCoatStrengthFloor);
	matFloor.setClearCoatRoughness(clearCoatRoughnessFloor);
}

//--------------------------------------------------------------
void PortailWindow::draw() {

	ofEnableDepthTest();



	if (light.shouldRenderShadowDepthPass()) {
		int numShadowPasses = light.getNumShadowDepthPasses();
		for (int j = 0; j < numShadowPasses; j++) {
			light.beginShadowDepthPass(j);
			renderScene();
			light.endShadowDepthPass(j);
		}
	}

	camera.begin(); {

		renderScene();

		if (cubeMap.hasPrefilteredMap()) {
			cubeMap.drawPrefilteredCube(0.2f);
		}

		if (bDebug) {
			light.draw();

			if (light.getIsEnabled() && light.getShadow().getIsEnabled()) {
				light.getShadow().drawFrustum();
			}
		}

	} camera.end();

	ofDisableDepthTest();

	ofSetColor(255);
	ofEnableAlphaBlending();
#ifdef USE_LIGHT
	for (auto& lp : lights) {
		lp->gui.draw();
	}
#endif

	stringstream ss;
	ss << "Appuyez sur la touche 0 pour changer la skybox, 1 pour toggle les sources de lumieres";
	ofDrawBitmapStringHighlight(ss.str(), 40, 40);
}

void PortailWindow::reloadShaderButtonPressed() {
	reloadShader();
}

void PortailWindow::renderScene() {

	matFloor.begin();
	ofPushMatrix();
	ofDrawBox(0, -275, 0, 3200, 50, 2200);
	ofPopMatrix();
	matFloor.end();

	matPlywood.begin();
	ofPushMatrix();
	float angle = ofGetElapsedTimef();
	if (rotate)
		ofTranslate(700.0 * cos(angle), -20, sin(angle) * 300.0 - 100);
	ofScale(120);
	meshPlySphere.draw();
	ofPopMatrix();
	matPlywood.end();

	matSphere.begin();
	ofPushMatrix();
	if (rotate)
		ofTranslate(700.0 * cos(angle - PI), -20, sin(angle - PI) * 300.0 - 100);
	ofScale(120);
	meshPlySphere.draw();
	ofPopMatrix();
	matSphere.end();
}

//--------------------------------------------------------------
bool PortailWindow::reloadShader() {
	// load the shader main functions //
	string vname = "shaders/main.vert";
	ofBuffer vbuffer = ofBufferFromFile(vname);
	string fname = "shaders/main.frag";
	ofBuffer fbuffer = ofBufferFromFile(fname);
	return false;
}

//--------------------------------------------------------------
void PortailWindow::keyPressed(int key) {
	if (key == '0') {
		changeSky();
	}

	if (key == '1') {
		if (light.getIsEnabled())
			light.disable();
		else
			light.enable();
	}

	if (key == '2') {
		rotate = !rotate;
	}

	if (key == '3') {
		if (skyboxActive) {
			cubeMap.clear();
			skyboxActive = false;
		}
		else {
			skyboxActive = true;
			changeSky();
		}
	}

	if (key == 'd')
		bDebug = !bDebug;
}

void PortailWindow::changeSky() {
	switch (cptSky) {
	case 0:
		cubeMap.load("kloppenheim_06_puresky_1k.exr", 512);
		cptSky++;
		break;
	case 1:
		cubeMap.load("rogland_clear_night_4k.exr", 512);
		cptSky++;
		break;
	case 2:
		cubeMap.load("satara_night_no_lamps_4k.exr", 512);
		cptSky = 0;
		break;
	}
}

void PortailWindow::updateValue(int index, float value) {
	switch (index)
	{
		case 1:
			metallicStrength = value;
			break;
		case 2:
			roughnessStrength = value;
			break;
		case 3:
			texCoordScale1 = value;
			break;
		case 4:
			texCoordScale2 = value;
			break;
		case 5:
			clearCoatStrength = value;
			break;
		case 6:
			clearCoatRoughness = value;
			break;
		case 7:
			texCoordScale1Floor = value;
			break;
		case 8:
			texCoordScale2Floor = value;
			break;
		case 9:
			clearCoatStrengthFloor = value;
			break;
		case 10:
			clearCoatRoughnessFloor = value;
			break;
	}
}