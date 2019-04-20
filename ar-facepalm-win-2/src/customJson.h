//
//  json.h
//  Follower
//
//  Created by Charles Holbrow on 11/30/18.
//

#ifndef json_h
#define json_h

#include "ofMain.h"
#include "ofJson.h"
#include "gesture.h"
#include "stepper.h"

using nlohmann::json;

// make Blips json de/serializable
void to_json(json& j, const Blip& b);
void from_json(const json& j, Blip& b);
// make ofVec2f json de/serializable
void to_json(json& j, const ofVec2f v2);
void from_json(const json& j, ofVec2f v2);
// Stepper
void to_json(json& j, const Stepper& s);
void from_json(const json& j, Stepper& s);

#endif /* json_h */
