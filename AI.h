#pragma once

#include "Board.h"
#include <vector>

class AI {
public:
    Move findBestMove(Board& board, Cell aiPlayer);

private:
    int fiveInARow  (int x, int y, Cell piece, const Board& board) const;
    int liveFour  (int x, int y, Cell piece, const Board& board) const;
    int deadFour  (int x, int y, Cell piece, const Board& board) const;
    int liveThree (int x, int y, Cell piece, const Board& board) const;
    int deadThree (int x, int y, Cell piece, const Board& board) const;
    int liveTwo   (int x, int y, Cell piece, const Board& board) const;
    int oneOne    (int x, int y, Cell piece, const Board& board) const;
    int oneTwo    (int x, int y, Cell piece, const Board& board) const;

    int scoreCell (int x, int y, Cell aiPlayer, const Board& board) const;

    Cell opponent(Cell player) const;
};