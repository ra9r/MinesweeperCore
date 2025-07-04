#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>

// Constructor initializes the board with given dimensions and mine count
Board::Board(int width, int height, int mineCount)
    : width(width), height(height), mineCount(mineCount)
{
    tiles.resize(width * height);
}

// Generate the board with mines and adjacent mine counts
void Board::generate()
{
    // Clear board
    for (auto& tile : tiles)
    {
        tile.hasMine = false;
        tile.state = TileState::Unrevealed;
        tile.adjacentMines = 0;
    }

    // Generate list of positions
    std::vector<int> positions(width * height);
    for (int i = 0; i < width * height; ++i)
        positions[i] = i;

    // Use modern random shuffle
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(positions.begin(), positions.end(), rng);

    // Place mines
    for (int i = 0; i < mineCount; ++i)
    {
        int index = positions[i];
        tiles[index].hasMine = true;
    }

    // Compute adjacent mines
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if ((*this)(x, y).hasMine)
                continue;

            (*this)(x, y).adjacentMines = countAdjacentMines(x, y);
        }
    }

    std::cout << "Board generated!" << std::endl;
}

// Reset the board with new dimensions and mine count
void Board::reset(int width, int height, int mineCount)
{
    this->width = width;
    this->height = height;
    this->mineCount = mineCount;
    tiles.resize(width * height);
    generate();
}

// Print the board to console
void Board::printBoard() const
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const Tile& tile = (*this)(x, y);
            if (tile.state == TileState::Revealed)
            {
                if (tile.hasMine)
                    std::cout << "* ";
                else
                    std::cout << tile.adjacentMines << " ";
            }
            else if (tile.state == TileState::Flagged)
            {
                std::cout << "F ";
            }
            else if (tile.state == TileState::Exploded)
            {
                std::cout << "X "; // Exploded tile
			}
            else
            {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

void Board::reveal(int x, int y)
{
    // Out of bounds, do nothing
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;
        //throw std::out_of_range("Board::reveal(): x or y out of range");

    Tile& tile = (*this)(x, y);

    if (tile.state == TileState::Revealed)
        return;
    
    if (tile.hasMine) {
		tile.state = TileState::Exploded;
    } else {
	    //std::cout << "reveal(" << x << ", " << y << ")" << std::endl;
		tile.state = TileState::Revealed;
        // If no adjacent mines, reveal adjacent tiles
        //std::cout << "Adjacent Mines: " << tile.adjacentMines << std::endl;
        if (tile.adjacentMines == 0)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                for (int dx = -1; dx <= 1; ++dx)
                {
                    if (dx == 0 && dy == 0) continue;
                    reveal(x + dx, y + dy);
                        
                }
            }
        } 
    }
}
// Flag or unflag a tile
void Board::flag(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        throw std::out_of_range("Board::flag(): Index out of range");
    }
    Tile& tile = (*this)(x, y);
    switch (tile.state) {
        case TileState::Unrevealed:
            tile.state = TileState::Flagged;
            break;
        case TileState::Flagged:
            tile.state = TileState::Unrevealed;
            break;
        case TileState::Revealed:
        case TileState::Exploded:
            // Cannot flag revealed or exploded tiles
            break;
	}
}

// Count adjacent mines for a tile at (x, y)
int Board::countAdjacentMines(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return 0;

    int count = 0;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < width && ny >= 0 && ny < height)
            {
                if ((*this)(nx, ny).hasMine)
                    ++count;
            }
        }
    }
    return count;
}

// Access tile at (x, y) with bounds checking
Tile& Board::operator()(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        //throw std::out_of_range("Board::operator(): Index out of range");
        std::ostringstream oss;
        oss << "Board::operator(): Index out of range (x=" << x << ", y=" << y << ")";
        throw std::out_of_range(oss.str());
    }
    return tiles[y * width + x];
}

// Access tile at (x, y) with bounds checking (const version)
const Tile& Board::operator()(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        //throw std::out_of_range("Board::operator(): Index out of range");
        std::ostringstream oss;
        oss << "Board::operator(): Index out of range (x=" << x << ", y=" << y << ")";
        throw std::out_of_range(oss.str());
    }
    return tiles[y * width + x];
}
