#pragma once

#include "Board.h"
#include <vector>

class AI {
public:
    Move findBestMove(Board& board, Cell aiPlayer);

private:
    bool cellEqual(const Board& b, int r, int c, Cell p, Cell cell) const;
    int checkShape(int x, int y, Cell p, const Board& b, const std::vector<Cell>& cells) const;

    int fiveInARow(int x, int y, Cell piece, const Board& board) const;
    int liveFour(int x, int y, Cell piece, const Board& board) const;
    int deadFour(int x, int y, Cell piece, const Board& board) const;
    int liveThree(int x, int y, Cell piece, const Board& board) const;
    int deadThree(int x, int y, Cell piece, const Board& board) const;
    int liveTwo(int x, int y, Cell piece, const Board& board) const;
    int oneOne(int x, int y, Cell piece, const Board& board) const;
    int oneTwo(int x, int y, Cell piece, const Board& board) const;

    int scoreCell(int x, int y, Cell aiPlayer, const Board& board) const;

    Cell opponent(Cell player) const;
};