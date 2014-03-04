#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <sstream>

#include "errno.h"

#include "FFT.h"
#include "PitchName.h"

#include "PCP.h"
#include "Sounddata.h"
#include "utilities.h"




// hoerbare Frequenzgrenzen, die zur analyse herangezogen werden sollen
#define MINFREQUENZ 52
#define MAXFREQUENZ 880

// index ab dem das spektrum betrachtet wird. bei kleinerem index ist der abstand zwischen 2 Halbtoenen bei ~2 -> das waere zu ungenau
#define IMIN 50
#define NEIGHBOUR 3


std::vector<pitch> getPeaks(float * freq, int freqSize, int samplerate, bool one_peak_per_mountain);
  

// computes PCP out of floating point buffer
 PCP::PCP(const float * srcbuffer, long srclen, int windowSize, int samplerate, int algo, int ac_BlockSizeMs) {
	
	// if ac_BlockSizeMS is 0 do the autocorrelation for the whole srcbuffer at once
	// default is 2 Seconds
	 int ac_BlockSize = srclen;
	 if (ac_BlockSizeMs) {
		 ac_BlockSize = (samplerate*ac_BlockSizeMs)/1000;
	 }
     
     //std::cout << windowSize << std::endl;
	
     
	 int aktsrclen = ac_BlockSize;
	
	 for (int i=0; i<PCPLEN; i++) {pcp[i] = 0; pcpCountPeaks[i]=0;}
	//std::cout << "i\tC\tC#\tD\tD#\tE\tF\tF#\tG\tG#\tA\tBb\tB" << std::endl;
	/* 
	if ac_BlockSize set and < srclen:
	 autocorrelation doesn't work on a long timespan -> split the srcbuffer
	 and do the autocorrelation on each block. Sum up the results */
	 for (int i=0; i<srclen; i+= ac_BlockSize) {
// 		 std::cout << i << std::endl;
		 int resSize = 0;
		 float * resbuffer = new float[windowSize];
		
		 if (i+aktsrclen > srclen) aktsrclen = srclen-i;
		
			// calculate enhanced autocorrelation
		 resSize = analysefrequencies(eaENHANCED_AUTOCORRELATION,	\
				 					  ewHANNING,					\
						 			  windowSize,					\
							 		  srcbuffer+i,					\
									  aktsrclen,					\
									  resbuffer);	
		
		// get local maxima and save the count of maxima to pcpCountPeaks
		 if (algo==1) {
		
             

			 std::vector<pitch> pitches = getPeaks(resbuffer, resSize, samplerate, true);
			 for (unsigned int i=0; i<pitches.size(); i++) {
                 //std::cout << pitches[i].herz << "hz (" << PitchName_Absolute(Freq2Pitch(pitches[i].herz)) <<") = " << pitches[i].db << std::endl;
			
				 pcp[pitches[i].getnum()] += pitches[i].db;		 
				 pcpCountPeaks[pitches[i].getnum()] += 1;
			 }
		 }
		 
		 if (algo==2||algo==3) {
			 std::vector<pitch> pitches = getPeaks(resbuffer, resSize, samplerate, false);
			 for (unsigned int i=0; i<pitches.size(); i++) {
// 				 std::cout << pitches[i].herz << "hz (" << PitchName_Absolute(Freq2Pitch(pitches[i].herz)) <<") = " << pitches[i].db << std::endl;
				 if (algo==2)	{
					 pcp[pitches[i].getnum()] += pitches[i].db;
				 }
				 pcpCountPeaks[pitches[i].getnum()] += 1;
			 } 
		 }
		 
	
		// ----------- Algo 3 ----------------------
		// frequenzen aufaddieren
		 if (algo==3) {
			 for (int i = int(fmax(samplerate/MAXFREQUENZ, IMIN)); i < fmin(resSize, samplerate/MINFREQUENZ); i++) {  
				 float hz = samplerate/((float)i);
				 int pi = PitchIndex(Freq2Pitch(hz));
				 pcp[pi] += resbuffer[i];
			 }	
		 }
// 		std::cout << (i/ac_BlockSize) << "\t";
// 		for (int i=0; i<12; i++) std::cout << std::setprecision(4) << pcp[i] << "\t";
// 		 std::cout << std::endl;
		
		 delete [] resbuffer;
	 }
	 for (int i=0; i<PCPLEN; i++) {
// 		 std::cout << "pcpCountPeaks[" << i << "] = " << pcpCountPeaks[i] << std::endl;
		 pcpCountPeaks[i]=pcpCountPeaks[i]/(NEIGHBOUR*2+1);
// 		 std::cout << "pcpCountPeaks[" << i << "] = " << pcpCountPeaks[i] << std::endl;
	 }
 }

 PCP::PCP(const float * fpcp) {
	 for (int i=0; i<12; i++) {
		 pcp[i] = fpcp[i];
		 pcpCountPeaks[i] = 1;
	 }
 }

 PCP::PCP(const float * fpcp, float * countPeaks) {
  for (int i=0; i<12; i++) {
	pcp[i] = fpcp[i];
	pcpCountPeaks[i] = countPeaks[i];
  }
}



  void PCP::addPCP(const PCP & pcp2, const int root) {
	  for (int i=0; i<PCPLEN; i++) {  
		  int j = (PCPLEN + i-root) % PCPLEN;
		  pcp[j] += pcp2.pcp[i];
		  pcpCountPeaks[j] += pcp2.pcpCountPeaks[i];
	  }
  }



