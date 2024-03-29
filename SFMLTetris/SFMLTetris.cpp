#include "SFML/Graphics.hpp"
#include <iostream>
#include <random>
#include "Piece.h"

using namespace sf;

const int ROWS = 20;
const int COLUMNS = 10;

int gameGrid[ROWS][COLUMNS];

int pieces[7][4] =
{
	1,3,5,7, //I
	2,4,5,7, //Z
	3,5,4,6, //S
	3,5,4,7, //T
	2,3,5,7, //L
	3,5,7,6, //J
	2,3,4,5, //O
};

#pragma region Game_System
RenderWindow renderWindow(sf::VideoMode(320, 430), "SFML Super Tetris");
bool isGameOver;
float gameTimer = 0;
float delay = 0.3f;
int score = 0;
#pragma endregion 

#pragma region Rendering
Font gameFont;
Text tScoreLabel;
Text tScoreValue;
Text tGameOver;

Texture texture;
Texture bgTexture;

Sprite sprite;
Sprite bgSprite;
#pragma endregion 

#pragma region Randomness
std::uniform_int_distribution<int> randomPieceRange(0, 6);
std::random_device rd;
std::mt19937 randomNumbers(rd());
#pragma endregion

Piece *currPiece;
Point previousPos[4];

void setupTextures()
{
	bgTexture.loadFromFile("assets/tetris-bg.png");
	bgSprite.setTexture(bgTexture);
	bgSprite.setTextureRect(IntRect(0, 0, 320, 430));

	texture.loadFromFile("assets/tiles.png");
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(currPiece->getColor() * 18, 0, 18, 18));
}

void setupFonts()
{
	gameFont.loadFromFile("assets/kenney-mini.ttf");

	tScoreLabel.setFont(gameFont);
	tScoreLabel.setCharacterSize(18);
	tScoreLabel.setPosition(13 * 18, 3 * 18);
	tScoreLabel.setFillColor(Color::Black);
	tScoreLabel.setString("Score");

	tScoreValue.setFont(gameFont);
	tScoreValue.setCharacterSize(18);
	tScoreValue.setPosition((13 * 18) + tScoreLabel.getGlobalBounds().width/2, 4 * 18);
	tScoreValue.setFillColor(Color::Black);
	tScoreValue.setString(std::to_string(score));

	tGameOver.setFont(gameFont);
	tGameOver.setCharacterSize(15);
	tGameOver.setPosition(12 * 18, 10 * 18);
	tGameOver.setFillColor(Color::Black);
	tGameOver.setString("Game Over \nPress R");
	
}


void initGame()
{
	isGameOver = false;

	memset(gameGrid, -1, sizeof gameGrid);

	currPiece = new Piece(pieces[randomPieceRange(randomNumbers)], COLUMNS / 2, 0, randomPieceRange(randomNumbers));

	setupTextures();
	setupFonts();
}

int dx = 0;
bool rotate;

void handleEvents()
{
	Event event;

	while (renderWindow.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			renderWindow.close();
		}
		if (event.type == Event::KeyPressed)
		{
			if (event.key.code == Keyboard::Left)
			{
				dx = -1;
			}
			else if (event.key.code == Keyboard::Right)
			{
				dx = 1;
			}
			else if (event.key.code == Keyboard::Up)
			{
				rotate = true;
			}
			else if (event.key.code == Keyboard::R && isGameOver)
			{
				initGame();
			}
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Down))
		delay = 0.05;
}

bool isBorderOrAnotherPiece()
{
	Point *tiles = currPiece->getPositions();

	for (int i = 0; i < 4; i++)
	{
		if (tiles[i].y >= ROWS || tiles[i].x >= COLUMNS || tiles[i].x < 0)
			return true;
		else if (gameGrid[tiles[i].y][tiles[i].x] != -1)
			return true;
	}

	return false;
}

void checkAndCleanFilledRows()
{
	int fallOffset = 0;

	for (int row = ROWS - 1; row >= 0; row--)
	{
		int count = 0;
		for (int col = 0; col < COLUMNS; col++)
		{
			gameGrid[row + fallOffset][col] = gameGrid[row][col];

			if (gameGrid[row][col] >= 0)
				count++;
			

			if (count == COLUMNS)
			{
				score += COLUMNS * 10;
				fallOffset++;
			}
		}
	}
}

void fixatePieces(Point unmovedPositions[4])
{
	Point * positions = currPiece->setPositions(unmovedPositions);
	
	int savedColor = currPiece->getColor();

	for (int i = 0; i < 4; i++)
	{
		isGameOver = gameGrid[positions[i].y][positions[i].x] != -1;

		if (!isGameOver)
			gameGrid[positions[i].y][positions[i].x] = savedColor;
		else
			gameGrid[0][positions[i].x] = savedColor;
	}

	if(!isGameOver)
		currPiece = new Piece(pieces[randomPieceRange(randomNumbers)], COLUMNS / 2, 0, randomPieceRange(randomNumbers));
	
}

void update()
{
	Point unmovedPositions[4];

	for (int i = 0; i < 4; i++)
	{
		unmovedPositions[i] = currPiece->getPositions()[i];
	}

	if (dx != 0)
		currPiece->movePieceX(dx);
	if (isBorderOrAnotherPiece())
		currPiece->setPositions(unmovedPositions);

	if (rotate)
		currPiece->rotate();
	if (isBorderOrAnotherPiece())
		currPiece->setPositions(unmovedPositions);

	if (gameTimer > delay)
	{
		currPiece->movePieceY(1);
		score++;
		tScoreValue.setString(std::to_string(score));
		gameTimer = 0;
	}

	if (isBorderOrAnotherPiece())
		fixatePieces(unmovedPositions);

	checkAndCleanFilledRows();

}

void drawCurrentPiece()
{
	renderWindow.draw(bgSprite);

	sprite.setTextureRect(IntRect(currPiece->getColor() * 18, 0, 18, 18));

	for (int i = 0; i < 4; i++)
	{
		sprite.setPosition(
			currPiece->getPositions()[i].x * 18,
			currPiece->getPositions()[i].y * 18
		);

		renderWindow.draw(sprite);
	}
}

void drawStillPieces()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			int color = gameGrid[i][j];

			if (color != -1)
			{
				sprite.setTextureRect(IntRect(color * 18, 0, 18, 18));
				sprite.setPosition(j * 18, i * 18);
				renderWindow.draw(sprite);
			}
		}
	}
}

void render()
{
	renderWindow.clear(Color::White);

	drawCurrentPiece();
	drawStillPieces();

	renderWindow.draw(tScoreLabel);
	renderWindow.draw(tScoreValue);
	
	if (isGameOver)
		renderWindow.draw(tGameOver);

	renderWindow.display();
}

void clearEventVariables()
{
	delay = 0.3;
	dx = 0;
	rotate = false;
}

int main()
{
	initGame();

	float delay = 0.3f;

	int dx = 0;

	Clock clock;

	while (renderWindow.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		gameTimer += time;

		handleEvents();

		if (!isGameOver)
			update();

		render();

		clearEventVariables();
	}

	delete currPiece;

	return 0;
}

