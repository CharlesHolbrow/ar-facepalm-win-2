#pragma once
#include "ofMain.h"

// IMPORTANT: Circles expects OpenGL 3.2

class Circles
{
public:
    Circles();
    ~Circles();

    // These function are aproximately in the order that you are likely to call
    // them in. First resize, then set color and matrix for each circle. Finally
    // updateGpu will send matrices to the gpu in one batch (colors will be sent
    // in the draw call if they are "dirty"). Then draw().
    //
    // setMatrix can also be called like this:
    // circles.matrices[i] = ofNode.getLocalTransformMatrix();
    //
    // setColor can also be called like this:
    // circles.mesh.getColors()[i] = ofColor;
    //
    // Make sure to updateGpu before drawing if you change any matrices
    void resize(size_t n);
    size_t size();
    void setColor(size_t i, ofColor color);
    void setMatrix(size_t i, const glm::mat4 matrix);
    void updateGpu();
    void draw();

    // The defaults probably work fine, but it can be useful to specify geometry
    void setGeometry(int resolution, float radius);

    ofTexture tex;
    ofBufferObject buffer;
    vector<ofMatrix4x4> matrices;
    ofVboMesh mesh;
    ofShader shader;
};

