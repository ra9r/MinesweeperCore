#include <iostream>
#include <string>
#include "Board.h"

bool checkExploded(const Board& board)
{
    for (int y = 0; y < board.height; ++y)
    {
        for (int x = 0; x < board.width; ++x)
        {
            if (board(x, y).state == TileState::Exploded)
                return true;
        }
    }
    return false;
}

int main()
{
    int width = 10;
    int height = 10;
    int mineCount = 3;

    Board board(width, height, mineCount);
    board.generate();

    while (true)
    {
        board.printBoard();

        if (checkExploded(board))
        {
            std::cout << "Game Over! You hit a mine." << std::endl;
            break;
        }

        std::cout << "Enter move (r x y) to reveal or (f x y) to flag: ";
        std::string command;
        int x, y;
        std::cin >> command >> x >> y;

        if (x < 0 || x >= width || y < 0 || y >= height)
        {
            std::cout << "Invalid coordinates." << std::endl;
            continue;
        }

        if (command == "r")
        {
            board.reveal(x, y);
        }
        else if (command == "f")
        {
            board.flag(x, y);
        }
        else
        {
            std::cout << "Unknown command." << std::endl;
        }
    }

    // Show final board state
    board.printBoard();
    return 0;
}
