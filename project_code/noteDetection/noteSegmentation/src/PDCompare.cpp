//
//  PDCompare.cpp
//  noteSegmentation
//
//  Created by Jason Levine on 2014-01-21.
//
//

#include "PDCompare.h"
#include "testApp.h"
#include "computeStats.h"


void PDCompare::setup(pitchDetectorManager * _PDM, int _bufferSize ) {
    PDM = _PDM;
    bufferSize = _bufferSize;
    nFrames = 25;
    
    
    scrollingGraph noteFoundGraph;
    noteFoundGraph.setup(PDM->graphWidth, 0, 0, 1.0);
    for (int i = 0; i < PDM->size(); i++) {
        noteFound.push_back(noteFoundGraph);
    }
    
    agreedNotes.assign(PDM->graphWidth, 0.0);
    means.assign(PDM->size(), 0.0);
    stdDevs.assign(PDM->size(), 0.0);
    
    stdDevThresh = 5;
    
    sum = false;
    
    minDuration = 25;
    minPitch = 20;
    nFramesRecording = 0;
}

void PDCompare::update(float * samples, int sampleTime){
    //clear stats
    means.assign(PDM->size(), 0.0);
    stdDevs.assign(PDM->size(), 0.0);
    
    for (int i = 0; i < PDM->size(); i++) {
        
        //calculate stats
        for (int j = PDM->graphWidth - nFrames; j < PDM->graphWidth; j++) {
            means[i]+=PDM->medianGraphs[i].valHistory[j];
        }
        means[i] /= nFrames;
        stdDevs[i] = computeStdDev(PDM->medianGraphs[i].valHistory.end()-nFrames, PDM->medianGraphs[i].valHistory.end(), means[i]);
        
        //if we are in a note go back nframes and make them all "in a note"
        int isNote = (stdDevs[i] < stdDevThresh && means[i] > 0) ? 1 : 0;
        noteFound[i].addValue(isNote);
        if (isNote == 1) {
            for (int j = nFrames; j > 0; j--) {
                noteFound[i].valHistory[PDM->graphWidth - j] = 1;
            }
        }
        
    }
    
    //find the intersection of PDs
    for (int i = 0; i < PDM->graphWidth; i++) {
        int agreeCount = 0;
        for (int j = 0; j < noteFound.size(); j++) {
            agreeCount += noteFound[j].valHistory[i];
        }
        agreedNotes[i] = (agreeCount == noteFound.size() ? 1 : 0);
    }
    
    // count how many frames in a row the vel is below the threshold
    if ( noteFound[2].getLast() == 1) { //agreedNotes[PDM->graphWidth-1] == 1
        //count buffers
        noteRun++;
        
        // record samples
//        for (int i = 0; i < bufferSize; i++ ) {
//            currentNote.samples.push_back(samples[i]);
//        }
        
        if (nFramesRecording == 0){
            currentNote.startTime = sampleTime;
        }
        
        currentNote.endTime = sampleTime + bufferSize;
        nFramesRecording++;
        
        //record pitches
        //pitchesForRecording.push_back(medianGraphs[PDM->PDMethod].getLast());
        analysisFrames.push_back(PDM->medianGraphs[2].getLast());
        
    }
    else  {
        
        /*
        // if the vel is above the thresh then check if the current run is longer than the min duration. If so save the note.  Regardless, set the run count to zero.
        if ( noteRun > minDuration) {
//            cout << "note duration = " << noteRun << endl;
            marker segment;
            segment.start = PDM->graphWidth - noteRun;
            segment.end = PDM->graphWidth;
            
            float avg = 0;
            for (int i = 0; i < analysisFrames.size(); i++){
                avg += analysisFrames[i];
            }
            avg /= (MAX(1.0, analysisFrames.size()));
            
            //            pitchesForRecording.clear();
            // zero periods look like 9, 10... etc...
            if (avg > minPitch){
                //add markers
                markers.push_back(segment);
                //add correspnding note
                currentNote.mostCommonPitch = findMostCommonPitch();
                
                
                
                float duration = (currentNote.endTime - currentNote.startTime ) / 44100. ;
                // sometimes, when we wrap over a loop, bad stuff happens, let's be careful:
                if (duration > 0 && currentNote.mostCommonPitch > 0){
                  //turned note saving off so we can run both segmanagers together.
//                    
//                    ((testApp *) ofGetAppPtr()) -> addNote(currentNote.startTime - nFrames * bufferSize, currentNote.endTime, currentNote.mostCommonPitch);
                    
                    cout << "note recorded - min duration = " << currentNote.startTime << endl << endl;
                }
                
            }
            
        }*/
        //reset
        noteRun = nFrames;
//        currentNote.samples.clear();
        analysisFrames.clear();
        nFramesRecording = 0;
        currentNote.startTime = 0;
        currentNote.endTime = 0;
    }


    if (markers.size() > 0) {
        for (int i = 0; i < markers.size(); i++) {
            markers[i].start--;
            markers[i].end--;
        }
    }

}


