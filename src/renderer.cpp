// IFT3100H24_Move/renderer.cpp
// Classe responsable du rendu de l'application.

#include "renderer.h"
#include <application.h>
#include <cmath>

void Renderer::setup()
{
    ofSetFrameRate(60);
    ofSetBackgroundColor(31);
    ofDisableArbTex();
    node.setOrientation(glm::vec3(5, 20, 50));
    //cam.setTarget(glm::vec3(0,0,0));
    node.setPosition(0.0f, 0.0f, 0.0f);
    cam.setTarget(node);
    //cam.setPosition(glm::vec3(1000, 500, 600));
    speed = 100.0f;

    delta_x = speed;
    delta_z = speed;

    // nombre maximal de primitives vectorielles dans le tableau
    buffer_count = 100;

    // position de la prochaine primitive
    buffer_head = 0;

    // calculer la taille de la structure générique commune à toutes les primitives vectorielles
    buffer_stride = sizeof(VectorPrimitive);

    // calculer le nombre d'octets à allouer en mémoire pour contenir le tableau de primitives vectorielles
    buffer_size = buffer_count * buffer_stride;

    // nombre maximal de primitives 3D dans le tableau
    buffer_count3D = 7;

    // position de la prochaine primitive
    buffer_head3D = 0;

    // allocation d'un espace mémoire suffisamment grand pour contenir les données de l'ensemble des primitives vectorielles et géométriques
    shapes = (VectorPrimitive*)std::malloc(buffer_size);
    shapes3D.resize(buffer_count3D);
    imported_models.resize(3);

    for (auto& model : imported_models)
        model.isEmpty = true;
    // mode de dessin par défaut
    draw_mode = VectorPrimitiveType::none;
    draw_mode3D = GeometricPrimitiveType::none;

    // largeur de la ligne de contour
    ofSetLineWidth(stroke_weight);

    // couleur de la ligne de contour
    stroke_color_r = stroke_color.r;
    stroke_color_g = stroke_color.g;
    stroke_color_b = stroke_color.b;
    stroke_color_a = stroke_color.a;


    // couleur de la zone de remplissage
    fill_color_r = fill_color.r;
    fill_color_g = fill_color.g;
    fill_color_b = fill_color.b;
    fill_color_a = fill_color.a;
    ofSetColor(fill_color);

    //Matériaux par défaut
    defaultMaterial.setDiffuseColor(ofColor(0, 180, 0));
    defaultMaterial.setEmissiveColor(ofColor(0, 50, 0));
    defaultMaterial.setSpecularColor(ofColor(127, 127, 127));
    defaultMaterial.setShininess(16.0f);

    //Matériaux lorsque sélecitonné
    selectedMaterial.setDiffuseColor(ofColor(180, 0, 0));
    selectedMaterial.setEmissiveColor(ofColor(50, 0, 0));
    selectedMaterial.setSpecularColor(ofColor(127, 127, 127));
    selectedMaterial.setShininess(16.0f);

    //Matériaux 2
    material2.setDiffuseColor(ofColor(0, 0, 180));
    material2.setEmissiveColor(ofColor(0, 0, 50));
    material2.setSpecularColor(ofColor(127, 127, 127));
    material2.setRoughness(0.9f);
    material3.setShininess(16.0f);

    //Matériaux 3
    material3.setDiffuseColor(ofColor(0, 0, 180));
    material3.setEmissiveColor(ofColor(0, 0, 50));
    material3.setSpecularColor(ofColor(127, 127, 127));
    material3.setMetallic(0.9f);
    material3.setShininess(16.0f);

    mouse_press_x = mouse_press_y = mouse_current_x = mouse_current_y = 0;

    is_mouse_button_pressed = false;

    radius = 4.0f;

    setupBezier();

    currentModeis2D = isMode2D;
    emptyStacks();
    reset();
}

void Renderer::update()
{
    center_x = ofGetWidth() / 2.0f;
    center_y = ofGetHeight() / 2.0f;

    // paramètres des outils de dessin
    ofSetColor(stroke_color);
    ofSetLineWidth(stroke_weight);
    ofSetColor(fill_color);

}

