// IFT3100H24_Move/application.cpp
// Classe principale de l'application.

#include "application.h"
#include "ofFileUtils.h"
Application::Application() : lighting(renderer) { }
void Application::setup()
{
    ofLog() << "<app::setup>";

    ofSetWindowTitle("Projet Session Test");

    is_key_press_up = false;
    is_key_press_down = false;
    is_key_press_left = false;
    is_key_press_right = false;
    is2DModeActive = renderer.isMode2D;
    lastSelectedDrawing2D = -1;
    currentSelectedDrawing2D = lastSelectedDrawing2D;
    isWireFrameActive = false;

    ofNoFill();

    gui.setup("interface");
    gui2.setup("objects interface");
    gui2.setPosition(225, 10);
    group_Mode2D3D.setup("Mode 2D/3D");
    group_draw.setup("Couleurs");
    group_tools.setup("Outils");
    group_parameters.setup("Parametres");
    group_tools3D.setup("Outils");
    group_listObjets.setup("List d'objets");
    group_transform2D.setup("Transform selected 2D");
    group_delete.setup("Delete selected");
    group_delete3D.setup("Delete objects");
    group_bezier.setup("Bezier");
    group_pbr_sphere1.setup("PBR Sphere 1");
    group_pbr_sphere2.setup("PBR Sphere 2");
    group_pbr_floor.setup("PBR Floor");
    group_light_options.setup("Lights");
    group_apply_material.setup("Apply materials");

    setupMode2D3DGui();
    setupColorGui();
    setupToolsGui();
    setupToolsGui3D();
    setup2DPrimitiveParameters();
    setupListObjets();
    setupHistory();
    setupTransform2D();
    setupDelete();
    setupBezierGui();
    setupLightGui();
    setupAmbientColorGui();
    setupApplyMaterialGui();

    gui.add(&group_Mode2D3D);
    gui.add(&group_draw);
    gui.add(&group_tools);
    gui.add(&group_parameters);
    gui.add(&group_history);
    gui.add(&group_transform2D);
    gui.add(&group_bezier);
    gui2.add(&group_delete);
    gui2.add(&group_listObjets);

    renderer.setup();
    lighting.setup(renderer);
    cursorImage.load("cursor_default.png");
    lastShapesIndex = 0;
    lastSelectedCount = 0;
    nextTimeCheck = 0;
    group_imageColor.setup("Image Color Settings");

    group_imageColor.add(useHSB.set("Use HSB", false));
    group_imageColor.add(sliderR.set("Red", 255, 0, 255));
    group_imageColor.add(sliderG.set("Green", 255, 0, 255));
    group_imageColor.add(sliderB.set("Blue", 255, 0, 255));
    group_imageColor.add(sliderH.set("Hue shift", 0, -128, 128));
    group_imageColor.add(sliderS.set("Sat shift", 0, -128, 128));
    group_imageColor.add(sliderV.set("Bri shift", 0, -128, 128));

    useHSB.addListener(this, &Application::onColorParamsChangedBool);
    sliderR.addListener(this, &Application::onColorParamsChangedFloat);
    sliderG.addListener(this, &Application::onColorParamsChangedFloat);
    sliderB.addListener(this, &Application::onColorParamsChangedFloat);
    sliderH.addListener(this, &Application::onColorParamsChangedFloat);
    sliderS.addListener(this, &Application::onColorParamsChangedFloat);
    sliderV.addListener(this, &Application::onColorParamsChangedFloat);

    group_filters.setup("Filtres");

    buttonFilterGray.setup("Grayscale");
    buttonFilterGray.addListener(this, &Application::applyFilterGray);

    buttonFilterBlur.setup("Blur");
    buttonFilterBlur.addListener(this, &Application::applyFilterBlur);

    buttonFilterToneMap.setup("Tone Mapping");
    buttonFilterToneMap.addListener(this, &Application::applyFilterToneMap);

    buttonFilterSharpen.setup("Sharpen (Convolution)");
    buttonFilterSharpen.addListener(this, &Application::applyFilterSharpen);

    group_filters.add(&buttonFilterGray);
    group_filters.add(&buttonFilterBlur);
    group_filters.add(&buttonFilterToneMap);
    group_filters.add(&buttonFilterSharpen);

    gui.add(&group_filters);
    group_tonemap.setup("Tone Mapping");

    buttonExposurePlus.setup("Exposure +");
    buttonExposurePlus.addListener(this, &Application::onExposurePlus);
    group_tonemap.add(&buttonExposurePlus);

    buttonExposureMinus.setup("Exposure -");
    buttonExposureMinus.addListener(this, &Application::onExposureMinus);
    group_tonemap.add(&buttonExposureMinus);

    buttonResetImage.setup("Reset Image");
    buttonResetImage.addListener(this, &Application::resetSelectedImageToOriginal);
    group_tonemap.add(&buttonResetImage);
    gui.add(&group_tonemap);
}

void Application::setupTransform2D() {
    group_transform2D.clear();
    if (lastSelectedDrawing2D == -1) {
        fieldPosX.setup("Pos 1 X: ", 0);
        group_transform2D.add(&fieldPosX);
        fieldPosY.setup("Pos 1 Y: ", 0);
        group_transform2D.add(&fieldPosY);

        fieldPos2X.setup("Pos 2 X: ", 0);
        group_transform2D.add(&fieldPos2X);
        fieldPos2Y.setup("Pos 2 Y: ", 0);
        group_transform2D.add(&fieldPos2Y);
    }
    else {
        fieldPosX.setup("Pos X: ", renderer.shapes[lastSelectedDrawing2D].position1[0]);
        group_transform2D.add(&fieldPosX);
        fieldPosY.setup("Pos Y: ", renderer.shapes[lastSelectedDrawing2D].position1[1]);
        group_transform2D.add(&fieldPosY);

        fieldPos2X.setup("Size X: ", renderer.shapes[lastSelectedDrawing2D].position2[0]);
        group_transform2D.add(&fieldPos2X);
        fieldPos2Y.setup("Size Y: ", renderer.shapes[lastSelectedDrawing2D].position2[1]);
        group_transform2D.add(&fieldPos2Y);
    }

    buttonTransform.setup("Apply Transform");
    buttonTransform.addListener(this, &Application::transformDrawing2D);
    group_transform2D.add(&buttonTransform);
}

