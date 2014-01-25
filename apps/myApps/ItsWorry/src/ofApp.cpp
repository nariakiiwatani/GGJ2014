#include "ofApp.h"
#include "Def.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	tuio_.setup();
	
	param_.setup("settings");
	for(int i = 0; i < 2; ++i) {
		param_.addVecSlider("pos "+ofToString(i), board_pos_[i], ofVec2f(0,0), ofVec2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		param_.addVecSlider("size "+ofToString(i), board_size_[i], ofVec2f(0,0), ofVec2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	}
	param_.beginGroup("calibration");
	param_.addToggle("enable", calibration_);
	param_.addVecSlider("cap pos", cap_pos_, ofVec2f(0,0), ofVec2f(1, 1));
	param_.addVecSlider("cap size", cap_size_, ofVec2f(0,0), ofVec2f(1, 1));
	param_.addVecSlider("cap draw pos", cap_draw_pos_, ofVec2f(0,0), ofVec2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	param_.addVecSlider("cap draw size", cap_draw_size_, ofVec2f(0,0), ofVec2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	param_.endGroup();
	param_.beginGroup("game");
	param_.addButton("start", this, &ofApp::reset);
	param_.endGroup();
	param_.load();
}

//--------------------------------------------------------------
void ofApp::update(){
	tuio_.setFlame(cap_pos_.x, cap_pos_.y, cap_size_.x, cap_size_.y);
	tuio_.update();
	board_.prepare();
	vector<MarkerData>& data = tuio_.getData();
	for(vector<MarkerData>::iterator it = data.begin(); it != data.end(); ++it) {
		MarkerData& d = *it;
		int x = (int)ofMap(d.pos.x, cap_pos_.x, cap_pos_.x+cap_size_.x, 0, Board::GRID_X);
		int y = (int)ofMap(d.pos.y, cap_pos_.y, cap_pos_.y+cap_size_.y, 0, Board::GRID_Y);
		int player = (90<=d.angle&&d.angle<270)?1:0;
		board_.setMan(x, y, player, d.id);
	}
	board_.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	ofPushStyle();
	ofSetColor(ofColor::white);
	for(int i = 0; i < 2; ++i) {
		board_.drawGrid(board_pos_[i].x, board_pos_[i].y, board_size_[i].x, board_size_[i].y);
		board_.drawForPlayer(i, board_pos_[i].x, board_pos_[i].y, board_size_[i].x, board_size_[i].y);
		board_.drawLastMoved(i, board_pos_[i].x, board_pos_[i].y, board_size_[i].x, board_size_[i].y);
	}
	ofPopStyle();
	
	if(calibration_) {
		tuio_.drawDebug(cap_draw_pos_.x, cap_draw_pos_.y, cap_draw_size_.x, cap_draw_size_.y);
	}
	if(param_.isOpen()) {
		param_.draw();
	}
}

void ofApp::reset()
{
	board_.reset();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case OF_KEY_RETURN:
			param_.toggleOpen();
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
