//
//  audioNote.cpp
//  noteSegmentation
//
//  Created by Jason Levine on 2014-01-22.
//
//

#include "audioNote.h"
#include "utils.h"

void audioNote::findMostCommonPitch(int minPitch){
    
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
    
    pctMostCommonPitch = (float)count / (float)(MAX(1, properPitches.size()));
    
    
    if (pctMostCommonPitch > 0.35){
        mostCommonPitch = mostCommon;
    }
    else {
        mostCommonPitch = -1;
    }
    
    
}
