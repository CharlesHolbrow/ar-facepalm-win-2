//
//  ticker.h
//  Hike
//
//  Created by Charles Holbrow on 11/11/18.
//

#ifndef stepper_h
#define stepper_h


// There are two types of time represented in a Stepper.
// - Frames (frame rate). We cannot control this precisely, but we can request
//   with ofSetFrameRate(fps). Frame time is passed to the advanceFrame function.
// - Steps are a smaller increment that we controll precisely. Every frame, we
//   step multiple times. The step size is specified exactly.
class Stepper {
public:
    double frameStart;
    double frameEnd;
    double stepZeroTime; // time of last step of previous frame
    int steps;
    double stepSize = 0.1;

    // Set our step duration. Noop if <= 0.
    void setStepSize(double t) { if (t > 0) stepSize = t; };

    // How long are our steps?
    double getStepSize() { return stepSize; };

    // How long was the last frame?
    double frameDuration() { return frameEnd - frameStart; };

    // How long were all the steps in the last frame;
    double stepsDuration() { return steps * stepSize; };

    // What time is the last step in the frame?
    double lastStepTime() { return stepZeroTime + (steps * stepSize); };

    // Restart the lastStepTime, but do not change the stepSize
    void restart() {
        frameStart = 0;
        frameEnd = 0;
        stepZeroTime = 0;
        steps = 0;
    };

    void advanceFrame(double frameDelta) {
        // update the stepZeroTime to the last step in the previous frame. Note
        // that this should be done before the other mutations in this function.
        stepZeroTime += stepsDuration();

        // update the frame times
        frameStart = frameEnd;
        frameEnd = frameStart + frameDelta;

        // how many steps to take in this frame?
        steps = floor((frameEnd - stepZeroTime) / stepSize);
    };
};

#endif /* stepper_h */
