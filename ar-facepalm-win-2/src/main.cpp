#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLWindowSettings settings;
	settings.setPosition(ofVec2f(100, 100));
	settings.windowMode = OF_WINDOW;
	settings.setSize(1020, 768);
    settings.setGLVersion(3, 2);
	ofCreateWindow(settings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