void Renderer::draw()
{
    ofMaterial material;
    //material.setAmbientColor(ofColor(63, 63, 63));
    material.setDiffuseColor(ofColor(127, 0, 0));
    material.setEmissiveColor(ofColor(31, 0, 0));
    material.setSpecularColor(ofColor(127, 127, 127));
    material.setShininess(16.0f);
    currentShapeIndex = 0;
    ofClear(background_color);
    ofBackground(20);
    if (!isMode2D) {
        if (currentModeis2D != isMode2D) {
            reset();
            currentModeis2D = isMode2D;
            draw_mode = VectorPrimitiveType::none;
        }
        cam.begin();
        ofEnableDepthTest();
        for (index = 0; index < buffer_count3D; ++index)
        {
            if (shapes3D[index].type != GeometricPrimitiveType::none) {
                ofPushMatrix();
                ofTranslate(shapes3D[index].position[0] + offset_x, shapes3D[index].position[1], shapes3D[index].position[2] + offset_z);

                shapes3D[index].material.begin();
                if (shapes3D[index].show_wireframe) {
                    shapes3D[index].mesh.drawWireframe();
                }
                else {
                    shapes3D[index].mesh.draw();
                }
                shapes3D[index].material.end();
                ofPopMatrix();
            }
            else {
                currentShapeIndex++;
            }
        }

        // transformer l'origine de la scène
        ofTranslate(center_x + offset_x, center_y, offset_z);

        // dessiner l'origine de la scène
        //draw_locator(2.0f);
        //ofNoFill();
        if (showBezier)
        {
            ofSetColor(255, 255, 255);
            resolution = 15 * (tessellation + 1);
            for (int i = 0; i < resolution; i++) {
                ofPolyline ligne;
                ofPolyline ligne2;
                for (int j = 0; j < resolution; j++) {
                    float u = float(i) / (resolution - 1);
                    float v = float(j) / (resolution - 1);
                    ofVec3f p = computeBezierSurface(u, v);
                    ofVec3f a = computeBezierSurface(v, u);
                    //ofDrawSphere(p.x, p.y, p.z, 2);
                    ligne.addVertex(p);
                    ligne2.addVertex(a);
                }
                ligne.draw();
                ligne2.draw();
            }
            
            ofSetColor(255, 0, 0);
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    ofDrawSphere(controlPoints3D[i][j].x, controlPoints3D[i][j].y, controlPoints3D[i][j].z, 5);
                }
            }
        }
        
        
        //ofFill();
        ofDisableDepthTest();
        cam.end();
        drawInteractionArea();
    }
    else {
        ofBackground(background_color);
        currentModeis2D = isMode2D;
        if (is_mouse_button_pressed)
        {
            draw_zone(
                mouse_press_x,
                mouse_press_y,
                mouse_current_x,
                mouse_current_y);
        }

        for (const auto& imgData : images)
        {
            imgData.image.draw(imgData.imgX, imgData.imgY);
        }
        if (showBezier)
        {
            ofFill();
            ofSetColor(0, 255, 0);
            vector<ofVec2f> pointsTemp;
            for (int i = 0; i < nbrPointsctrl; i++) {
                ofDrawCircle(controlPoints2D[i], 5);
                pointsTemp.push_back(controlPoints2D[i]);
            }

            ofSetColor(255, 0, 0);
            for (float t = 0; t <= 1.0; t += 0.005) {
                ofVec2f p = computeBezierCurve(pointsTemp, t);
                if (t + 0.005 <= 1.0)
                {
                    ofVec2f pPlus = computeBezierCurve(pointsTemp, t + 0.005);
                    ofDrawLine(p, pPlus);
                }
                //ofDrawCircle(p, 2);
            }
            ofNoFill();
        }
        for (index = 0; index < buffer_count; ++index)
        {
            if (shapes[index].type != VectorPrimitiveType::none)
                currentShapeIndex++;
            else
                return;
            switch (shapes[index].type)
            {
            case VectorPrimitiveType::polygon:
                if (shapes[index].filled == NULL && shapes[index].stroke == NULL)
                {
                    shapes[index].filled = fillEnabled;
                    shapes[index].stroke = strokeEnabled;
                }
                if (shapes[index].filled) {
                    fill_color_r = fill_color.r;
                    fill_color_g = fill_color.g;
                    fill_color_b = fill_color.b;
                    ofFill();
                    ofSetLineWidth(0);
                    ofSetCircleResolution(6);
                    ofSetColor(
                        shapes[index].fill_color[0],
                        shapes[index].fill_color[1],
                        shapes[index].fill_color[2]);
                    draw_ellipse(
                        shapes[index].position1[0],
                        shapes[index].position1[1],
                        shapes[index].position2[0],
                        shapes[index].position2[1]);
                }
                if (shapes[index].stroke) {
                    stroke_color_r = stroke_color.r;
                    stroke_color_g = stroke_color.g;
                    stroke_color_b = stroke_color.b;
                    ofNoFill();
                    ofSetLineWidth(shapes[index].stroke_width);
                    ofSetColor(
                        shapes[index].stroke_color[0],
                        shapes[index].stroke_color[1],
                        shapes[index].stroke_color[2]);
                    draw_ellipse(
                        shapes[index].position1[0],
                        shapes[index].position1[1],
                        shapes[index].position2[0],
                        shapes[index].position2[1]);
                }
                break;

            case VectorPrimitiveType::point:
                if (shapes[index].filled == NULL && shapes[index].stroke == NULL)
                {
                    shapes[index].filled = fillEnabled;
                    shapes[index].stroke = strokeEnabled;
                }
                if (shapes[index].filled) {
                    fill_color_r = fill_color.r;
                    fill_color_g = fill_color.g;
                    fill_color_b = fill_color.b;
                    ofFill();
                    ofSetLineWidth(0);
                    ofSetCircleResolution(128);
                    ofSetColor(
                        shapes[index].fill_color[0],
                        shapes[index].fill_color[1],
                        shapes[index].fill_color[2]);
                    draw_point(
                        shapes[index].position2[0],
                        shapes[index].position2[1],
                        shapes[index].stroke_width);
                }
                break;

            case VectorPrimitiveType::line:
                if (shapes[index].filled == NULL && shapes[index].stroke == NULL)
                {
                    shapes[index].filled = fillEnabled;
                    shapes[index].stroke = strokeEnabled;
                }
                if (shapes[index].filled) {
                    fill_color_r = fill_color.r;
                    fill_color_g = fill_color.g;
                    fill_color_b = fill_color.b;
                    ofNoFill();
                    ofSetLineWidth(shapes[index].stroke_width);
                    ofSetColor(
                        shapes[index].fill_color[0],
                        shapes[index].fill_color[1],
                        shapes[index].fill_color[2]);
                    draw_line(
                        shapes[index].position1[0],
                        shapes[index].position1[1],
                        shapes[index].position2[0],
                        shapes[index].position2[1]);
                }
                break;

            case VectorPrimitiveType::rectangle:
                if (shapes[index].filled == NULL && shapes[index].stroke == NULL)
                {
                    shapes[index].filled = fillEnabled;
                    shapes[index].stroke = strokeEnabled;
                }
                if (shapes[index].filled) {
                    fill_color_r = fill_color.r;
                    fill_color_g = fill_color.g;
                    fill_color_b = fill_color.b;
                    ofFill();
                    ofSetLineWidth(0);
                    ofSetColor(
                        shapes[index].fill_color[0],
                        shapes[index].fill_color[1],
                        shapes[index].fill_color[2]
                    );
                    draw_rectangle(
                        shapes[index].position1[0],
                        shapes[index].position1[1],
                        shapes[index].position2[0],
                        shapes[index].position2[1]);
                }
                if (shapes[index].stroke) {
                    stroke_color_r = stroke_color.r;
                    stroke_color_g = stroke_color.g;
                    stroke_color_b = stroke_color.b;
                    ofNoFill();
                    ofSetLineWidth(shapes[index].stroke_width);
                    ofSetColor(
                        shapes[index].stroke_color[0],
                        shapes[index].stroke_color[1],
                        shapes[index].stroke_color[2]);
                    draw_rectangle(
                        shapes[index].position1[0],
                        shapes[index].position1[1],
                        shapes[index].position2[0],
                        shapes[index].position2[1]);
                }
                break;

            case VectorPrimitiveType::ellipse:
                if (shapes[index].filled == NULL && shapes[index].stroke == NULL)
                {
                    shapes[index].filled = fillEnabled;
                    shapes[index].stroke = strokeEnabled;
                }
                if (shapes[index].filled) {
                    fill_color_r = fill_color.r;
                    fill_color_g = fill_color.g;
                    fill_color_b = fill_color.b;
                    ofFill();
                    ofSetLineWidth(0);
                    ofSetCircleResolution(128);
                    ofSetColor(
                        shapes[index].fill_color[0],
                        shapes[index].fill_color[1],
                        shapes[index].fill_color[2]);
                    draw_ellipse(
                        shapes[index].position1[0],
                        shapes[index].position1[1],
                        shapes[index].position2[0],
                        shapes[index].position2[1]);
                }
                if (shapes[index].stroke) {
                    stroke_color_r = stroke_color.r;
                    stroke_color_g = stroke_color.g;
                    stroke_color_b = stroke_color.b;
                    ofNoFill();
                    ofSetLineWidth(shapes[index].stroke_width);
                    ofSetColor(
                        shapes[index].stroke_color[0],
                        shapes[index].stroke_color[1],
                        shapes[index].stroke_color[2]);
                    draw_ellipse(
                        shapes[index].position1[0],
                        shapes[index].position1[1],
                        shapes[index].position2[0],
                        shapes[index].position2[1]);
                }
                break;

            default:
                break;
            }
        }
    }

    // afficher la zone de sélection
    if (is_mouse_button_pressed)
    {
        ofLog() << "zoneDessin";
        draw_zone(
            mouse_press_x,
            mouse_press_y,
            mouse_current_x,
            mouse_current_y);
    }
    // copier la matrice de transformation courante sur le dessus de la pile
    ofPushMatrix();


    // revenir à la matrice de transformation précédente dans la pile
    ofPopMatrix();

    //ofSetColor(255);
}

