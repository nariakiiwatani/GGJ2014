#include "Board.h"
#include "ofGraphics.h"

Board::Board()
{
	for(int p = 0; p < 2; ++p) {
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
		man_[p].push_back(new Pawn());
	}
	for(int p = 0; p < 2; ++p) {
		for(vector<Man*>::iterator it = man_[p].begin(); it != man_[p].end(); ++it) {
			Man *man = *it;
			man->setup();
		}
	}
	reset();
}

Board::~Board()
{
	for(int p = 0; p < 2; ++p) {
		while(!man_[p].empty()) {
			delete man_[p].front();
			man_[p].erase(man_[p].begin());
		}
	}
}

void Board::clear()
{
	for(int i = 0; i < GRID_X; ++i) {
		for(int j = 0; j < GRID_Y; ++j) {
			board_[i][j] = NULL;
		}
	}
}

void Board::reset()
{
	for(int p = 0; p < 2; ++p) {
		for(vector<Man*>::iterator it = man_[p].begin(); it != man_[p].end(); ++it) {
			Man *man = *it;
			man->reset();
		}
	}
	last_moved_ = NULL;
	clear();
}

void Board::setMan(int x, int y, int player, int id)
{
	if(x < 0 || Board::GRID_X <= x || y < 0 || Board::GRID_Y <= y) {
		ofLog(OF_LOG_WARNING, "grid index out of bounds: "+ofToString(x)+","+ofToString(y));
		return;
	}
	if(player < 0 || 2 <= player) {
		ofLog(OF_LOG_WARNING, "player index out of bounds: "+ofToString(player));
		return;
	}
	if(id < 0 || man_[0].size() <= id) {
		ofLog(OF_LOG_WARNING, "man id index out of bounds: "+ofToString(id));
		return;
	}
	Man *man = man_[player][id];
	man->setSide(player);
	
	if(man->isMoved()) {
		Pos pos = man->getPos();
		if(pos.first != x || pos.second != y) {
			man->moveTo(x, y);
			last_moved_ = man;
		}
	}
	else {
		man->moveTo(x, y);
	}
	board_[x][y] = man;
}

void Board::draw(float x, float y, float w, float h)
{
	ofPushMatrix();
	ofTranslate(x, y);
	float interval_x = w/(float)GRID_X;
	float interval_y = h/(float)GRID_Y;
	for(int i = 0; i < GRID_X; ++i) {
		for(int j = 0; j < GRID_Y; ++j) {
			if(board_[i][j]) {
				board_[i][j]->draw(i*interval_x, j*interval_y, interval_x, interval_y);
			}
		}
	}
	ofPopMatrix();
}

void Board::drawForPlayer(int p, float x, float y, float w, float h)
{
	ofPushMatrix();
	ofTranslate(x, y);
	float interval_x = w/(float)GRID_X;
	float interval_y = h/(float)GRID_Y;
	for(int i = 0; i < GRID_X; ++i) {
		for(int j = 0; j < GRID_Y; ++j) {
			if(board_[i][j]) {
				if(board_[i][j]->getSide() == p) {
					board_[i][j]->draw(i*interval_x, j*interval_y, interval_x, interval_y);
				}
				else {
					ofPushStyle();
					ofSetColor(ofColor::white, 128);
					ofRect(i*interval_x, j*interval_y, interval_x, interval_y);
					ofPopStyle();
				}
			}
		}
	}
	ofPopMatrix();
}

void Board::drawLastMoved(int p, float x, float y, float w, float h)
{
	if(last_moved_) {
		ofPushMatrix();
		ofTranslate(x, y);
		float interval_x = w/(float)GRID_X;
		float interval_y = h/(float)GRID_Y;
		Pos pos = last_moved_->getPos();
		int i = pos.first;
		int j = pos.second;
		if(last_moved_->getSide() == p) {
			ofPushStyle();
			if(last_moved_->isLastMoveSafe()) {
				ofSetColor(ofColor::blue);
			}
			else {
				ofSetColor(ofColor::red);
			}
			last_moved_->draw(i*interval_x, j*interval_y, interval_x, interval_y);
			ofPopStyle();
		}
		else {
			ofPushStyle();
			ofSetColor(ofColor::white, 128);
			ofRect(i*interval_x, j*interval_y, interval_x, interval_y);
			ofPopStyle();
		}
		ofPopMatrix();
	}
}


void Board::drawGrid(float x, float y, float w, float h)
{
	ofPushMatrix();
	ofTranslate(x, y);
	
	float interval_x = w/(float)GRID_X;
	float interval_y = h/(float)GRID_Y;
	for(int i = 0; i <= GRID_X; ++i) {
		ofLine(i*interval_x,0, i*interval_x, h);
	}
	for(int i = 0; i <= GRID_Y; ++i) {
		ofLine(0,i*interval_y, w, i*interval_y);
	}
	
	ofPopMatrix();
}

/* EOF */