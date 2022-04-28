#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main()
{
	ofGLWindowSettings settings;
	settings.setSize(W, H);

	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
