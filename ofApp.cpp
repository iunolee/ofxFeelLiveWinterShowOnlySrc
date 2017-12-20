#include "ofApp.h"
#include "ofxRipples.h"
#include "Boid.hpp"
#include "attractor.h"


vector <vector<Boid>> boids;
vector<attractor> attractors;
//--------------------------------------------------------------

void ofApp::setup(){
    
    // GRAPHIC SETTING
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    
    // BOOLEAN SETTING
    bRipple = true;
    bHide = true;
    bLine = false;
    bMass = false;
    ofHideCursor();
  
    
    forceToCenter = false;
    forceToScatter = false;
    
    // GUI
    gui.setup();
    gui.setPosition(ofGetWindowSize()[0]/2, guiPosition);
    gui.add(rippleSize.setup("size", 100, 1, 150));
    gui.add(rippleDistance.setup("distance", 2, 1, 10));
    gui.add(rippleDamping.setup("damping", 0.900, 0.900, 0.999));
    gui.add(urlYpos.setup("urlYpos", 300, 0, ofGetWindowHeight()));
    gui.add(urlXpos1.setup("urlXpos1", 1075, 0, ofGetWindowSize()[0]));
    gui.add(urlXpos2.setup("urlXpos2", 3380, 0, ofGetWindowSize()[0]));
    gui.add(urlXpos3.setup("urlXpos3", 5450, 0, ofGetWindowSize()[0]));
    gui.add(urlSize.setup("urlSize", 100, 10, 300));
    gui.add(instructionSize.setup("instructionSize", 26, 10, 300));
    gui.add(effectAStart.setup("effectAStart", 50, 0, 1000));
    gui.add(effectATime.setup ("effectATime", 10,  0, 1000));
    gui.add(effectBStart.setup("effectBStart", 150, 0, 1000));
    gui.add(effectBTime.setup ("effectBTime", 10,  0, 1000));
    gui.add(effectCStart.setup("effectCStart", 250, 0, 1000));
    gui.add(effectCTime.setup ("effectCTime", 10,  0, 1000));
    //    gui.loadFromFile("settings.xml");
    ofHideCursor();
    
    // SCENE MANAGER
    sceneManager.setup();
    sceneManager.setPosition(ofGetWindowSize()[0]/2 + 230, guiPosition);
    sceneManager.add(scene0.setup("scene0",  100, 0, 3000 )); // blobs go to attraction
    sceneManager.add(scene1.setup("scene1",  200, 0, 3000 )); // video 1 play
    sceneManager.add(scene2.setup("scene2", 200, 0, 3000 )); // video 2 play, blobs disappear
    sceneManager.add(scene3.setup("scene3", 1810, 0, 3000 )); // sentences come and go out
    sceneManager.add(scene4.setup("scene4", 1910, 0, 3000 )); // words particles come in
    sceneManager.add(scene5.setup("scene5", 2120, 0, 3000 )); // words go out letters drop
    
    // SET STARTING POINT OF RIPPLE
    rip.allocate(ofGetWindowSize()[0], ofGetWindowHeight());
    
    // SOCKET SETUP
    isConnected = false;
    address = "http://67.205.153.66:9999";
    status = "not connected";
    socketIO.setup(address);
    ofAddListener(socketIO.notifyEvent, this, &ofApp::gotEvent);
    ofAddListener(socketIO.connectionEvent, this, &ofApp::onConnection);

    
    // VIDEO LOAD
    for(int i = 0 ; i<8 ; i++){
        videos[i].setPixelFormat(OF_PIXELS_RGBA);
    }
    
    videos[0].load("video/500_Joy_h264.mov");
    videos[1].load("video/500_Trust_h264.mov");
    videos[2].load("video/500_Fear_h264.mov");
    videos[3].load("video/500_Surprise_h264.mov");
    videos[4].load("video/500_Sadness_h264.mov");
    videos[5].load("video/500_Disgust_h264.mov");
    videos[6].load("video/500_Anger_h264.mov");
    videos[7].load("video/500_Anticipation_h264.mov");
    
    for(int i = 0 ; i<8 ; i++){
        videos[i].play();
        videos[i].setLoopState(OF_LOOP_NORMAL);
    }
    
    tempPosSet[0].set(50, 15);
    tempPosSet[1].set(ofGetWindowWidth()/2+40, 15);
    tempPosSet[2].set(50, ofGetWindowHeight()/4+15);
    tempPosSet[3].set(ofGetWindowWidth()/2+40, ofGetWindowHeight()/4+15);
    tempPosSet[4].set(50, ofGetWindowHeight()*2/4+15);
    tempPosSet[5].set(ofGetWindowWidth()/2+40, ofGetWindowHeight()*2/4+15);
    tempPosSet[6].set(50, ofGetWindowHeight()*3/4+15);
    tempPosSet[7].set(ofGetWindowWidth()/2+40, ofGetWindowHeight()*3/4+15);
    
    for(int i = 0 ; i<8 ; i++){
        vidPosSet.push_back(tempPosSet[i]);
    }
    
    // VIDEO INITIAL POSITION & MOVING SPEED
    
    for(int i = 0 ; i<8 ; i++){
        vidPos[i].set(vidPosSet[i]);
        vidVel[i].set(0,0);
    }


    // URL AND INSTRUCTION
    //    url.load("Roboto-Thin.ttf",urlSize, true, false, true);
    //    instruction.load("Roboto-Thin.ttf",instructionSize, true, false, true);
    
    // SOUND SETUP
    soundSetup();
    
    // ATTRACTOR SETUP
    for (int i = 0; i < 8 ; i++) {
        attractor myAttractors;
        attractors.push_back(myAttractors);
        attractors[i].setup();
        attractors[i].setLocation(vidPos[i].x+225, vidPos[i].y+225);
    }
    
    // BLOB ARRAY SETUP
    boids.push_back(blobs1);
    boids.push_back(blobs2);
    boids.push_back(blobs3);
    boids.push_back(blobs4);
    boids.push_back(blobs5);
    boids.push_back(blobs6);
    boids.push_back(blobs7);
    boids.push_back(blobs8);
    
    initSetting();
}

