#pragma once

#include "gesture.h"

template <class Iter>
class Range {
    Iter b;
    Iter e;
public:
    Range(Iter b, Iter e) : b(b), e(e) {}
    Iter begin() { return b; }
    Iter end() { return e; }
};


// As of December 3, 2018,GesturePlayhead methods (other than init() assume that
// init(Gesture*) has been called.
struct GesturePlayhead {
    Gesture* gesture = NULL;
    size_t playbackIndex; // the last unplayed point
    double playbackTime;
    void init(Gesture* g);
    Range<std::vector<Blip>::iterator> update(double delta);
    bool playbackComplete();
};
