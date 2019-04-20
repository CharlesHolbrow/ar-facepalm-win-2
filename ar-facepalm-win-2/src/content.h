#pragma once

#include <string>
#include <map>
#include <list>
#include "gesture.h"
#include "trail.h"
#include "stepper.h"
#include "ofMain.h"

using namespace std;

string randomString(int size);

class Content {
private:
    map<int, vector<string>> keymap;

public:
    Gesture* mainGesture;
    map<string, Gesture> gestures;
    list<Trail> trails;

    void update(Stepper stepper, MouseEvent mouse);
    void render();
    void replayMainGesture();
    void terminateMainGesture();
    void mapMainGestureToKey(int key);
    void playSavedGesture(int key);
};
