//
//  piano.h
//  audioFileLoader
//
//  Created by Jason Levine on 2014-01-20.
//
//

#include "ofMain.h"

struct pianoKey {
    bool blackKey;
    float pos;
    char keyBinding;
    bool keyPressed;
};

class piano {
public:
    void setup();
    void draw();
    void keyPressed( int key );
    void keyReleased( int key );
    void octaveDown();
    void octaveUp();
    
    bool octvUpPressed, octvDownPressed;
    
    int getNote( int key ) { return keyBindings[key] + (octave+1) * 12; }
    
    map<int, int> keyBindings;
    vector<pianoKey> pianoKeys;
    
    int octave;
    
    int whiteKeyHeight;
    int whiteKeyWidth;
    int blackKeyHeight;
    int blackKeyWidth;
};
