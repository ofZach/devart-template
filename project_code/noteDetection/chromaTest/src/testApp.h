#pragma once

#include "ofMain.h"


typedef struct {
    float pos;
    float vals[12];
} timeVal;


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        vector < timeVal > vals;
    
        vector < float > samples;
    
        ofSoundPlayer player;
        ofSoundStream ss;
        void audioOut(float * buffer, int bufferSize, int nChannels);
        float sinAngle[12];
        float sinAngleAdder[12];
    
        
    
};
