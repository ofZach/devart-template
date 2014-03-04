#include "testApp.h"
#include <sndfile.hh>

/*
A4	 440.00	 78.41
A#4/Bb4 	 466.16	 74.01
B4	 493.88	 69.85
C5	 523.25	 65.93
C#5/Db5 	 554.37	 62.23
D5	 587.33	 58.74
D#5/Eb5 	 622.25	 55.44
E5	 659.25	 52.33
F5	 698.46	 49.39
F#5/Gb5 	 739.99	 46.62
G5	 783.99	 44.01
G#5/Ab5 	 830.61
 */

#include "Sounddata.h"
#include "PCP.h"
#include "utilities.h"





////


void loadAudioToData(string fileName, vector < float > & audioSamples){
    
    SndfileHandle myf = SndfileHandle( ofToDataPath(fileName).c_str());
    
    //    printf ("Opened file '%s'\n", ofToDataPath(fileName).c_str()) ;
    //    printf ("  Sample rate : %d\n", myf.samplerate ()) ;
    //    printf ("  Channels  : %d\n", myf.channels ()) ;
    //    printf ("  Error   : %s\n", myf.strError());
    //    printf ("  Frames   : %d\n", int(myf.frames())); // frames is essentially samples
    //    puts("");
    
    int nChannels = myf.channels();
    int nFrames = (int) myf.frames();
    
    if (nChannels != 1){
        vector < float > audioSamplesMultiChannel;
        audioSamples.resize(myf.frames());
        audioSamplesMultiChannel.resize(myf.frames() * myf.channels());
        myf.read(&audioSamplesMultiChannel[0], myf.frames() * myf.channels());
        for (int i = 0; i < myf.frames(); i++){
            audioSamples[i] = audioSamplesMultiChannel[i * nChannels];
        }
    } else {
        audioSamples.resize(myf.frames());
        myf.read(&audioSamples[0], myf.frames());
    }
    
    
}


float freqs[12] = { 440.0,466.16, 493.88,523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61};


float newMaxVal = 0;
float maxVal = 0;
PCPTrack pt;
//--------------------------------------------------------------
void testApp::setup(){

    
//    ofBuffer buf;
//    buf = ofBufferFromFile("out.txt");
//    
//    string s = buf.getFirstLine();
//    
//    
//    while ( s != ""){
//        
//        vector < string > sSplit = ofSplitString(s, " ");
//        cout <<sSplit.size() << endl;
//        
//    for (int i = 0; i < sSplit.size(); i++){
//        timeVal val;
//        
//        
//        
//        if (i == 0){
//            string s = sSplit[0];
//            s.erase(s.end()-1);
//            //cout << s << endl;
//            val.pos = ofToFloat( s );
//            
//            
//        } else if (i < 13){
//            val.vals[i-1] = ofToFloat(sSplit[i]);
//            maxVal = max(maxVal,val.vals[i-1]);
//        }
//        //cout << val.val << endl;
//        vals.push_back(val);
//    }
//        s= buf.getNextLine();
//    
//    }
////    while ( (s= buf.getNextLine()) != ""){
////        cout << s << endl;
////        vector < string > str = ofSplitString(s, ",");
////        timeVal val;
////        val.pos = 0;
////        val.val = ofToFloat(str[1]);
////        vals.push_back(val);
////    }
    
    player.loadSound("rebelradi.wav");
    
    loadAudioToData("rebelradi.wav", samples);
    player.setVolume(0.4);
    player.setLoop(true);
    player.play();
    
    ss.setup(this, 1, 0, 44100, 512, 4);
    
    
    
    Sounddata sd( ofToDataPath("rebelradi.wav").c_str());
    
//    cout << sd.samplerate() << " " << sd.frames() << " " <<  endl;
//    for (int i = 0; i < sd.frames(); i++){
//        cout << sd.getsample(i) << endl;
//    }
    
    
    int blockSizeMSec = 100;
    int windowSize = 0;
    int pcpalgo=1;
    
    windowSize = idealWindowSize(sd.samplerate());
    windowSize *= 2;
    
    cout << windowSize << endl;
    
    pt = PCPTrack(sd, pcpalgo, blockSizeMSec, true, windowSize);
    
    
    std::vector<PCPItem> vec = pt.getPCPVec();
    
    for (int i = 0; i < vec.size(); i++){
        
        //cout << vec[i].mStartMs << ": ";
        for (int j = 0; j < 12; j++){
            newMaxVal = MAX(newMaxVal,vec[i].mPCP.pcp[j]);
            //cout << vec[i].mPCP.pcp[j] << ",";
        }
        //cout << endl;
        
    }
    
}




