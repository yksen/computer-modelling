#include "ofApp.h"
#include <iostream>
#include <fstream>
#include <random>

void ofApp::setup()
{
    shader.setupShaderFromFile(GL_COMPUTE_SHADER, "computeShader.glsl");
    shader.linkProgram();

    texture.allocate(W, H, GL_RGBA8);
    texture.bindAsImage(4, GL_WRITE_ONLY);

    std::mt19937 mt(time(NULL));
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int x = 0; x < W; x++)
        for (int y = 0; y < H; y++)
        {
            int idx = x + y * W;
            if (dist(mt) < 0.7f)
                A1cpu[idx] = 1.0f;
            else
                A1cpu[idx] = 0.0f;
            A2cpu[idx] = 0.0f;
        }

    A1.allocate(W * H * sizeof(float), A1cpu, GL_DYNAMIC_DRAW);
    A2.allocate(W * H * sizeof(float), A2cpu, GL_DYNAMIC_DRAW);
    A1.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    A2.bindBase(GL_SHADER_STORAGE_BUFFER, 1);
}

void ofApp::update()
{
    static int c = 1;
    c = 1 - c;
    A1.bindBase(GL_SHADER_STORAGE_BUFFER, 0 + c);
    A2.bindBase(GL_SHADER_STORAGE_BUFFER, 0 + 1 - c);

    shader.begin();
    shader.dispatchCompute(W / 20, H / 20, 1);
    shader.end();

    float aliveCount = 0.0f;
    float *data1;
    data1 = A1.map<float>(GL_READ_ONLY);
    for (int x = 0; x < W; ++x)
        for (int y = 0; y < H; ++y)
        {
            int idx = x + y * W;
            aliveCount += data1[idx];
        }
    A1.unmap();

    std::ofstream file("ratio_0.70.txt", ios::app);
    file << tick << "\t" << aliveCount << "\t" << W * H - aliveCount << std::endl;
    file.close();

    ++tick;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    texture.draw(0, 0);
}