void Renderer::reset()
{
    offset_x = 0.0f;
    offset_z = 0.0f;

    for (index = 0; index < buffer_count; ++index)
        shapes[index].type = VectorPrimitiveType::none;

    buffer_head = 0;

    ofLog() << "<reset>";
}

void Renderer::draw_locator(float scale)
{
    ofSetLineWidth(4);
    ofSetColor(127);
    ofFill();
    ofPushMatrix();
    ofScale(scale, scale, scale);
    node3.setPosition(-500.0f, -300.0f, 0.0f);
    node3.draw();
    ofPopMatrix();
}

void Renderer::add_vector_shape(VectorPrimitiveType type)
{
    shapes[buffer_head].type = type;

    shapes[buffer_head].position1[0] = mouse_press_x;
    shapes[buffer_head].position1[1] = mouse_press_y;

    shapes[buffer_head].position2[0] = mouse_current_x;
    shapes[buffer_head].position2[1] = mouse_current_y;

    shapes[buffer_head].stroke_color[0] = stroke_color_r;
    shapes[buffer_head].stroke_color[1] = stroke_color_g;
    shapes[buffer_head].stroke_color[2] = stroke_color_b;
    shapes[buffer_head].stroke_color[3] = stroke_color_a;

    shapes[buffer_head].fill_color[0] = fill_color_r;
    shapes[buffer_head].fill_color[1] = fill_color_g;
    shapes[buffer_head].fill_color[2] = fill_color_b;
    shapes[buffer_head].fill_color[3] = fill_color_a;

    shapes[buffer_head].filled = fillEnabled;
    shapes[buffer_head].stroke = strokeEnabled;

    switch (shapes[buffer_head].type)
    {
    case VectorPrimitiveType::point:
        shapes[buffer_head].stroke_width = 16;
        break;

    case VectorPrimitiveType::line:
        shapes[buffer_head].stroke_width = stroke_weight;
        break;

    case VectorPrimitiveType::rectangle:
        shapes[buffer_head].stroke_width = stroke_weight;
        break;

    case VectorPrimitiveType::ellipse:
        shapes[buffer_head].stroke_width = stroke_weight;
        break;

    default:
        shapes[buffer_head].stroke_width = stroke_weight;
        break;
    }

    ofLog() << "<new primitive at index: " << buffer_head << ">";

    buffer_head = ++buffer_head >= buffer_count ? 0 : buffer_head; // boucler sur le tableau si plein
}

