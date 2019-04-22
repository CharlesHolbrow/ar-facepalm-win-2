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
    ofColor ofc = WHITE;

    void update(double deltaTime);
    ofColor color();
    bool isDead();
    void render();
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
    
    void updateLast(double deltaTime);

    // Add a single particle, and update with deltaTime. Calls ofGetStyle for color.
    void add(ofVec3f pos, double size);

    // Draw the trail
    void render();
    
    bool isDead();
};


#endif /* trail_h */
