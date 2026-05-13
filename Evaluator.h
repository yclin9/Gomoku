#pragma once

#include "Board.h"

class Evaluator {
public:
    int evaluate(Board& board, Cell aiPlayer);

private:
    int evaluatePosition(Board& board,
                         Move move,
                         Cell player);

    int evaluateDirection(Board& board,
                          Move move,
                          int dRow,
                          int dCol,
                          Cell player);

    Cell opponent(Cell player) const;
};