void Renderer::add_geometric_shape(GeometricPrimitiveType type, float x, float y , float z , bool redo )
{
    if (type != GeometricPrimitiveType::none) {
        // Find next empty spot in vector
        if (shapes3D[buffer_head3D].type != GeometricPrimitiveType::none) {
            for (int i = 0; i < buffer_count3D; i++) {
                if (shapes3D[i].type == GeometricPrimitiveType::none) {
                    buffer_head3D = i;
                    break;
                }
            }
        }
        // Fill/overrite values
            shapes3D[buffer_head3D].type = type;
            shapes3D[buffer_head3D].material = defaultMaterial;
            shapes3D[buffer_head3D].chosenMaterial = 0;
            glm::vec3 world_pos = screen_to_world_pos(mouse_current_x, mouse_current_y, 750.0f, cam);
            if (!redo) {
                shapes3D[buffer_head3D].position[0] = world_pos.x;
                shapes3D[buffer_head3D].position[1] = world_pos.y;
                shapes3D[buffer_head3D].position[2] = world_pos.z;
            }
            else {
                shapes3D[buffer_head3D].position[0] = x;
                shapes3D[buffer_head3D].position[1] = y;
                shapes3D[buffer_head3D].position[2] = z;
            }

            shapes3D[buffer_head3D].size = 50.0f;
            shapes3D[buffer_head3D].show_wireframe = false;
            shapes3D[buffer_head3D].selected = false;
            ofSpherePrimitive spherePrimitive;
            switch (type) {
            case GeometricPrimitiveType::cube:
                shapes3D[buffer_head3D].name = " Cube";
                shapes3D[buffer_head3D].mesh = build_geometric_mesh(shapes3D[buffer_head3D]);
                break;
            case GeometricPrimitiveType::pyramid:
                shapes3D[buffer_head3D].name = " Pyramid";
                shapes3D[buffer_head3D].mesh = build_geometric_mesh(shapes3D[buffer_head3D]);
                break;
            case GeometricPrimitiveType::sphere:
                shapes3D[buffer_head3D].name = " Sphere";
                spherePrimitive.setRadius(50);
                spherePrimitive.setResolution(50);
                shapes3D[buffer_head3D].mesh = spherePrimitive.getMesh();
                break;
            case GeometricPrimitiveType::imported:
                shapes3D[buffer_head3D].mesh = imported_models[selected_imported_model - 1].mesh;
                shapes3D[buffer_head3D].name = " " + imported_models[selected_imported_model - 1].name;
                break;
            default:
                break;
            }
            for (auto& vertex : shapes3D[buffer_head3D].mesh.getVertices()) {
                vertex *= buffer_head3D * 0.2f + 1;
            }
            stackUndo3D.push(std::pair(buffer_head3D, std::pair(ActionType::add, shapes3D[buffer_head3D])));
            buffer_head3D = ++buffer_head3D >= buffer_count3D ? 0 : buffer_head3D; // boucler sur le tableau si plein
            ((Application*)ofGetAppPtr())->openPortailWindow();
    }
}

