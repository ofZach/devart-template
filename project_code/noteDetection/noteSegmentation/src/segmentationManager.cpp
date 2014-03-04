//
//  segmentationManager.cpp
//  noteSegmentation
//
//  Created by zach on 1/17/14.
//
//

#include "segmentationManager.h"
#include "testApp.h"
#include "computeStats.h"

void segmentationManager::setup( int numPitchDetectors, int _bufferSize ){
    
    bufferSize = _bufferSize;
    
    coarseThreshold = fineThreshold = 1.0;
    
    minDuration = 10;
    drawMarkers = true;
    bAmRecording = false;
    minPitch = 20;
    
    nFramesRecording = 0;
    
    bDebug = false;
}

void segmentationManager::update(float * samples, int sampleTime){
//    cout << sampleTime << " : " << PDM->pitchDetectors[PDM->PDMethod]->getPitch() << endl;
     // count how many frames in a row the vel is below the threshold
    if ( PDM->velGraphs[PDM->PDMethod].getLast() < (bVelFine ? fineThreshold : coarseThreshold)) {
        //count buffers
        noteRun++;
        
        if (nFramesRecording == 0){
            currentNote.startTime = sampleTime;
        }
        
        currentNote.endTime = sampleTime + bufferSize;
        nFramesRecording++;
        
        //record pitches
        analysisFrames.push_back(PDM->medianGraphs[PDM->PDMethod].getLast());

    }
    else  {
        
        // if the vel is above the thresh then check if the current run is longer than the min duration. If so save the note.  Regardless, set the run count to zero.
        if ( noteRun > minDuration) {
            
            float avg = 0;
            for (int i = 0; i < analysisFrames.size(); i++){
                avg += analysisFrames[i];
            }
            avg /= (MAX(1.0, analysisFrames.size()));
            
            // zero periods look like 9, 10... etc...
            if (avg > minPitch){
                
                float duration = (currentNote.endTime - currentNote.startTime ) / 44100. ;
                
                currentNote.mostCommonPitch = findMostCommonPitch();
                // sometimes, when we wrap over a loop, bad stuff happens, let's be careful:
                if (duration > 0 && currentNote.mostCommonPitch > 0){
                    
                    //add markers
                    marker segment;
                    segment.start = PDM->graphWidth - 1 - noteRun;
                    segment.end = PDM->graphWidth - 1;
                    markers.push_back(segment);
                    
                    //note metadata
                    calcPDStats(segment.start, segment.end);
                    calcPDAgreement(segment.start, segment.end);
                    
                    ((testApp *) ofGetAppPtr()) -> addNote(currentNote, currentMetadata);
                    if (bDebug) cout << "note recorded: " << currentNote.startTime << " " << currentNote.mostCommonPitch << endl;
                }
                
            }
            else {
                if (bDebug) cout << "note not recorded - minPitch > avg" << endl << endl;
            }
        }
        //reset
        noteRun = 0;
        analysisFrames.clear();
        nFramesRecording = 0;
        currentNote.startTime = 0;
        currentNote.endTime = 0;
    }

    scrollMarkers();

}

void segmentationManager::draw(){
    
    //min duration measure
    ofSetColor(25);
    ofLine(ofGetWidth() - minDuration * 2, 90, ofGetWidth(), 90);
    ofLine(ofGetWidth() - minDuration * 2, 92, ofGetWidth() - minDuration * 2, 88);
    
    //current run
    (noteRun < minDuration) ? ofSetColor(255,0,0) : ofSetColor(0,255,0);
    ofLine(ofGetWidth() - noteRun * 2, 95, ofGetWidth(), 95);
    
    //median graph of chosen PD
    ofPushMatrix();
    ofTranslate(0, 100);
    ofSetColor(255, 10, 10);
    
    PDM->medianGraphs[ PDM->getMethod() ].draw(PDM->graphHeight);

    //markers
    for (int i = 0; i < markers.size(); i++) {
        ofSetColor(255,255,255,127);
        ofRect(markers[i].start * 2, 0, markers[i].end * 2 - markers[i].start * 2, PDM->graphHeight);
    }

    ofPopMatrix();
    
    //velocity graphs
    ofPushMatrix();
    ofTranslate(0, PDM->graphHeight + 100);
    ofSetColor(255,0,0);

    if (bVelFine) {
        PDM->velGraphs[ PDM->getMethod() ].draw(PDM->graphHeight, 0.0, 2.0);
    }
    else {
        PDM->velGraphs[ PDM->getMethod() ].draw(PDM->graphHeight);
    }
    
    //thresh line
    ofSetColor(0);
    float threshLineHeight = PDM->graphHeight - ((bVelFine ? fineThreshold/2*PDM->graphMax : coarseThreshold ) / PDM->graphMax * PDM->graphHeight);
    ofLine(0, threshLineHeight, ofGetWidth(), threshLineHeight);
    
    
    //cut lines
    ofSetColor(0,0,0,127);
    for (int j = 0; j < PDM->velGraphs[ PDM->getMethod() ].valHistory.size(); j++) {
        if ( PDM->velGraphs[ PDM->getMethod() ].valHistory[j] > (bVelFine ? fineThreshold : coarseThreshold ) ) {
            ofLine(j * 2, -PDM->graphHeight, j * 2, PDM->graphHeight);
        }
    }

    ofPopMatrix();
}



