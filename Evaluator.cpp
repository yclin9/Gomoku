#include "Evaluator.h"

Cell Evaluator::opponent(Cell player) const
{
    return (player == Cell::X)
        ? Cell::O
        : Cell::X;
}

int Evaluator::evaluate(Board& board,
                        Cell aiPlayer)
{
    int score = 0;

    for (int row = 0;
         row < Board::SIZE;
         ++row)
    {
        for (int col = 0;
             col < Board::SIZE;
             ++col)
        {
            Move move{row, col};

            if (!board.isValidMove(move))
                continue;

            score += evaluatePosition(
                board,
                move,
                aiPlayer
            );

            score -= evaluatePosition(
                board,
                move,
                opponent(aiPlayer)
            );
        }
    }

    return score;
}

int Evaluator::evaluatePosition(
    Board& board,
    Move move,
    Cell player)
{
    static const int directions[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1}
    };

    int score = 0;

    for (const auto& dir : directions)
    {
        score += evaluateDirection(
            board,
            move,
            dir[0],
            dir[1],
            player
        );
    }

    return score;
}

int Evaluator::evaluateDirection(
    Board& board,
    Move move,
    int dRow,
    int dCol,
    Cell player)
{
    board.placeMove(move, player);

    int consecutive = 1;
    int openEnds = 0;

    // Forward direction
    int row = move.row + dRow;
    int col = move.col + dCol;

    while (row >= 0 &&
           row < Board::SIZE &&
           col >= 0 &&
           col < Board::SIZE)
    {
        Move current{row, col};

        if (!board.isValidMove(current))
        {
            ++consecutive;
            row += dRow;
            col += dCol;
        }
        else
        {
            ++openEnds;
            break;
        }
    }

    // Backward direction
    row = move.row - dRow;
    col = move.col - dCol;

    while (row >= 0 &&
           row < Board::SIZE &&
           col >= 0 &&
           col < Board::SIZE)
    {
        Move current{row, col};

        if (!board.isValidMove(current))
        {
            ++consecutive;
            row -= dRow;
            col -= dCol;
        }
        else
        {
            ++openEnds;
            break;
        }
    }

    board.undoMove(move);

    // Scoring system

    if (consecutive >= 5)
        return 1000000;

    if (consecutive == 4)
    {
        if (openEnds == 2)
            return 100000; // open four

        if (openEnds == 1)
            return 10000; // closed four
    }

    if (consecutive == 3)
    {
        if (openEnds == 2)
            return 5000; // open three

        if (openEnds == 1)
            return 500; // closed three
    }

    if (consecutive == 2)
    {
        if (openEnds == 2)
            return 200;

        if (openEnds == 1)
            return 50;
    }

    return 10;
}