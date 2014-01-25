#pragma once

#include "ofTexture.h"
#include "ofImage.h"


typedef pair<int,int> Pos;
class Board;

class Man
{
public:
	Man(){}
	virtual ~Man(){}
	void setBoard(Board *board) { board_=board; }
	bool isLastMoveSafe();
	void reset();
	bool isMoved();
	bool isPosSet();
	void setPos(int x, int y);
	void moveTo(int x, int y);
	Pos getPos() { return move_history_.back(); }
	void setSide(int player) { player_=player; }
	int getSide() { return player_; }
	void draw(float x, float y, float w, float h);
	virtual void updatePossibleMoves()=0;
protected:
	Board *board_;
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
	Pawn() {
		ofLoadImage(texture_, "pawn.png");
	}
	void updatePossibleMoves();
};

/* EOF */