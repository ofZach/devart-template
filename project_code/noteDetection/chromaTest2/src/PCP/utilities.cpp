
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>


#include "utilities.h"

int UTILITIES_DEBUGLEVEL = 6;


void debugm(std::string m, int level) {
	extern int UTILITIES_DEBUGLEVEL;
	//std::cout << "UTLITIES_DEBUGLEVEL: "<< UTILITIES_DEBUGLEVEL << std::endl;
	if (UTILITIES_DEBUGLEVEL >= level) {
		std::cout << m << std::endl;
	}
}
/* *******************************************************
 * Computes the maxmium of vector values of size vallen 
 * *******************************************************/
float vmax(float * values,int vallen) {
	float max = 0;
	for (int i=0; i<vallen; i++) {
		if (values[i]>max) {
			max = values[i];
		}
	}
	return max;
}

/* *******************************************************
 * Normalizes a float vector (values) so that the conatined data is between 0 and 1
 * 
 * *******************************************************/

 
void normalize(float * values, int vallen) {
	float max = vmax(values, vallen);
	
// // shift the vector down so that the minimal value is 0
// 	float min=max;
// 	for (int i=0; i<vallen; i++) {
// 		if (values[i]<min) {
// 			min = values[i];
// 		}
// 	}
// 	
// 	/* shift down */
// 	if (min>0) {
// 		for (int i=0; i<vallen; i++) {
// 			values[i]=values[i]-min;
// 		}
// 	}
// 	max = max-min;
	
	
	if (max>0) {
		for (int i=0; i<vallen; i++) {
			values[i] = values[i]/max;
		}
	}
}


/* ***********************
 * berechnet lineare distanz zwischen den vektoren val1 und val2
 * Auch bekannt als City-Block, Taxi- bzw Manhatten Distanz
 * d(x,y) = |x0-y0| + ... + |xi-yi| + ... + |xn-yn|; n = laenge der vektoren
 * die vektoren muessen beide von laenge len sein
 * ***********************/

float distance(float * val1, float * val2, int len) {
	
	float dist = 0;
  
	for (int i=0; i<len; i++) {
		dist += fabs(val1[i]-val2[i]);
	}
	return dist;
}


/* **************************
*
* d(x,y) = |x-y| = wurzel aus [(x0-y0)^2 + ... + (xn-yn)^2]
*
* *****************************/

float euclideanDistance(float * val1, float * val2, int len) {
	float dist = 0;
	for (int i=0; i<len; i++) {
		dist += pow((val1[i]-val2[i]),2);
	}
	return sqrt(dist);
}

int savestring(std::string s, std::string filename) {
	std::ofstream fout(filename.c_str());
	if (!fout) {
		std::cerr << "Unable to open file " << filename;
		return 1;
	}
	fout << s;
	fout.close();
	return 0;
}


int idealWindowSize(int samplerate) {
	switch(samplerate) {
		case 22050:
			return 1024; break;
		case 44100:
			return 2048; break;
		default:
			return 1024;			
	}
}







