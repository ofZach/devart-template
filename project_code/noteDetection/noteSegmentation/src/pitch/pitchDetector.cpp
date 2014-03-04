//
//  pitchDetector.cpp
//  aubioSSTPD
//
//  Created by Jason Levine on 2014-01-13.
//
//

#include "pitchDetector.h"
#include "ofMain.h"

void aubioPitchDetector::setup(char_t * unit, char_t * method, uint_t _win_s, uint_t _hop_s) {
    
    
    samplerate = 44100;
    uint_t win_s = _win_s; // window size
    uint_t hop_s = _hop_s;  // hop size
    
    inputBuffer = new_fvec (_hop_s);
    
    pitch_tolerance = 0;
    silence_threshold = -90.;
    pitch_unit = unit;
    pitch_method = method;
    blocks = 0;
    
    o = new_aubio_pitch (pitch_method, win_s, hop_s, samplerate);
    if (pitch_tolerance != 0.)
        aubio_pitch_set_tolerance (o, pitch_tolerance);
    if (silence_threshold != -90.)
        aubio_pitch_set_silence (o, silence_threshold);
    if (pitch_unit != NULL)
        aubio_pitch_set_unit (o, pitch_unit);
    
    pitch = new_fvec (1);
    
    
    name = string(method);


}


void aubioPitchDetector::process_pitch(fvec_t * in) {
    
    aubio_pitch_do (o, in, pitch);
    pitchFound = fvec_get_sample(pitch, 0);
    
}

void aubioPitchDetector::cleanup() {
    del_aubio_pitch (o);
    del_fvec (pitch);
}