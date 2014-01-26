#include "Board.h"
#include "ofGraphics.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

Board::Board()
{
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
		man_[p].push_back(new Queen());
		man_[p].push_back(new King());
	}
	for(int p = 0; p < 2; ++p) {
		for(vector<Man*>::iterator it = man_[p].begin(); it != man_[p].end(); ++it) {
			Man *man = *it;
			man->setBoard(this);
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
			board_last_[i][j] = NULL;
			board_prev_[i][j] = NULL;
			board_[i][j] = NULL;
		}
	}
	moved_frame_ = false;
}

void Board::reset()
{
	for(int i = 0; i < GRID_X; ++i) {
		for(int j = 0; j < GRID_Y; ++j) {
			board_stable_count_[i][j] = 0;
		}
	}
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
	
	if(man->isPosSet()) {
		Pos pos = man->getPos();
		if(pos.first != x || pos.second != y) {
			board_prev_[x][y] = board_last_[x][y];
			man->updatePossibleMoves();
			man->moveTo(x, y);
			for(int i = 0; i < GRID_X; ++i) {
				for(int j = 0; j < GRID_Y; ++j) {
					board_last_[i][j] = board_[i][j];
				}
			}
			last_moved_ = man;
			moved_frame_ = true;
		}
	}
	else {
		man->setPos(x, y);
	}
	board_[x][y] = man;
}

bool Board::isMovedFrame()
{
	return moved_frame_;
}

Man* Board::getMan(int x, int y)
{
	if(0 <= x && x < GRID_X && 0 <= y && y < GRID_Y) {
		return board_[x][y];
	}
	return NULL;
}

Man* Board::getManPrev(int x, int y)
{
	if(0 <= x && x < GRID_X && 0 <= y && y < GRID_Y) {
		return board_prev_[x][y];
	}
	return NULL;
}

bool Board::isInBounds(int x, int y)
{
	if(0 <= x && x < GRID_X && 0 <= y && y < GRID_Y) {
		return true;
	}
	return false;
}

void Board::prepare()
{
	for(int i = 0; i < GRID_X; ++i) {
		for(int j = 0; j < GRID_Y; ++j) {
			if(board_prev_[i][j] == board_[i][j]) {
				if(++board_stable_count_[i][j] >= 30) {
					board_last_[i][j] = board_[i][j];
				}
			}
			else {
				board_stable_count_[i][j] = 0;
			}
			board_prev_[i][j] = board_[i][j];
			board_[i][j] = NULL;
		}
	}
	moved_frame_ = false;
}

void Board::draw(float x, float y, float w, float h)
{
	ofPushMatrix();
	ofTranslate(x, y);
	float interval_x = w/(float)GRID_X;
	float interval_y = h/(float)GRID_Y;
	for(int i = 0; i < GRID_X; ++i) {
		for(int j = 0; j < GRID_Y; ++j) {
			if(board_last_[i][j]) {
				board_last_[i][j]->draw(i*interval_x, j*interval_y, interval_x, interval_y);
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
			if(board_last_[i][j]) {
				if(board_last_[i][j]->getSide() == p) {
					board_last_[i][j]->draw(i*interval_x, j*interval_y, interval_x, interval_y);
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
	if(last_moved_ && last_moved_ == board_last_[last_moved_->getPos().first][last_moved_->getPos().second]) {
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
				ofSetColor(ofColor::green);
			}
			else {
				ofSetColor(ofColor::yellow);
			}
			ofRect(i*interval_x, j*interval_y, interval_x, interval_y);
			ofPopStyle();
			last_moved_->draw(i*interval_x, j*interval_y, interval_x, interval_y);
		}
		else {
//			ofPushStyle();
//			ofSetColor(ofColor::gray);
//			ofRect(i*interval_x, j*interval_y, interval_x, interval_y);
//			ofPopStyle();
		}
		ofPopMatrix();
	}
}

bool Board::doubt()
{
	doubt_frame_ = 0;
	if(last_moved_
	   && last_moved_ == board_last_[last_moved_->getPos().first][last_moved_->getPos().second] 
	   && !last_moved_->isLastMoveSafe()) {
			return true;
	}
	return false;
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