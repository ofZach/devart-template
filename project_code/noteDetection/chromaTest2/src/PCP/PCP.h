#include "Sounddata.h"
#include <vector>
#include "PitchName.h"

#ifndef __GENCHORDS__PCP_H__
#define __GENCHORDS__PCP_H__

/* ******************************
 * Pitch Class Profile
 *
 * AKA Chroma Vector
 * ******************************/

const int PCPLEN = 12;


class pitch {
	public:
		pitch():herz(0), db(0) {};
		pitch(float h, float d):herz(h), db(d) {};
		int getnum() {return PitchIndex(Freq2Pitch(herz));};
		
		float herz;
		float db;
};

 
class PCP {
 public:
  float  pcp [PCPLEN];
  float  pcpCountPeaks[PCPLEN];

  PCP() {
	  for (int i=0; i<PCPLEN; i++) {
		  pcp[i]=0.0;
		  pcpCountPeaks[i] = 0;
	  }
  };
  // computes PCP out of floating point buffer
  PCP(const float * srcbuffer, long srclen, int windowSize, int samplerate, int algo, int ac_BlockSizeMs);
  PCP(const float * fpcp);
  PCP(const float * fpcp, float * countPeaks);
  
  void addPCP(const PCP & pcp2, const int root);
  
  std::string tostring() const;
  std::string bargraph() const;
};

class PCPItem {
	public:
		PCPItem(const float startMs, const PCP & pcp): mStartMs(startMs) {mPCP=pcp;}
		float mStartMs; // start time in milliseconds
		PCP mPCP;
};


class PCPTrack {
 public:
	 PCPTrack() {};
	 PCPTrack(const Sounddata & sd, int algo, int blockSize, bool blockSizeInMs, int windowSize);
	 PCPTrack(const Sounddata & sd, int algo, std::string timefile, int windowSize);
	

	int getblockSizeMs() const { return mBlockSizeMs; } 
	const std::vector<PCPItem>& getPCPVec() const { return mPCPVec; }
	bool initialized() const {return (mPCPVec.size()>0); }
	PCP at(int index) const { return mPCPVec.at(index).mPCP; }
	float startTime(int index) const {return mPCPVec.at(index).mStartMs; }
	unsigned int size() const {return mPCPVec.size(); }
	std::string tostring();

 private:
  int mWindowSize;
  int mBlockSize;
  int mBlockSizeMs;
  std::vector<PCPItem> mPCPVec;
  
};

#endif