void Renderer::add_model3D_to_list(ofMesh mesh, string file_name) {
    imported_models[buffer_head_imported_model3D].isEmpty = false;
    imported_models[buffer_head_imported_model3D].name = file_name;
    imported_models[buffer_head_imported_model3D].mesh = mesh;
    buffer_head_imported_model3D = ++buffer_head_imported_model3D >= imported_models.size() ? 0 : buffer_head_imported_model3D;
}

void Renderer::reset_geometric_primitive(int index, bool redo) {
    if (!redo)
    {
        stackUndo3D.push(std::pair(index, std::pair(ActionType::remove, shapes3D[index])));
    }
    shapes3D[index].type = GeometricPrimitiveType::none;
    shapes3D[index].position[0] = 0;
    shapes3D[index].position[1] = 0;
    shapes3D[index].position[2] = 0;
    shapes3D[index].size = 0;
    shapes3D[index].selected = false;
}

// fonction qui dessine un pixel
void Renderer::draw_pixel(float x, float y) const
{
    int pixel_x = floorf(x);
    int pixel_y = floorf(y);

    ofDrawRectangle(pixel_x, pixel_y, 1, 1);
}

// fonction qui dessine un point
void Renderer::draw_point(float x, float y, float radius) const
{
    ofDrawEllipse(x, y, radius, radius);
}

// fonction qui dessine une ligne
void Renderer::draw_line(float x1, float y1, float x2, float y2) const
{
    ofDrawLine(x1, y1, x2, y2);
}

// fonction qui dessine un rectangle
void Renderer::draw_rectangle(float x1, float y1, float x2, float y2) const
{
    ofDrawRectangle(x1, y1, x2 - x1, y2 - y1);
}

// fonction qui dessine une ellipse
void Renderer::draw_ellipse(float x1, float y1, float x2, float y2) const
{
    float diameter_x = x2 - x1;
    float diameter_y = y2 - y1;

    ofDrawEllipse(x1 + diameter_x / 2.0f, y1 + diameter_y / 2.0f, diameter_x, diameter_y);
}

void Renderer::draw_zone(float x1, float y1, float x2, float y2) const
{
    float x2_clamp = min(max(0.0f, x2), (float)ofGetWidth());
    float y2_clamp = min(max(0.0f, y2), (float)ofGetHeight());
    if (draw_mode == VectorPrimitiveType::rectangle)
    {
        ofSetLineWidth(radius);
        ofSetColor(255);
        ofNoFill();
        ofDrawRectangle(x1, y1, x2_clamp - x1, y2_clamp - y1);
        ofFill();
        ofDrawEllipse(x1, y1, radius, radius);
        ofDrawEllipse(x1, y2_clamp, radius, radius);
        ofDrawEllipse(x2_clamp, y1, radius, radius);
        ofDrawEllipse(x2_clamp, y2_clamp, radius, radius);
    }
    else if (draw_mode == VectorPrimitiveType::ellipse || draw_mode == VectorPrimitiveType::polygon)
    {
        if (draw_mode == VectorPrimitiveType::polygon)
        {
            ofSetCircleResolution(6);
        }
        else {
            ofSetCircleResolution(128);
        }
        //ofSetLineWidth(radius);
        ofSetColor(255);
        ofNoFill();
        float heightdiff = (y2 - y1) / 2.0f;
        float widthdiff = (x2 - x1) / 2.0f;
        ofDrawEllipse(x1 + widthdiff, y1 + heightdiff, x2_clamp - x1, y2_clamp - y1);
    }
    else if (draw_mode == VectorPrimitiveType::line)
    {
        ofDrawLine(x1, y1, x2_clamp, y2_clamp);
    }
}
void Renderer::loadImage(const std::string& path, float imgX, float imgY)
{
    ImageData imgData;
    if (imgData.image.load(path))
    {
        imgData.original = imgData.image;
        imgData.imgX = imgX;
        imgData.imgY = imgY;
        imgData.isDragging = false;
        images.push_back(imgData);
        ((Application*)ofGetAppPtr())->openHistogramWindow(imgData.image);
    }
}

