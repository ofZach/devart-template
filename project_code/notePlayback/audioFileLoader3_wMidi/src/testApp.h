#pragma once

#include "ofMain.h"
#include "note.h"
#include "piano.h"

#include "ofxMidi.h"

class testApp : public ofBaseApp, public ofxMidiListener {


public:
    void setup();
    void update();
    void draw();

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
    
    void tryToPlay(int note);
    void newMidiMessage(ofxMidiMessage& eventArgs);
	
	ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
    
    vector < bool > onNotes;
    
    
    
};
