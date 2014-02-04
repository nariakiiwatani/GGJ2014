#include "ofMain.h"
#include "ofApp.h"
#include "Def.h"

//========================================================================
int main( ){
	ofSetupOpenGL(SCREEN_WIDTH,SCREEN_HEIGHT,OF_WINDOW);			// <-------- setup the GL context
	ofSetWindowPosition(0,0);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
