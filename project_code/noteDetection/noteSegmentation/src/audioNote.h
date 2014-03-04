//
//  audioNote.h
//  noteSegmentation
//
//  Created by Jason Levine on 2014-01-22.
//
//

#pragma once

#include "ofMain.h"

class audioNote {

    
    void findMostCommonPitch(int minPitch);
    
    float startFrame;
    float endFrame;
    
    bool bPlaying;
    bool bWasPlaying;
    
    vector < float > samples;
    vector < float > analysisFrames;
    
    int mostCommonPitch;
    float pctMostCommonPitch;
    int playhead;
    
    int nFramesRecording;
    int startTime;
    int endTime;
};



//struct marker {
//    float start;
//    float end;
//};
//
//struct audioNote {
//    
//    bool bPlaying;
//    bool bWasPlaying;
//    
//    vector < float > samples;
//    vector < float > analysisFrames;
//    int mostCommonPitch;
//    int playhead;
//    
//    int nFramesRecording;
//    int startTime;
//    int endTime;
//    
//};
