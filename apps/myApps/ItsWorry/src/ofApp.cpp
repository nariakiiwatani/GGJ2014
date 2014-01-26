#include "ofApp.h"
#include "Def.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	tuio_.setup();
	
	sound_ok_.loadSound("sound/ok.mp3");
	sound_ng_.loadSound("sound/ng.mp3");
	sound_ok_.setLoop(false);
	sound_ng_.setLoop(false);
	
	param_.setup("settings");
	param_.beginGroup("preview");
	param_.addVecSlider("pos", board_pos_, ofVec2f(0,0), ofVec2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	param_.addVecSlider("size", board_size_, ofVec2f(0,0), ofVec2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	for(int i = 0; i < 2; ++i) {
		param_.addVecSlider("sub pos "+ofToString(i), board_sub_pos_[i], ofVec2f(0,0), ofVec2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	}
	param_.addVecSlider("sub size", board_sub_size_, ofVec2f(0,0), ofVec2f(SCREEN_WIDTH, SCREEN_HEIGHT));
//	param_.addSlider("rotate", board_rotate_, 0, 3);
	param_.endGroup();
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
	if(board_.isMovedFrame()) {
		exportFile(0);
		exportFile(1);
	}
	else if(ofGetFrameNum()%30 == 0) {
		exportFile(0);
		exportFile(1);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(128);
	board_.draw(board_pos_.x, board_pos_.y, board_size_.x, board_size_.y);
	board_.drawLastMoved(0, board_pos_.x, board_pos_.y, board_size_.x, board_size_.y);
	board_.drawLastMoved(1, board_pos_.x, board_pos_.y, board_size_.x, board_size_.y);
	ofPushStyle();
	ofSetColor(ofColor::black);
	board_.drawGrid(board_pos_.x, board_pos_.y, board_size_.x, board_size_.y);
	ofPopStyle();
	for(int i = 0; i < 2; ++i) {
		board_.drawForPlayer(i, board_sub_pos_[i].x, board_sub_pos_[i].y, board_sub_size_.x, board_sub_size_.y);
		board_.drawLastMoved(i, board_sub_pos_[i].x, board_sub_pos_[i].y, board_sub_size_.x, board_sub_size_.y);
		ofPushStyle();
		ofSetColor(ofColor::black);
		board_.drawGrid(board_sub_pos_[i].x, board_sub_pos_[i].y, board_sub_size_.x, board_sub_size_.y);
		ofPopStyle();
	}
	
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

void ofApp::exportFile(int side)
{
	string exp = "{\"board\":[";
	switch(side) {
		case 0:
			for(int y = 0; y < Board::GRID_Y; ++y) {
				exp += "[";
				for(int x = 0; x < Board::GRID_X; ++x) {
					Man *man = board_.getMan(x, y);
					int number;
					if(!man) {
						number = 0;
					}
					else if(man->getSide() != side) {
						number = -1;
					}
					else {
						number = man->getTypeId()+1;
					}
					exp += ofToString(number);
					if(x != Board::GRID_X-1) {
						exp += ",";
					}
				}
				exp += "]";
				if(y != Board::GRID_Y-1) {
					exp += ",";
				}
			}
			break;
		case 1:
			for(int y = Board::GRID_Y-1; y >= 0; --y) {
				exp += "[";
				for(int x = Board::GRID_X-1; x >= 0; --x) {
					Man *man = board_.getMan(x, y);
					int number;
					if(!man) {
						number = 0;
					}
					else if(man->getSide() != side) {
						number = -1;
					}
					else {
						number = man->getTypeId()+1;
					}
					exp += ofToString(number);
					if(x != 0) {
						exp += ",";
					}
				}
				exp += "]";
				if(y != 0) {
					exp += ",";
				}
			}
			break;
	}
	exp += "]}";

	cout << exp << endl;
	ofBuffer buf(exp);
#if defined TARGET_OSX
	string path = string("../../../data/");
#elif defined TARGET_ANDROID
	string path = string("sdcard/");
#elif defined(TARGET_LINUX) || defined(TARGET_WIN32)
	string path = string(ofFilePath::join(ofFilePath::getCurrentExeDir(),  "data/"));
#else
	string path = string("data/");
#endif
	ofSetDataPathRoot("~/Sites/GGJ2014/");
	bool ref = ofBufferToFile("data"+ofToString(side)+".json", buf);
	ofSetDataPathRoot(path);
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
	if(board_.doubt()) {
		sound_ok_.play();
	}
	else {
		sound_ng_.play();
	}
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
