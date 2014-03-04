//
//  piano.cpp
//  audioFileLoader
//
//  Created by Jason Levine on 2014-01-20.
//
//

#include "piano.h"


//--------------------------------------------------------------
void piano::setup() {
    //setup dimensions
    whiteKeyHeight = 300;
    whiteKeyWidth = 100;
    blackKeyHeight = 200;
    blackKeyWidth = 75;
    
    
    // setup keys
    pianoKey c;
    c.blackKey = false;
    c.pos = 0.0;
    c.keyBinding = 'a';
    c.keyPressed = false;
    pianoKeys.push_back(c);
    
    pianoKey csharp;
    csharp.pos = 0.5;
    csharp.keyBinding = 'w';
    csharp.blackKey = true;
    csharp.keyPressed = false;
    pianoKeys.push_back(csharp);
    
    pianoKey d;
    d.pos = 1.0;
    d.keyBinding = 's';
    d.blackKey = false;
    d.keyPressed = false;
    pianoKeys.push_back(d);
    
    pianoKey dsharp;
    dsharp.pos = 1.5;
    dsharp.keyBinding = 'e';
    dsharp.blackKey = true;
    dsharp.keyPressed = false;
    pianoKeys.push_back(dsharp);
    
    pianoKey e;
    e.pos = 2.0;
    e.keyBinding = 'd';
    e.blackKey = false;
    e.keyPressed = false;
    pianoKeys.push_back(e);
    
    pianoKey f;
    f.pos = 3.0;
    f.keyBinding = 'f';
    f.blackKey = false;
    f.keyPressed = false;
    pianoKeys.push_back(f);
    
    pianoKey fsharp;
    fsharp.pos = 3.5;
    fsharp.keyBinding = 't';
    fsharp.blackKey = true;
    fsharp.keyPressed = false;
    pianoKeys.push_back(fsharp);
    
    pianoKey g;
    g.pos = 4.0;
    g.keyBinding = 'g';
    g.blackKey = false;
    g.keyPressed = false;
    pianoKeys.push_back(g);
    
    pianoKey gsharp;
    gsharp.pos = 4.5;
    gsharp.keyBinding = 'y';
    gsharp.blackKey = true;
    gsharp.keyPressed = false;
    pianoKeys.push_back(gsharp);
    
    pianoKey a;
    a.pos = 5.0;
    a.keyBinding = 'h';
    a.blackKey = false;
    a.keyPressed = false;
    pianoKeys.push_back(a);
    
    pianoKey asharp;
    asharp.pos = 5.5;
    asharp.keyBinding = 'u';
    asharp.blackKey = true;
    asharp.keyPressed = false;
    pianoKeys.push_back(asharp);
    
    pianoKey b;
    b.pos = 6.0;
    b.keyBinding = 'j';
    b.blackKey = false;
    b.keyPressed = false;
    pianoKeys.push_back(b);
    
    pianoKey cOctvUp;
    cOctvUp.pos = 7.0;
    cOctvUp.keyBinding = 'k';
    cOctvUp.blackKey = false;
    cOctvUp.keyPressed = false;
    pianoKeys.push_back(cOctvUp);
    
    
    //setup keybindings
    for (int i = 0; i < pianoKeys.size(); i++) {
        keyBindings[pianoKeys[i].keyBinding] = i;
    }
    
    octave = 4;
    
}


//--------------------------------------------------------------
void piano::draw() {
    
    //white keys
    for (int i = 0; i < pianoKeys.size(); i++) {
        if (!pianoKeys[i].blackKey) {
            ofSetColor((pianoKeys[i].keyPressed ? 0 : 255));
            ofFill();
            ofRect(pianoKeys[i].pos * whiteKeyWidth, 0, whiteKeyWidth, whiteKeyHeight);
            
            ofSetColor((pianoKeys[i].keyPressed ? 255 : 0));
            ofNoFill();
            ofRect(pianoKeys[i].pos * whiteKeyWidth, 0, whiteKeyWidth, whiteKeyHeight);
            
            float kbX = pianoKeys[i].pos * whiteKeyWidth + whiteKeyWidth / 2;
            float kbY = whiteKeyHeight - whiteKeyHeight / 6;
            ofDrawBitmapString(ofToString(pianoKeys[i].keyBinding), kbX, kbY);
        }
    }
    
    //black keys
    for (int i = 0; i < pianoKeys.size(); i++) {
        if (pianoKeys[i].blackKey) {
            ofSetColor((pianoKeys[i].keyPressed ? 0 : 255));
            ofFill();
            ofRect(pianoKeys[i].pos * whiteKeyWidth + blackKeyWidth/6, 0, blackKeyWidth, blackKeyHeight);
            
            ofSetColor((pianoKeys[i].keyPressed ? 255 : 0));            ofNoFill();
            ofRect(pianoKeys[i].pos * whiteKeyWidth + blackKeyWidth/6, 0, blackKeyWidth, blackKeyHeight);
            
            float kbX = pianoKeys[i].pos * whiteKeyWidth + blackKeyWidth / 2;
            float kbY = blackKeyHeight - blackKeyHeight / 6;
            ofDrawBitmapString(ofToString(pianoKeys[i].keyBinding), kbX, kbY);
        }
    }
    
    
    ofPushMatrix();
    ofTranslate(9 * whiteKeyWidth + 15, 0);
    //octave up/down
    ofSetColor((octvDownPressed ? 0 : 255));
    ofFill();
    ofTriangle(0,0, -whiteKeyWidth, whiteKeyWidth, whiteKeyWidth, whiteKeyWidth);
    
    
    ofSetColor((octvDownPressed ? 255 : 0));
    ofNoFill();
    ofTriangle(0,0, -whiteKeyWidth, whiteKeyWidth, whiteKeyWidth, whiteKeyWidth);
    ofDrawBitmapString("x", 0, whiteKeyWidth / 2);
    
    
    ofSetColor((octvUpPressed ? 0 : 255));
    ofFill();
    ofTriangle(0,whiteKeyHeight, -whiteKeyWidth, whiteKeyHeight - whiteKeyWidth, whiteKeyWidth, whiteKeyHeight - whiteKeyWidth);
    
    ofSetColor((octvUpPressed ? 255 : 0));
    ofNoFill();
    ofTriangle(0,whiteKeyHeight, -whiteKeyWidth, whiteKeyHeight - whiteKeyWidth, whiteKeyWidth, whiteKeyHeight - whiteKeyWidth);
    ofDrawBitmapString("z", 0, whiteKeyHeight - whiteKeyWidth / 2);
    
    ofSetColor(0);
    ofDrawBitmapString("octv " + ofToString(octave), 0, whiteKeyHeight /2);
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void piano::keyPressed( int key ) {
    if (keyBindings[key] != 0 || key == 'a')
        pianoKeys[keyBindings[key]].keyPressed = true;
    
    if (key == 'z') octvUpPressed = true;
    if (key == 'x') octvDownPressed = true;
}

//--------------------------------------------------------------
void piano::keyReleased( int key ) {
    if (keyBindings[key] != 0 || key == 'a')
        pianoKeys[keyBindings[key]].keyPressed = false;
    
    if (key == 'z') octvUpPressed = false;
    if (key == 'x') octvDownPressed = false;
}

//--------------------------------------------------------------
void piano::octaveDown() {
    octave--;
    if (octave < 0) octave = 0;
}

//--------------------------------------------------------------
void piano::octaveUp() {
    octave++;
    if (octave > 6) octave = 6;
}
