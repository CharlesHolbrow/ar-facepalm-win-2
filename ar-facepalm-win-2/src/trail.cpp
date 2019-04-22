#include "trail.h"

void Particle::update(double deltaTime) {
    pos += velocity * deltaTime;
    size -= 0.0006 * deltaTime;
    size = (size < 0) ? 0 : size;
    age += deltaTime;
};

bool Particle::isDead() {
    return (size < 0.001);
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
        add(b.pos, .016);
        updateLast(playhead.playbackTime - b.gestureTime); // this broke when I moved to windows
    }
};

void Trail::updateLast(double deltaTime) {
    if (!parts.size()) return;
    parts.back().update(deltaTime * speed);
};

// Add a single particle, and update with deltaTime.
void Trail::add(ofVec3f pos, double size) {
    Particle p;
    p.size = size;
    p.pos = pos;
    p.velocity.x = 0;
    p.velocity.y = 0;
    parts.push_back(p);
}

bool Trail::isDead() {
    // For now, let's consider Trails with uninitialized gestures dead. I'm
    // not totally sure that this is the best practice, but it's probably
    // best to initialize the gesture as soon as the trail is created, until
    // we decide exactly how we want to handle constructors.
    if (playhead.gesture == NULL) return true;

    // verify playback is complete AND there are no remaining blips
    return playhead.playbackComplete() && parts.empty();
};
