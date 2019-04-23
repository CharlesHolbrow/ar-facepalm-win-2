#pragma once

#include <string>
#include <map>
#include <list>
#include "gesture.h"
#include "trail.h"
#include "stepper.h"
#include "ofMain.h"
#include "Circles.h"

using namespace std;

string randomString(int size);

class Content {
private:
    map<int, vector<string>> keymap;
    Circles circles;

public:
    Content();
    Gesture* mainGesture;
    map<string, Gesture> gestures;
    list<Trail> trails;
    // set the colors to control how things look
    ofColor baseColor;
    double externalTime;
    float saturation; //  set me! 0-1
    float brightness = 1.;

    void update(Stepper stepper, MouseEvent mouse);
    void render();
    void replayMainGesture();
    void terminateMainGesture();
    void mapMainGestureToKey(int key);
    void playSavedGesture(int key);
};
