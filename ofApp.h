#pragma once

#include "ofMain.h"
//#include "particle.hpp"
#include "ofEvents.h"
#include "ofxSocketIO.h"
#include "ofxRipples.h"
#include "ofxGUI.h"
#include "ofxFX.h"
#include "ofxJSON.h"
#include "ofxFontStash.h"
#include "Boid.hpp"



class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void initSetting();

    
    // SOCKET PART
    bool isConnected;
    void onConnection();
    void bindEvents();
    void gotEvent(std::string& name);
    void onServerEvent(ofxSocketIOData& data);
    
    // VIDEO & SOUND PART
    void soundSetup();
    void soundEffectStop();
    void videoSetup();
    
    ofxSocketIO socketIO;
    
    ofEvent<ofxSocketIOData&> serverEvent;
    ofEvent<ofxSocketIOData&> pingEvent;
    ofEvent<ofxSocketIOData&> nspingEvent;
    
    std::string address;
    std::string status;
    
    
    ofColor *colors;
    


    
    float countSentence;
    float spd;
    int countWord;
    
    int gotDataTotalNum;
    
    // GRAPHIC SIDE @YUN
    ofAppGLFWWindow * WINDOW;
    int screenHeight;
    ofxIntSlider screenWidth;
    
    // COLOR
    int colorNum = 8;
    float alpha = 100;
    ofBlendMode blendMode;
    
    // RIPPLE
    ofxRipples rip;
    int rippleFlag = 0;
    
    // BLOB ARRAYS
    vector <int> xpos;
    vector <int> ypos;
    
    // BLOB
    vector<Boid> blobs1;
    vector<Boid> blobs2;
    vector<Boid> blobs3;
    vector<Boid> blobs4;
    vector<Boid> blobs5;
    vector<Boid> blobs6;
    vector<Boid> blobs7;
    vector<Boid> blobs8;
    
    // BOOLEAN FOR SCENE CHANGE
    bool bHide;
    bool bRipple;
    bool bLine;
    bool bMass;
    
    
    
    // GUI
    ofxFloatSlider rippleSize;
    ofxFloatSlider rippleDistance;
    ofxFloatSlider rippleDamping;
    ofxIntSlider urlYpos;
    ofxIntSlider urlXpos1;
    ofxIntSlider urlXpos2;
    ofxIntSlider urlXpos3;
    ofxIntSlider urlSize;
    ofxIntSlider instructionSize;
    ofxIntSlider effectAStart;
    ofxIntSlider effectATime;
    ofxIntSlider effectBStart;
    ofxIntSlider effectBTime;
    ofxIntSlider effectCStart;
    ofxIntSlider effectCTime;
//    ofxToggle line;
    ofxPanel gui;
    
    // Scene Managament
    ofxIntSlider scene0;
    ofxIntSlider scene1;
    ofxIntSlider scene2;
    ofxIntSlider scene3;
    ofxIntSlider scene4;
    ofxIntSlider scene5;
    ofxPanel sceneManager;
    
    //TIMER
    float startTime;
    float timer;
    
    // VIDEO
    ofAVFoundationPlayer videos[8];
    vector<ofVec2f> vidPosSet;
    ofVec2f tempPosSet[8];
    ofVec2f vidPos[8];
    ofVec2f vidVel[8];
    ofVec2f vidFrc[8];
    
    bool forceToCenter;
    bool forceToScatter;


//    float vidPosX[8];
//    float vidPosY[8];
//    float vidVelX[8];
//    float vidVelY[8];
    
    //vector <ofAVFoundationPlayer> videos;
    
//    bool bVideo1;
//    bool bVideo2;
//    bool vid1isPlayed;
//    bool vid2isPlayed;

    // SOUND PLAYER
    ofSoundPlayer  sound1;
    vector <ofSoundPlayer> sclouds;
    ofDirectory dir;
    
    float myArray[100];
    float frequency;
    float phase;
    float amplitude;
    float phaseInc;
    
    // INTRO URL
    ofTrueTypeFont url;
    ofTrueTypeFont instruction;
    bool urlShow;
    int urlAlpha = 100;
    int guiPosition = -500;
};

