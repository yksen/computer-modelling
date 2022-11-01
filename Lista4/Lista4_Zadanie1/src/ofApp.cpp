#include "ofApp.h"

void ofApp::setup()
{
    shader.setupShaderFromFile(GL_COMPUTE_SHADER, "computeShader.glsl");
    shader.linkProgram();

    A1.allocate(W * H * sizeof(float), A1cpu, GL_DYNAMIC_DRAW);
    A1.bindBase(GL_SHADER_STORAGE_BUFFER, 0);

    texture.allocate(W, H, GL_RGBA8);
    texture.bindAsImage(1, GL_WRITE_ONLY);
}

void ofApp::update()
{
    shader.begin();
    shader.dispatchCompute(W / 20, H / 20, 1);
    shader.end();
}

void ofApp::draw()
{
    texture.draw(0, 0);
}
