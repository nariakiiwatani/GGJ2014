#include "Tuio.h"
#include "Def.h"

void Tuio::setup()
{
	tuio_.start(3333);
}

void Tuio::setFlame(float x, float y, float w, float h)
{
	x_ = x;
	y_ = y;
	w_ = w;
	h_ = h;
}

void Tuio::update()
{
	data_.clear();
	tuio_.getMessage();
    list<ofxTuioObject*> obj = tuio_.getTuioObjects(); 
	for(list<ofxTuioObject*>::iterator it = obj.begin(); it != obj.end(); ++it) {
        ofxTuioObject *blob = (*it);
		MarkerData data;
		data.id = getManIdFromTiuoId(blob->getFiducialId());
		data.pos.set(blob->getX(), blob->getY());
		data.angle = blob->getAngleDegrees();
		data_.push_back(data);
    }
}

int Tuio::getManIdFromTiuoId(int tuio_id)
{
	return tuio_id;
}

void Tuio::drawDebug(float x, float y, float w, float h)
{
	ofPushMatrix();
	ofTranslate(x,y);
	ofScale(w,h);
	ofPushStyle();
	ofSetColor(ofColor::blue);
	ofRect(x_,y_,w_,h_);
	for(vector<MarkerData>::iterator it = data_.begin(); it != data_.end(); ++it) {
		MarkerData& m = *it;
		ofPushMatrix();
		ofSetColor(ofColor::white);
		ofDrawBitmapString(ofToString(m.id), m.pos);
		ofPopMatrix();
	}
	ofPopStyle();
	ofPopMatrix();
}


/* EOF */