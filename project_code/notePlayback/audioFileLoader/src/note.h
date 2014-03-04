

#pragma once

#include "ofMain.h"
#include "aubioAnalyzer.h"
#include <sndfile.hh>

typedef struct {
    
    float freq;
    float confidence;
    float volume;
    
} analysisFrame;

class note {
   
public: 
    vector <analysisFrame> analysisFrames;
    vector <float> samples;
    
    
    
    int nChannels;
    int nFrames;
    
    int mostCommonNote;
    float mostCommonNotePct;
    
    string name;
    
//    std::vector<int> histogram(101,0);
//    for( int i=0; i<100; ++i )
//        ++histogram[ numVector[i] ];
//    return std::max_element( histogram.begin(), histogram.end() ) - histogram.begin();
//
    
    
    bool bPlaying;
    int playFrame = 0;
    
    void play(){
        cout << "playing " << name << endl;
        bPlaying = true;
        playFrame = 0;
    }
    
    void stop(){
        bPlaying = false;
        playFrame = 0;

    }
    
    void addToSoundBuffer(float * output, int bufferSize, int _nChannels){
        if (!bPlaying) return;
        
        float volume = sin((playFrame / (float)samples.size()) * PI);
        
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
        
    }
    
    
    
    //-------------------------------------------------------------------------------
    void process(){
        
        
        aubioAnalyzer AA;
        AA.setup();
        float values[256];
        float smoothVal = 0;
        
        int total = samples.size()/nChannels;
        
        for (int i =0; i < samples.size()/nChannels; i+=256){
            for (int j = 0; j < 256; j++){
                smoothVal = 0.7 * smoothVal + 0.3* samples[(i + j)*nChannels];
                values[j] = samples[(i + j)*nChannels];
            }
            AA.processAudio(values, 256);
            analysisFrame frame;
            frame.freq = AA.pitch;
            frame.confidence = AA.confidence;
            frame.volume = AA.amplitude;
            analysisFrames.push_back(frame);
        }
        findMostCommonPitch();
    }
    
    inline int freq2midi(float freq) {
        return (int)( 69 + 12*log(freq/440)/log((float)2) );
    }
    
    void findMostCommonPitch(){
        
        vector < int > notes;
        for (int i = 0; i < analysisFrames.size(); i++){
            float freq = analysisFrames[i].freq;
            
            if (freq > 0){
                int note = freq2midi(freq);
                if (note > 0 && note < 150 && analysisFrames[i].confidence > 0.3) notes.push_back(note);
            }
        }
        std::vector<int> histogram(150,0);
        for( int i=0; i<notes.size(); ++i )
            ++histogram[ notes[i] ];
        int maxElement =std::max_element( histogram.begin(), histogram.end() )  - histogram.begin();
        int maxCount =histogram[maxElement];
        float pct = (float)maxCount/ (float)notes.size();
        
        mostCommonNote = maxElement;
        mostCommonNotePct = pct;
        //cout << maxElement << " " << pct << endl;
        
    }
    
    
    
};