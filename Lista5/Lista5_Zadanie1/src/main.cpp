#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main()
{
	ofGLWindowSettings settings;
	settings.windowMode = OF_FULLSCREEN;
	settings.setGLVersion(4, 5);

	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
