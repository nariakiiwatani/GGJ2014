#pragma once

#include "ofMain.h"
#include "ofxParamEdit.h"
#include "Tuio.h"
#include "Board.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
private:
	void reset();
	void exportFile(int side);
	bool turn();
	bool checkValid(BoardDiff& diff);
	void drawBoard(Board *board, float x, float y, float w, float h);
	void drawPlayerBoard(Board *board, int side, float x, float y, float w, float h);

private:
	Board board_raw_, board_out_;
	StableBoard board_stable_;
	Tuio tuio_;
	ofxParamEdit param_;
	
	vector<Man*> man_[2];
	int active_side_;
	
	ofSoundPlayer sound_ok_, sound_ng_, sound_judge_;
	
	ofVec2f board_pos_;
	ofVec2f board_size_;
	ofVec2f board_sub_pos_[2];
	ofVec2f board_sub_size_;
	int board_rotate_;
	ofVec2f cap_pos_;
	ofVec2f cap_size_;
	ofVec2f cap_draw_pos_;
	ofVec2f cap_draw_size_;
	bool calibration_;
};
