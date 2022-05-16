#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main()
{
	ofGLWindowSettings settings;
	settings.setSize(W, H);
	settings.setGLVersion(4, 5);

	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