void Renderer::startDrag(float mouseX, float mouseY)
{
    selectedImageIndex = -1;
    if (draw_mode == VectorPrimitiveType::none)
    {
        for (int i = 0; i < images.size(); i++)
        {
            auto& imgData = images[i];

            if (mouseX >= imgData.imgX && mouseX <= imgData.imgX + imgData.image.getWidth() &&
                mouseY >= imgData.imgY && mouseY <= imgData.imgY + imgData.image.getHeight())
            {
                imgData.isDragging = true;
                imgData.dragOffsetX = mouseX - imgData.imgX;
                imgData.dragOffsetY = mouseY - imgData.imgY;

                selectedImageIndex = i;
                break;
            }
        }
    }
}

void Renderer::updateDrag(float mouseX, float mouseY)
{
    for (auto& imgData : images)
    {
        if (imgData.isDragging)
        {
            imgData.imgX = mouseX - imgData.dragOffsetX;
            imgData.imgY = mouseY - imgData.dragOffsetY;
        }
    }
}

void Renderer::stopDrag()
{
    for (auto& imgData : images)
    {
        imgData.isDragging = false;
    }
}

Renderer::~Renderer()
{
    std::free(shapes);
}
void Renderer::saveScreenshot(const std::string& filename)
{
    ofImage screenshot;
    screenshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    screenshot.save(filename);
    ofLog() << "Screenshot saved: " << filename;
}

void Renderer::drawInteractionArea() {
    ofRectangle vp = ofGetCurrentViewport();
    float r = std::min<float>(vp.width, vp.height) * 0.5f;
    float x = vp.width * 0.5f;
    float y = vp.height * 0.5f;

    ofPushStyle();
    ofSetLineWidth(3);
    ofSetColor(255, 255, 0);
    ofNoFill();
    glDepthMask(false);
    ofSetCircleResolution(100);
    ofDrawCircle(x, y, r);
    glDepthMask(true);
    ofPopStyle();
}

ofMesh Renderer::build_geometric_mesh(GeometricPrimitive primitive3D)
{
    vector<glm::vec3> vertices;
    vector<unsigned int> indices;
    float size = primitive3D.size;

    switch (primitive3D.type) {
    case GeometricPrimitiveType::cube:
        vertices.push_back(glm::vec3(-size, -size, size));
        vertices.push_back(glm::vec3(size, -size, size));
        vertices.push_back(glm::vec3(size, size, size));
        vertices.push_back(glm::vec3(-size, size, size));
        vertices.push_back(glm::vec3(-size, -size, -size));
        vertices.push_back(glm::vec3(size, -size, -size));
        vertices.push_back(glm::vec3(+size, +size, -size));
        vertices.push_back(glm::vec3(-size, +size, -size));

        indices.insert(indices.end(), {
            // Front
            0, 1, 2, 2, 3, 0,
            // Right
            1, 5, 6, 6, 2, 1,
            // Back
            5, 4, 7, 7, 6, 5,
            // Left
            4, 0, 3, 3, 7, 4,
            // Top
            3, 2, 6, 6, 7, 3,
            // Bottom
            4, 5, 1, 1, 0, 4
            });
        break;
    case GeometricPrimitiveType::pyramid:
        vertices.push_back(glm::vec3(-size, -size, size));
        vertices.push_back(glm::vec3(size, -size, size));
        vertices.push_back(glm::vec3(-size, -size, -size));
        vertices.push_back(glm::vec3(size, -size, -size));
        vertices.push_back(glm::vec3(0, size, 0));

        indices.insert(indices.end(), {
            // Front
            0, 1, 4,
            // Right
            1, 3, 4,
            // Back
            3, 2, 4,
            // Left
            2, 0, 4,
            // Bottom
            2, 3, 1,
            1, 0, 2
            });
        break;
    default:
        break;
    }

    ofMesh newMesh;
    newMesh.clear();

    // Add vertices to mesh
    for (const auto& v : vertices) {
        newMesh.addVertex(v);
    }

    // Add indices to mesh to build triangles
    for (const auto& i : indices) {
        newMesh.addIndex(i);
    }

    // Approximate normalsw
    for (const auto& v : primitive3D.mesh.getVertices()) {
        newMesh.addNormal(normalize(v));
    }

    return newMesh;
}

