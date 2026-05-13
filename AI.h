#pragma once

#include "Board.h"
#include <vector>

class AI {
public:
    Move findBestMove(Board& board, Cell aiPlayer);

private:
    static constexpr int SEARCH_DEPTH = 3;

    int minimax(Board& board,
                int depth,
                bool maximizingPlayer,
                int alpha,
                int beta,
                Cell aiPlayer);

    int evaluateBoard(Board& board, Cell aiPlayer);

    int evaluatePosition(Board& board,
                         Move move,
                         Cell player);

    int evaluateDirection(Board& board,
                          Move move,
                          int dRow,
                          int dCol,
                          Cell player);

    std::vector<Move> generateCandidateMoves(Board& board);

    Cell opponent(Cell player) const;
};