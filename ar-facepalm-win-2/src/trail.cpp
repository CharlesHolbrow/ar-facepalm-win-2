#include "trail.h"

void Particle::update(double deltaTime) {
    pos += velocity * deltaTime;
    size -= 0.0006 * deltaTime;
    size = (size < 0) ? 0 : size;
};

ofColor Particle::color() {
    const float scale = ofMap(size, 0, .012, 0.01, 1, true);
    return BLACK.getLerped(ofc, scale);
};

bool Particle::isDead() {
    return (size < 0.001);
};

void Particle::render() {
    ofSetColor(color());
    ofDrawCircle(pos.x, pos.y, pos.z, size);
};


////////////////////////////////////////////////////////////////
// Trail

// Update each particle
void Trail::update(double deltaTime) {
    for (auto p = parts.begin(); p != parts.end(); p++) {
        p->update(deltaTime * speed);
    }
    // remove small particles from the "front"
    while (parts.size() > 0 && parts.front().isDead()) {
        parts.pop_front();
    }

    // see if there are new blips from the gesture
    double updateTime = 0;
    for (auto b : playhead.update(deltaTime)) {
        ofSetColor(127. + 64. * sin(b.gestureTime),
            200. + 50 * cos(ofGetElapsedTimef() * 0.1),
            127. + 127. * cos(ofGetElapsedTimef()* 0.0001));
        add(b.pos, .016);
        updateLast(playhead.playbackTime - b.gestureTime); // this broke when I moved to windows
    }
};

void Trail::updateLast(double deltaTime) {
    if (!parts.size()) return;
    parts.back().update(deltaTime * speed);
};

// Add a single particle, and update with deltaTime. Calls ofGetStyle for color.
void Trail::add(ofVec3f pos, double size) {
    Particle p;
    p.size = size;
    p.pos = pos;
    p.velocity.x = 0;
    p.velocity.y = 0;
    p.ofc = ofGetStyle().color;
    parts.push_back(p);
}

// Draw the trail
void Trail::render() {
    for (auto p = parts.begin(); p != parts.end(); p++) {
        p->render();
    }
};

bool Trail::isDead() {
    // For now, let's consider Trails with uninitialized gestures dead. I'm
    // not totally sure that this is the best practice, but it's probably
    // best to initialize the gesture as soon as the trail is created, until
    // we decide exactly how we want to handle constructors.
    if (playhead.gesture == NULL) return true;

    // verify playback is complete AND there are no remaining blips
    return playhead.playbackComplete() && parts.empty();
};
