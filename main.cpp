#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    // Full resolution
    int w1 = 11520;
    int h1 = 972;
    
    // Middle resolution
    int w2 = 7680;
    int h2 = w2*0.084375;
    
    // Only macbook
    int w3 = 1280;
    int h3 = w3*0.084375;
    int w4 = 3840;
    int h4 = w4*0.084375;
    
    int w5 = 1080;
    int h5 = 1920;
    //    ofAppGlutWindow window;
    //    ofSetupOpenGL(&window, w1,h1, OF_FULLSCREEN);            // <-------- setup the GL context
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    
    // Multi-display mode
    
    ofGLFWWindowSettings settings;
    
    settings.width = w5;
    settings.height = h5;
    
    settings.glVersionMajor = 2;
    settings.glVersionMinor = 1;
    //    settings.numSamples = 4;
    
    settings.multiMonitorFullScreen = true;
    settings.windowMode = OF_WINDOW;
    settings.decorated = false;            // eliminated the frame bar
    settings.setPosition(ofVec2f(0,0));  // didn't work at IAC
//    settings.setPosition(ofVec2f(-settings.width/2.5,0));
//
    //    settings.resizable = false;            // need to be tested (comment out)
    //    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    ofCreateWindow(settings);
    //ofEnableAntiAliasing();
    
    ofRunApp(new ofApp());
    
    // manipulating by Zach Lieberman
    
    //     ofAppGLFWWindow w;
    //    w.setMultiDisplayFullscreen(true);
    ////     ofSetupOpenGL(&w, w1,h1,OF_GAME_MODE);            // <-------- setup the GL context
    //    ofSetupOpenGL(&w, w1, h1, OF_FULLSCREEN);
    ////    ofSetWindowPosition(0, 0);
    ////     ofApp * app = new ofApp();
    //     ofRunApp( new ofApp());
    ////     app->WINDOW = &w;
    ////     ofRunApp(app);
}
