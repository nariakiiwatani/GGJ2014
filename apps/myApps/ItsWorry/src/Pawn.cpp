#include "Pawn.h"
#include "Board.h"

bool Pawn::isJump()
{
	if(isPromoted()) {
		return false;
	}
	if(!isMoved()) {
		return false;
	}
	vector<Pos>::reverse_iterator it = move_history_.rbegin();
	
	int y0 = (*it++).second;
	int y1 = (*it).second;
	return abs(y0-y1)==2;
}

void Pawn::reset()
{
	is_promoted_ = false;
	Man::reset();
}
bool Pawn::canMove(int x, int y, int move_x, int move_y)
{
	if(isPromoted()) {
		return move_x == 0 || move_y == 0 || abs(move_x)==abs(move_y);
	}
	int dir = getSide()==0?-1:1;
	if(move_x == 0) {
		if(board_->get(x,y+dir) != NULL) {
			return false;
		}
		if(move_y == dir) {
			return true;
		}
		if(!isMoved() && move_y==dir*2) {
			return board_->get(x,y+move_y)==NULL;
		}
		return false;
	}
	if(move_y != dir) {
		return false;
	}
	if(abs(move_x) != 1) {
		return false;
	}
	Man *target = board_->get(x+move_x,y+move_y);
	return target?target->getSide()!=getSide():false;
}

void Pawn::updatePossibleMoves()
{
	possible_move_.clear();
	int x = move_history_.back().first;
	int y = move_history_.back().second;
	if(isPromoted()) {
		checkLine(x,y, -1,-1);
		checkLine(x,y, -1, 0);
		checkLine(x,y, -1, 1);
		checkLine(x,y,  0,-1);
		checkLine(x,y,  0, 1);
		checkLine(x,y,  1,-1);
		checkLine(x,y,  1, 0);
		checkLine(x,y,  1, 1);
	}
	else {
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
		if(board_->isInBounds(next_x, next_y) && !board_->get(next_x, next_y)) {
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
				if(board_->isInBounds(next_x, next_y) && !board_->get(next_x, next_y)) {
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
			Man *target = board_->get(next_x, next_y);
			if(target) {
				if(target->getSide() != getSide()) {
					possible_move_.push_back(pair<int,int>(next_x,next_y));
				}
			}
			else {
				switch(player_) {
					case 0:
						target = board_->get(next_x, next_y+1);
						break;
					case 1:
						target = board_->get(next_x, next_y-1);
						break;
				}
				/**
				Pawn *pawn = dynamic_cast<Pawn*>(board_->getLastMoved());
				if(pawn && pawn->isJump() && pawn == target) {
					possible_move_.push_back(pair<int,int>(next_x,next_y));
				}
				/**/
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
			Man *target = board_->get(next_x, next_y);
			if(target) {
				if(target->getSide() != getSide()) {
					possible_move_.push_back(pair<int,int>(next_x,next_y));
				}
			}
			else {
				switch(player_) {
					case 0:
						target = board_->get(next_x, next_y+1);
						break;
					case 1:
						target = board_->get(next_x, next_y-1);
						break;
				}
				/**
				Pawn *pawn = dynamic_cast<Pawn*>(board_->getLastMoved());
				if(pawn && pawn->isJump() && pawn == target) {
					possible_move_.push_back(pair<int,int>(next_x,next_y));
				}
				/**/
			}
		}
	}
}

/* EOF */