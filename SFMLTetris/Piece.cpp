#include "Piece.h"

Piece::Piece(int codes[4], int x, int y, int colorNum)
{
	piecesPos = new Point[4];

	colorNumber = colorNum;

	for (int i = 0; i < 4; i++)
	{
		piecesPos[i].x = (codes[i] % 2) + x ;
		piecesPos[i].y = (codes[i] / 2) + y;
	}
}

Piece:: ~Piece()
{
	delete[] piecesPos;
}

void Piece::movePieceX(int x)
{
	for (int i = 0; i < 4; i++)
	{
		piecesPos[i].x += x;
	}
}

void Piece::movePieceY(int y)
{
	for (int i = 0; i < 4; i++)
	{
		piecesPos[i].y += y;
	}
}

void Piece::rotate()
{
	Point rotPoint = piecesPos[1];
	for(int i = 0; i < 4; i++)
	{
		int x = piecesPos[i].y - rotPoint.y;
		int y = piecesPos[i].x - rotPoint.x;

		piecesPos[i].x = rotPoint.x - x;
		piecesPos[i].y = rotPoint.y + y;
	}
}

Point* Piece::getPositions()
{
	return piecesPos;
}

Point* Piece::setPositions(Point arr[4])
{
	for(int i = 0; i < 4; i++)
	{
		piecesPos[i] = arr[i];
	}
	return piecesPos;
}

int Piece::getColor()
{
	return colorNumber;
}
