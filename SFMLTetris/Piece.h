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
	Point *piecesPos;
	int colorNumber;
public:
	
	Piece(int codes[4], int x, int y, int colorNum);
	~Piece();

	Point* getPositions();
	Point* setPositions(Point arr[4]);
	int getColor();

	void movePieceX(int x);
	void movePieceY(int y);
	void rotate();
};

