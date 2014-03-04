

#pragma once

#include "ofMain.h"
#include <sndfile.hh>


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


class note {
   
public: 
    
    
    
    vector <float> samples;
    
    int nChannels;
    int nFrames;
    int mostCommonNote;
    
    string fileName;
    
    metadata MD;
    
    string name;
    
    bool bPlaying;
    int playFrame = 0;
    
    void play(){
        cout << "playing " << name << endl;
        cout << "preContext " << MD.preContext << endl
        << "postContext " << MD.postContext << endl
        << "pctMostCommon " << MD.pctMostCommon << endl
        << "meloStdDev " << MD.meloStdDev << endl
        << "yinStdDev " << MD.yinStdDev << endl
        << "yinFFTStdDev " << MD.yinFFTStdDev << endl
        << "yinAgree " << MD.yinAgree << endl
        << "yinFFTAgree " << MD.yinFFTAgree << endl
        << "meloKurtosis " << MD.meloKurtosis << endl << endl;
        
        bPlaying = true;
        playFrame = 44100 * 0.5;
        
        reallyLoadSound();
    }
    
    void stop(){
        
        reallyUnloadSound();
        bPlaying = false;
        playFrame = 44100 * 0.5;
    }
    
    void reallyLoadSound(){
        
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

    }
    
    void reallyUnloadSound(){
        
        samples.clear();
        
    }
    
    void addToSoundBuffer(float * output, int bufferSize, int _nChannels){
        if (!bPlaying) return;
        if (samples.size() == 0) return;
        
        float volume = sin(((playFrame - 44100*0.5)/ (float)(samples.size()-44100)) * PI);
        
        volume = powf(volume, 0.3);
        if (playFrame < (samples.size() - bufferSize - 44100* 1.0)){
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
    void loadFile( string _fileName ){
        
        fileName = _fileName;
        name = fileName;
        bPlaying = false;
        
        string fileNamePart = fileName.substr(fileName.find_last_of("/") + 1);
        string notePart = fileNamePart.substr(fileNamePart.find_last_of("(") + 1,2);
        
        name = fileNamePart;
        mostCommonNote = ofToInt(notePart);
        
        //jason added for loading metadata
        string metadataFilename = fileName.substr(0, fileName.find_last_of("."));
        metadataFilename += ".txt";
//        cout << metadataFilename << endl;

        ofFile metadataFile(metadataFilename);
        ofBuffer buffer(metadataFile);
        
        //Read file line by line
        while (!buffer.isLastLine()) {
            string line = buffer.getNextLine();
            
            //Split line into strings
            vector<string> data = ofSplitString(line, ":");
            
            if (data[0] == "preContext") MD.preContext = ofToInt(data[1]);
            if (data[0] == "postContext") MD.postContext = ofToInt(data[1]);
            if (data[0] == "pctMostCommon") MD.pctMostCommon = ofToFloat(data[1]);
            if (data[0] == "meloStdDev") MD.meloStdDev = ofToFloat(data[1]);
            if (data[0] == "yinStdDev") MD.yinStdDev = ofToFloat(data[1]);
            if (data[0] == "yinFFTStdDev") MD.yinFFTStdDev = ofToFloat(data[1]);
            if (data[0] == "yinAgree") MD.yinAgree = ofToFloat(data[1]);
            if (data[0] == "yinFFTAgree") MD.yinFFTAgree = ofToFloat(data[1]);
            if (data[0] == "meloKurtosis") MD.meloKurtosis = ofToFloat(data[1]);
            
        }
        
        playFrame = 44100*1.0;
        

    }
    
};