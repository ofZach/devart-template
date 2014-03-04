

#pragma once

#include "ofMain.h"
#include <sndfile.hh>
#include "aubio.h"


typedef struct {
    
    float freq;
    float confidence;
    float volume;
    
} analysisFrame;



class note {
   
public:
    
    
    vector <analysisFrame> allAnalysisFrames;
    vector <analysisFrame> analysisFrames;
    vector <float> samples;
    
    
    // ---------------------------------------------------------------------------------------
    int nChannels;
    int nFrames;
    
    int mostCommonNote;
    float mostCommonNotePct;
    
    string name;
    bool bPlaying;
    int playFrame = 0;
    
    
    void play();
    void stop();
    
    void findMostCommonPitch();
        //-------------------------------------------------------------------------------
    void process();
            //-------------------------------------------------------------------------------
    void loadFile( string fileName );
    void addToSoundBuffer(float * output, int bufferSize, int _nChannels);
    
    float volumeSmooth;
    
    
    
    
    
};