//--------------------------------------------------------------

void ofApp::initSetting () {
    //SET COLOR LIST
    colors = new ofColor[colorNum+1];
    
    //ALPHA VALUE
    for(int i = 0; i< colorNum+1; i++){
        colors[i].a = alpha;
    }
    
    //initial value
    colors[0].r = 255;
    colors[0].g = 255;
    colors[0].b = 255;
    
    //joy
    colors[1].r = 248;
    colors[1].g = 198;
    colors[1].b = 51;
    
    //trust
    colors[2].r = 178;
    colors[2].g = 187;
    colors[2].b = 58;
    
    //fear
    colors[3].r = 10;
    colors[3].g = 108;
    colors[3].b = 67;
    
    //surprise
    colors[4].r = 6;
    colors[4].g = 128;
    colors[4].b = 157;
    
    //sadness
    colors[5].r = 118;
    colors[5].g = 59;
    colors[5].b = 123;
    
    //disgust
    colors[6].r = 145;
    colors[6].g = 30;
    colors[6].b = 71;
    
    //anger
    colors[7].r = 186;
    colors[7].g = 39;
    colors[7].b = 58;
    
    //anticipation
    colors[8].r = 216;
    colors[8].g = 97;
    colors[8].b = 43;
    
    
    //SET COUNT TO VISUALIZE EACH WORD ONE BY ONE
    countSentence = 0;
    spd = 0;
    countWord = 0;
}



//--------------------------------------------------------------

