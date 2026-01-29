// IFT3100H24_Move/Application.h
// Classe principale de l'application.

#pragma once

#include "ofMain.h"
#include "renderer.h"
#include "lighting.h"
#include "ofxGui.h"
#include "HistogramWindow.h"
#include "PortailWindow.h"
#include <algorithm>

class Application : public ofBaseApp
{
public:
    Application();
    Renderer renderer;
    Lighting lighting;

    float time_current;
    float time_last;
    float time_elapsed;

    int lastShapesIndex;
    int lastSelectedDrawing2D;
    int currentSelectedDrawing2D;
    int exportCount = 0;
    int lastSelectedCount;

    bool is_key_press_up;
    bool is_key_press_down;
    bool is_key_press_left;
    bool is_key_press_right;
    bool is2DModeActive;
    bool isImageColorGroupAdded = false;
    bool isWireFrameActive;

    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);

    void mouseReleased(int x, int y, int button);

    void exit();

    ofxPanel gui;
    ofxPanel gui2;
    ofxGuiGroup group_Mode2D3D;
    ofxGuiGroup group_draw;
    ofxGuiGroup group_tools;
    ofxGuiGroup group_tools3D;
    ofxGuiGroup group_parameters;
    ofxGuiGroup group_listObjets;
    ofxGuiGroup group_transform2D;
    ofxGuiGroup group_delete;
    ofxGuiGroup group_delete3D;
    ofxGuiGroup group_history;
    ofxGuiGroup group_bezier;
    ofxGuiGroup group_pbr_sphere1;
    ofxGuiGroup group_pbr_sphere2;
    ofxGuiGroup group_pbr_floor;

    ofxGuiGroup group_imageColor;

    ofxGuiGroup group_light_options;
    ofxGuiGroup group_light_ambient;
    ofxGuiGroup group_apply_material;

    ofParameter<bool> useHSB;
    ofParameter<float> sliderR;
    ofParameter<float> sliderG;
    ofParameter<float> sliderB;

    ofParameter<float> sliderH;
    ofParameter<float> sliderS;
    ofParameter<float> sliderV;
    
    ofParameter<float> bezierHeight;
    ofParameter<int> controlPoint;
    ofParameter<bool> activateBezier;
    ofParameter<int> tessellation;
    ofParameter<bool> blackHole;
    ofParameter<int> nbrPointsCtrl;

    ofxFloatSlider metallicSlider;
    ofxFloatSlider roughnessSlider;
    ofxFloatSlider texCoordScale1Slider;
    ofxFloatSlider texCoordScale2Slider;
    ofxFloatSlider clearCoatStrengthSlider;
    ofxFloatSlider clearCoatRoughnessSlider;
    ofxFloatSlider texCoordScale1SliderFloor;
    ofxFloatSlider texCoordScale2SliderFloor;
    ofxFloatSlider clearCoatStrengthSliderFloor;
    ofxFloatSlider clearCoatRoughnessSliderFloor;

    void onColorParamsChanged(float& val);
    void onColorParamsChanged(bool& val);
    void onColorParamsChangedFloat(float& val);
    void onColorParamsChangedBool(bool& val);

    void applyColorTransformToSelected();

    ofParameter<ofColor> color_picker_background;
    ofParameter<ofColor> color_picker_stroke;
    ofParameter<ofColor> color_picker_fill;

    ofxButton buttonMode2D3D;
    ofxButton buttonNone;
    ofxButton buttonPixel;
    ofxButton buttonPoint;
    ofxButton buttonLine;
    ofxButton buttonRectangle;
    ofxButton buttonEllipse;
    ofxButton buttonNone3D;
    ofxButton buttonCube;
    ofxButton buttonPyramid;
    ofxButton buttonSphere;
    ofxButton buttonImport1;
    ofxButton buttonImport2;
    ofxButton buttonImport3;
    ofxButton buttonWireframe;
    ofxButton buttonProjectionMode;
    ofxButton buttonTransform;
    ofxButton buttonDelete;
    ofxButton buttonDeleteAll;
    ofxButton buttonExport;
    ofxButton buttonUndo;
    ofxButton buttonRedo;
    ofxButton buttonDirectionalLight;
    ofxButton buttonPointLight;
    ofxButton buttonSpotLight;
    ofxButton buttonApplyMat0;
    ofxButton buttonApplyMat1;
    ofxButton buttonApplyMat2;
    ofxButton buttonApplyMat3;
    ofParameter<ofColor> color_picker_ambient;
    ofxIntField fieldPosX;
    ofxIntField fieldPosY;
    ofxIntField fieldPos2X;
    ofxIntField fieldPos2Y;

    //Delete dessins
    std::vector<std::unique_ptr<ofxToggle>> toggles;
    std::vector<std::unique_ptr<ofxLabel>> labels;

    //Selection dessins
    std::vector<std::unique_ptr<ofColor>> colors;
    float nextTimeCheck;
    float intervalCheckTime = 0.5f;

    ofParameter<float> slider_stroke_weight;

    ofParameter<string> textbox;

    ofParameter<bool> checkboxFill;
    ofParameter<bool> checkboxStroke;

    void mode2D3D();
    void noneChosen();
    void polygonChosen();
    void pointChosen();
    void lineChosen();
    void rectangleChosen();
    void ellipseChosen();
    void noneChosen3D();
    void cubeChosen();
    void pyramidChosen();
    void sphereChosen();
    void imported_model_1_chosen();
    void imported_model_2_chosen();
    void imported_model_3_chosen();
    void ProjectionModeChange();
    void transformDrawing2D();
    void deleteDrawing();
    void deleteAll3D();
    void setColorSelected();
    void undo();
    void redo();
    void updateBezier();

    void setupMode2D3DGui();
    void setupToolsGui();
    void setupToolsGui3D();
    void setupColorGui();
    void setup2DPrimitiveParameters();
    void setupListObjets();
    void setupTransform2D();
    void setupDelete();
    void setupHistory();
    void setupBezierGui();
    void setupPBRGui();
    void setupLightGui();
    void setupAmbientColorGui();
    void setupApplyMaterialGui();
    int update_selected_3D();
    void toggle_3D_object(bool toggleValue, int indexToToggle);
    void toggle_wireframe();

    void directionalLightChosen();
    void pointLightChosen();
    void spotLightChosen();
    void updateAmbientLight();

    void material0Chosen();
    void material1Chosen();
    void material2Chosen();
    void material3Chosen();
    void applyMaterialToSelected(int chosenMat);

    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);

    //import et export image
    void dragEvent(ofDragInfo dragInfo);
    void captureScreenshot();
    bool showScreenshotMessage = false;
    float screenshotMessageTimer = 0.0f;
    void drawScreenshotMessage();

    //curseur
    ofImage cursorImage;
    bool customCursorActive = false;
    void updateCursor();
    //histogramme
    shared_ptr<ofAppBaseWindow> histogramWindow;
    shared_ptr<HistogramWindow> histogramApp;

    void openHistogramWindow(ofImage& image);
    void exportSelectedImage();

    //Portail
    shared_ptr<ofAppBaseWindow> portailWindow;
    shared_ptr<PortailWindow> portailApp;

    void openPortailWindow();

    ofxGuiGroup group_filters;
    ofxButton buttonFilterGray;
    ofxButton buttonFilterBlur;
    ofxButton buttonFilterToneMap;
    ofxButton buttonFilterSharpen;

    ofxGuiGroup group_tonemap;
    ofxButton buttonExposurePlus;
    ofxButton buttonExposureMinus;
    ofxButton buttonResetImage;
    float toneMapExposure = 2.0f;

    void onExposurePlus();
    void onExposureMinus();
    void applyToneMapWithCurrentExposure();
    void resetSelectedImageToOriginal();
    void applyFilterGray();
    void applyFilterBlur();
    void applyFilterToneMap();
    void applyFilterSharpen();
};