void Application::setupPBRGui() {
    // Initialisez les sliders avec vos valeurs initiales
    metallicSlider.setup("Metallic value", portailApp.get()->metallicStrength, 0.0f, 1.0f);
    roughnessSlider.setup("Roughness value", portailApp.get()->roughnessStrength, 0.0f, 1.0f);
    texCoordScale1Slider.setup("Texture Coordonnée X", portailApp.get()->texCoordScale1, 0.0f, 1.0f);
    texCoordScale2Slider.setup("Texture Coordonnée Y", portailApp.get()->texCoordScale2, 0.0f, 1.0f);
    clearCoatStrengthSlider.setup("Metallic value", portailApp.get()->clearCoatStrength, 0.0f, 1.0f);
    clearCoatRoughnessSlider.setup("Roughness value", portailApp.get()->clearCoatRoughness, 0.0f, 1.0f);
    texCoordScale1SliderFloor.setup("Texture Coordonnée X", portailApp.get()->texCoordScale1Floor, 0.0f, 1.0f);
    texCoordScale2SliderFloor.setup("Texture Coordonnée Y", portailApp.get()->texCoordScale2Floor, 0.0f, 1.0f);
    clearCoatStrengthSliderFloor.setup("Metallic value", portailApp.get()->clearCoatStrengthFloor, 0.0f, 1.0f);
    clearCoatRoughnessSliderFloor.setup("Roughness value", portailApp.get()->clearCoatRoughnessFloor, 0.0f, 1.0f);

    // Ajout des contrôles dans le panel
    group_pbr_sphere1.add(&metallicSlider);
    group_pbr_sphere1.add(&roughnessSlider);
    group_pbr_sphere2.add(&texCoordScale1Slider);
    group_pbr_sphere2.add(&texCoordScale2Slider);
    group_pbr_sphere2.add(&clearCoatStrengthSlider);
    group_pbr_sphere2.add(&clearCoatRoughnessSlider);
    group_pbr_floor.add(&texCoordScale1SliderFloor);
    group_pbr_floor.add(&texCoordScale2SliderFloor);
    group_pbr_floor.add(&clearCoatStrengthSliderFloor);
    group_pbr_floor.add(&clearCoatRoughnessSliderFloor);

    gui.add(&group_pbr_sphere1);
    gui.add(&group_pbr_sphere2);
    gui.add(&group_pbr_floor);
}


void Application::updateAmbientLight() {
    lighting.setAmbientColor(color_picker_ambient);
}

void Application::setupAmbientColorGui()
{
    group_light_ambient.setup("Ambient color");
    color_picker_ambient.set("Color Picker", ofColor(0), ofColor(0, 0), ofColor(255, 255));
    group_light_ambient.add(color_picker_ambient);
}

void Application::setupLightGui() {
    buttonDirectionalLight.setup("Directional light");
    buttonDirectionalLight.addListener(this, &Application::directionalLightChosen);
    group_light_options.add(&buttonDirectionalLight);
    buttonPointLight.setup("Point light");
    buttonPointLight.addListener(this, &Application::pointLightChosen);
    group_light_options.add(&buttonPointLight);
    buttonSpotLight.setup("Spotlight");
    buttonSpotLight.addListener(this, &Application::spotLightChosen);
    group_light_options.add(&buttonSpotLight);
}

void Application::directionalLightChosen() {
    renderer.draw_mode3D = GeometricPrimitiveType::none;
    lighting.light_draw_mode = LightType::directional;
}

void Application::pointLightChosen() {
    renderer.draw_mode3D = GeometricPrimitiveType::none;
    lighting.light_draw_mode = LightType::point;
}

void Application::spotLightChosen() {
    renderer.draw_mode3D = GeometricPrimitiveType::none;
    lighting.light_draw_mode = LightType::spot;
}

void Application::setupApplyMaterialGui() {
    buttonApplyMat0.setup("Default");
    buttonApplyMat0.addListener(this, &Application::material0Chosen);
    group_apply_material.add(&buttonApplyMat0);
    buttonApplyMat1.setup("Material Sphere PBR 1");
    buttonApplyMat1.addListener(this, &Application::material1Chosen);
    group_apply_material.add(&buttonApplyMat1);
    buttonApplyMat2.setup("Material Rough");
    buttonApplyMat2.addListener(this, &Application::material2Chosen);
    group_apply_material.add(&buttonApplyMat2);
    buttonApplyMat3.setup("Material Metal");
    buttonApplyMat3.addListener(this, &Application::material3Chosen);
    group_apply_material.add(&buttonApplyMat3);
}

void Application::material0Chosen() {
    applyMaterialToSelected(0);
}

void Application::material1Chosen() {
    applyMaterialToSelected(1);
}

void Application::material2Chosen() {
    applyMaterialToSelected(2);
}

void Application::material3Chosen() {
    applyMaterialToSelected(3);
}

void Application::applyMaterialToSelected(int chosenMat) {
    ofMaterial material;
    switch (chosenMat) {
    case 0:
        material = renderer.defaultMaterial;
        break;
    case 1:
        material = portailApp.get()->matSphere;
        break;
    case 2:
        material = renderer.material2;
        break;
    case 3:
        material = renderer.material3;
        break;
    default:
        return;
    }
    for (int i = 0; i < renderer.shapes3D.size(); ++i)
    {
        if (renderer.shapes3D[i].selected) {
            renderer.shapes3D[i].material = material;
            renderer.shapes3D[i].chosenMaterial = chosenMat;
        }
    }
}

void Application::transformDrawing2D() {
    if (lastSelectedDrawing2D != -1) {
        renderer.shapes[lastSelectedDrawing2D].position1[0] = std::stof(fieldPosX.getParameter().toString());
        renderer.shapes[lastSelectedDrawing2D].position1[1] = std::stof(fieldPosY.getParameter().toString());
        renderer.shapes[lastSelectedDrawing2D].position2[0] = std::stof(fieldPos2X.getParameter().toString());
        renderer.shapes[lastSelectedDrawing2D].position2[1] = std::stof(fieldPos2Y.getParameter().toString());
        lastSelectedDrawing2D == -1;
        setupTransform2D();
    }
}

