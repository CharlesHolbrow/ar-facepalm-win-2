#include "gesturePlayhead.h"

void GesturePlayhead::init(Gesture* g) {
    gesture = g;
    playbackIndex = 0;
    playbackTime = 0;
};

// Update returns a range of blips, which is my (Charles') template, which wraps
// a beginning and end iterator. The range of blips covers all the blips that
// were added during this update.
Range<std::vector<Blip>::iterator> GesturePlayhead::update(double delta) {
    double until = playbackTime + delta; // play until this time
    size_t start = playbackIndex; // The index we start playing from
    size_t& end = playbackIndex;  // using a ref here clearifies loop behavior

	while (gesture->blipsVec.size() > end && !playbackComplete()) {
        // we know that blipsVec[end] exists
        // we know that blipsVec[end] was added before gesture termination
        if (gesture->blipsVec[end].gestureTime > until) break;
        // we know that blipsVec[end] time is <= until
        end++; // increments playbackIndex
        // end (and playbackIndex) now point to the next blip to look at
    }
    // note that because `end` is a refernce to `playbackIndex`, we have been
    // updating playbackIndex inside the loop.
    playbackTime = until;
    return Range<std::vector<Blip>::iterator>(gesture->blipsVec.begin() + start, gesture->blipsVec.begin() + end);
};

bool GesturePlayhead::playbackComplete() {
    if (!gesture->isTerminated()) return false; // gesture is still recording
    return playbackIndex >= gesture->size();
};
