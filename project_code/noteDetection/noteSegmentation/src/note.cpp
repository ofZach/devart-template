//
//  note.cpp
//  audioFileLoader
//
//  Created by zach on 1/9/14.
//
//

#include "note.h"

bool bAubioSetup = false;
aubio_pitch_t *o;
aubio_wavetable_t *wavetable;
fvec_t *pitch;
fvec_t *ibuf;
fvec_t *obuf;
uint_t hop_size = 1024;
uint_t buffer_size = 2048;
uint_t samplerate = 44100;
float pitch_tolerance = 0;
float silence_threshold = -90.;
char_t * pitch_unit = "default";
char_t * pitch_method = "default";
int blocks = 0;
float pitchVal;
float confidenceVal;
float rmsVal;



// ---------------------------------------------------------------------------------------
void setupAubio(){
    bAubioSetup = true;
    buffer_size = 2048;
    ibuf = new_fvec (hop_size);
    obuf = new_fvec (hop_size);
    pitch_method = "yinfft";
    pitch_unit = "midi";
    
    o = new_aubio_pitch (pitch_method, buffer_size, hop_size, samplerate);
    if (pitch_tolerance != 0.)
        aubio_pitch_set_tolerance (o, pitch_tolerance);
    if (silence_threshold != -90.)
        aubio_pitch_set_silence (o, silence_threshold);
    if (pitch_unit != NULL)
        aubio_pitch_set_unit (o, pitch_unit);
    
    pitch = new_fvec (1);
}

// ---------------------------------------------------------------------------------------
void process_block(fvec_t * ibuf, fvec_t * obuf) {
    aubio_pitch_do (o, ibuf, pitch);
    pitchVal = fvec_get_sample(pitch, 0);
    confidenceVal = aubio_pitch_get_confidence(o);
    //cout << confidenceVal << endl;
    //        float rmsAmplitude  = 0;
    //        for (int i = 0; i < ibuf->length; i++){
    //            //calculate the root mean square amplitude
    //            rmsAmplitude += sqrt(ibuf->data[i]*ibuf->data[i]);
    //        }
    rmsVal = 0;///= (float)ibuf->length;
    
}



void note::play(){
    cout << "playing " << name << endl;
    bPlaying = true;
    playFrame = 0;
    volumeSmooth = 0;
}

void note::stop(){
    bPlaying = false;
    playFrame = 0;
    
}

void note::addToSoundBuffer(float * output, int bufferSize, int _nChannels){
    if (!bPlaying) return;
    
    float volume = sin((playFrame / (float)samples.size()) * PI);
    
    
    float pct = (float)playFrame / (float)samples.size();
    if (pct > 1) pct = 1;
    
    int which = (int)ofMap(pct, 0, 1, 0, allAnalysisFrames.size()-1);
    
    float val = allAnalysisFrames[which].freq;
    
    //cout << pct << " " << val << " " << mostCommonNote << endl;
    
    float dist = abs(val - mostCommonNote) / 6.0;
    
    
    volumeSmooth = 0.93f * volumeSmooth + 0.07 * ofMap(dist, 0, 2, 1,0.5, true) ;
    //cout << ofMap(dist, 0, 5, 1,0, true) << endl;
    //cout << dist << endl;
    
    
    if (playFrame < (samples.size() - bufferSize*2)){
        for (int i = 0; i < _nChannels; i++){
            for (int j = 0; j < bufferSize; j++){
                output[ j * _nChannels + i] += samples[(playFrame + j)*nChannels] * 0.3 * volumeSmooth;
            }
        }
        playFrame+=bufferSize;
    } else {
        bPlaying=false;
    }
    
}


//-------------------------------------------------------------------------------
void note::loadFile( string fileName ){
    
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
void note::process(){
    
    int processLen = hop_size;
    float values[processLen];
    float smoothVal = 0;
    
    if (!bAubioSetup) setupAubio();
    
   
    int total = (samples.size()-(processLen*nChannels));
    
    for (int i =0; i < (samples.size()-(processLen*nChannels)); i+=processLen*nChannels){
        
        for (int j = 0; j < processLen; j++){
            
//            if ( (i + j*nChannels) > samples.size() ){
//                //printf("(error! %i, (%i, %i), %i, %i)", nChannels, i, j, i + j*nChannels, samples.size());
//            }
            //cout << (i + j)*nChannels << endl;
            smoothVal = 0.9 * smoothVal + 0.1* samples[i + j*nChannels];
            //values[j] = samples[(i + j)*nChannels];
            ibuf->data[j] = smoothVal; //samples[i + j*nChannels];//smoothVal;//samples[(i + j)*nChannels];
        }
        process_block(ibuf, obuf);
        //            AA.processAudio(values, 256);
        analysisFrame frame;
        frame.freq = pitchVal;
        frame.confidence = confidenceVal;
        frame.volume = rmsVal;
        if (i > 0.2 * total && i < 0.8 * total)analysisFrames.push_back(frame);
        allAnalysisFrames.push_back(frame);
    }
    findMostCommonPitch();
}

inline int freq2midi(float freq) {
    return (int)( 69 + 12*log(freq/440)/log((float)2) );
}

void note::findMostCommonPitch(){
    
    vector < int > notes;
    for (int i = 0; i < analysisFrames.size(); i++){
        float freq = analysisFrames[i].freq;
        
        if (freq > 0){
            int note = freq; //freq2midi(freq);
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