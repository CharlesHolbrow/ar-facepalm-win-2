#pragma once

#include "ofMain.h"
//#include "ofxVideoRecorder.h"
#include "stepper.h"
#include "gesture.h"
#include "gesturePlayhead.h"
#include "trail.h"
#include "customJson.h"
#include "content.h"
#include "receiver.h"
#include "msgDelay.h"

enum STATE {
	RECORDING,
	PLAYING,
};

#include "ofMain.h"

#define NUM_BILLBOARDS 5000

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

    // stuff for billboards
    void setupBillboards();
    ofVboMesh billboards;
    int billboardIndex = 0;
    float billboardSizeTarget[NUM_BILLBOARDS];
    ofShader billboardShader;
    ofImage texture;
    Trail billboardTrail;

	// Caching the previous frame make it easy to get the delta
	uint64_t previousMicroseconds = 0;
	MouseEvent previousMouse;
	ofEasyCam cam;

	// Handle recording
	STATE state = PLAYING;

	Content content;
	Stepper stepper;
	Receiver receiver;
	MsgDelay<Orientation7> camDelay;

	// These are for debugging camera issues
	vector<ofNode> nodes;

	// This is some hacky junk for rotating the camera
	double previousPressTime;// last time the camera was pressed
	ofNode previousControllerAtPressTime;
};