void PDCompare::draw(){
    
    int height = ofGetHeight() / (PDM->size()+1);
    for (int i = 0; i < PDM->size(); i++) {
        ofPushMatrix();
        ofTranslate(0, i * height);
        
        //graphs
        ofSetColor(255,0,0);
        PDM->medianGraphs[i].draw(height);
        
        ofSetColor(25,200,25,50);
        noteFound[i].draw(height, 0, 1, true);
        
        
        //mean
        float meanScaled = ofMap(means[i], 0, PDM->graphMax, height, 0, true);
        ofSetColor(0);
        ofLine(ofGetWidth() - nFrames * 2, meanScaled, ofGetWidth(), meanScaled);
        
        //stdDEv
        float stdDevScaled = ofMap(stdDevs[i], 0, PDM->graphMax, 0, height, true);
        (stdDevs[i] < stdDevThresh) ? ofSetColor(25,200,25,100) : ofSetColor(200,25,25,100);
        ofRect(ofGetWidth() - nFrames * 2, meanScaled - stdDevScaled, ofGetWidth(), stdDevScaled * 2);
        
        
        ofSetColor(0);
        ofDrawBitmapStringHighlight(PDM->pitchDetectors[i]->name, ofPoint(ofGetWidth()/2, height / 2));
        
        ofPopMatrix();
        
        if(sum) {
            ofPushMatrix();
            ofTranslate(0, (PDM->size()) * height);
            ofSetColor(25,200,25,100);
            noteFound[i].draw(height, 0, 1, true);
            ofPopMatrix();
        }

        
    }
    
    if (!sum) {
        ofPushMatrix();
        ofTranslate(0, (PDM->size()) * height);
        ofSetColor(25,200,25,200);
        for (int i = 0; i < PDM->graphWidth; i++) {
            ofRect(i*2, 0, 2, agreedNotes[i] * height);
        }
        ofPopMatrix();
    }

    
    
    
    
    ofSetColor(255,255,255,100);
    ofRect(ofGetWidth() - nFrames * 2, 0, nFrames * 2, ofGetHeight());
    
    
    for (int i = 0; i < markers.size(); i++) {
        ofSetColor(255,255,255,127);
        ofRect(markers[i].start * 2, 0, markers[i].end * 2 - markers[i].start * 2, ofGetHeight());
    }

}

float PDCompare::findMostCommonPitch(){
    
    vector < int > properPitches;
    
    for (int i = 0; i < analysisFrames.size(); i++){
        float detectedPitch = analysisFrames[i];
        if (detectedPitch > minPitch && detectedPitch < 150) properPitches.push_back(detectedPitch);
    }
    // see utils.h
    
    int mostCommon = findMostCommon(properPitches);
    
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
    
    float pct = (float)count / (float)(MAX(1, properPitches.size()));
    
    
    if (pct < 0.35){
        return -1;
    } else
        
        return (int)mostCommon;
    
}




