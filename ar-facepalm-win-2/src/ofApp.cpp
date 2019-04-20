#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofEnableDepthTest();
	ofSetFrameRate(90);
	stepper.setStepSize(1. / 1000.);
	ofLog() << "Ticks per frame @ 60fps: " << 1. / 60. / stepper.getStepSize();

	// Threaded OSC Receive
	receiver.startThread();

	// Setup the 3d scene camera
	cam.disableMouseInput();
	cam.setPosition(110, 110, 665);
	cam.lookAt(ofVec3f(0), ofVec3f(0, 1, 0));
	cam.setFov(40.); // this is overwritten by the osc receiver
    cam.setNearClip(0.05);
    cam.setFarClip(30);
    ofLog() << "cam far clip  " << cam.getFarClip();
    ofLog() << "cam near clip " << cam.getNearClip();
	//cam.setAspectRatio(float) // By default the screen size is used
    setupBillboards();
}

//--------------------------BILLBOARDS---------------------------
void ofApp::setupBillboards() {
    billboards.getVertices().resize(NUM_BILLBOARDS);
    billboards.getColors().resize(NUM_BILLBOARDS);
    billboards.getNormals().resize(NUM_BILLBOARDS, ofVec3f(0));

    for (int i = 0; i < NUM_BILLBOARDS; i++) {
        float v = i / 1000.;
        Particle p;
        p.pos = { v - 1, 0, 0 };
        p.size = .2;
        p.ofc = ofColor::fromHsb(int(i / 10.) % 256, 255, 255);
        billboardTrail.parts.push_back(p);

    }
    billboards.setUsage(GL_DYNAMIC_DRAW);
    billboards.setMode(OF_PRIMITIVE_POINTS);
    //billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
    //billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);

    // load the billboard shader
    // this is used to change the
    // size of the particle
    if (ofIsGLProgrammableRenderer()) {
        billboardShader.load("shadersGL3/Billboard");
    }
    else {
        billboardShader.load("shadersGL2/Billboard");
    }
    // we need to disable ARB textures in order to use normalized texcoords
    ofDisableArbTex();
    texture.load("dot.big.png");
    ofEnableAlphaBlending();
}


