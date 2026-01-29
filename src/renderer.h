// IFT3100H24_Move/renderer.h
// Classe responsable du rendu de l'application.

#pragma once
#include <vector> 
#include <ofxAssimpModelLoader.h>

#include "ofMain.h"
#include <stack>

enum class VectorPrimitiveType { none, polygon, point, line, rectangle, ellipse };

enum class ActionType { add, remove, transform };

struct VectorPrimitive
{
	VectorPrimitiveType type;            // 1 * 4 = 4  octets
	float               position1[2];    // 2 * 4 = 8  octets
	float               position2[2];    // 2 * 4 = 8  octets
	float               stroke_width;    // 1 * 4 = 4  octets
	unsigned char       stroke_color[4]; // 4 * 1 = 4  octets
	unsigned char       fill_color[4];   // 4 * 1 = 4  octets
	bool				  filled = NULL;
	bool				  stroke = NULL;
};

enum class GeometricPrimitiveType { none, cube, pyramid, sphere, imported };

struct GeometricPrimitive
{
	GeometricPrimitiveType type;
	string name;
	bool selected;
	float               position[3];
	float               size;
	unsigned char       color[4];
	bool				show_wireframe;
	ofMesh mesh;
	int					ordre;
	int					chosenMaterial; //0 (default), 1, 2, 3
	ofMaterial			material;
};

struct ImportedModel3D
{
	bool isEmpty;
	string name;
	ofMesh mesh;
};

class Renderer
{
public:
	ofMaterial defaultMaterial;
	ofMaterial selectedMaterial;
	ofMaterial material2;
	ofMaterial material3;

	ofSpherePrimitive node;
	ofPlanePrimitive node2;
	ofBoxPrimitive node3;

	float center_x;
	float center_y;

	float offset_x;
	float offset_z;

	float delta_x;
	float delta_z;

	float speed;

	void setup();
	void update();
	void draw();

	void reset();

	void draw_locator(float scale);

	VectorPrimitiveType draw_mode;
	VectorPrimitive* shapes;

	GeometricPrimitiveType draw_mode3D;
	vector<GeometricPrimitive> shapes3D;
	vector<ImportedModel3D> imported_models;
	int selected_imported_model;

	int index;

	int buffer_count;
	int buffer_stride;
	int buffer_size;
	int buffer_head;
	int currentShapeIndex;
	int selectedImageIndex = -1; // -1 si auxcune s lectionn 

	int buffer_count3D;
	int buffer_stride3D;
	int buffer_size3D;
	int buffer_head3D;
	int buffer_head_imported_model3D;

	int mouse_press_x;
	int mouse_press_y;

	int mouse_current_x;
	int mouse_current_y;

	float stroke_width_default;
	float radius;

	unsigned char stroke_color_r;
	unsigned char stroke_color_g;
	unsigned char stroke_color_b;
	unsigned char stroke_color_a;

	unsigned char fill_color_r;
	unsigned char fill_color_g;
	unsigned char fill_color_b;
	unsigned char fill_color_a;

	bool is_mouse_button_pressed;
	bool fillEnabled;
	bool strokeEnabled;
	bool isMode2D = true;
	bool currentModeis2D;

	ofTrueTypeFont font;

	ofColor background_color;
	ofColor fill_color;
	ofColor stroke_color;

	ofRectangle bounding_box;

	string text;
	float stroke_weight;

	float line_offset;

	int font_size;
	bool currentFont;
	void add_vector_shape(VectorPrimitiveType type);
	void add_geometric_shape(GeometricPrimitiveType type, float x = 0.0f, float y = 0.0f, float z = 0.0f, bool redo = false);//false pour ajout avec clic, true pour ajout par undo/redo
	void reset_geometric_primitive(int index, bool redo = false);//false pour supprimer par UI, true pour supprimer par undo/redo
	void add_model3D_to_list(ofMesh mesh, string file_name);
	void draw_pixel(float x, float y) const;
	void draw_point(float x, float y, float radius) const;
	void draw_line(float x1, float y1, float x2, float y2) const;
	void draw_rectangle(float x1, float y1, float x2, float y2) const;
	void draw_ellipse(float x1, float y1, float x2, float y2) const;

	void draw_zone(float x1, float y1, float x2, float y2) const;

	ofMesh build_geometric_mesh(GeometricPrimitive primitive3D);
	static glm::vec3 screen_to_world_pos(float x, float y, float depth, ofEasyCam& cam);

public:
	ofImage image;
	bool hasImage = false;
	struct ImageData
	{
		ofImage image;
		ofImage original;//copie
		float imgX, imgY;
		bool isDragging;
		float dragOffsetX, dragOffsetY;
	};

	std::vector<ImageData> images;

	float imgX, imgY;
	bool isDragging = false;
	float dragOffsetX, dragOffsetY;

	void loadImage(const std::string& path, float imgX, float imgY);
	void startDrag(float mouseX, float mouseY);
	void updateDrag(float mouseX, float mouseY);
	void stopDrag();
	void saveScreenshot(const std::string& filename);


	void drawInteractionArea();
	ofEasyCam cam;

	void undo2D();
	void redo2D();
	void undo3D();
	void redo3D();
	void emptyStacks();

	std::stack<std::pair<int, std::pair<ActionType, GeometricPrimitive>>> stackUndo3D;
	std::stack<std::pair<int, std::pair<ActionType, GeometricPrimitive>>> stackRedo3D;

	void updateBezier();
	int controlPointChosen = 0;
	float bezierHeight;
	float computePoint(int i, float t);
	ofVec3f computeBezierSurface(float u, float v);
	ofVec2f computeBezierCurve(vector<ofVec2f> points, float t);
	ofVec3f controlPoints3D[4][4];  //pour 3D
	ofVec3f controlPoints3DOrigine[4][4];  //pour 3D
	vector<ofVec2f> controlPoints2DOrigine; //pour 2D
	vector<ofVec2f> controlPoints2D; //pour 2D
	int resolution = 15;
	void setupBezier();
	bool showBezier = true;
	bool blackHole = false;
	int tessellation = 0;
	int nbrPointsctrl = 4;

	~Renderer();
};