/* **********************************
 * Prints the PCP to stdout
 * x-axis is the pitch (frequency), y-axis the intensity
 * **+++++++************************/

std::string PCP::bargraph() const{

  float max = 0;
  for (int i=0; i<12; i++) {
	if (pcp[i]>max) max = pcp[i];
  }

  std::ostringstream ost;
  
  ost << max << std::endl;
  ost << "\t^" << std::endl;
  int steps = 10;
  for (int i=steps; i>0; i--) {
	ost << "\t|";
	for (int j=0; j<12; j++) {
	  if ((pcp[j]/max) >= ((float)i)/steps) {
		ost << "x";
	  }
	  else {
		ost << " ";
	  }
	  ost << " ";
	}
	ost << std::endl;
  }
  ost << "0\t|------------------------" << std::endl;

  ost << "\t|";
  for (int i=0; i<12; i++) {
	ost << PitchName(i);
	if (strlen(PitchName(i)) <=1) ost << " ";
  }
  ost << std::endl;
  
  return ost.str();
}

std::string PCP::tostring() const{
	std::ostringstream ost;
	ost << "C\tC#\tD\tD#\tE\tF\tF#\tG\tG#\tA\tBb\tB" << std::endl;
	for (int i=0; i<12; i++) {
		ost << std::fixed << std::setprecision(2) << pcp[i] << "\t";
	}
	ost << std::endl;
	for (int i=0; i<12; i++) {
		ost << pcpCountPeaks[i] << "\t";
	}
	ost << std::endl;


	return ost.str();
}



PCPTrack::PCPTrack(const Sounddata & sd, int algo, int blockSize, bool blockSizeInMs, int windowSize) {

  mWindowSize = windowSize;

  if (blockSizeInMs) {
	mBlockSizeMs = blockSize;
	mBlockSize = (int) (((float)sd.samplerate() * blockSize) / 1000);
  }
  else {
	  mBlockSize = blockSize;
	  mBlockSizeMs = blockSize / sd.samplerate();
  }
  int j=0;
  for (int i=0; i<sd.frames(); i+=mBlockSize) {
	  int aktBlockSize = mBlockSize;
	  if (i > sd.frames()-mBlockSize) {
		  aktBlockSize = sd.frames()-i;
	  }
	  //std::cout << "PCPTrack:\t" << i <<"/" << sd.frames() << std::endl;
	  PCPItem pi = PCPItem(j*mBlockSizeMs, PCP(sd.srcbuffer()+i, aktBlockSize, mWindowSize, sd.samplerate(), algo, 0));
      mPCPVec.push_back(pi);
	  j++;
  }
  
  // std::cout << "PCP_Track erfolgreich angelegt." << std::endl;
}

