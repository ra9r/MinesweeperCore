#pragma once

#include <vector>

enum class TileState
{
	Unrevealed,
	Revealed,
	Flagged,
	Unknown,
	Exploded
};

struct Tile
{
	bool hasMine = false;
	int adjacentMines = 0; // Number of mines in adjacent tiles
	TileState state = TileState::Unrevealed;
};

class Board
{
public:
	int width;
	int height;
	int mineCount;
	
	Board(int width, int height, int mineCount);

	// Accessor to allow board(x, y) syntax
	Tile& operator()(int x, int y);
	const Tile& operator()(int x, int y) const;

	void generate();
	void reset(int width, int height, int mineCount);
	void reveal(int x, int y);
	void flag(int x, int y);

	void printBoard() const;
	

private:
	std::vector<Tile> tiles;
	int countAdjacentMines(int x, int y) const;
	

};// Board.h

 