void testApp::audioOut(float * buffer, int bufferSize, int nChannels){
    
    std::vector<PCPItem> vec = pt.getPCPVec();
    
    if (vec.size() < 100) return;
    
    cout << player.getPosition() << endl;
    float pos = player.getPosition();
    
    
    
    float frequencies[] = {
		65.406,	 // C 2
		69.296,	 // C#
		73.416,  // D
		77.782,  // D#
		82.407,  // E
		87.307,  // F
		92.499,	 // F#
		97.999,  // G
		103.826, // G#
		110.000, // A
		116.541, // A#
		123.471  // B
	};

    
//    
////    if (val < 0) val = 0;
////    cout << val << endl;
////    val *= 1;
////    float val = 0;
////    float pos = player.getPosition();
////    
////    for (int i = 0; i < vals.size()-1; i++){
////        
////        if (pos >= vals[i].pos && pos < vals[i+1].pos){
////            val = vals[i].val;
////        }
////    }
//    
//    


    for (int j = 0; j < 12; j++){
        sinAngleAdder[j] =  6*frequencies[j] * TWO_PI / 44100.0;
    }
//
    for (int i = 0; i < bufferSize; i++){
        
        for (int j = 0; j < 12; j++){

    
    float val = vec[ (int)(pos * vec.size()) % vec.size()].mPCP.pcp[j] / newMaxVal;
    val *= 1.2;
    val = ofClamp(val, 0, 1);
    
    
            float volume = powf(val, 1.5);
            if (volume < 0.4) volume = 0.0;
            buffer[i] +=  sin(sinAngle[j]) * 0.5 * volume;
            sinAngle[j] += sinAngleAdder[j];
            while (sinAngle[j] > PI) sinAngle[j] -= TWO_PI;
        }
    }
    
   
}


//--------------------------------------------------------------
void testApp::update(){

    
    
    
}

//--------------------------------------------------------------
void testApp::draw(){

    
    std::vector<PCPItem> vec = pt.getPCPVec();
    
    
    float time = player.getPositionMS() / 1000.0;
    
    //cout << time << endl;
    //cout << pct << endl;
    
    float timeDiff = 1.0;
    
    float startPct = time - timeDiff;
    float endPct = time + timeDiff;
    
    int startPos = startPct * 44100;
    int endPos = endPct * 44100;
    
    vector < float > datas;
    
    for (int i = startPos; i < endPos; i++){
        if (i < 0 || i >= samples.size()){
            datas.push_back( 0 );
        } else{
            datas.push_back( samples[i]);
        }
    }
    
    
    
    float height = ofGetHeight()/12.0;
    
    for (int i = 0; i < 12; i++){
        
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
        for (int j = 0; j < 200; j++){
            
            float pctMe = ofMap(j, 0, 199, startPos / (float)samples.size(), endPos / (float)samples.size());
            pctMe = ofClamp(pctMe, 0, 1);
            
            float val = 0;
            if (ofGetMousePressed() == false){
                 val = vec[ (int)(pctMe * vec.size()) % vec.size()].mPCP.pcp[i] / newMaxVal;
                val *= 1.3;
                val = ofClamp(val, 0, 1);
            } else {
            
                 val = vals[ (int)(pctMe * vals.size()) % vals.size()].vals[i] / maxVal;
            }
            
            ofColor temp;
            temp.setHsb(  ofMap(val, 0, 1, 100,0), 255, 255);
            
            mesh.addColor(   temp );
            mesh.addVertex( ofPoint(ofMap(j, 0, 199, 0, ofGetWidth()), i * height));
            mesh.addColor(   temp );
            mesh.addVertex( ofPoint(ofMap(j, 0, 199, 0, ofGetWidth()), (i+1) * height));
            
            
        }
        
        mesh.draw();
    }
    
    ofSetColor(180);
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    for (int i = 0; i < datas.size(); i++){
        mesh.addVertex( ofPoint( ofMap(i, 0, datas.size(), 0, ofGetWidth()), 500 + datas[i]*100 ));
    }
    mesh.draw();
    
    
    ofSetColor(80);
    ofLine(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());

    
    
    
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
