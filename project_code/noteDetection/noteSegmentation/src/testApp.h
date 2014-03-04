#pragma once

#include "ofMain.h"

#include "ofxUI.h"
#include "ofxXmlSettings.h"
#include "pitchDetectorManager.h"
#include "audioUnitManager.h"
#include "segmentationManager.h"
#include "PDCompare.h"


class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void loadAudio( string fileName );
    
    void setupGUI();
    void guiEvent(ofxUIEventArgs &e);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    vector < note > notes;
//    void addNote( int startTime, int endTime, int avgTone);
    void addNote( note foundNote, metadata noteMetadata);
    
    
    int samplerate, windowSize, hopSize;
    ofSoundStream ss;
    void audioIn(float * input, int bufferSize, int nChannels);
    void audioOut(float * output, int bufferSize, int nChannels);


    pitchDetectorManager PDM;
    PDCompare PDC;
    segmentationManager SM;
    audioUnitManager AU;
    ofxUICanvas * gui;
    bool bSaveGui;
    
    string outputFolder;
    string absOutputFolder;
    
    vector < float > audioSamples;      // raw audio of what's playing.
    
    void processAudioFileOffline();
    
    bool bSaving;
    bool bPlayMidi;
    bool bPlayingSamples;
    
    bool bProcessOffline;
    
    float audioVol, sinVol;
    int samplerOctavesUp, sinOctavesUp;
    float sinAngle;
    
    int state;
    
    int preContext, postContext;

};