void Application::setupDelete() {
    buttonDelete.setup("Delete all selected");
    buttonDelete.addListener(this, &Application::deleteDrawing);
    group_delete.add(&buttonDelete);
    
    buttonExport.setup("Export selected image");
    buttonExport.addListener(this, &Application::exportSelectedImage);
    group_delete.add(&buttonExport);

    buttonDeleteAll.setup("Delete all");
    buttonDeleteAll.addListener(this, &Application::deleteAll3D);
    group_delete3D.add(&buttonDeleteAll);
    group_delete3D.add(&buttonDelete);
    group_delete3D.add(&buttonExport);
}

void Application::setupHistory() {
    buttonUndo.setup("Undo");
    buttonUndo.addListener(this, &Application::undo);
    group_history.add(&buttonUndo);
    buttonRedo.setup("Redo");
    buttonRedo.addListener(this, &Application::redo);
    group_history.add(&buttonRedo);
}

void Application::undo() {
    if (renderer.isMode2D)
    {
        renderer.undo2D();
    }
    else {
        renderer.undo3D();
    }
}

void Application::redo() {
    if (renderer.isMode2D)
    {
        renderer.redo2D();
    }
    else {
        renderer.redo3D();
    }
}

void Application::setupListObjets() {
    group_listObjets.clear();
    toggles.clear(); // Nettoyer les anciens boutons
    labels.clear();
    if (is2DModeActive) {
        for (int i = 0; i < renderer.currentShapeIndex; i++)
        {
            auto newToggle = std::make_unique<ofxToggle>();
            auto newLable = std::make_unique<ofxLabel>();

            switch (renderer.shapes[i].type)
            {
            case VectorPrimitiveType::polygon:
                newLable->setup(std::to_string(i) + " polygone");
                break;
            case VectorPrimitiveType::ellipse:
                newLable->setup(std::to_string(i) + " ellipse");
                break;
            case VectorPrimitiveType::line:
                newLable->setup(std::to_string(i) + " line");
                break;
            case VectorPrimitiveType::point:
                newLable->setup(std::to_string(i) + " point");
                break;
            case VectorPrimitiveType::rectangle:
                newLable->setup(std::to_string(i) + " rectangle");
                break;
            default:
                continue;
            }
            newToggle->setup(false);

            group_listObjets.add(newLable.get());
            group_listObjets.add(newToggle.get());

            toggles.push_back(std::move(newToggle));
            labels.push_back(std::move(newLable));
        }
    }
    else {
        for (int i = 0; i < renderer.shapes3D.size(); ++i)
        {
            auto newToggle = std::make_unique<ofxToggle>();
            auto newLable = std::make_unique<ofxLabel>();
            if (renderer.shapes3D[i].type == GeometricPrimitiveType::none) {
                toggles.push_back(std::move(newToggle));
                labels.push_back(std::move(newLable));
            }
            else {
                newLable->setup(std::to_string(i) + " " + renderer.shapes3D[i].name);
                newToggle->setup(renderer.shapes3D[i].selected);

                group_listObjets.add(newLable.get());
                group_listObjets.add(newToggle.get());

                toggles.push_back(std::move(newToggle));
                labels.push_back(std::move(newLable));
            }
        }
        for (int i = 0; i < MAX_LIGHTS; ++i)
        {
            auto newToggle = std::make_unique<ofxToggle>();
            auto newLable = std::make_unique<ofxLabel>();
            if (lighting.lights[i].type == LightType::none) {
                toggles.push_back(std::move(newToggle));
                labels.push_back(std::move(newLable));
            }
            else {
                newLable->setup(std::to_string(i + renderer.shapes3D.size()) + " " + lighting.lights[i].name + " light");
                newToggle->setup(lighting.lights[i].selected);

                group_listObjets.add(newLable.get());
                group_listObjets.add(newToggle.get());

                toggles.push_back(std::move(newToggle));
                labels.push_back(std::move(newLable));
            }
        }
    }
}

void Application::deleteDrawing() {
    std::vector<int> listToDelete;
    for (int i = 0; i < toggles.size(); i++) {
        if (toggles[i].get()->getParameter().toString() == "1") {
            listToDelete.push_back(i);
        }
    }
    if (is2DModeActive) {
        for (int j = listToDelete.size() - 1; j >= 0; j--) {
            if (listToDelete[j] == renderer.buffer_head - 1) {
                renderer.shapes[listToDelete[j]].type = VectorPrimitiveType::none;
            }
            else {
                for (int l = renderer.buffer_head - 1; l > 0; l--) {
                    if (std::find(listToDelete.begin(), listToDelete.end(), l) != listToDelete.end()) {
                        continue;
                    }
                    else {
                        renderer.shapes[listToDelete[j]] = renderer.shapes[l];
                        renderer.shapes[l].type = VectorPrimitiveType::none;
                        listToDelete[j] = l;
                        break;
                    }
                }
            }
            renderer.buffer_head--;
        }
    }
    else {
        for (int i = 0; i < listToDelete.size(); i++) {
            if (listToDelete[i] >= renderer.shapes3D.size())
                lighting.remove_light(listToDelete[i] - renderer.shapes3D.size());
            else
                renderer.reset_geometric_primitive(listToDelete[i]);
        }
    }
    setupListObjets();
    lastSelectedDrawing2D = -1;
}

void Application::deleteAll3D() {
    for (int i = 0; i < MAX_LIGHTS; i++)
        lighting.remove_light(i);
    for(int i = 0; i < renderer.shapes3D.size(); i++)
        renderer.reset_geometric_primitive(i);
    setupListObjets();
}

void Application::ProjectionModeChange()
{
    if (renderer.cam.getOrtho())
    {
        renderer.cam.disableOrtho();
    }
    else
    {
        renderer.cam.enableOrtho();
    };
}

void Application::setupMode2D3DGui()
{
    buttonMode2D3D.setup("2D");
    buttonMode2D3D.addListener(this, &Application::mode2D3D);
    group_Mode2D3D.add(&buttonMode2D3D);
}

void Application::setup2DPrimitiveParameters()
{
    checkboxFill.setName("Fill");
    checkboxStroke.setName("Stroke");

    checkboxFill = true;
    checkboxStroke = true;

    slider_stroke_weight.set("largeur de la ligne", 1.0f, 1.0f, 10.0f);

    group_parameters.add(checkboxFill);
    group_parameters.add(checkboxStroke);
    group_parameters.add(slider_stroke_weight);
}

