//
// C++ Implementation: Sounddata
//
// Description: 
//
//
// Author: Veronika Zenz <veronika.zenz@google.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "sndfile.h"
#include <string>
#include <iostream>

#include "Sounddata.h"
#include "PCP.h"
#include "utilities.h"
//#include "Chordtype.h"


Sounddata::Sounddata(const std::string soundfilename) {
	
	const std::string extension = soundfilename.substr(soundfilename.rfind("."));
	
	if (extension == ".mp3") {
		importMP3(soundfilename);
	}
	else import(soundfilename);
	
}

void Sounddata::init(const std::string soundfilename) {
	delete [] mSrcBuffer;
	
	const std::string extension = soundfilename.substr(soundfilename.rfind("."));
	
	if (extension == ".mp3") {
		importMP3(soundfilename);
	}
	else import(soundfilename);
}

Sounddata::~Sounddata() { 
	//std::cout << "Sounddata Destruktor" << std::endl;
	if (mSrcBuffer) {
		delete [] mSrcBuffer;
	} 
}

void Sounddata::importMP3(const std::string mp3name) {
	// __TODO__: Create only temporary wav file and delete it afterwards
    // __TODO__: use libmad instead of lame. don't create wav file
	 
	std::cout << "-------Decoding MP3 file using L.A.M.E.-----------" <<std::endl;
	std::string wavname = mp3name + ".wav";
	
	std::string systemcall = "lame --decode \"" 
			+ std::string(mp3name) + "\" \"" + std::string(wavname) + "\" &> /dev/null";
	//std::cout << systemcall << std::endl;
	if (system(systemcall.c_str())) {
		std::cerr << "genchords: importMP3: lame decoding of mp3 failed" <<std::endl;
		throw "Lame decoding error";
	}
	
	import(wavname);
}

void Sounddata::import(const std::string soundfilename) {
	SF_INFO info;
	SNDFILE * sndFile = sf_open(soundfilename.c_str(), SFM_READ, &info);
	if (!sndFile) {
		std::cerr << "import: soundfile error: " << sf_strerror(sndFile) << std::endl;
		throw "soundfileerror";
	}
	
	mSrcBuffer = new float[info.frames];
	mFrames = info.frames;
	mSamplerate = info.samplerate;
	
	if (info.channels == 1) {
		long srclen = sf_readf_float(sndFile, mSrcBuffer, info.channels*info.frames);
		if (srclen != info.frames) {
			std::cerr << "File contains " << info.frames << " frames. But" 
					<< srclen << " have been read." << std::endl;
			throw "soundfileerror";
		}
		//std::cout << "1 channel file read in successfully" << std::endl;
		//std::cout << "sample data 1000 - 1200: " << std::endl;
		//for (int i=1000; i<1200; i++) std::cout << mSrcBuffer[i] << std::endl;
	}
  	else {
		float * interleaved = new float[info.frames * info.channels];
		long srclen = sf_readf_float(sndFile, interleaved, info.channels*info.frames);
		if (srclen != info.frames) {
			std::cerr << "File contains " << info.frames << " frames for each of " << info.channels << "channels. But " 
					<< srclen << " have been read." << std::endl;
			throw "soundfileerror";
		}
			
		for (long i=0; i<info.frames; i++) {
// 			// take only the left channels
// 			mSrcBuffer[i] = nterleaved[i*info.channels]
			
			// mix the channels together and adjust the volume
			mSrcBuffer[i] = 0;
			for (long c=0; c<info.channels; c++) {
				mSrcBuffer[i] += interleaved[i*info.channels+c];
			}
			if (info.channels > 1) {
				mSrcBuffer[i] = mSrcBuffer[i]/info.channels;	
			}
		}
	}
	
}


float Sounddata::getsample(long nr) const {
	if (nr < mFrames) {
		return mSrcBuffer[nr]; 
	}
	std::cerr << "getsample - nr out of range";
	//throw "out of range";
	return -100;
}


void printSfInfo(const std::string soundfilename) {
	printf("********* File Information *************\n");
	const std::string extension = soundfilename.substr(soundfilename.rfind("."));
	
	if (extension == ".mp3") {
		std::cout << "type:\tMP3" << std::endl;
	}
	else {
		SF_INFO info;
		SNDFILE * sndFile = sf_open(soundfilename.c_str(), SFM_READ, &info);
		if (!sndFile) {
			std::cerr << "import: soundfile error: " << sf_strerror(sndFile) << std::endl;
			throw "soundfileerror";
		}
	
		printf("length: %.1fs\n\n",
			   (float)info.frames / info.samplerate);
		printf("frames: %d\n"							\
				"samplerate: %d\n"						\
				"channels: %d\n"						\
				"format: %x\n"							\
				"sections: %d\n"						\
				"seekable %d\n",						
		(int)info.frames, info.samplerate, info.channels, info.format, info.sections, info.seekable);
		sf_close(sndFile);
	}
	printf("********* File Information *************\n");
	printf("\n");
}

