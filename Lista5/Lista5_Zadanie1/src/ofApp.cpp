#include "ofApp.h"
#include <iostream>
#include <fstream>

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
            if (rand() / float(RAND_MAX) < 0.5)
                A1cpu[idx] = 0.0f;
            else
                A1cpu[idx] = 1.0f;
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

    float density1 = 0.0f, density2 = 0.0f;
    float *data1, *data2;
    data1 = A1.map<float>(GL_READ_ONLY);
    data2 = A2.map<float>(GL_READ_ONLY);
    for (int i = 0; i < W * H; ++i)
    {
        density1 += data1[i];
        density2 += data2[i];
    }
    A1.unmap();
    A2.unmap();

    if (tick > 0)
    {
        density1 /= float(W * H);
        density2 /= float(W * H);

        float ratio = density1 / density2 - 3.0f;

        std::ofstream file("density.txt", ios::app);
        file << tick << "\t" << ratio << std::endl;
        file.close();
    }

    ++tick;

    shader.begin();
    shader.dispatchCompute(W / 20, H / 20, 1);
    shader.end();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    texture.draw(0, 0);
}