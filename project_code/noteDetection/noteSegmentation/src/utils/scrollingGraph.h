//
//  scrollingGraph.h
//  audioshadersmesh
//
//  Created by Jason Levine on 2014-01-06.
//
//
#pragma once

#include "ofMain.h"

class scrollingGraph {
public:
    void setup(int _size, float init, float min, float max);
    void addValue(float val);
    float getLast() { return valHistory[valHistory.size()-1]; }
    void draw(float height, float min = 0, float max = 0, bool fill = false);
    
    vector<float> valHistory;
    float minVal, maxVal;
    int size;
};