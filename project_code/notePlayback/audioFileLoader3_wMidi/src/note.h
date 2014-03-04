

#pragma once

#include "ofMain.h"
#include <sndfile.hh>

class note {
   
public: 
    
    
    
    vector <float> samples;
    
    int nChannels;
    int nFrames;
    int mostCommonNote;
    
    string name;
    
    bool bPlaying;
    int playFrame = 0;
    
    
    
    void play(){
        //cout << "playing " << name << endl;
        bPlaying = true;
        playFrame = 0;
    }
    
    void reallyLoadFile(){
        
        
    }
    
    void reallyUnloadFile(){

        
    }
    
    void stop(){
        bPlaying = false;
        playFrame = 0;

    }
    
    void addToSoundBuffer(float * output, int bufferSize, int _nChannels){
        if (!bPlaying) return;
        
        float volume = sin((playFrame / (float)samples.size()) * PI);
        
        volume = powf(volume, 0.3);
        if (playFrame < (samples.size() - bufferSize)){
            for (int i = 0; i < _nChannels; i++){
                for (int j = 0; j < bufferSize; j++){
                    output[ j * _nChannels + i] += samples[(playFrame + j)*nChannels] * 0.3 * volume;
                }
            }
            playFrame+=bufferSize;
        } else {
            bPlaying=false;
        }
        
    }
    
    
    //-------------------------------------------------------------------------------
    void loadFile( string fileName ){
        
        
        name = fileName;
        bPlaying = false;
        
        SndfileHandle myf = SndfileHandle( ofToDataPath(fileName).c_str());
        printf ("Opened file '%s'\n", ofToDataPath(fileName).c_str()) ;
        printf ("  Sample rate : %d\n", myf.samplerate ()) ;
        printf ("  Channels  : %d\n", myf.channels ()) ;
        printf ("  Error   : %s\n", myf.strError());
        printf ("  Frames   : %d\n", int(myf.frames())); // frames is essentially samples
        puts("");
        nChannels = myf.channels();
        nFrames = (int) myf.frames();
        
        samples.resize(myf.frames()*myf.channels());
        myf.read(&samples[0], myf.frames()*myf.channels());
        
        
        string fileNamePart = fileName.substr(fileName.find_last_of("/") + 1);
        string notePart = fileNamePart.substr(fileNamePart.find_last_of("(") + 1,2);
        
        name = fileNamePart;
        mostCommonNote = ofToInt(notePart);
        
    }
    
    
  
    
    
    
};