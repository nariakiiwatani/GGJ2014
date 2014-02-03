#include "Board.h"
#include "ofGraphics.h"
#include "Pawn.h"

Board::Board()
{
	clear();
}

Board::~Board()
{
}

void Board::clear()
{
	for(int i = 0; i < GRID_X; ++i) {
		for(int j = 0; j < GRID_Y; ++j) {
			board_[i][j] = NULL;
		}
	}
	is_moved_ = false;
	last_moved_ = NULL;
}

void Board::set(int x, int y, Man *man)
{
	if(x < 0 || Board::GRID_X <= x || y < 0 || Board::GRID_Y <= y) {
		ofLog(OF_LOG_WARNING, "grid index out of bounds: "+ofToString(x)+","+ofToString(y));
		return;
	}
	board_[x][y] = man;
}

void Board::set(Board *board)
{
	for(int x = 0; x < GRID_X; ++x) {
		for(int y = 0; y < GRID_Y; ++y) {
			set(x,y,board->get(x,y));
		}
	}
}

Man* Board::get(int x, int y)
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

BoardDiff Board::getDiff(Board *board)
{
	BoardDiff ret;
	for(int x = 0; x < GRID_X; ++x) {
		for(int y = 0; y < GRID_Y; ++y) {
			if(board_[x][y] != board->get(x,y)) {
				Man *man = board_[x][y];
				if(man) {
					BoardDiff::iterator it = ret.find(man);
					if(it != ret.end()) {
						(*it).second.from.x = x;
						(*it).second.from.y = y;
					}
					else {
						Diff diff;
						diff.from.x = x;
						diff.from.y = y;
						ret[man] = diff;
					}
				}
				man = board->get(x,y);
				if(man) {
					BoardDiff::iterator it = ret.find(man);
					if(it != ret.end()) {
						(*it).second.to.x = x;
						(*it).second.to.y = y;
					}
					else {
						Diff diff;
						diff.to.x = x;
						diff.to.y = y;
						ret[man] = diff;
					}
				}
			}
		}
	}
	return ret;
}

void Board::merge(BoardDiff& diff)
{
	last_moved_ = NULL;
	for(map<Man*,Diff>::iterator it = diff.begin(); it != diff.end(); ++it) {
		Diff& diff = (*it).second;
		if(diff.to.x != -1) {
			Man *man = (*it).first;
			is_last_move_valid_ = man->canMove(diff.from.x, diff.from.y, diff.to.x-diff.from.x, diff.to.y-diff.from.y);
		}
	}
	for(map<Man*,Diff>::iterator it = diff.begin(); it != diff.end(); ++it) {
		Diff& diff = (*it).second;
		if(diff.from.x != -1) {
			set(diff.from.x, diff.from.y, NULL);
		}
	}
	for(map<Man*,Diff>::iterator it = diff.begin(); it != diff.end(); ++it) {
		Diff& diff = (*it).second;
		if(diff.to.x != -1) {
			Man *man = (*it).first;
			set(diff.to.x, diff.to.y, man);
			if(man && man->getTypeId() == TYPE_PAWN) {
				if((man->getSide()==0&&diff.to.y==0) || (man->getSide()==1&&diff.to.y==GRID_Y-1)) {
					static_cast<Pawn*>(man)->setPromoted(true);
					cout << "promoted" << endl;
				}
			}
			man->setMoved(true);
			last_moved_ = man;
			cout << "(" << diff.from.x << "," << diff.from.y << ")->(" << diff.to.x << "," << diff.to.y << ")" << endl;
		}
	}
}

void Board::updatePromotion()
{
	for(int x = 0; x < GRID_X; ++x) {
		for(int y = 0; y < GRID_Y; ++y) {
			Man *man = board_[x][y];
			if(man && man->getTypeId() == TYPE_PAWN) {
				if((man->getSide()==0&&y==0) || (man->getSide()==1&&y==GRID_Y-1)) {
					static_cast<Pawn*>(man)->setPromoted(true);
					cout << "promoted" << endl;
				}
			}
		}
	}
}

bool Board::isLastMoveValid()
{
	return is_last_move_valid_;
//	Diff& diff = last_move_.second;
//	return last_move_.first->canMove(diff.from.x, diff.from.y, diff.to.x-diff.from.x, diff.to.y-diff.from.y);
}

void Board::removeLast()
{
	if(!last_moved_) {
		return;
	}
	for(int x = 0; x < GRID_X; ++x) {
		for(int y = 0; y < GRID_Y; ++y) {
			if(board_[x][y] == last_moved_) {
				board_[x][y] = NULL;
			}
		}
	}
	last_moved_ = NULL;
}

void StableBoard::merge(Board *board)
{
	for(int x = 0; x < GRID_X; ++x) {
		for(int y = 0; y < GRID_Y; ++y) {
			Man *man = board->get(x,y);
			if(cache_[x][y] == man) {
				if(++stable_count_[x][y] >= STABLE_THRESHOLD) {
					set(x,y,man);
				}
			}
			else {
				cache_[x][y] = man;
				stable_count_[x][y] = 0;
			}
		}
	}
}

/* EOF */