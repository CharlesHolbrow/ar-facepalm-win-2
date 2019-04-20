//
//  json.m
//  Follower
//
//  Created by Charles Holbrow on 11/30/18.
//

//#import <Foundation/Foundation.h>
#include "customJson.h"

using nlohmann::json;
// Blip json
void to_json(json& j, const Blip& b) {
    j = json{{"pos", b.pos}, {"t", b.gestureTime}};
};
void from_json(const json& j, Blip& b) {
    b.pos = j.at("pos").get<ofVec2f>();
    b.gestureTime = j.at("t").get<float>();
};
// ofVec2f json
void to_json(json& j, const ofVec2f v2){
    j = json{{"x", v2.x}, {"y", v2.y}};
};
void from_json(const json& j, ofVec2f v2){
    v2.x = j.at("x").get<float>();
    v2.y = j.at("y").get<float>();
};
// stepper json
void to_json(json& j, const Stepper& s) {
    j = json{
        {"fs", s.frameStart},
        {"fe", s.frameEnd},
        {"szt", s.stepZeroTime},
        {"s", s.steps},
        {"ss", s.stepSize}
    };
};
void from_json(const json& j, Stepper& s) {
    s.frameStart = j.at("fs").get<double>();
    s.frameEnd = j.at("fe").get<double>();
    s.stepZeroTime = j.at("szt").get<double>();
    s.steps = j.at("s").get<int>();
    s.stepSize = j.at("ss").get<double>();
};

