#include "Man.h"
#include "ofGraphics.h"

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
	return !move_history_.empty();
}

void Man::moveTo(int x, int y)
{
	if(move_history_.empty()) {
		initial_pos_ = Pos(x,y);
		current_pos_ = Pos(x,y);
	}
	move_history_.push_back(Pos(x-current_pos_.first, y-current_pos_.second));
	current_pos_ = Pos(x,y);
}

bool Man::isLastMoveSafe()
{
	Pos last_move = move_history_.back();
	switch(player_) {
		case 0:
			break;
		case 1:
			last_move.first *= -1;
			last_move.second *= -1;
			break;
	}
	for(vector<Pos>::iterator it = possible_move_.begin(); it != possible_move_.end(); ++it) {
		if(*it == last_move) {
			return true;
		}
	}
	return false;
}

/* EOF */