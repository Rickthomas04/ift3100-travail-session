#pragma once

#include <vector>
#include <string>
#include "ofMain.h"

#define MAX_LIGHTS 8

class Renderer;

enum class LightType { none, ambient, directional, point, spot };

struct LightObject
{
    LightType           type = LightType::none;
    ofLight             light;
    std::string         name;
    bool                selected = false;
};

class Lighting
{
public:
    Lighting(Renderer& renderer);

    LightType light_draw_mode;

    std::vector<LightObject> lights;
    int buffer_head;

    float oscillation_frequency;
    float oscillation_amplitude;

    float materialShininess;
    ofColor ambientLightColor;

    void setup(Renderer& renderer);
    void update();
    void draw();

    void add_light(LightType type);
    void remove_light(int index);

    void setAmbientColor(ofColor);
    ~Lighting();

private:
    void configure_light(int index, LightType type);
    float oscillate(float time, float frequency, float amplitude);
    Renderer& renderer;
};