void ofApp::update(){
    int ranSize = ofRandom(-10.0, 10.0);
    
    if(bRipple){
        rip.begin();
        ofFill();
        
        
        for (unsigned int i = 0; i < boids.size(); i++){
            for (unsigned int j = 0; j < boids[i].size(); j++){
                
                // SET FORCES
                ofVec2f force = attractors[i].attract(boids[i][j]);
                boids[i][j].flock(boids[i]);
                
                // SCENE 1 GO TO THE ATTRACTORS
                //                if(timer >= scene0){
                boids[i][j].applyForce(force*(2+i*0.1));
               if(bLine){
                    if(j>0){
                        ofSetColor(125,125,125,255);

                        ofDrawLine(boids[i][j].location.x, boids[i][j].location.y, boids[i][j-1].location.x, boids[i][j-1].location.y);
                    }
                }
                //                }
                
                boids[i][j].update();
                boids[i][j].borders();
            }
        }
        
        for (unsigned int i = 0; i < boids.size(); i++){
            for (unsigned int j = 0; j < boids[i].size(); j++){
                ofEnableBlendMode(OF_BLENDMODE_SCREEN);
                boids[i][j].draw(rippleSize + ofRandom(10));
                //                if(bVideo1){
                //                     MAKE ATTRACTORS VISIBLE
                //                    for (int i = 0 ; i < 8; i++ ){
                //                        attractors[i].draw();
                //                    }
                //                }
            }
        }
        
        for (int i = 0; i < 8 ; i++){
            videos[i].update();
        }
        
        if(fmod(timer, 71) == 1 || fmod(timer, 71) == 3){
            rippleDistance = ofRandom(6, 7);
        }
        
        rip.end();
        rip.update();
    }
    
    
    //Attraction for blob
    for (int i = 0; i < attractors.size() ; i++) {
        attractors[i].setLocation(vidPos[i].x+225, vidPos[i].y+225);
    }
    
    // Update move for video
    for(int i = 0; i<8; i++) {
        vidVel[i] = vidVel[i] + vidFrc[i];
        vidPos[i] = vidPos[i] + vidVel[i];
    }
    

    
    //Attraction for video to move to center
    if(forceToCenter) {
        
        ofVec2f posOfForce;
        posOfForce.set(ofGetWindowSize()[0]/2-225, ofGetWindowHeight()/2-225);
        
        ofVec2f forces[8];
        float distance[8];
        float strength[8];
        for(int i = 0; i<8; i++) {
            // Calculate direction of force
            forces[i] = posOfForce - vidPos[i];
            // Distance between objects
            distance[i] = forces[i].length();

            // Limiting the distance to eliminate "extreme" results for very close or very far objects
            distance[i] = ofClamp(distance[i],10, 15);
            // Normalize vector (distance doesn't matter here, we just want this vector for direction)
            forces[i].normalize();
            // Calculate gravitional force magnitude
            strength[i] = (60 / (distance[i] * distance[i]));
            // Get force vector --> magnitude * direction
            forces[i] *= strength[i];
            vidFrc[i] = forces[i];
            
            vidFrc[i] = vidFrc[i] - vidVel[i] * 0.05;

        }
        
    }
    
    
    
    if(fmod(timer, 272) == 1) { // center 202
        forceToCenter = true;
        forceToScatter = false;
        bLine = true;
        bMass = false;

        random_shuffle(vidPosSet.begin(), vidPosSet.end());
        
    }
    
    if(fmod(timer, 152)== 1) { // scatter 272
        forceToCenter = false;
        forceToScatter = true;
        bLine = false;

    }
  
    
    //Attraction for video to move to center
    if(forceToScatter) {
        
        
        ofVec2f posOfForce[8];
        ofVec2f forces[8];
        float distance[8];
        float strength[8];
        for(int i = 0; i<8; i++) {
            posOfForce[i].set(vidPosSet[i]);
            // Calculate direction of force
            forces[i] = posOfForce[i] - vidPos[i];
            // Distance between objects
            distance[i] = forces[i].length();
            // Limiting the distance to eliminate "extreme" results for very close or very far objects
            distance[i] = ofClamp(distance[i],10,15);
            // Normalize vector (distance doesn't matter here, we just want this vector for direction)
            forces[i].normalize();
            
            cout<<forces[i]<<endl;
            
            // Calculate gravitional force magnitude
            strength[i] = 40 / (distance[i] * distance[i]);
            // Get force vector --> magnitude * direction
            forces[i] *= strength[i];
            vidFrc[i] = forces[i];
         
            vidFrc[i] = vidFrc[i] - vidVel[i] * 0.05;

            if(vidPos[i].distance(posOfForce[i]) <= 0.5) {
                vidFrc[i].set(0,0);
                vidVel[i].set(0,0);
            }
            
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    // SET TIMER
    timer = int(ofGetElapsedTimeMillis() - startTime)/100;
    
    
    //    if(urlShow) {
    //        //URL AND INSTRUCTION INITIAL
    //        string urlText = "www.myfeel.live";
    //        float urlWidth = url.stringWidth(urlText);
    //        float urlHeight = url.stringHeight(urlText);
    //        ofSetColor(255,255,255,200);
    //        url.drawString(urlText, urlXpos1 - urlWidth/2, urlYpos+urlHeight/2);
    //        url.drawString(urlText, urlXpos2 - urlWidth/2, urlYpos+urlHeight/2);
    //        url.drawString(urlText, urlXpos3 - urlWidth/2, urlYpos+urlHeight/2);
    //
    //        string instructionText = "Please open up your mobile and go to this link with any brower";
    //        ofSetColor(255,255,255,100);
    //        instruction.drawString(instructionText, urlXpos1 - urlWidth/2, urlYpos+ urlHeight);
    //        instruction.drawString(instructionText, urlXpos2 - urlWidth/2, urlYpos+ urlHeight);
    //        instruction.drawString(instructionText, urlXpos3 - urlWidth/2, urlYpos+ urlHeight);
    //    }
    
    if(bRipple){
        // URL REMAINED
        ofSetColor(255,255,255,255);
        
        //        string urlText = "www.myfeel.live";
        //        float urlWidth = url.stringWidth(urlText);
        //        float urlHeight = url.stringHeight(urlText);
        //        url.drawString(urlText, urlXpos1 - urlWidth/2, urlYpos+urlHeight/2);
        //        url.drawString(urlText, urlXpos2 - urlWidth/2, urlYpos+urlHeight/2);
        //        url.drawString(urlText, urlXpos3 - urlWidth/2, urlYpos+urlHeight/2);
        //
        rip.draw(0,0, ofGetWindowSize()[0], ofGetWindowHeight());
        rip.distance = rippleDistance;
        rip.damping = rippleDamping;
        
        // ADJUSTING BLOB SIZE
        //        if(bVideo1 == false && bVideo2 == false){
        float sinOfTime               = sin( ofGetElapsedTimef()/2);
        float sinOfTimeMapped         = ofMap(sinOfTime, -1, 1, 5, 25);
        float sinOfTimeMappedDamp     = ofMap(sinOfTime, -1, 1, 0.910, 0.930);
        float sinOfTimeMappedDistance = ofMap(sinOfTime, -1, 1, 1.0, 3.0);
        
        rippleSize = sinOfTimeMapped;
        rippleDamping = sinOfTimeMappedDamp;
        rippleDistance = sinOfTimeMappedDistance; // 9  - 2
        
        
        
    }
    
    
    // VIDEO PLAY, DELETE ALPHA CHANNEL
    ofEnableAlphaBlending();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    for (int i = 0; i < 8 ; i++){
        videos[i].draw(vidPos[i].x, vidPos[i].y, 450, 450);
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ofDisableAlphaBlending();
    
    
    
    // BLOB DISAPPEAR AFTER SCENE 2
    if(rippleSize <= 1){
        rippleSize = 0;
        for (unsigned int i = 0; i < 8 ; i++){
            for (unsigned int j = 0; j < boids[i].size(); j++){
                boids[i].pop_back();
            }
        }
    }
    int total = boids[0].size()+boids[1].size()+boids[2].size()+boids[3].size()+boids[4].size()+boids[5].size()+boids[6].size()+boids[7].size();
    
    if(total > 300) {
        for (int i = 0 ; i < 8 ; i++){
            if(boids[i].size()>0){
                boids[i].pop_back();
            }
        }
    }

    
    // GUI & INFORMATION TEXT DRAW
    gui.draw();
    sceneManager.draw();
    
    string fr = "fr: " + ofToString(ofGetFrameRate());
    string bn = "total blob: " + ofToString(boids[0].size()+boids[1].size()+boids[2].size()+boids[3].size()+boids[4].size()+boids[5].size()+boids[6].size()+boids[7].size());
    string t = "time: " +ofToString(timer);

    
    ofSetColor(255,255,255);
    ofDrawBitmapString(fr, ofGetWindowSize()[0]/2-500, guiPosition);
    ofDrawBitmapString(bn, ofGetWindowSize()[0]/2-500, guiPosition+15);
    ofDrawBitmapString(t, ofGetWindowSize()[0]/2-500, guiPosition+30);
    ofDrawBitmapString(mouseX, ofGetWindowSize()[0]/2-500, guiPosition+45);
    ofDrawBitmapString(mouseY, ofGetWindowSize()[0]/2-450, guiPosition+45);
    
    
    for(int i = 0; i<8; i++) {
        ofDrawBitmapString(boids[i].size(), ofGetWindowSize()[0]/2-500+(i*20), guiPosition+60);
    }
    
    if(bHide == false) {
        guiPosition = ofGetWindowHeight()/10;
        gui.setPosition(ofGetWindowSize()[0]/2, guiPosition);
        sceneManager.setPosition(ofGetWindowSize()[0]/2+230, guiPosition);
        ofShowCursor();
        
    } else {
        guiPosition = -500;
        gui.setPosition(ofGetWindowSize()[0]/2, guiPosition);
        sceneManager.setPosition(ofGetWindowSize()[0]/2+230, guiPosition);
        ofHideCursor();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    //HIDE GUI
    if(key == 'h') {
        bHide = !bHide;
    }
}

//--------------------------------------------------------------

void ofApp::soundSetup(){
    sound1.load("bgm_1207_final.wav");
    int nFiles = dir.listDir("cloud");
    
    if(nFiles) {
        for(int i=0; i<dir.size(); i++) {
            // add the sound to the vector
            string filePath = dir.getPath(i);
            sclouds.push_back(ofSoundPlayer());
            sclouds.back().load(filePath);
        }
        
        for(int i=0; i<sclouds.size(); i++) {
            sclouds[i].setVolume(1.0f);
        }
    }
}


//--------------------------------------------------------------

void ofApp::soundEffectStop(){
    for(int i=0; i<sclouds.size(); i++) {
        sclouds[i].stop();
    }
}

//--------------------------------------------------------------

void ofApp::onConnection () {
    isConnected = true;
    bindEvents();
}

//--------------------------------------------------------------

void ofApp::bindEvents () {
    std::string serverEventName = "server-event";
    socketIO.bindEvent(serverEvent, serverEventName);
    ofAddListener(serverEvent, this, &ofApp::onServerEvent);
}

//--------------------------------------------------------------

void ofApp::gotEvent(string& name) {
    status = name;
}

//--------------------------------------------------------------

void ofApp::onServerEvent (ofxSocketIOData& data) {
    
    //COUNT TOTAL INPUT NUMBER
    gotDataTotalNum++;
    
    int gotData = data.getIntValue("intData");
    
    
    //DATA -> MAKE RIPPLE
    if (gotData){
//        urlShow = false;
        bRipple = true;
        
        sclouds[ofRandom(9)].play();
        
        // CREATE A BLOB
        Boid v1;
        v1.setup();
        v1.color = colors[gotData]; // data range : 1~8
        v1.setLocation(ofGetWindowSize()[0]*1/4 + ofRandom(ofGetWindowSize()[0]/2), ofGetWindowHeight()/4 + ofRandom(ofGetWindowHeight()/2));
        // ALLOCATE TO DIFFERENT ARRAYS GROUPED BY COLORS
        boids[gotData-1].push_back(v1); // now has size
    }
}
