//
//  pitchDetectorsManager.cpp
//  noteSegmentation
//
//  Created by zach on 1/17/14.
//
//



#include "pitchDetectorManager.h"
#include "utils.h"




void pitchDetectorManager::setup(int winSize, int hopSize){

    numAPDs = 2;
    
    for (int i = 0; i < numAPDs; i++) {
        pitchDetectors.push_back(new aubioPitchDetector);
    }
    
    methods.push_back("yin");
    methods.push_back("yinfft");
//    methods.push_back("specacf");
//    methods.push_back("schmitt");
//    methods.push_back("mcomb");
//    methods.push_back("fcomb");
    
    for (int i = 0; i < numAPDs; i++) {
        aubioPitchDetector * APD = static_cast<aubioPitchDetector*>(pitchDetectors[i]);
        APD->setup("midi", methods[i], winSize, hopSize);
    }
    

    
    melo = new filePitchDetector();
    melo->name = "melodia";
    ((filePitchDetector*)melo)->loadAssociatedFile(getAudioDirectory() + "pop.vals.txt");
    pitchDetectors.push_back(melo);

    
    nPitchDetectors = pitchDetectors.size();
    
    PDMethod = 2;
    
    
    smoother tempSmoother;
    tempSmoother.setNumPValues(11);
    for (int i = 0; i < pitchDetectors.size(); i++) {
        smoothers.push_back(tempSmoother);
    }
    
    graphWidth = ofGetWidth()/2;
    graphHeight = ofGetHeight()/3;
    graphMax = 120;
    
    scrollingGraph tempGraph;
    tempGraph.setup(graphWidth, 0, 0, graphMax);
    for (int i = 0; i < pitchDetectors.size(); i++) {
        pitchGraphs.push_back(tempGraph);
        medianGraphs.push_back(tempGraph);
        velGraphs.push_back(tempGraph);
    }
    
    
    for (int i = 0; i < pitchDetectors.size(); i++) {
        ofColor tempColor;
        tempColor.setHsb(i*40, 255, 180, 200);
        graphColors.push_back(tempColor);
    }

}


void pitchDetectorManager::processPitchDetectors(float * buffer, int bufferSize, int sampleTime){
    
    for (int i = 0; i < pitchDetectors.size(); i++) {
        pitchDetectors[i]->calculatePitch(buffer, bufferSize, sampleTime);
    }
    
}
float pitchDetectorManager::getPitch(){
    return pitchDetectors[PDMethod]->getPitch();
}

float pitchDetectorManager::getPitch(int whichOne){
     return pitchDetectors[whichOne]->getPitch();
    
}

void pitchDetectorManager::updateGraphs(){
    for (int i = 0; i < pitchDetectors.size(); i++) {
        //pitchGraphs[i].addValue(PDM->getPitch(i));
        smoothers[i].addValue(getPitch(i));
        medianGraphs[i].addValue(smoothers[i].getMedian());
        
        
        float lastVal = medianGraphs[i].valHistory[medianGraphs[i].valHistory.size() - 1];
        float secondLastVal = medianGraphs[i].valHistory[medianGraphs[i].valHistory.size() - 2];
        float vel = abs(lastVal - secondLastVal);
        velGraphs[i].addValue(vel);
    }
}

void pitchDetectorManager::draw(bool bVelFine){
     ofDrawBitmapStringHighlight(pitchDetectors[PDMethod]->name, ofPoint(300,50));
    
//    ofPushMatrix();
//    ofTranslate(0, 100);

//    ofSetColor(255, 10, 10);
//    medianGraphs[PDMethod].draw(graphHeight);
//
//    ofPopMatrix();
//    
//    ofPushMatrix();
//    ofTranslate(0, graphHeight + 100);
//    ofSetColor(255,0,0);
//    
//    if (bVelFine) {
//        velGraphs[PDMethod].draw(graphHeight, 0.0, 2.0);
//    }
//    else {
//        velGraphs[PDMethod].draw(graphHeight);
//    }
//    
//
//    ofSetColor(0);
//    float threshLineHeight = graphHeight - ((bVelFine ? fineThreshold/2*graphMax : coarseThreshold ) / graphMax * graphHeight);
//    ofLine(0, threshLineHeight, ofGetWidth(), threshLineHeight);
//    
//    ofSetColor(0,0,0,127);
//    for (int j = 0; j < velGraphs[ PDM->getMethod() ].valHistory.size(); j++) {
//        if ( velGraphs[ PDM->getMethod() ].valHistory[j] > (bVelFine ? fineThreshold : coarseThreshold ) ) {
//            ofLine(j * 2, -graphHeight, j * 2, graphHeight);
//        }
//    }
//
//    
//    ofPopMatrix();

}


