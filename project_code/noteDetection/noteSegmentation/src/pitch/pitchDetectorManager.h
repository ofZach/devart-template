//
//  pitchDetectorsManager.h
//  noteSegmentation
//
//  Created by zach on 1/17/14.
//
//
#pragma once

#include "ofMain.h"
#include "pitchDetector.h"
#include "scrollingGraph.h"
#include "smoother.h"

// I hold the pitch detectors.  I process buffers and split out pitch

class pitchDetectorManager {

public:
    int numAPDs;
    int PDMethod;
    int nPitchDetectors;
    
    vector<char_t *> methods;
    vector<basePitchDetector*> pitchDetectors;


    void setup(int winSize, int hopSize);
    void updateGraphs();
    void processPitchDetectors(float * buffer, int bufferSize, int sampleTime);   //samples, bufferSize, au.getSampleTime()
    float getPitch();
    float getPitch(int whichOne);
    
    void draw(bool bVelFine);
    
    void setPitchMethod(int whichMethod){
        PDMethod = whichMethod;
    }
    
    int getMethod(){
        return PDMethod;
    }
    // maybe I'll rename this?
    int size(){
        return nPitchDetectors;
    }
    
    filePitchDetector * melo;
    filePitchDetector * pyin;
    
    vector<smoother> smoothers;
    vector<scrollingGraph> pitchGraphs, medianGraphs, velGraphs;
    vector<ofColor> graphColors;
    float graphWidth;
    float graphMax;
    float graphHeight;

};

