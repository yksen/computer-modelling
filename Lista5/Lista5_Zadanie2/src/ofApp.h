#pragma once

#include "ofMain.h"

#define W 1920
#define H 1080

class ofApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();

	ofBufferObject A1, A2;

	ofTexture texture;
	ofShader shader;

	float A1cpu[W * H];
	float A2cpu[W * H];
};
