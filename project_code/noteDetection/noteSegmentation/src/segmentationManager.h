//
//  segmentationManager.h
//  noteSegmentation
//
//  Created by zach on 1/17/14.
//
//

#include "ofMain.h"
#include "pitchDetectorManager.h"
#include "PDCompare.h"
#include "utils.h"

#pragma once


class segmentationManager {
    
public: 
    
    void setup( int numPitchDetectors, int _bufferSize );
    void update(float * samples, int sampleTime);
    void draw();
    
    void calcPDStats(int start, int end);
    void calcPDAgreement(int start, int end);
    
    void scrollMarkers();
    
    float findMostCommonPitch();
    
    int bufferSize;
    
    pitchDetectorManager * PDM;
    PDCompare * PDC;
    
    int minPitch;
    bool bBelowMinPitch;
    
    bool bVelFine;
    
    //markers
    vector<marker> markers;
    bool drawMarkers;

    float coarseThreshold, fineThreshold;
    float minDuration;
    float noteRun;
    
    bool bAmRecording;
    
    //notes
    vector<float> analysisFrames;
    int nFramesRecording;
    
    note currentNote;
    metadata currentMetadata;
    
    
    bool bDebug;
    
};