void Application::setupColorGui()
{
    group_draw.setup("Couleurs");

    color_picker_background.set("couleur du canevas", ofColor(31), ofColor(0, 0), ofColor(255, 255));
    color_picker_stroke.set("couleur du trait", ofColor(0), ofColor(0, 0), ofColor(255, 255));
    color_picker_fill.set("couleur de la primitive", ofColor(255), ofColor(0, 0), ofColor(255, 255));

    group_draw.add(color_picker_background);
    group_draw.add(color_picker_stroke);
    group_draw.add(color_picker_fill);
    

}

void Application::setupBezierGui()
{
    group_bezier.setup("Bezier");
    bezierHeight.set("hauteur du point", 0.0f, -200.0f, 200.f);
    controlPoint.set("point de controle", 0, 0, 15);
    activateBezier.set("Montre bezier", true);
    blackHole.set("Creer trou noir", false);
    tessellation.set("Tessellation", 0, 0, 2);
    nbrPointsCtrl.set("Nombres points controle", 4, 4, 8);
    group_bezier.add(activateBezier);
    group_bezier.add(bezierHeight);
    group_bezier.add(controlPoint);
    group_bezier.add(tessellation);
    group_bezier.add(blackHole);
    group_bezier.add(nbrPointsCtrl);
}

void Application::setupToolsGui()
{
    buttonNone.setup("None");
    buttonNone.addListener(this, &Application::noneChosen);
    group_tools.add(&buttonNone);
    buttonPixel.setup("Polygon");
    buttonPixel.addListener(this, &Application::polygonChosen);
    group_tools.add(&buttonPixel);
    buttonPoint.setup("Point");
    buttonPoint.addListener(this, &Application::pointChosen);
    group_tools.add(&buttonPoint);
    buttonLine.setup("Line");
    buttonLine.addListener(this, &Application::lineChosen);
    group_tools.add(&buttonLine);
    buttonRectangle.setup("Rectangle");
    buttonRectangle.addListener(this, &Application::rectangleChosen);
    group_tools.add(&buttonRectangle);
    buttonEllipse.setup("Ellipse");
    buttonEllipse.addListener(this, &Application::ellipseChosen);
    group_tools.add(&buttonEllipse);
}

void Application::setupToolsGui3D()
{
    buttonNone3D.setup("None");
    buttonNone3D.addListener(this, &Application::noneChosen3D);
    group_tools3D.add(&buttonNone3D);
    buttonCube.setup("Cube");
    buttonCube.addListener(this, &Application::cubeChosen);
    group_tools3D.add(&buttonCube);
    buttonPyramid.setup("Pyramid");
    buttonPyramid.addListener(this, &Application::pyramidChosen);
    group_tools3D.add(&buttonPyramid);
    buttonSphere.setup("Sphere");
    buttonSphere.addListener(this, &Application::sphereChosen);
    group_tools3D.add(&buttonSphere);

    buttonImport1.setup("1st import");
    buttonImport1.addListener(this, &Application::imported_model_1_chosen);
    group_tools3D.add(&buttonImport1);
    buttonImport2.setup("2nd import");
    buttonImport2.addListener(this, &Application::imported_model_2_chosen);
    group_tools3D.add(&buttonImport2);
    buttonImport3.setup("3rd import");
    buttonImport3.addListener(this, &Application::imported_model_3_chosen);
    group_tools3D.add(&buttonImport3);

    buttonWireframe.setup("Toggle wireframe");
    buttonWireframe.addListener(this, &Application::toggle_wireframe);
    group_tools3D.add(&buttonWireframe);
    buttonProjectionMode.setup("Change Projection Mode");
    buttonProjectionMode.addListener(this, &Application::ProjectionModeChange);
    group_tools3D.add(&buttonProjectionMode);
}

void Application::setColorSelected() {
    for (int i = 0; i < toggles.size(); i++) {
        if (toggles[i].get()->getParameter().toString() == "1") {
            renderer.shapes[i].fill_color[0] = 255;
            renderer.shapes[i].fill_color[1] = 0;
            renderer.shapes[i].fill_color[2] = 0;
            lastSelectedDrawing2D = i;
        }
        else {
            if (colors.size() > 0) {
                renderer.shapes[i].fill_color[0] = colors[i].get()->r;
                renderer.shapes[i].fill_color[1] = colors[i].get()->g;
                renderer.shapes[i].fill_color[2] = colors[i].get()->b;
            }
        }
    }
    renderer.draw();
}
    
