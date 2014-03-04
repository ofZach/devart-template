#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    
    ofBuffer buf;
    buf = ofBufferFromFile("lankra.csv");
    
    string s;
    while ( (s= buf.getNextLine()) != ""){
        cout << s << endl;
        vector < string > str = ofSplitString(s, ",");
        timeVal val;
        val.pos = ofToFloat(str[0]);
        val.val = ofToFloat(str[1]);
        vals.push_back(val);
    }
    
    player.loadSound("lankra.wav");
    player.setVolume(0.1);
    player.setLoop(true);
    player.play();
    
    
    ss.setup(this, 1, 0, 44100, 512, 4);
    
}




void testApp::audioOut(float * buffer, int bufferSize, int nChannels){
    
    cout << player.getPosition() << endl;
    
    float val = vals[ (int)(player.getPosition() * vals.size()) % vals.size()].val;
    
    if (val < 0) val = 0;
    cout << val << endl;
    val *= 1;
//    float val = 0;
//    float pos = player.getPosition();
//    
//    for (int i = 0; i < vals.size()-1; i++){
//        
//        if (pos >= vals[i].pos && pos < vals[i+1].pos){
//            val = vals[i].val;
//        }
//    }
    
    float sinAngleAdder =  val * TWO_PI / 44100.0;

    
    for (int i = 0; i < bufferSize; i++){
        
        
        buffer[i] =  sin(sinAngle) * 0.5;
        
        sinAngle+= sinAngleAdder;
    }
    
    while (sinAngle > PI) sinAngle -= TWO_PI;
}


//--------------------------------------------------------------
void testApp::update(){

    
    
    
}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
