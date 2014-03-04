//
//  audioUnitManager.cpp
//  noteSegmentation
//
//  Created by Jason Levine on 2014-01-16.
//
//

#include "ofMain.h"
#include "audioUnitManager.h"

void audioUnitManager::setup(string filename, int _bufferSize){
    bufferSize = _bufferSize;
    
    lpf = ofxAudioUnit(kAudioUnitType_Effect, kAudioUnitSubType_LowPassFilter);
    lpf.setParameter(kLowPassParam_CutoffFrequency, kAudioUnitScope_Global, 20000);
    
    player.setFile(ofToDataPath(filename)); //Marc Terenzi - Love To Be Loved By You [692].mp3
    
    tap.setBufferLength(bufferSize);
    
    sampler = ofxAudioUnitSampler('aumu', 'dls ', 'appl');
    sampler.setBank(0, 0);
    sampler.setProgram(0);
    
    mixer.setInputBusCount(2);
    mixer.setInputVolume(0.0, 0);
    mixer.setInputVolume(0.2, 1);
    
    player.connectTo(lpf).connectTo(tap).connectTo(mixer, 0);
    sampler.connectTo(mixer, 1);
    mixer.connectTo(output);
    
    output.start();
    
}

void audioUnitManager::playFile() {
    player.play();
}

void audioUnitManager::getTapSamples(float * samples){
    vector<float> tapSamples;
    tap.getSamples(tapSamples);
//    float samples[bufferSize];
    
    //pitch detection
    if (tapSamples.size() > 0) {
        for (int i = 0; i < bufferSize; i++){
            samples[i] = tapSamples[i];
        }
    }
}


void audioUnitManager::startNote(int midiNote){
    sampler.midiNoteOn(midiNote, 127);
}

void audioUnitManager::stopNote(int midiNote){
    sampler.midiNoteOff(midiNote, 127);
}