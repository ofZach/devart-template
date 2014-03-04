

#include "ofMain.h"

#pragma once

float freq2midi(float freq);
int findMostCommon (vector < int > & vals);
string getAudioDirectory();

void loadAudioToData(string fileName, vector < float > & audioSamples);
void saveDataToAudio(string fileName, vector < float > & audioSamples);
string zeroPadNumber(int num, int sizeToBePaddedTo);

struct metadata {
    int preContext;
    int postContext;
    
    float pctMostCommon;
    
    float meloStdDev;
    float yinStdDev;
    float yinFFTStdDev;
    
    float yinAgree;
    float yinFFTAgree;
    
    float meloKurtosis;
};

void saveMetaDataToFile(string fileName, metadata noteMetadata);


struct marker {
    float start;
    float end;
};

struct note {
    
    int startTime;
    int endTime;
    
    int mostCommonPitch;
    
    bool bPlaying;
    int playbackTime;

};


