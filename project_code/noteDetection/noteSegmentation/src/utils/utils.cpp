//
//  utils.cpp
//  noteSegmentation
//
//  Created by zach on 1/17/14.
//
//

#include "utils.h"
#include <sndfile.hh>


//---------------------------------------------------------------------------------------
string getAudioDirectory(){
    return "../../../../audio/";        // string for locating where the audio is
//    return "../../audio/";
}


//---------------------------------------------------------------------------------------
float freq2midi(float freq) {
    
    // log 0 is not defined.
    // here, we check for really low or negative freq and return 0
    if (freq < 0.001) return 0;
    
    // else rock and roll:
    return ( 69. + 12.*log(freq/440.)/log((float)2.) );
}



//---------------------------------------------------------------------------------------
int findMostCommon (vector < int > & vals){
    
    //--------------------------------------
    //    vector < int > val;
    //    val.push_back(3);
    //    val.push_back(3);
    //    val.push_back(7);
    //    cout << findMostCommon (val) << endl;
    //    output: 3
    //--------------------------------------
    
    // find the range...
    
    int minElement = *(std::min_element(vals.begin(), vals.end()));
    int maxElement = *(std::max_element(vals.begin(), vals.end()));

    int range = maxElement - minElement + 1;
    
    // note: +1 since we need to include both min and max element
    // (its not min up to max but min up to and including max)
    
    // make a histogram:
    
    std::vector<int> histogram(range,0);
    for( int i=0; i<vals.size(); ++i ){
        ++histogram[ vals[i] - minElement ];
    }
    
    // find max of histogram:
    
    std::vector<int>::iterator result;
    result = (std::max_element( histogram.begin(), histogram.end() ));
    
    // find the value which is here the most.  also, if we want, the count (which isn't returned yet):
    
    
    int valMax =  std::distance(histogram.begin(), result) + minElement;
    int valMaxCount = *result;
    

// this is checking the second biggest element, sometimes they are really close in count, and far or close in range
// so maybe we should bail if it's hard to find "most common?"
// or return a struct and calc pct and stats outside of this function?
    

//    histogram[valMax-minElement] = 0;
//    
//    result = (std::max_element( histogram.begin(), histogram.end() ));
//    int valMax2 =  std::distance(histogram.begin(), result) + minElement;
//    int valMaxCount2 = *result;
//    
//    cout << valMax << " " << valMaxCount << " " << valMax2 << " " << valMaxCount2 << endl;
//    
    return valMax;
    
}


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
void saveDataToAudio(string fileName, vector < float > & audioSamples){
    
    const int format=SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    const int channels=1;
    const int sampleRate=44100;
    
    SndfileHandle outfile( ofToDataPath(fileName), SFM_WRITE, format, channels, sampleRate);
    if (not outfile) return;
    
    outfile.write(&audioSamples[0], audioSamples.size());
 
}

void saveMetaDataToFile(string fileName, metadata noteMetadata){
    ofFile relativeToAbsolute(fileName);
//    cout << relativeToAbsolute.getAbsolutePath() << endl;
    
    
    ofFile metadataFile(relativeToAbsolute.getAbsolutePath(), ofFile::WriteOnly);
    
    metadataFile << "preContext:" << noteMetadata.preContext << endl;
    metadataFile << "postContext:" << noteMetadata.postContext << endl;
    
    metadataFile << "pctMostCommon:" << noteMetadata.pctMostCommon << endl;

    metadataFile << "yinStdDev:" << noteMetadata.yinStdDev << endl;
    metadataFile << "yinFFTStdDev:" << noteMetadata.yinFFTStdDev << endl;
    metadataFile << "meloStdDev:" << noteMetadata.meloStdDev << endl;
    metadataFile << "meloKurtosis:" << noteMetadata.meloKurtosis << endl;
    
    metadataFile << "yinAgree:" << noteMetadata.yinAgree << endl;
    metadataFile << "yinFFTAgree:" << noteMetadata.yinFFTAgree << endl;
    
//    metadataFile.close();

}



string zeroPadNumber(int num, int sizeToBePaddedTo)
{
	stringstream ss;
	
	// the number is converted to string with the help of stringstream
	ss << num;
	string ret;
	ss >> ret;
	
	// Append zero chars
	int str_length = ret.length();
	for (int i = 0; i < sizeToBePaddedTo - str_length; i++)
		ret = "0" + ret;
	return ret;
}



