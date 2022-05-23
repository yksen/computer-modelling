#pragma once

#include "ofMain.h"

#define W 1280
#define H 720

class ofApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();

	ofBufferObject Sand1, Sand2, Ground;

	ofTexture texture;
	ofShader shader;

	float Sand1cpu[W * H];
	float Sand2cpu[W * H];
	float Groundcpu[W * H];
};
