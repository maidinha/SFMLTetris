#pragma once

struct Point
{
	int x, y;

	Point()
	{
		x = 0;
		y = 0;
	}

	Point(int x1, int y1)
	{
		x = x1;
		y = y1;
	}
};

class Piece 
{
private:
	Point piecesPos[4];
public:
	
	Piece(int codes[4]);

	Point *getPositions();
	
	void movePieceX(int x);
	void movePieceY(int y);
	void rotate();
};