glm::vec3 Renderer::screen_to_world_pos(float x, float y, float depth, ofEasyCam& cam) {
    glm::vec3 screenPos(x, y, 0.0f);

    glm::vec3 worldNear = cam.screenToWorld(screenPos);
    screenPos.z = 1.0f;
    glm::vec3 worldFar = cam.screenToWorld(screenPos);

    glm::vec3 worldPos = worldNear + normalize(worldFar - worldNear) * depth;
    return worldPos;
}

void Renderer::undo2D() {
    /*ofLog() << "on rentre";
    VectorPrimitive objet = stackUndo2D.top();
    for (index = 0; index < buffer_count; ++index)
    {
        if (shapes[index].type == objet.type && shapes[index].position1[0] == objet.position1[0] && shapes[index].position1[1] == objet.position1[1])
        {
            shapes[index].type = VectorPrimitiveType::none;
        }
    }
    stackRedo2D.push(objet);
    stackUndo2D.pop();*/
}

void Renderer::undo3D() {
    if (!stackUndo3D.empty())
    {
        std::pair<int, std::pair<ActionType, GeometricPrimitive>> objet = stackUndo3D.top();
        stackUndo3D.pop();
        if (objet.second.first == ActionType::add)
        {
            reset_geometric_primitive(objet.first, true);
            stackRedo3D.push(std::pair(objet.first, std::pair(ActionType::remove, objet.second.second)));
        }
        else if (objet.second.first == ActionType::remove)
        {
            add_geometric_shape(objet.second.second.type, objet.second.second.position[0], objet.second.second.position[1], objet.second.second.position[2], true);
            stackRedo3D.push(std::pair(objet.first, std::pair(ActionType::add, objet.second.second)));
        }
    }
    
}

void Renderer::redo2D() {

}

void Renderer::redo3D() {
    if (!stackRedo3D.empty())
    {
        std::pair<int, std::pair<ActionType, GeometricPrimitive>> objet = stackRedo3D.top();
        stackRedo3D.pop();
        if (objet.second.first == ActionType::remove)
        {
            add_geometric_shape(objet.second.second.type, objet.second.second.position[0], objet.second.second.position[1], objet.second.second.position[2], true);
        }
        else if (objet.second.first == ActionType::add)
        {
            reset_geometric_primitive(objet.first, true);
        }
    }
}

void Renderer::emptyStacks() {
    while (!stackUndo3D.empty())
    {
        stackUndo3D.pop();
    }
    while (!stackRedo3D.empty())
    {
        stackRedo3D.pop();
    }
}