void segmentationManager::scrollMarkers(){

    if (markers.size() > 0) {
        for (int i = 0; i < markers.size(); i++) {
            markers[i].start--;
            markers[i].end--;
        }
    }
}


void segmentationManager::calcPDStats(int start, int end) {
    vector<float> yinValues, yinFFTValues, meloValues;
    float yinMean = 0;
    float yinFFTMean = 0;
    float meloMean = 0;
    
    for (int i = start; i < end; i++) {
        yinValues.push_back(PDM->medianGraphs[0].valHistory[i]);
        yinMean += PDM->medianGraphs[0].valHistory[i];
        
        yinFFTValues.push_back(PDM->medianGraphs[1].valHistory[i]);
        yinFFTMean += PDM->medianGraphs[1].valHistory[i];
        
        meloValues.push_back(PDM->medianGraphs[2].valHistory[i]);
        meloMean += PDM->medianGraphs[2].valHistory[i];
    }
    yinMean /= yinValues.size();
    yinFFTMean /= yinFFTValues.size();
    meloMean /= meloValues.size();
    
    currentMetadata.yinStdDev = computeStdDev(yinValues.begin(), yinValues.end(), yinMean);
    currentMetadata.yinFFTStdDev = computeStdDev(yinFFTValues.begin(), yinFFTValues.end(), yinFFTMean);
    currentMetadata.meloStdDev = computeStdDev(meloValues.begin(), meloValues.end(), meloMean);
    currentMetadata.meloKurtosis = computeKurtosisExcess(meloValues.begin(), meloValues.end(), meloMean);
    
//    cout << "yin stddev " << currentMetadata.yinStdDev << " yinFFT stddev " << currentMetadata.yinFFTStdDev << " melo stddev " << currentMetadata.meloStdDev << " melo kurtosis " << currentMetadata.meloKurtosis <<endl;

    
    //A flatter distribution has a negative kurtosis, A distribution more peaked than a Gaussian distribution has a positive kurtosis.
}

void segmentationManager::calcPDAgreement(int start, int end) {
    float yinAgreement = 0;
    float yinFFTAgreement = 0;
    
    for (int i = start; i < end; i++) {
        yinAgreement += PDC->noteFound[0].valHistory[i];
        yinFFTAgreement += PDC->noteFound[1].valHistory[i];
    }
    
    yinAgreement /= (end-start);
    yinFFTAgreement /= (end-start);
    
    currentMetadata.yinAgree = yinAgreement;
    currentMetadata.yinFFTAgree = yinFFTAgreement;

}



float segmentationManager::findMostCommonPitch(){
    
    vector < int > properPitches;
    
    for (int i = 0; i < analysisFrames.size(); i++){
        float detectedPitch = analysisFrames[i];
        if (detectedPitch > minPitch && detectedPitch < 150) properPitches.push_back(detectedPitch);
    }
    // see utils.h
    
    int mostCommon =findMostCommon(properPitches);
    
    //cout << "-----------" << endl;
    float avg = 0;
    for (int i = 0; i < properPitches.size(); i++){
      //  cout << properPitches[i] << endl;
        avg +=properPitches[i];
    }
    avg /= (float)properPitches.size();
    
    //cout << avg << " " << mostCommon << endl;
    int count = 0;
    for (int i = 0; i < properPitches.size(); i++){
        if (properPitches[i] == mostCommon){
            count ++;
        }
    }
    
    currentMetadata.pctMostCommon = (float)count / (float)(MAX(1, properPitches.size()));
    
    if (currentMetadata.pctMostCommon < 0.35){
        return -1;
    } else
    
    return (int)mostCommon;
    
}
