//
//  trail.h
//  Follower
//
//  Created by Charles Holbrow on 11/26/18.
//

#ifndef trail_h
#define trail_h

#include "gesturePlayhead.h"

const static ofColor WHITE = ofColor(255, 255, 255);
const static ofColor BLACK = ofColor(0, 0, 0);

class Particle {
public:
    ofVec3f pos;
    ofVec3f velocity;
    double size;
    float age = 0;

    void update(double deltaTime);
    bool isDead();
};

class Trail {
public:
    GesturePlayhead playhead;
    double speed = 1;
    std::list <Particle> parts;
    void clear() {
        parts.clear();
    };

    // Update each particle
    void update(double deltaTime);

    // update the most recently added particle
    void updateLast(double deltaTime);

    // Add a single particle, and update with deltaTime. Calls ofGetStyle for color.
    void add(ofVec3f pos, double size);

    // Is playback complete?
    bool isDead();
};


#endif /* trail_h */
