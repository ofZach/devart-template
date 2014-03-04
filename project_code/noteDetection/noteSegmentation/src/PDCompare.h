//
//  PDCompare.h
//  noteSegmentation
//
//  Created by Jason Levine on 2014-01-21.
//
//

#pragma once

#include "scrollingGraph.h"
#include "smoother.h"
#include "pitchDetectorManager.h"
#include "utils.h"


class PDCompare {
public:
    void setup(pitchDetectorManager * _PDM, int _bufferSize);
    void update(float * samples, int sampleTime);
    void draw();
    
    float findMostCommonPitch();
    
    int nFrames;
    int bufferSize;
    
    pitchDetectorManager * PDM;

    vector<scrollingGraph> noteFound;
    vector<float> agreedNotes;
    vector<float> means;
    vector<float> stdDevs;

    
    float stdDevThresh;
    
    bool sum;
    
    //from segMan
    float minDuration;
    float noteRun;
    
    bool bAmRecording;
    vector < float > pitchesForRecording;
    
    //notes
    vector<float> analysisFrames;
    int nFramesRecording;
    
    note currentNote;
    
    int minPitch;
    vector<marker> markers;
};
