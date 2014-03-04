//
//  pitchDetector.h
//  aubioSSTPD
//
//  Created by Jason Levine on 2014-01-13.
//
//

#include "aubio.h"
#include "ofMain.h"


//---------------------------------------------------------------------------------
class basePitchDetector {
public:
    virtual void calculatePitch(float * buffer, int bufferSize, int bufferSamplePos){}
    virtual float getPitch(){};
    basePitchDetector(){};
    
    string name;
    
};


//---------------------------------------------------------------------------------
class aubioPitchDetector : public basePitchDetector {

public:
    
    void setup(char_t * unit, char_t * method, uint_t _win_s, uint_t _hop_s);
    
    fvec_t * inputBuffer;
    //= new_fvec (hop_s); // input buffer
    
    
    virtual void calculatePitch(float * buffer, int bufferSize, int bufferSamplePos){
        
        // TODO: put this into a fvec_t;;;
//        if (inputBuffer == NULL){
//            inputBuffer = new_fvec (bufferSize);
//        }
        
        for (int i = 0; i < bufferSize; i++){
            inputBuffer->data[i] = buffer[i];
        }
        
        process_pitch(inputBuffer);
    }
    
    void process_pitch(fvec_t * in);
    float getPitch() { return pitchFound; }
    void cleanup();
    
    //pitch detection
    aubio_pitch_t *o;
    aubio_wavetable_t *wavetable;
    fvec_t *pitch;
    
    uint_t samplerate;
    float pitch_tolerance;
    float silence_threshold;
    char_t * pitch_unit;
    char_t * pitch_method;
    int blocks = 0;
    smpl_t pitchFound;

};


//---------------------------------------------------------------------------------
class filePitchDetector : public basePitchDetector {
public:
    
  
    
    typedef struct {
        float pos;
        float val;
    } timeVal;
    
    vector < timeVal > vals;
    float pitch;
    
    void  loadAssociatedFile( string fileName, string deliminator = ":" ){
        ofBuffer buf;
        buf = ofBufferFromFile(fileName);
        
        string s;
        while ( (s= buf.getNextLine()) != ""){
            vector < string > str = ofSplitString(s, deliminator);
            timeVal val;
            val.pos = ofToFloat(str[0]);
            val.val = ofToFloat(str[1]);
            
            //cout << val.pos << " " << val.val << endl;
            vals.push_back(val);
        }
        
//        name = "vamp";
        
    }
    
    float freq2MIDI(float freq) {
        // was int before
        //return (int)( 69. + 12.*log(freq/440.)/log((float)2.) );
        return ( 69. + 12.*log(freq/440.)/log((float)2.) );
    }

    
    void calculatePitch(float * buffer, int bufferSize, int bufferSamplePos){
        
        
        //cout << bufferSamplePos << " " << vals[vals.size()-1].pos <<  endl;
        
        //cout << vals.size() << endl;
        
        pitch = -1;
        for (int i = 0; i < vals.size()-1; i++){
            if (bufferSamplePos >= (int)vals[i].pos && bufferSamplePos <= (int)vals[i+1].pos){
                pitch = vals[i].val;
            }
        }
        
       
    }
    
    
    float  getPitch(){

        float ret = -1;
        if (pitch > 50){
            ret =  freq2MIDI(pitch);
        }
        
//        cout << ret << endl;
        return ret;
    }
    
    
    
};