void Application::update()
{
    int selected_toggles_count = update_selected_3D();
    if (is2DModeActive) {
        if (lastSelectedDrawing2D != currentSelectedDrawing2D) {
            currentSelectedDrawing2D = lastSelectedDrawing2D;
            setupTransform2D();
            gui.clear();
            gui.add(&group_Mode2D3D);
            if (is2DModeActive) {
                gui.add(&group_draw);
                gui.add(&group_tools);
                gui.add(&group_parameters);
                gui.add(&group_transform2D);
                gui.add(&group_bezier);
            }
            else {
                gui.add(&group_tools3D);
                gui.add(&group_bezier);
                gui.add(&group_pbr_sphere1);
                gui.add(&group_pbr_sphere2);
                gui.add(&group_pbr_floor);
            }
            gui.add(&group_history);
            gui.add(&group_delete);
            gui.add(&group_listObjets);
        }
        if (lastShapesIndex != renderer.currentShapeIndex) {
            lastShapesIndex = renderer.currentShapeIndex;
            setupListObjets();
            colors.clear();
            for (int i = 0; i < toggles.size(); i++) {
                auto newColor = std::make_unique<ofColor>();
                newColor.get()->r = renderer.shapes[i].fill_color[0];
                newColor.get()->g = renderer.shapes[i].fill_color[1];
                newColor.get()->b = renderer.shapes[i].fill_color[2];
                colors.push_back(std::move(newColor));
            }
        }
    }
    else if (renderer.currentShapeIndex != lastShapesIndex) {
        lastShapesIndex = renderer.currentShapeIndex;
        lastSelectedCount = selected_toggles_count;
        setupListObjets();
    }
    if (is2DModeActive != renderer.isMode2D) {
        lastSelectedDrawing2D = -1;
        is2DModeActive = renderer.isMode2D;
        gui.clear();
        gui2.clear();
        gui.add(&group_Mode2D3D);
        if (is2DModeActive) {
            gui.add(&group_draw);
            gui.add(&group_tools);
            gui.add(&group_parameters);
            gui.add(&group_transform2D);
            gui.add(&group_bezier);
            gui2.add(&group_delete);
        }
        else {
            gui.add(&group_tools3D);
            gui.add(&group_bezier);
            gui.add(&group_pbr_sphere1);
            gui.add(&group_pbr_sphere2);
            gui.add(&group_pbr_floor);
            gui.add(&group_light_options);
            gui.add(&group_light_ambient);
            gui.add(&group_apply_material);
            gui2.add(&group_delete3D);
        }
        gui.add(&group_history);
        gui2.add(&group_listObjets);
        noneChosen();
    }

    if (portailApp) {
        portailApp.get()->updateValue(1, metallicSlider);
        portailApp.get()->updateValue(2, roughnessSlider);
        portailApp.get()->updateValue(3, texCoordScale1Slider);
        portailApp.get()->updateValue(4, texCoordScale2Slider);
        portailApp.get()->updateValue(5, clearCoatStrengthSlider);
        portailApp.get()->updateValue(6, clearCoatRoughnessSlider);
        portailApp.get()->updateValue(7, texCoordScale1SliderFloor);
        portailApp.get()->updateValue(8, texCoordScale2SliderFloor);
        portailApp.get()->updateValue(9, clearCoatStrengthSliderFloor);
        portailApp.get()->updateValue(10, clearCoatRoughnessSliderFloor);
    }

    if (is2DModeActive && nextTimeCheck < ofGetElapsedTimef() + intervalCheckTime) {
        nextTimeCheck = ofGetElapsedTimef() + intervalCheckTime;
        setColorSelected();
    }
    bool hasImageSelected = (renderer.selectedImageIndex >= 0);

    if (hasImageSelected && !isImageColorGroupAdded) {
        gui.add(&group_imageColor);
        isImageColorGroupAdded = true;
    }
    else if (!hasImageSelected && isImageColorGroupAdded) {
        gui.clear();
        gui.add(&group_Mode2D3D);
        gui.add(&group_draw);
        gui.add(&group_tools);
        gui.add(&group_parameters);
        gui.add(&group_delete);
        gui.add(&group_listObjets);

        isImageColorGroupAdded = false;
    }

    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;

    // déplacement sur le plan XZ
    if (is_key_press_up)
        renderer.offset_z += renderer.delta_z * time_elapsed;
    if (is_key_press_down)
        renderer.offset_z -= renderer.delta_z * time_elapsed;
    if (is_key_press_left)
        renderer.offset_x += renderer.delta_x * time_elapsed;
    if (is_key_press_right)
        renderer.offset_x -= renderer.delta_x * time_elapsed;

    renderer.background_color = color_picker_background;
    renderer.stroke_color = color_picker_stroke;
    renderer.stroke_weight = slider_stroke_weight;
    renderer.fill_color = color_picker_fill;
    renderer.text = textbox;
    renderer.fillEnabled = checkboxFill;
    renderer.strokeEnabled = checkboxStroke;
    updateBezier();
    updateAmbientLight();
    lighting.update();
    renderer.update();
}

void Application::updateBezier()
{
    renderer.showBezier = activateBezier;
    renderer.controlPointChosen = controlPoint;
    renderer.bezierHeight = bezierHeight;
    renderer.blackHole = blackHole;
    renderer.tessellation = tessellation;
    renderer.nbrPointsctrl = nbrPointsCtrl;
    renderer.updateBezier();
}

void Application::draw()
{
    if (is2DModeActive)
        renderer.draw();
    else
        lighting.draw();

    gui.draw();
    gui2.draw();
    drawScreenshotMessage();
    if (customCursorActive)
    {
        cursorImage.draw(ofGetMouseX(), ofGetMouseY());
    }
}

void Application::keyPressed(int key)
{
    if (key == OF_KEY_LEFT || key == 'a' || key == 'A')
        is_key_press_left = true;
    else if (key == OF_KEY_UP || key == 'w' || key == 'W')
        is_key_press_up = true;
    else if (key == OF_KEY_RIGHT || key == 'd' || key == 'D')
        is_key_press_right = true;
    else if (key == OF_KEY_DOWN || key == 's' || key == 'S')
        is_key_press_down = true;
    //screenshot
    else if (key == 'g' || key == 'G')
    {
        captureScreenshot();
    }
}

void Application::keyReleased(int key)
{
    if (key == OF_KEY_LEFT || key == 'a' || key == 'A')
        is_key_press_left = false;
    else if (key == OF_KEY_UP || key == 'w' || key == 'W')
        is_key_press_up = false;
    else if (key == OF_KEY_RIGHT || key == 'd' || key == 'D')
        is_key_press_right = false;
    else if (key == OF_KEY_DOWN || key == 's' || key == 'S')
        is_key_press_down = false;
    else if (key == 'r' || key == 'R')
        renderer.reset();
}

void Application::mouseReleased(int x, int y, int button)
{
    renderer.cam.enableMouseInput();
    renderer.is_mouse_button_pressed = false;

    renderer.mouse_current_x = x;
    renderer.mouse_current_y = y;

    if (is2DModeActive && renderer.draw_mode != VectorPrimitiveType::none)
        renderer.add_vector_shape(renderer.draw_mode);

    renderer.stopDrag();

    ofLog() << "<app::mouse released at: (" << x << ", " << y << ")>";
}

void Application::exit()
{
    buttonPixel.removeListener(this, &Application::polygonChosen);
    buttonPoint.removeListener(this, &Application::pointChosen);
    buttonLine.removeListener(this, &Application::lineChosen);
    buttonRectangle.removeListener(this, &Application::rectangleChosen);
    buttonEllipse.removeListener(this, &Application::ellipseChosen);
    ofLog() << "<app::exit>";
}

