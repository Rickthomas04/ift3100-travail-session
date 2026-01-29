// IFT3100H24_Move/main.cpp
// Dans cet exemple, une transformation de type translation est appliquée sur l'origine de la scène en fonction du temps
// pour permettre un mouvement dans l'espace sur le plan XZ à partir de l'état courant des flèches du clavier.

#include "ofMain.h"
#include "application.h"

int main()
{
	ofGLFWWindowSettings window_settings;
	window_settings.setSize(1920, 1080);
	window_settings.setGLVersion(3, 3);
	window_settings.numSamples = 4;
	window_settings.doubleBuffering = true;
	ofCreateWindow(window_settings);
	ofRunApp(new Application());
}