//--------------------------------------------------------------
void ofApp::update() {
	uint64_t microseconds = ofGetElapsedTimeMicros();
	uint64_t deltaMicroseconds = microseconds - previousMicroseconds;

	// What time did the frame start?
	double frameStart = static_cast<double>(previousMicroseconds * 0.000001);
	// time in seconds
	double frameEnd = static_cast<double>(microseconds * 0.000001);
	// delta time in seconds
	double frameDelta = static_cast<double>(deltaMicroseconds * 0.000001);

	// Setup the Stepper for this frame
	stepper.advanceFrame(frameDelta);

	// Get the position of the Tracker
	Orientation7 cor = receiver.getCamera();
	Orientation7 camTriggerPosition = receiver.getPreviousCameraTrigger();
	Orientation7 controller = receiver.getController();

	// Process all input
	bool down = ofGetMousePressed(); // draw with mouse click
	down = controller.trigger > .9; // draw with controller trigger
	MouseEvent mouse;
	mouse.press = down && !previousMouse.isDown;
	mouse.release = !down && previousMouse.isDown;
	mouse.isDown = down;
	mouse.pos = ofVec2f(ofGetMouseX(), ofGetMouseY());
	mouse.previousPos = previousMouse.pos;
	mouse.vel = (mouse.pos - previousMouse.pos) / stepper.stepsDuration();

	// calculate the world position of the mouse
	ofVec3f world = cam.screenToWorld(ofVec3f(mouse.pos.x, mouse.pos.y, 0));
	ofVec3f ray = (world - cam.getGlobalPosition()).normalize() * 900;
//	mouse.worldPos = cam.getGlobalPosition() + ray; // draw with mouse
    mouse.worldPos = controller.pos; // draw with controller
	mouse.previousWorldPos = previousMouse.worldPos;
	mouse.worldVel = (mouse.worldPos - mouse.previousWorldPos) / stepper.stepsDuration();

	////////////// Insert hacky stuff to rotate the camera
	if (cor.trigger < 0.9) { // if the cammera is not fully depressed...
		float timeSincePreviousPress = stepper.frameEnd - previousPressTime;
		cor = camTriggerPosition; // ...use the last time it was pressed
		ofNode tempController;
		ofNode tempCamera;
		tempController.setPosition(controller.pos);
		tempCamera.setPosition(camTriggerPosition.pos);
		tempCamera.lookAt(previousControllerAtPressTime, ofVec3f(0, 1, 0));
		ofVec3f v1 = previousControllerAtPressTime.getPosition();
		ofVec3f v2 = tempCamera.getPosition();
		ofVec3f v3 = ofVec3f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
		v3 = ofVec3f(v3.x*v3.x, v3.y*v3.y, v3.z*v3.z);
		float radius = sqrt(v3.x+v3.y+v3.z);
		if (radius > 0) {
			tempCamera.orbitDeg(timeSincePreviousPress * 20., 20, radius, previousControllerAtPressTime);
			tempCamera.lookAt(previousControllerAtPressTime, ofVec3f(0, 1, 0));
		}

		// this is required to actually update the camera
		cor.quat = tempCamera.getOrientationQuat();
		cor.pos = tempCamera.getGlobalPosition();
	} else {
		previousPressTime = stepper.frameEnd;
		previousControllerAtPressTime.setOrientation(controller.quat);
		previousControllerAtPressTime.setPosition(controller.pos);
	}
	///////////////////// End hacky stuff to rotate the camera

	cam.setOrientation(cor.quat);
	cam.setPosition(cor.pos);
	cam.setFov(receiver.getFov()); // Can also set this in the main view

	// tick our content
	content.update(stepper, mouse);

	previousMicroseconds = microseconds;
	previousMouse = mouse;

    // billboard stufff
    billboardTrail.sortForRender(cam.getGlobalPosition());
    int i = 0;
    for (auto p = billboardTrail.parts.begin(); p != billboardTrail.parts.end(); p++) {
        if (i >= NUM_BILLBOARDS) break;
        billboards.getVertices()[i] = p->pos;
        billboards.getColors()[i].set(p->ofc);
        billboardSizeTarget[i] = .1;
        billboards.setNormal(i, ofVec3f(p->size, 0, 0)); // first value is the size
        i++;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0);
	ofSetColor(255);

	// DEBUG: info about our camera
	//ofVec3f cPos = cam.getPosition();
	//char str[100];
	//sprintf_s(str, "Camera: % 4.2f % 4.2f % 4.2f\tp:% 4.0f y:% 4.0f r:% 4.0f",
	//	cPos.x, cPos.y, cPos.z,
	//	cam.getPitchDeg(),
	//	cam.getHeadingDeg(),
	//	cam.getRollDeg());
	//ofSetColor(255, 0, 0);
	//ofDrawBitmapString(str, 2, 12);

	cam.begin();
	content.render();
	// Debug Info
	ofDrawAxis(0.1);
	for (ofNode n : nodes) {
		n.draw();
	}

	// draw the dam controller
	ofNode n;
	Orientation7 controller = receiver.getController();
	n.setOrientation(controller.quat);
	n.setPosition(controller.pos);
	n.setScale((controller.trigger + 1) * 0.001);
	n.draw();

   
    // bind the shader so that wee can change the
    // size of the points via the vert shader
    billboardShader.begin();  // BEGIN BILLBOARDS!!!!!!!!!
    ofDisableDepthTest();
    ofEnablePointSprites();   // not needed for GL3/4
    texture.getTexture().bind();
    billboards.draw();
    texture.getTexture().unbind();
    ofDisablePointSprites();  // not needed for GL3/4
    ofEnableDepthTest();
    billboardShader.end();    // END BILLBOARDS!!!!!!!!!

	cam.end();

	if (state == RECORDING) {
		ofSetColor(155 + 100 * sin(ofGetElapsedTimef() * 4), 0, 0);
		ofDrawCircle(10, 10, 5);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofLog() << "Press: " << key;
	// Tab toggles recording state
	if (key == OF_KEY_TAB) {
		if (state == PLAYING) {
			state = RECORDING;
		}
		else {
			state = PLAYING;
			content.terminateMainGesture();
		}
		return;
	}
	ofNode n;
	switch (key) {
    case OF_KEY_ALT:
        break;
	case OF_KEY_RETURN:
		// Alt+Enter to toggle full screen
		// Enter to replay current gesture
		if (ofGetKeyPressed(OF_KEY_ALT)) ofToggleFullscreen();
		else content.replayMainGesture();
		break;
	case OF_KEY_BACKSPACE:
		content.terminateMainGesture();
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_RIGHT_SHIFT:
		break;
	case ' ':
		n.setPosition(cam.getGlobalPosition());
		n.setOrientation(cam.getGlobalOrientation());
		nodes.push_back(n);
		break;
	default:
		if (state == PLAYING) {
			content.playSavedGesture(key);
		}
		else if (state == RECORDING) {
			content.mapMainGestureToKey(key);
			content.terminateMainGesture();
		}
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	ofLog() << "Window Resized: " << w << "x" << h;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