PCPTrack::PCPTrack(const Sounddata & sd, int algo, std::string timefile, int windowSize) {
	
	mWindowSize = windowSize;
	
	// lese timefile ein	
	std::ifstream fin(timefile.c_str());
	std::vector<float> v;	
	if (!fin) {
		std::cerr << "File not found " << timefile <<std::endl;
		throw ("IO Error");
	}		
	std::string line;
	while (getline(fin, line)) {
		// zahlen mit Beistrich als komma sind erlaubt -> , in . umwandeln
		std::string::size_type st_ws = line.find_first_of(" \t");
		std::string::size_type st_komma = line.find_first_of(",");
		if (st_ws > st_komma) line[st_komma] = '.';
		if (line.length() > 1 && line[0] != '#') {
			float second;
			std::istringstream ist(line);
			if (!(ist >> second)) {
				std::cerr << "wrong format: ." << line << "." << std::endl;
				throw "label format error";
			}
			v.push_back(second*1000); // in millisekunden speichern
		}
	}
	if (v.size() == 0) {
		std::cerr <<  "beatfile " << timefile << " contains no timelabels" << std::endl;
		throw "label format error";
	}
	if (v[0] != 0.0) {
		v.insert(v.begin(),0); 
		std::cout << "first time was " << v[1] << ". inserted 0 at the beginning." << std::endl;
	}
	
	
	// PCPs erzeugen
	int aktpos = 0;
	for (unsigned int i=0; (i<v.size()-1 && aktpos < sd.frames()); i++) {		
		// die blockgroesse aendert sich hier potentiell jedesmal
		float blockSizeMs = v[i+1] - v[i];
		int aktBlockSize = (int) (((float)sd.samplerate() * blockSizeMs)/1000); 
// 		std::cout << aktpos << std::endl;
// 		std::cout << i << ": " << v[i] << " length(ms) " << blockSizeMs << " = length(frames)" << aktBlockSize << std::endl;
		if (aktpos > sd.frames()-aktBlockSize) {
			aktBlockSize = sd.frames()-aktpos;
// 			std::cout << "angepasst: "<< aktBlockSize << std::endl;
		}
		PCPItem pi = PCPItem(v[i], PCP(sd.srcbuffer()+aktpos, aktBlockSize, mWindowSize, sd.samplerate(), algo, 0));
		//PCPItem pi = PCPItem(v[i], PCP(sd.srcbuffer()+aktpos, 2*sd.samplerate()/10, mWindowSize, sd.samplerate(), algo));
		mPCPVec.push_back(pi);
		aktpos += aktBlockSize;
	}		
}

std::string PCPTrack::tostring() {
	std::ostringstream ost;
	for (unsigned int i=0; i<mPCPVec.size(); i++) {
		ost << "------------------ " << float(mPCPVec[i].mStartMs)/1000 << " sec ------------------" << std::endl;
		ost << mPCPVec[i].mPCP.bargraph() << std::endl;
		ost << mPCPVec[i].mPCP.tostring() << std::endl;
	}
	return ost.str();
}