void Renderer::updateBezier()
{
    if (!currentModeis2D)
    {
        if (!blackHole)
        {
            switch (controlPointChosen) {
            case 0: controlPoints3D[0][0][1] = controlPoints3DOrigine[0][0][1] + bezierHeight; break;
            case 1: controlPoints3D[0][1][1] = controlPoints3DOrigine[0][1][1] + bezierHeight; break;
            case 2: controlPoints3D[0][2][1] = controlPoints3DOrigine[0][2][1] + bezierHeight; break;
            case 3: controlPoints3D[0][3][1] = controlPoints3DOrigine[0][3][1] + bezierHeight; break;
            case 4: controlPoints3D[1][0][1] = controlPoints3DOrigine[1][0][1] + bezierHeight; break;
            case 5: controlPoints3D[1][1][1] = controlPoints3DOrigine[1][1][1] + bezierHeight; break;
            case 6: controlPoints3D[1][2][1] = controlPoints3DOrigine[1][2][1] + bezierHeight; break;
            case 7: controlPoints3D[1][3][1] = controlPoints3DOrigine[1][3][1] + bezierHeight; break;
            case 8: controlPoints3D[2][0][1] = controlPoints3DOrigine[2][0][1] + bezierHeight; break;
            case 9: controlPoints3D[2][1][1] = controlPoints3DOrigine[2][1][1] + bezierHeight; break;
            case 10: controlPoints3D[2][2][1] = controlPoints3DOrigine[2][1][1] + bezierHeight; break;
            case 11: controlPoints3D[2][3][1] = controlPoints3DOrigine[2][1][1] + bezierHeight; break;
            case 12: controlPoints3D[3][0][1] = controlPoints3DOrigine[3][0][1] + bezierHeight; break;
            case 13: controlPoints3D[3][1][1] = controlPoints3DOrigine[3][1][1] + bezierHeight; break;
            case 14: controlPoints3D[3][2][1] = controlPoints3DOrigine[3][2][1] + bezierHeight; break;
            case 15: controlPoints3D[3][3][1] = controlPoints3DOrigine[3][3][1] + bezierHeight; break;
            }
        }
        else {
            controlPoints3D[1][1][1] += -10;
            controlPoints3D[1][2][1] += -10;
            controlPoints3D[2][1][1] += -10;
            controlPoints3D[2][2][1] += -10;
        }
    }
    else {
        switch (controlPointChosen) {
        case 0 : controlPoints2D[0][1] = controlPoints2DOrigine[0][1] + bezierHeight; break;
        case 1 : controlPoints2D[1][1] = controlPoints2DOrigine[1][1] + bezierHeight; break;
        case 2 : controlPoints2D[2][1] = controlPoints2DOrigine[2][1] + bezierHeight; break;
        case 3 : controlPoints2D[3][1] = controlPoints2DOrigine[3][1] + bezierHeight; break;
        case 4 : controlPoints2D[4][1] = controlPoints2DOrigine[4][1] + bezierHeight; break;
        case 5 : controlPoints2D[5][1] = controlPoints2DOrigine[5][1] + bezierHeight; break;
        case 6 : controlPoints2D[6][1] = controlPoints2DOrigine[6][1] + bezierHeight; break;
        case 7 : controlPoints2D[7][1] = controlPoints2DOrigine[7][1] + bezierHeight; break;
        }
    }
}

ofVec3f Renderer::computeBezierSurface(float u, float v) 
{
    ofVec3f p(0, 0, 0);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float bx = computePoint(i, u);
            float by = computePoint(j, v);
            p += controlPoints3D[i][j] * bx * by;
        }
    }
    return p;
}

float Renderer::computePoint(int i, float t) {
    switch (i) {
    case 0: return (1 - t) * (1 - t) * (1 - t);
    case 1: return 3 * t * (1 - t) * (1 - t);
    case 2: return 3 * t * t * (1 - t);
    case 3: return t * t * t;
    }
    return 0.0f;
}

void Renderer::setupBezier()
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            controlPoints3D[i][j] = ofVec3f(i * 200 - 1250, ofRandom(-200, 200) - 500, j * 200);
            controlPoints3DOrigine[i][j] = controlPoints3D[i][j];
        }
    }
    controlPoints2D.push_back(ofVec2f(400, ofRandom(-300, 300) + (1080 / 2)));
    controlPoints2DOrigine.push_back(controlPoints2D[0]);
    controlPoints2D.push_back(ofVec2f(550, ofRandom(-300, 300) + (1080 / 2)));
    controlPoints2DOrigine.push_back(controlPoints2D[1]);
    controlPoints2D.push_back(ofVec2f(700, ofRandom(-300, 300) + (1080 / 2)));
    controlPoints2DOrigine.push_back(controlPoints2D[2]);
    controlPoints2D.push_back(ofVec2f(850, ofRandom(-300, 300) + (1080 / 2)));
    controlPoints2DOrigine.push_back(controlPoints2D[3]);
    controlPoints2D.push_back(ofVec2f(1000, ofRandom(-300, 300) + (1080 / 2)));
    controlPoints2DOrigine.push_back(controlPoints2D[4]);
    controlPoints2D.push_back(ofVec2f(1150, ofRandom(-300, 300) + (1080 / 2)));
    controlPoints2DOrigine.push_back(controlPoints2D[5]);
    controlPoints2D.push_back(ofVec2f(1300, ofRandom(-300, 300) + (1080 / 2)));
    controlPoints2DOrigine.push_back(controlPoints2D[6]);
    controlPoints2D.push_back(ofVec2f(1450, ofRandom(-300, 300) + (1080 / 2)));
    controlPoints2DOrigine.push_back(controlPoints2D[7]);
}

ofVec2f Renderer::computeBezierCurve(vector<ofVec2f> points, float t) {
    if (points.size() == 1) {
        return points[0];
    }
    vector<ofVec2f> newPoints;
    for (int i = 0; i < points.size() - 1; i++) {
        float x = (1 - t) * points[i].x + t * points[i + 1].x;
        float y = (1 - t) * points[i].y + t * points[i + 1].y;
        newPoints.push_back(ofVec2f(x, y));
    }
    return computeBezierCurve(newPoints, t);
}