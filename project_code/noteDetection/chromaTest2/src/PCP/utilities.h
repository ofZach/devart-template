
#ifndef  __GENCHORDS__UTILITIES_H__
#define  __GENCHORDS__UTILITIES_H__

// int UTILITIES_DEBUGLEVEL = 0;

void debugm(std::string m, int level=100);

 
void normalize(float * values, int vallen);

float distance(float * val1, float * val2, int f_normalize);
float euclideanDistance(float * val1, float * val2, int len);

/* 
saves the string s to file filename
returns 0 on success
*/
int savestring(std::string s, std::string filename);

int idealWindowSize(int samplerate);
#endif
