#pragma once

#include "ofTexture.h"
#include "ofImage.h"


typedef pair<int,int> Pos;

class Man
{
public:
	virtual ~Man(){}
	virtual void setup()=0;
	bool isLastMoveSafe();
	void reset();
	bool isMoved();
	void setPos(int x, int y);
	void moveTo(int x, int y);
	Pos getPos() { return current_pos_; }
	void setSide(int player) { player_=player; }
	int getSide() { return player_; }
	void draw(float x, float y, float w, float h);
	vector<Pos>& getPossibleMoves() { return possible_move_; }
protected:
	ofTexture texture_;
	int player_;
	Pos initial_pos_;
	Pos current_pos_;
	vector<Pos> possible_move_;
	vector<Pos> move_history_;
};


class Pawn : public Man
{
public:
	void setup() {
		ofLoadImage(texture_, "pawn.png");
		possible_move_.push_back(pair<int,int>(0,-1));
	}
};

/* EOF */