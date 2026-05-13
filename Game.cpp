#include "Game.h"
#include <iostream>
#include <cctype>
#include <string>

using std::cin;
using std::cout;
using std::string;

void Game::run()
{
    currentPlayer = Cell::X; // X starts first

    while (!gameOver)
    {
        board.print();
        processTurn();
    }
}

void Game::processTurn()
{
    if (currentPlayer == Cell::X)
    {
        handleHumanMove();
    }
    else
    {
        handleAIMove();
    }
}

void Game::handleHumanMove()
{
    while (true)
    {
        cout << "Your move (e.g. H8): ";
        string input;
        cin >> input;

        if (input.size() < 2 || input.size() > 3)
        {
            cout << "Invalid format.\n";
            continue;
        }

        char colChar = std::toupper(input[0]);

        if (colChar < 'A' || colChar >= 'A' + Board::SIZE)
        {
            cout << "Invalid column.\n";
            continue;
        }

        int col = colChar - 'A';

        int row;
        try
        {
            row = std::stoi(input.substr(1));
        }
        catch (...)
        {
            cout << "Invalid row.\n";
            continue;
        }

        if (row < 1 || row > Board::SIZE)
        {
            cout << "Row out of range.\n";
            continue;
        }

        int boardRow = Board::SIZE - row;

        Move move{boardRow, col};

        if (!board.isValidMove(move))
        {
            cout << "Cell already occupied.\n";
            continue;
        }

        board.placeMove(move, currentPlayer);

        if (board.isWin(move, currentPlayer))
        {
            board.print();
            cout << "You win!\n";
            gameOver = true;
            return;
        }

        if (board.isFull())
        {
            board.print();
            cout << "Tie game.\n";
            gameOver = true;
            return;
        }

        currentPlayer = Cell::O;
        break;
    }
}

void Game::handleAIMove()
{
    cout << "AI is thinking...\n";

    Move move = ai.findBestMove(board, Cell::O);

    board.placeMove(move, currentPlayer);

    cout << "AI played: "
         << static_cast<char>('A' + move.col)
         << (Board::SIZE - move.row)
         << "\n";

    if (board.isWin(move, currentPlayer))
    {
        board.print();
        cout << "AI wins!\n";
        gameOver = true;
        return;
    }

    if (board.isFull())
    {
        board.print();
        cout << "Tie game.\n";
        gameOver = true;
        return;
    }

    currentPlayer = Cell::X;
}