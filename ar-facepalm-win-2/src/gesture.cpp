//
//  gesture.mm
//  Hike
//
//  Created by Charles Holbrow on 11/10/18.
//

#include "gesture.h"

void Gesture::reset() {
    terminated = false; // "un-terminate" the gesture
    recordingTime = 0;
    filter.fill(ofVec3f(0, 0, 0));
    blipsVec.clear();
};

void Gesture::update(Stepper stepper, MouseEvent mouse) {
    if (!mouse.isDown) {
        recordingTime += stepper.stepsDuration();
        return;
    };
    if (mouse.press) {
        filter.fill(mouse.worldPos);
    }

    // Note that the step at "stepZeroTime" has already been processed.
    // For that reason, we start on stepIndex 1, and loop while "<="
    int stepIndex = 1; // start
    while (stepIndex <= stepper.steps) {
        // time since stepZero
        double sinceTime = stepIndex * stepper.stepSize;
        recordingTime += stepper.stepSize;

        ofVec3f input = mouse.previousWorldPos + mouse.worldVel * sinceTime;
        filter.push(input);

        Blip b;
        b.pos = filter.average();
        b.gestureTime = recordingTime;

        blipsVec.push_back(b);
        stepIndex++;
    }
};

size_t Gesture::size() {
    return blipsVec.size();
};

void Gesture::terminate() {
	terminated = true;
};

bool Gesture::isTerminated() {
	return terminated;
};
