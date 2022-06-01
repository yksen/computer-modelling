#include "ofApp.h"
#include <iostream>
#include <random>

void ofApp::setup()
{
    shader.setupShaderFromFile(GL_COMPUTE_SHADER, "computeShader.cs");
    shader.linkProgram();

    texture.allocate(W, H, GL_RGBA8);
    texture.bindAsImage(4, GL_WRITE_ONLY);

    std::mt19937 mt(time(NULL));
    std::uniform_real_distribution<float> dist(0.0, 1.0);

    // for (int x = 0; x < W; ++x)
    //     for (int y = 0; y < H; ++y)
    //     {
    //         int idx = x + y * W;
    //         if (dist(mt) < 0.10)
    //         {
    //             Sand1cpu[idx] = 1.0f;
    //             Sand2cpu[idx] = 1.0f;
    //         }
    //         else
    //         {
    //             Sand1cpu[idx] = 0.0f;
    //             Sand2cpu[idx] = 0.0f;
    //         }
    //     }

    Sand1cpu[W / 2 + (H / 2 * W)] = 1.0f;

    // for (int x = 0; x < W; ++x)
    // {
    //     Groundcpu[x + (H - 1) * W] = 1.0f;
    //     Sand1cpu[x + (H - 1) * W] = 0.0f;
    //     Sand2cpu[x + (H - 1) * W] = 0.0f;
    // }

    Sand1.allocate(W * H * sizeof(float), Sand1cpu, GL_DYNAMIC_DRAW);
    Sand2.allocate(W * H * sizeof(float), Sand2cpu, GL_DYNAMIC_DRAW);
    Ground.allocate(W * H * sizeof(float), Groundcpu, GL_DYNAMIC_DRAW);
    Sand1.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    Sand2.bindBase(GL_SHADER_STORAGE_BUFFER, 1);
    Ground.bindBase(GL_SHADER_STORAGE_BUFFER, 2);
}

void ofApp::update()
{
    static int c = 1;
    c = 1 - c;
    Sand1.bindBase(GL_SHADER_STORAGE_BUFFER, c);
    Sand2.bindBase(GL_SHADER_STORAGE_BUFFER, 1 - c);

    shader.begin();
    shader.setUniform1i("c", c);
    shader.setUniform1i("time", ofGetElapsedTimef());
    shader.setUniform4i("mousePos", ofGetMouseX(), ofGetMouseY(), ofGetMousePressed(0), ofGetMousePressed(2));
    shader.dispatchCompute(W / 8, H / 8, 1);
    shader.end();
}

void ofApp::draw()
{
    texture.draw(0, 0);
}