#pragma once

#include "ofxTuio.h"
#include "Man.h"

struct MarkerData
{
	int id;
	float angle;
	ofVec2f pos;
};


class Tuio
{
public:
	void setup();
	void setFlame(float x, float y, float w, float h);
	void update();
	void drawDebug(float x, float y, float w, float h);
	vector<MarkerData>& getData() { return data_; }
private:
	int getManIdFromTiuoId(int tuio_id);
private:
	ofxTuioClient tuio_;
	float x_, y_, w_, h_;
	vector<MarkerData> data_;
};

/* EOF */