void Application::mode2D3D() {
    renderer.isMode2D = !renderer.isMode2D;
    if (!renderer.isMode2D)
        ((Application*)ofGetAppPtr())->openPortailWindow();
    else {
        for (int i = 0; i < MAX_LIGHTS; i++)
        {
            if (lighting.lights[i].type != LightType::none)
                lighting.remove_light(i);
        }
        for (int index = 0; index < renderer.buffer_count3D; ++index)
            renderer.reset_geometric_primitive(index);
    }

    isWireFrameActive = false;
}

// 2D
void Application::noneChosen()
{
    renderer.draw_mode = VectorPrimitiveType::none;
    updateCursor();
}

void Application::polygonChosen()
{
    renderer.draw_mode = VectorPrimitiveType::polygon;
    updateCursor();
}

void Application::pointChosen()
{
    renderer.draw_mode = VectorPrimitiveType::point;
    updateCursor();
}

void Application::lineChosen()
{
    renderer.draw_mode = VectorPrimitiveType::line;
    updateCursor();
}

void Application::rectangleChosen()
{
    renderer.draw_mode = VectorPrimitiveType::rectangle;
    updateCursor();
}

void Application::ellipseChosen()
{
    renderer.draw_mode = VectorPrimitiveType::ellipse;
    updateCursor();
}

// 3D
void Application::noneChosen3D()
{
    renderer.draw_mode3D = GeometricPrimitiveType::none;
    lighting.light_draw_mode = LightType::none;
}

void Application::cubeChosen()
{
    renderer.draw_mode3D = GeometricPrimitiveType::cube;
    lighting.light_draw_mode = LightType::none;
}

void Application::pyramidChosen()
{
    renderer.draw_mode3D = GeometricPrimitiveType::pyramid;
    lighting.light_draw_mode = LightType::none;
}

void Application::sphereChosen()
{
    renderer.draw_mode3D = GeometricPrimitiveType::sphere;
    lighting.light_draw_mode = LightType::none;
}

void Application::imported_model_1_chosen()
{
    renderer.draw_mode3D = GeometricPrimitiveType::imported;
    renderer.selected_imported_model = 1;
    lighting.light_draw_mode = LightType::none;
}

void Application::imported_model_2_chosen()
{
    renderer.draw_mode3D = GeometricPrimitiveType::imported;
    renderer.selected_imported_model = 2;
    lighting.light_draw_mode = LightType::none;
}

void Application::imported_model_3_chosen()
{
    renderer.draw_mode3D = GeometricPrimitiveType::imported;
    renderer.selected_imported_model = 3;
    lighting.light_draw_mode = LightType::none;
}

void Application::mouseMoved(int x, int y)
{
    renderer.mouse_current_x = x;
    renderer.mouse_current_y = y;
}

void Application::mouseDragged(int x, int y, int button)
{
    renderer.mouse_current_x = x;
    renderer.mouse_current_y = y;
    renderer.updateDrag(mouseX, mouseY);
}
void Application::dragEvent(ofDragInfo dragInfo)
{
    if (!dragInfo.files.empty()) {
        for (auto& file : dragInfo.files)
        {
            string ext = ofToLower(ofFilePath::getFileExt(file));
            if (ext == "jpg" || ext == "png" || ext == "jpeg" || ext == "bmp") {
                float imgX = dragInfo.position.x;
                float imgY = dragInfo.position.y;
                renderer.loadImage(file, imgX, imgY);
            }
            else if (ext == "obj" || ext == "3ds" || ext == "ply" || ext == "stl" || ext == "glb") {
                ofxAssimpModelLoader model;
                if (model.loadModel(file)) {
                    ofMesh combinedMesh;
                    int numMeshes = model.getNumMeshes();
                    for (int i = 0; i < numMeshes; ++i) {
                        ofMesh mesh = model.getMesh(i);

                        combinedMesh.append(mesh);
                        combinedMesh.addNormals(mesh.getNormals());
                        combinedMesh.addTexCoords(mesh.getTexCoords());
                        combinedMesh.addColors(mesh.getColors());
                    }
                    string file_name = ofFilePath::removeExt(ofFilePath::getFileName(file));
                    switch (renderer.buffer_head_imported_model3D) {
                        case 0: 
                            buttonImport1.setName(file_name);
                            break;
                        case 1:
                            buttonImport2.setName(file_name);
                            break;
                        case 2:
                            buttonImport3.setName(file_name);
                            break;
                    }
                    renderer.add_model3D_to_list(combinedMesh, file_name);
                }
            }
        }
    }
}



void Application::mousePressed(int x, int y, int button)
{
    renderer.is_mouse_button_pressed = true;

    renderer.mouse_current_x = x;
    renderer.mouse_current_y = y;

    renderer.mouse_press_x = x;
    renderer.mouse_press_y = y;
    if (!renderer.isMode2D && button == 2)
    {
        renderer.cam.disableMouseInput();
        if (renderer.draw_mode3D != GeometricPrimitiveType::none)
            renderer.add_geometric_shape(renderer.draw_mode3D);
        else if (lighting.light_draw_mode != LightType::none)
            lighting.add_light(lighting.light_draw_mode);
        setupListObjets();
    }
    renderer.startDrag(mouseX, mouseY);

    ofLog() << "<app::mouse pressed  at: (" << x << ", " << y << ")>";
}

void Application::mouseEntered(int x, int y)
{
    renderer.mouse_current_x = x;
    renderer.mouse_current_y = y;

    ofLog() << "<app::mouse entered  at: (" << x << ", " << y << ")>";
}

