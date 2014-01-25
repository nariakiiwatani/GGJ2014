#include "Man.h"
#include "ofGraphics.h"
#include "Board.h"

namespace {
	ofTexture textures_[TYPE_NUM][2];
	void loadTex() {
		for(int i = 0; i < 2; ++i) {
			for(int j = TYPE_PAWN; j < TYPE_NUM; ++j) {
				ofLoadImage(textures_[j][i], "p"+ofToString(i)+"/"+ofToString(j+1)+".png");
			}
		}
	}
}
void Man::loadTexture()
{
	loadTex();
}

void Man::draw(float x, float y, float w, float h)
{
	switch(player_) {
		case 0:
			textures_[getTypeId()][player_].draw(x,y,w,h);
			break;
		case 1:
			ofPushMatrix();
			ofTranslate(x+w, y+h);
			ofRotate(180);
			textures_[getTypeId()][player_].draw(0,0,w,h);
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

void Man::checkOne(int from_x, int from_y, int move_x, int move_y)
{
	int next_x = from_x+move_x;
	int next_y = from_y+move_y;
	if(board_->isInBounds(next_x, next_y)) {
		Man *target = board_->getManPrev(next_x, next_y);
		if(target) {
			if(target->getSide() != getSide()) {
				possible_move_.push_back(Pos(next_x,next_y));
			}
		}
		else {
			possible_move_.push_back(Pos(next_x,next_y));
		}
	}
}

void Man::checkLine(int from_x, int from_y, int move_x, int move_y)
{
	int next_x = from_x+move_x;
	int next_y = from_y+move_y;
	while(board_->isInBounds(next_x, next_y)) {
		Man *target = board_->getManPrev(next_x, next_y);
		if(target) {
			if(target->getSide() != getSide()) {
				possible_move_.push_back(Pos(next_x,next_y));
			}
			break;
		}
		else {
			possible_move_.push_back(Pos(next_x,next_y));
		}
		next_x += move_x;
		next_y += move_y;
	}
}

/* EOF */