/* ********************************************
   saves pitches in *pitches
   returns number of pitches found
***********************************************/
std::vector<pitch> getPeaks(float * freq, int freqSize, int samplerate, bool one_peak_per_mountain) {
	std::vector<pitch> pitches;
	float leftmin=0;
	freq[0]=0;
   
	bool up = 1;
	int lasti=0;
	if (one_peak_per_mountain) {
		for (int i = int(fmax(samplerate/MAXFREQUENZ, IMIN)); i < fmin(freqSize, samplerate/MINFREQUENZ); i++) {  
	        // up -> down
			if (freq[i] < freq[i-1] && up) {
				up = 0;
// 				std::cout << i << "\t" << 
// 						float(samplerate)/(i-1) << "\t" <<  
// 						PitchName_Absolute(Freq2Pitch(float(samplerate)/(i-1))) << "\t" <<  freq[i-1] << std::endl;	  
				// neue spitze
				if (freq[i-1] > freq[lasti] ) {
// 					std::cout << "NEUE SPITZE" << std::endl;
					lasti = i-1;	
				}
			}
	  		// down -> up
			else if (freq[i] > freq[i-1] && ! up) {
				up = 1;
			}
		 	// erst wenn wir wieder bei null sind, sind wir bereit fuer einen neuen gipfel ;-)
			if (lasti > 0 && freq[i-1] == 0) {
// 				std::cout << "reached 0" << std::endl;
				if (NEIGHBOUR == 0) {
					pitches.push_back(pitch(float(samplerate)/(lasti), freq[lasti]));
				} else {
					for (int n = -NEIGHBOUR; n<=NEIGHBOUR; n++) {
						float herz = float(samplerate)/(lasti+n);
						pitches.push_back(pitch(herz, freq[lasti+n]));
					}
				}
				lasti=0;
			}
		}
	}
	else{
		
		float max = 0;
		bool newmountain;
		for (int i = int(fmax(samplerate/MAXFREQUENZ, IMIN)); i < fmin(freqSize, samplerate/MINFREQUENZ); i++) {
			if (freq[i] > max) max = freq[i];
		}
		
		float MINDIFF = max/10; // besser anhand der durchschnitts db ? oder berge in der umgebung?
// 		std::cout << "mindiff:\t" << MINDIFF << std::endl;
		for (int i = int(fmax(samplerate/MAXFREQUENZ, IMIN)); i < fmin(freqSize, samplerate/MINFREQUENZ); i++) {  
	 		 // up -> down
			if (freq[i] < freq[i-1] && up) {
				up = 0;
// 				std::cout << i << "\t" << float(samplerate)/(i-1) << "\t" <<  
// 						PitchName_Absolute(Freq2Pitch(float(samplerate)/(i-1))) << "\t" << freq[i-1] << std::endl; 	
	 	  		// hoehe MINDIFF im vergleich zum letzten tal erreicht (linkes ok)
				if (freq[i-1] > freq[lasti] && ((freq[i-1]-leftmin) > MINDIFF || newmountain)) {
					lasti=i-1;	
				}
			}
	  		// down -> up
			else if (freq[i] > freq[i-1] && ! up) {
				up = 1;
				if (lasti==0 && freq[i-1] < leftmin) leftmin = freq[i-1];
	      		// hohe MINDIFF im vergleich zum letzten Berg reicht aus (rechts ok)
				if ((freq[lasti]-freq[i-1]) > MINDIFF) {
// 					std::cout << "SPITZE GESPEICHERT" << std::endl;
					leftmin = freq[i-1];
				  	// lokales maximum speichern
					if (NEIGHBOUR == 0) {
						pitches.push_back(pitch(float(samplerate)/(lasti), freq[lasti]));
					} else {
				
						for (int n = -NEIGHBOUR; n<=NEIGHBOUR; n++) {
							float herz = float(samplerate)/(lasti+n);
							pitches.push_back(pitch(herz, freq[lasti+n]));
						}
					}
					newmountain=false;
					lasti=0;
				}
				// auch von einzelstehenden bergen kleiner MINDIFF sollte die spitze gespeichert werden
				if (freq[i-1]==0) {
					if (lasti>0) {
						if (NEIGHBOUR == 0) {
							pitches.push_back(pitch(float(samplerate)/(lasti), freq[lasti]));
						} else {
							for (int n = -NEIGHBOUR; n<=NEIGHBOUR; n++) {
								float herz = float(samplerate)/(lasti+n);
								pitches.push_back(pitch(herz, freq[lasti+n]));
							}
						}
						lasti=0;
					}
					newmountain = true;
				}
			}
		}
	}
	if (lasti>0) {
		if (NEIGHBOUR == 0) {
			pitches.push_back(pitch(float(samplerate)/(lasti), freq[lasti]));
		} else {
			for (int n = -NEIGHBOUR; n<=NEIGHBOUR; n++) {
				float herz = float(samplerate)/(lasti+n);
				pitches.push_back(pitch(herz, freq[lasti+n]));
			}
		}
	}
	//std::cout << std::endl;

//   printf("\n");
//   for (unsigned int i = 0; i<pitches.size(); i++) {
// 	printf("localmax: %8f\t%s\t%8f\n", pitches[i].herz, PitchName_Absolute(Freq2Pitch(pitches[i].herz)), pitches[i].db);
//   }
// 	printf("\n");

	return pitches;
}
