#include "ofApp.h"
#include <iostream>
void ofApp::setup()
{
    shader.setupShaderFromFile(GL_COMPUTE_SHADER, "computeShader.cs");
    shader.linkProgram();

    texture.allocate(W, H, GL_RGBA8);
    texture.bindAsImage(4, GL_WRITE_ONLY);

    for (int x = 0; x < W; x++)
        for (int y = 0; y < H; y++)
        {
            int idx = x + y * W;
            A1cpu[idx] = 1.0;
            A2cpu[idx] = 1.0;
            if (rand() / float(RAND_MAX) < 0.000021)
                B1cpu[idx] = 1.0;
            else
                B1cpu[idx] = 0.0;
            B2cpu[idx] = 0.0;
        }

    A1.allocate(W * H * sizeof(float), A1cpu, GL_STATIC_DRAW);
    A2.allocate(W * H * sizeof(float), A2cpu, GL_STATIC_DRAW);
    B1.allocate(W * H * sizeof(float), B1cpu, GL_STATIC_DRAW);
    B2.allocate(W * H * sizeof(float), B2cpu, GL_STATIC_DRAW);
    A1.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    A2.bindBase(GL_SHADER_STORAGE_BUFFER, 1);
    B1.bindBase(GL_SHADER_STORAGE_BUFFER, 2);
    B2.bindBase(GL_SHADER_STORAGE_BUFFER, 3);
}

//--------------------------------------------------------------
void ofApp::update()
{
    static int c = 1;
    c = 1 - c;
    A1.bindBase(GL_SHADER_STORAGE_BUFFER, 0 + c);
    A2.bindBase(GL_SHADER_STORAGE_BUFFER, 0 + 1 - c);
    B1.bindBase(GL_SHADER_STORAGE_BUFFER, 2 + c);
    B2.bindBase(GL_SHADER_STORAGE_BUFFER, 2 + 1 - c);

    shader.begin();
    shader.dispatchCompute(W / 20, H / 20, 1);
    shader.end();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    texture.draw(0, 0);
}