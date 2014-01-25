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
			board_[i][j] = NULL;
			board_filter_[i][j].first = NULL;
			board_filter_[i][j].second = 0;
			board_filter_prev_[i][j] = NULL;
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
	
	if(man->isPosSet()) {
		Pos pos = man->getPos();
		if(pos.first != x || pos.second != y) {
			man->updatePossibleMoves();
			man->moveTo(x, y);
			last_moved_ = man;
		}
	}
	else {
		man->setPos(x, y);
	}
	board_filter_[x][y].first = man;
	if(board_filter_prev_[x][y] != man) {
		board_filter_[x][y].second = 0;
	}
//	board_[x][y] = man;
}

Man* Board::getMan(int x, int y)
{
	if(0 <= x && x < GRID_X && 0 <= y && y < GRID_Y) {
		return board_[x][y];
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
			board_filter_prev_[i][j] = board_filter_[i][j].first;
			board_filter_[i][j].first = NULL;
		}
	}
}

void Board::update()
{
	for(int i = 0; i < GRID_X; ++i) {
		for(int j = 0; j < GRID_Y; ++j) {
			if(board_filter_[i][j].first == NULL && board_filter_prev_[i][j] != NULL) {
				board_filter_[i][j].second = 0;
			}
		}
	}
	static const int FILTER_LENGTH = 90;
	for(int i = 0; i < GRID_X; ++i) {
		for(int j = 0; j < GRID_Y; ++j) {
			if(++board_filter_[i][j].second >= FILTER_LENGTH) {
				board_[i][j] = board_filter_[i][j].first;
			}
		}
	}
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
	if(last_moved_ && last_moved_ == getMan(last_moved_->getPos().first, last_moved_->getPos().second)) {
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
			ofSetColor(ofColor::gray);
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