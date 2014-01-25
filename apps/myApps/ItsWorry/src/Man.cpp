#include "Man.h"
#include "ofGraphics.h"
#include "Board.h"

void Man::draw(float x, float y, float w, float h)
{
	switch(player_) {
		case 0:
			texture_.draw(x,y,w,h);
			break;
		case 1:
			ofPushMatrix();
			ofTranslate(x+w, y+h);
			ofRotate(180);
			texture_.draw(0,0,w,h);
			ofPopMatrix();
			break;
	}
}

void Man::reset()
{
	move_history_.clear();
}

bool Man::isMoved()
{
	return move_history_.size() > 1;
}

bool Man::isPosSet()
{
	return move_history_.size() > 0;
}

void Man::moveTo(int x, int y)
{
	move_history_.push_back(Pos(x,y));
}

void Man::setPos(int x, int y)
{
	move_history_.push_back(Pos(x,y));
}

bool Man::isLastMoveSafe()
{
	Pos last_move = move_history_.back();
	for(vector<Pos>::iterator it = possible_move_.begin(); it != possible_move_.end(); ++it) {
		if(*it == last_move) {
			return true;
		}
	}
	return false;
}


void Pawn::updatePossibleMoves()
{
	possible_move_.clear();
	int x = move_history_.back().first;
	int y = move_history_.back().second;
	Man *target = NULL;
	int next_x, next_y;
	switch(player_) {
		case 0:
			next_x = x;
			next_y = y-1;
			break;
		case 1:
			next_x = x;
			next_y = y+1;
			break;
	}
	if(board_->isInBounds(next_x, next_y) && !board_->getMan(next_x, next_y)) {
		possible_move_.push_back(pair<int,int>(next_x,next_y));
		if(!isMoved()) {
			switch(player_) {
				case 0:
					next_x = x;
					next_y = y-2;
					break;
				case 1:
					next_x = x;
					next_y = y+2;
					break;
			}
			if(board_->isInBounds(next_x, next_y) && !board_->getMan(next_x, next_y)) {
				possible_move_.push_back(pair<int,int>(next_x,next_y));
			}
		}
	}
	switch(player_) {
		case 0:
			next_x = x+1;
			next_y = y-1;
			break;
		case 1:
			next_x = x-1;
			next_y = y+1;
			break;
	}
	if(board_->isInBounds(next_x, next_y)) {
		Man *target = board_->getMan(next_x, next_y);
		if(target && target->getSide() != getSide()) {
			possible_move_.push_back(pair<int,int>(next_x,next_y));
		}
	}
	switch(player_) {
		case 0:
			next_x = x-1;
			next_y = y-1;
			break;
		case 1:
			next_x = x+1;
			next_y = y+1;
			break;
	}
	if(board_->isInBounds(next_x, next_y)) {
		Man *target = board_->getMan(next_x, next_y);
		if(target && target->getSide() != getSide()) {
			possible_move_.push_back(pair<int,int>(next_x,next_y));
		}
	}
}

/* EOF */