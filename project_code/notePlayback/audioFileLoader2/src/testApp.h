#pragma once

#include "ofMain.h"
#include "ofxUI.h"

#include "note.h"
#include "piano.h"

class testApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();
    
    void setupGUI();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);

    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    

    ofSoundStream soundStream;
    void audioOut(float * output, int bufferSize, int nChannels);

    vector < note * > notes;
    piano myPiano;

    bool bPlayingTone;
    float sinAngle;
    float startTime;
    float sinAngleAdder;
    
    
    vector < int > counter;
    
    ofxUICanvas * gui;
    
    int attack, release;
    float minPctMCN, maxMeloStdDev, maxYinStdDev, maxYinFFTStdDev, minYinAgree,minYinFFTAgree, minMeloKurtosis;
};