void Application::mouseExited(int x, int y)
{
    renderer.mouse_current_x = x;
    renderer.mouse_current_y = y;

    ofLog() << "<app::mouse exited   at: (" << x << ", " << y << ")>";
}
void Application::captureScreenshot()
{
    std::string filename = "screenshot_" + ofGetTimestampString("%Y-%m-%d_%H-%M-%S") + ".png";
    renderer.saveScreenshot(filename);

    showScreenshotMessage = true;
    screenshotMessageTimer = ofGetElapsedTimef();
}
void Application::drawScreenshotMessage()
{
    if (showScreenshotMessage)
    {
        float elapsedTime = ofGetElapsedTimef() - screenshotMessageTimer;

        if (elapsedTime < 2.0f)
        {
            ofSetColor(255, 255, 255);
            ofDrawRectangle(ofGetWidth() / 2 - 100, ofGetHeight() - 50, 200, 40);

            ofSetColor(0, 0, 0);
            ofDrawBitmapString("Screenshot done !", ofGetWidth() / 2 - 90, ofGetHeight() - 25);
        }
        else
        {
            showScreenshotMessage = false;
        }
    }

}
void Application::updateCursor()
{
    std::string cursorPath = "";

    if (renderer.draw_mode == VectorPrimitiveType::polygon)
        cursorPath = "cursor_pixel.png";
    else if (renderer.draw_mode == VectorPrimitiveType::point)
        cursorPath = "cursor_point.png";
    else if (renderer.draw_mode == VectorPrimitiveType::line)
        cursorPath = "cursor_line.png";
    else if (renderer.draw_mode == VectorPrimitiveType::rectangle)
        cursorPath = "cursor_rectangle.png";
    else if (renderer.draw_mode == VectorPrimitiveType::ellipse)
        cursorPath = "cursor_ellipse.png";
    else
    {
        ofShowCursor();
        customCursorActive = false;
        return;
    }

    if (cursorImage.load(cursorPath))
    {
        ofLog() << "Curseur chargé: " << cursorPath;
        customCursorActive = true;
        ofHideCursor();
    }
    else
    {
        ofLogError() << "Échec du chargement du curseur: " << cursorPath;
        customCursorActive = false;
        ofShowCursor();
    }
}
void Application::openHistogramWindow(ofImage& image) {
    if (!histogramApp) {
        ofGLFWWindowSettings settings;
        settings.setSize(400, 300);
        settings.resizable = false;
        histogramWindow = ofCreateWindow(settings);
        histogramApp = make_shared<HistogramWindow>();
        ofRunApp(histogramWindow, histogramApp);
    }

    if (histogramApp) {
        histogramApp->computeHistogram(image);
    }
}

void Application::openPortailWindow() {
    if (!portailWindow) {
        ofGLFWWindowSettings settings;
        settings.setGLVersion(3, 3);
        settings.setSize(800, 600);
        settings.numSamples = 4;
        settings.doubleBuffering = true;
        settings.resizable = false;
        portailWindow = ofCreateWindow(settings);
        portailApp = make_shared<PortailWindow>();
        ofRunApp(portailWindow, portailApp);
        setupPBRGui();
    }  
}
void Application::applyColorTransformToSelected()
{
    int idx = renderer.selectedImageIndex;
    if (idx < 0 || idx >= renderer.images.size()) return;

    auto& imgData = renderer.images[idx];
    ofPixels& originalPixels = imgData.original.getPixels();
    ofPixels& displayPixels = imgData.image.getPixels();

    displayPixels = originalPixels;

    int w = displayPixels.getWidth();
    int h = displayPixels.getHeight();

    bool useHSBvalue = useHSB.get();

    // Sliders RGB
    float rVal = sliderR.get();
    float gVal = sliderG.get();
    float bVal = sliderB.get();

    float hVal = sliderH.get();
    float sVal = sliderS.get();
    float vVal = sliderV.get();


    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            ofColor col = displayPixels.getColor(x, y);

            if (useHSBvalue)
            {
                float oldH = col.getHue();
                float oldS = col.getSaturation();
                float oldB = col.getBrightness();

                float newH = oldH + hVal;
                float newS = oldS + sVal;
                float newB = oldB + vVal;

                newH = ofClamp(newH, 0, 255);
                newS = ofClamp(newS, 0, 255);
                newB = ofClamp(newB, 0, 255);

                col.setHue(newH);
                col.setSaturation(newS);
                col.setBrightness(newB);
            }

            col.r *= (rVal / 255.0f);
            col.g *= (gVal / 255.0f);
            col.b *= (bVal / 255.0f);

            displayPixels.setColor(x, y, col);
        }
    }
    imgData.image.update();
    if (histogramApp) {
        histogramApp->computeHistogram(imgData.image);
    }
}

void Application::onColorParamsChanged(float& val)
{
    applyColorTransformToSelected();
}
void Application::onColorParamsChanged(bool& val)
{
    applyColorTransformToSelected();
}
// Pour les sliders float (R, G, B, H, S, V)
void Application::onColorParamsChangedBool(bool& val)
{
    applyColorTransformToSelected();
}
void Application::onColorParamsChangedFloat(float& val)
{
    applyColorTransformToSelected();
}
void Application::exportSelectedImage()
{
    int idx = renderer.selectedImageIndex;
    if (idx < 0 || idx >= renderer.images.size()) {
        ofLogWarning() << "Aucune image n'est sélectionnée pour l'export.";
        return;
    }

    auto& imgData = renderer.images[idx];

    std::string filename = "exported_image_" + std::to_string(exportCount) + ".png";
    std::string filepath = ofToDataPath(filename, true);

    bool success = imgData.image.save(filepath);
    if (success) {
        ofLog() << "Image exportée dans : " << filepath;
        exportCount++;
    }
    else {
        ofLogError() << "Échec de la sauvegarde à : " << filepath;
    }
}

int Application::update_selected_3D() {
    int count = 0;
    for (int i = 0; i < toggles.size(); i++) {
        if (toggles[i].get()->getParameter().toString() == "1") {
            toggle_3D_object(true, i);
            count++;
        }
        else
            toggle_3D_object(false, i);
    }
    return count;
}

void Application::toggle_3D_object(bool toggleValue, int indexToToggle) {
    if (indexToToggle < renderer.shapes3D.size() && renderer.shapes3D[indexToToggle].selected != toggleValue){
        if (toggleValue) {
            renderer.shapes3D[indexToToggle].material = renderer.selectedMaterial;
        }
        else {
            switch (renderer.shapes3D[indexToToggle].chosenMaterial) {
            case 1:
                renderer.shapes3D[indexToToggle].material = portailApp.get()->matSphere;
                break;
            case 2:
                renderer.shapes3D[indexToToggle].material = renderer.material2;
                break;
            case 3:
                renderer.shapes3D[indexToToggle].material = renderer.material3;
                break;
            case 0:
            default:
                renderer.shapes3D[indexToToggle].material = renderer.defaultMaterial;
                break;
            }
        }
        renderer.shapes3D[indexToToggle].selected = toggleValue;
    }
}

