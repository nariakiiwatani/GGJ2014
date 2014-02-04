#include "ofApp.h"
#include "Def.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	tuio_.setup();

	Man::loadTexture();
	for(int p = 0; p < 2; ++p) {
		for(int i = 0; i < 8; ++i) {
			man_[p].push_back(new Pawn());
		}
		for(int i = 0; i < 2; ++i) {
			man_[p].push_back(new Rook());
		}
		for(int i = 0; i < 2; ++i) {
			man_[p].push_back(new Knight());
		}
		for(int i = 0; i < 2; ++i) {
			man_[p].push_back(new Bishop());
		}
		man_[p].push_back(new King());
		man_[p].push_back(new Queen());
	}
	for(int p = 0; p < 2; ++p) {
		for(vector<Man*>::iterator it = man_[p].begin(); it != man_[p].end(); ++it) {
			Man *man = *it;
			man->setBoard(&board_out_);
		}
	}

	sound_ok_.loadSound("sound/ok.mp3");
	sound_ng_.loadSound("sound/ng.mp3");
	sound_judge_.loadSound("sound/judge.mp3");
	sound_ok_.setLoop(false);
	sound_ng_.setLoop(false);
	sound_judge_.setLoop(false);
	
	send_fbo_.allocate(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB);
	syphon_.setName("judgement");
	
	param_.setup("settings");
	param_.beginGroup("preview");
	param_.addToggle("enable", preview_);
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
	
	board_raw_.clear();
	vector<MarkerData>& data = tuio_.getData();
	for(vector<MarkerData>::iterator it = data.begin(); it != data.end(); ++it) {
		MarkerData& d = *it;
		int x = (int)ofMap(d.pos.x, cap_pos_.x, cap_pos_.x+cap_size_.x, 0, Board::GRID_X);
		int y = (int)ofMap(d.pos.y, cap_pos_.y, cap_pos_.y+cap_size_.y, 0, Board::GRID_Y);
		int player = (90<=d.angle&&d.angle<270)?1:0;
		if(d.id < 0 || man_[player].size() <= d.id) {
			ofLog(OF_LOG_WARNING, "man id out of bounds: %d", d.id);
			continue;
		}
		Man *man = man_[player][d.id];
		man->setSide(player);
		board_raw_.set(x, y, man);
	}
	
	board_stable_.merge(&board_raw_);
	
	if((ofGetFrameNum()&0x0f) == 0x08) {
		exportFile(0);
		exportFile(1);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	send_fbo_.begin();
	ofBackground(128);
	drawBoard(&board_stable_, board_pos_.x, board_pos_.y, board_size_.x, board_size_.y);
	for(int i = 0; i < 2; ++i) {
		drawPlayerBoard(&board_stable_, i, board_sub_pos_[i].x, board_sub_pos_[i].y, board_sub_size_.x, board_sub_size_.y);
	}
	send_fbo_.end();
	syphon_.publishTexture(&send_fbo_.getTextureReference());

	if(preview_) {
		send_fbo_.draw(0,0);
	}
	if(calibration_) {
		tuio_.drawDebug(cap_draw_pos_.x, cap_draw_pos_.y, cap_draw_size_.x, cap_draw_size_.y);
	}
	if(param_.isOpen()) {
		param_.draw();
	}
}

void ofApp::drawBoard(Board *board, float x, float y, float w, float h)
{
	board->draw(x, y, w, h);
	//	board_out_.drawLastMoved(0, board_pos_.x, board_pos_.y, board_size_.x, board_size_.y);
	//	board_out_.drawLastMoved(1, board_pos_.x, board_pos_.y, board_size_.x, board_size_.y);
	ofPushStyle();
	ofSetColor(ofColor::black);
	board->drawGrid(x, y, w, h);
	ofPopStyle();
}

void ofApp::drawPlayerBoard(Board *board, int side, float x, float y, float w, float h)
{
	board->drawForPlayer(side, x, y, w, h);
	//		board_out_.drawLastMoved(i, board_sub_pos_[i].x, board_sub_pos_[i].y, board_sub_size_.x, board_sub_size_.y);
	ofPushStyle();
	ofSetColor(ofColor::black);
	board->drawGrid(x, y, w, h);
	ofPopStyle();
}

bool ofApp::turn()
{
	BoardDiff diff = board_out_.getDiff(&board_stable_);
	if(diff.empty()) {
		if(board_out_.isMoved()) {
			if(board_out_.isLastMoveValid()) {
				sound_ng_.play();
				board_out_.setMoved(false);
				cout << "valid move" << endl;
			}
			else {
				sound_ok_.play();
				board_out_.removeLast();
				board_out_.setMoved(false);
				cout << "invalid move" << endl;
			}
		}
		else {
			cout << "no moved" << endl;
			return false;
		}
	}
	else {
		if(!checkValid(diff)) {
			return false;
		}
//		board_out_.set(&board_stable_);
		board_out_.merge(diff);
		board_out_.setMoved(true);
		sound_judge_.play();
		cout << "moved: " << endl;
	}
	return true;
}

bool ofApp::checkValid(BoardDiff& diff)
{
	bool moved[2] = {false,false};
	for(map<Man*,Diff>::iterator it = diff.begin(); it != diff.end(); ++it) {
		Man *man = (*it).first;
		Diff& diff = (*it).second;
		int side = man->getSide();
		if(moved[side]) {
			cout << "invalid diff: multipul move" << endl;
			return false;
		}
		moved[side] = true;
		if(diff.from.x == -1) {
			cout << "invalid diff: something appeared from somewhere" << endl;
			return false;
		}
		if(side == active_side_) {
			if(diff.to.x == -1) {
				cout << "invalid diff: active side removed" << endl;
				return false;
			}
		}
		else {
			if(diff.to.x != -1) {
				cout << "invalid diff: opposite side moved" << endl;
				return false;
			}
		}
	}
	return moved[active_side_];
}

void ofApp::reset()
{
	for(int p = 0; p < 2; ++p) {
		for(vector<Man*>::iterator it = man_[p].begin(); it != man_[p].end(); ++it) {
			Man *man = *it;
			man->reset();
		}
	}

	active_side_ = 0;
	board_out_.set(&board_stable_);
	board_out_.updatePromotion();
}

void ofApp::exportFile(int side)
{
	string exp = "{\"board\":[";
	switch(side) {
		case 0:
			for(int y = 0; y < Board::GRID_Y; ++y) {
				exp += "[";
				for(int x = 0; x < Board::GRID_X; ++x) {
					Man *man = board_stable_.get(x, y);
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
					Man *man = board_stable_.get(x, y);
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

//	cout << exp << endl;
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
		case 32:
		case '1':
			if(active_side_ == 0 && turn()) {
				active_side_ = 1;
			}
			break;
		case '2':
			if(active_side_ == 1 && turn()) {
				exportFile(0);
				exportFile(1);
				active_side_ = 0;
			}
			break;
		case 'r':
			reset();
			break;
		case 'f':
			ofToggleFullscreen();
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
