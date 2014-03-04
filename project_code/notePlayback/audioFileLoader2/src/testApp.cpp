#include "testApp.h"
#include <sndfile.hh>


bool sortNotes(note * a, note *b){
    return a->mostCommonNote < b->mostCommonNote ? true : false;
}


//--------------------------------------------------------------
void testApp::setup(){

    counter.assign(150,0);
    
    ofDirectory dir;
    dir.listDir( "/Users/zach/Desktop/DEVDEVARTART/googleaudio/jasonoutput/output" );
    
    vector < string > files;
    
    for (int i = 0; i < dir.size(); i++){
    
        ofDirectory dir2;
        dir2.allowExt("wav");
        dir2.listDir(dir.getPath(i));
        
        for (int j = 0; j < dir2.size(); j++){
            files.push_back(dir2.getPath(j));
        }
        
    }
    
    cout << files.size() << endl;
    
    
    for (int i = 0; i < files.size(); i++){
        note * tempNote = new note();
        //tempNote->AA = &AA;
        tempNote->loadFile(files[i]);
        notes.push_back(tempNote);
        
    }
    
    ofSort(notes, sortNotes);
    
    for (int i = 0; i < notes.size(); i++){
        cout << notes[i]->mostCommonNote << endl;
    }
    
    myPiano.setup();
    
    soundStream.setup(this, 2, 0, 44100, 256, 4);

    setupGUI();
}

void testApp::setupGUI(){
    //init gui dims
    float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
    //gui!
    gui = new ofxUICanvas(0, 300, length+xInit, ofGetHeight());
    
    gui->addFPSSlider("FPS SLIDER", length-xInit, dim*.25, 60);
    
    gui->addSpacer(length-xInit, 1);
    gui->addLabel("CONTEXT");
    gui->addIntSlider("attack", 0, 44100, &attack);
    gui->addIntSlider("release", 0, 44100, &release);
    
    gui->addSpacer(length-xInit, 1);
    gui->addLabel("THRESHOLDS");
    gui->addSlider("min pct MCN", 0.0, 1.0, &minPctMCN);
    gui->addSlider("max melo stdDev", 5.0, 0.0, &maxMeloStdDev);
    gui->addSlider("max yin stdDev", 15.0, 0.0, &maxYinStdDev);
    gui->addSlider("max yinFFT stdDev", 15.0, 0.0, &maxYinFFTStdDev);
    gui->addSlider("min yin agree", 0.0, 1.0, &minYinAgree);
    gui->addSlider("min yinFFT agree", 0.0, 1.0, &minYinFFTAgree);
    gui->addSlider("min melo kurtosis", -5.0, 5.0, &minMeloKurtosis);
    

}


//--------------------------------------------------------------
void testApp::audioOut(float * output, int bufferSize, int nChannels){
    
    
    for (int i = 0; i < notes.size(); i++){
        notes[i]->addToSoundBuffer(output, bufferSize, nChannels);
    }
    
    if (bPlayingTone == true){
        if (ofGetElapsedTimef()-startTime > 1.0) bPlayingTone = false;
        
            float pct = (ofGetElapsedTimef()-startTime)  / 1.0;
        if (pct > 1) pct = 1;
        float scalar = sin(pct * PI);
        
            for (int j = 0; j < bufferSize; j++){
                for (int i = 0; i < nChannels; i++){
                //    output[j *nChannels + i] += sin(sinAngle) * 0.05;
                }
                sinAngle+=sinAngleAdder;
            }
//    bPlayingTone = true;
//    startTime = 0;
//    float freq = MIDI2freq(note);
//    sinAngleAdder = freq / 44100;
    }
    
    

}


//--------------------------------------------------------------
void testApp::update(){

   
}

//--------------------------------------------------------------
void testApp::draw(){
    myPiano.draw();
}



float MIDI2freq(int MIDI_note) {
	return 440.0 * powf(2.0,(((float)MIDI_note-(float)69)/(float)12));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key < 97) return;
    
    if (key == 'z') {
        myPiano.octaveDown();
    }
    
    if (key == 'x') {
        myPiano.octaveUp();
    }

    if (myPiano.keyBindings[key] != 0 || key == 'a') {
        int note = myPiano.getNote(key);
        //cout << "trying to play note " << note << endl;
        
        vector < int > anyGoods;
        for (int i = 0; i < notes.size(); i++){
            int which = i;
            if (notes[which]->mostCommonNote == note){
                if ( notes[which]->MD.pctMostCommon > minPctMCN
                    && notes[which]->MD.meloStdDev < maxMeloStdDev
                    && notes[which]->MD.yinStdDev < maxYinStdDev
                    && notes[which]->MD.yinFFTStdDev < maxYinFFTStdDev
                    && notes[which]->MD.yinAgree > minYinAgree
                    && notes[which]->MD.yinFFTAgree > minYinFFTAgree
                    && notes[which]->MD.meloKurtosis > minMeloKurtosis) {
                anyGoods.push_back(i);
                }
                //notes[which]->play();
                //break;
            }
        }
        
        //cout << anyGoods.size() << endl;
        //if (ofRandom(0,1) > 0.3){
        if (anyGoods.size() > 0){
            
            counter[note]++;
            
            
            int which = anyGoods[ (int)ofRandom(0,100000) /*counter[note] */ % anyGoods.size() ];
            notes[which]->play();
        }
        //} else {
            bPlayingTone = true;
            startTime = ofGetElapsedTimef();
            sinAngle = 0;
            float freq = MIDI2freq(note);
            sinAngleAdder = (freq * TWO_PI) / 44100.0;
        
        
            
       // }
    }
    
    myPiano.keyPressed(key);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    myPiano.keyReleased(key);
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
