#include "Lighting.h"
#include "Renderer.h"

Lighting::Lighting(Renderer& renderer) : renderer(renderer)
{

}
void Lighting::setup(Renderer& renderer)
{
	lights.resize(MAX_LIGHTS);
	buffer_head = 0;

	materialShininess = 32.0f;
	ambientLightColor = ofFloatColor(0, 0, 0);

	oscillation_amplitude = 28.0f;
	oscillation_frequency = 2.0f;
}

void Lighting::update()
{
	materialShininess = 32.0f + oscillation_amplitude * (0.5f + 0.5f * sin(ofGetElapsedTimef() * oscillation_frequency));
}


void Lighting::draw()
{
	for(int i = 0; i < MAX_LIGHTS; i++) {
		if(lights[i].type != LightType::none)
			lights[i].light.enable();
	}
	renderer.draw();
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (lights[i].type != LightType::none)
			lights[i].light.disable();
	}
}

Lighting::~Lighting()
{

}

void Lighting::add_light(LightType type)
{
	if (type == LightType::none) return;

	int insert_index = buffer_head;
	if(lights[buffer_head].type != LightType::none)

	for (int i = 0; i < MAX_LIGHTS; ++i) {
		if (lights[i].type == LightType::none) {
			insert_index = i;
			break;
		}
	}

	lights[insert_index].selected = false;
	configure_light(insert_index, type);
	buffer_head = (insert_index + 1);
	if (buffer_head >= MAX_LIGHTS)
		buffer_head = 0;

	ofLogNotice("Lighting") << "Added " << lights[insert_index].name << " at index " << insert_index;
}

void Lighting::remove_light(int index) {
	if (index >= 0 && index < MAX_LIGHTS) {
		if (lights[index].type != LightType::none) {
			lights[index].light.disable();
			lights[index].type = LightType::none;
			lights[index].name = "";
			lights[index].selected = false;

			// Reset light properties to default
			lights[index].light.setAmbientColor(ofColor(0, 0, 0));
			lights[index].light.setDiffuseColor(ofColor(0, 0, 0));
			lights[index].light.setSpecularColor(ofColor(0, 0, 0));
			lights[index].light.setPosition(0, 0, 0);
			lights[index].light.setOrientation(glm::vec3(0, 0, 0));
			lights[index].light.setAttenuation(1.0, 0.0, 0.0);
			lights[index].light.setSpotlightCutOff(180.0f);
			lights[index].light.setSpotConcentration(0.0f);
		}
	}
	else {
		ofLogWarning("Lighting") << "Cannot remove light, invalid index: " << index;
	}
}


void Lighting::configure_light(int index, LightType type)
{
	if (index < 0 || index >= MAX_LIGHTS) return;

	lights[index].type = type;
	lights[index].light.disable();

	glm::vec3 camPos = renderer.cam.getPosition() + glm::vec3(renderer.offset_x, 0, renderer.offset_z);
	glm::quat camOrientation = renderer.cam.getOrientationQuat();

	switch (type) {
	case LightType::directional:
		lights[index].name = "Directional " + ofToString(index);
		lights[index].light.setDirectional();
		lights[index].light.setDiffuseColor(ofColor(230, 230, 230));
		lights[index].light.setSpecularColor(ofColor(255, 255, 255));
		lights[index].light.setOrientation(camOrientation);
		lights[index].light.setAttenuation(1.0, 0.0, 0.0);
		break;

	case LightType::point:
		lights[index].name = "Point " + ofToString(index);
		lights[index].light.setPointLight();
		lights[index].light.setDiffuseColor(ofColor(230, 230, 200));
		lights[index].light.setSpecularColor(ofColor(255, 255, 255));
		lights[index].light.setPosition(camPos);
		break;

	case LightType::spot:
		lights[index].name = "Spot " + ofToString(index);
		lights[index].light.setDiffuseColor(ofColor(191, 191, 191));
		lights[index].light.setSpecularColor(ofColor(191, 191, 191));
		lights[index].light.setOrientation(camOrientation);
		lights[index].light.setPosition(camPos);
		lights[index].light.setSpotConcentration(2);
		lights[index].light.setSpotlightCutOff(30);
		lights[index].light.setSpotlight();
		break;

	default:
		lights[index].type = LightType::none;
		return;
	}
}

void Lighting::setAmbientColor(ofColor color) {
	ambientLightColor = color;
	ofSetGlobalAmbientColor(ambientLightColor);
}

float Lighting::oscillate(float time, float frequency, float amplitude)
{
	return sinf(time * frequency) * amplitude;
}