void Application::toggle_wireframe() {
    isWireFrameActive = !isWireFrameActive;
    for (int i = 0; i < renderer.buffer_count3D; i++) {
        if(renderer.shapes3D[i].selected)
            renderer.shapes3D[i].show_wireframe = !renderer.shapes3D[i].show_wireframe;
    }
}
void Application::applyFilterGray()
{
    int idx = renderer.selectedImageIndex;

    ofImage& img = renderer.images[idx].image;
    ofPixels& pix = img.getPixels();

    for (int i = 0; i < pix.size(); i += 3) {
        unsigned char r = pix[i];//juste char donne un rendu étrange style drawing
        unsigned char g = pix[i + 1];
        unsigned char b = pix[i + 2];

        unsigned char gray = (r + g + b) / 3;

        pix[i] = gray;
        pix[i + 1] = gray;
        pix[i + 2] = gray;
    }
    img.update();

    if (histogramApp) {
        histogramApp->computeHistogram(img);
    }
}
void Application::applyFilterBlur()
{
    int idx = renderer.selectedImageIndex;

    ofImage& img = renderer.images[idx].image;
    ofPixels original = img.getPixels();
    ofPixels& pix = img.getPixels();

    int w = pix.getWidth();
    int h = pix.getHeight();

    int kernelSize = 3;
    int halfK = kernelSize / 2;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int sumR = 0, sumG = 0, sumB = 0, count = 0;

            for (int ky = -halfK; ky <= halfK; ky++) {
                for (int kx = -halfK; kx <= halfK; kx++) {
                    int nx = x + kx;
                    int ny = y + ky;
                    if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
                        ofColor c = original.getColor(nx, ny);
                        sumR += c.r;
                        sumG += c.g;
                        sumB += c.b;
                        count++;
                    }
                }
            }
            sumR /= count;
            sumG /= count;
            sumB /= count;

            pix.setColor(x, y, ofColor(sumR, sumG, sumB));
        }
    }
    img.update();

    if (histogramApp) {
        histogramApp->computeHistogram(img);
    }
}

void Application::applyFilterSharpen()
{
    int idx = renderer.selectedImageIndex;

    int kernel[9] = {
         0, -1,  0,
        -1,  5, -1,
         0, -1,  0
    };
    int kernelSize = 3;
    int halfK = kernelSize / 2;

    ofImage& img = renderer.images[idx].image;
    ofPixels original = img.getPixels();
    ofPixels& pix = img.getPixels();
    int w = pix.getWidth();
    int h = pix.getHeight();

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            int sumR = 0, sumG = 0, sumB = 0;
            int indexK = 0;

            for (int ky = -halfK; ky <= halfK; ky++) {
                for (int kx = -halfK; kx <= halfK; kx++) {
                    int nx = x + kx;
                    int ny = y + ky;
                    if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
                        ofColor c = original.getColor(nx, ny);
                        int coeff = kernel[indexK];

                        sumR += c.r * coeff;
                        sumG += c.g * coeff;
                        sumB += c.b * coeff;
                    }
                    indexK++;
                }
            }

            sumR = ofClamp(sumR, 0, 255);
            sumG = ofClamp(sumG, 0, 255);
            sumB = ofClamp(sumB, 0, 255);

            pix.setColor(x, y, ofColor(sumR, sumG, sumB));
        }
    }

    img.update();
    if (histogramApp) {
        histogramApp->computeHistogram(img);
    }
}

void Application::onExposurePlus()
{
    toneMapExposure += 0.2f;
    if (toneMapExposure > 10.0f) toneMapExposure = 10.0f;
    applyToneMapWithCurrentExposure();
}

void Application::onExposureMinus()
{
    toneMapExposure -= 0.2f;
    if (toneMapExposure < 0.0f) toneMapExposure = 0.0f;
    applyToneMapWithCurrentExposure();
}

void Application::applyToneMapWithCurrentExposure()
{
    int idx = renderer.selectedImageIndex;

    auto& imgData = renderer.images[idx];
    imgData.image.setFromPixels(imgData.original.getPixels());

    ofPixels& pix = imgData.image.getPixels();
    for (int i = 0; i < (int)pix.size(); i += 3)
    {
        float r = pix[i];
        float g = pix[i + 1];
        float b = pix[i + 2];

        r *= toneMapExposure;
        g *= toneMapExposure;
        b *= toneMapExposure;

        r = ofClamp(r, 0, 255);
        g = ofClamp(g, 0, 255);
        b = ofClamp(b, 0, 255);

        pix[i] = (unsigned char)r;
        pix[i + 1] = (unsigned char)g;
        pix[i + 2] = (unsigned char)b;
    }
    imgData.image.update();

    if (histogramApp) {
        histogramApp->computeHistogram(imgData.image);
    }
}

void Application::resetSelectedImageToOriginal()
{
    int idx = renderer.selectedImageIndex;
    auto& imgData = renderer.images[idx];
    imgData.image.setFromPixels(imgData.original.getPixels());
    imgData.image.update();

    if (histogramApp) {
        histogramApp->computeHistogram(imgData.image);
    }
}
void Application::applyFilterToneMap()
{
    int idx = renderer.selectedImageIndex;

    auto& imgData = renderer.images[idx];
    imgData.image.setFromPixels(imgData.original.getPixels());

    float gammaVal = 2.2f;
    float exposure = toneMapExposure;

    ofPixels& pix = imgData.image.getPixels();
    for (int i = 0; i < (int)pix.size(); i += 3)
    {
        float r = pix[i];
        float g = pix[i + 1];
        float b = pix[i + 2];
        r = powf((r * exposure) / 255.0f, 1.0f / gammaVal) * 255.0f;
        g = powf((g * exposure) / 255.0f, 1.0f / gammaVal) * 255.0f;
        b = powf((b * exposure) / 255.0f, 1.0f / gammaVal) * 255.0f;

        r = ofClamp(r, 0, 255);
        g = ofClamp(g, 0, 255);
        b = ofClamp(b, 0, 255);

        pix[i] = (unsigned char)r;
        pix[i + 1] = (unsigned char)g;
        pix[i + 2] = (unsigned char)b;
    }
    imgData.image.update();
    if (histogramApp) {
        histogramApp->computeHistogram(imgData.image);
    }
}