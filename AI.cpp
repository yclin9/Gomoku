#include "AI.h"

#include <algorithm>
#include <cstdlib>
#include <limits>

Cell AI::opponent(Cell player) const
{
    return (player == Cell::X)
        ? Cell::O
        : Cell::X;
}

Move AI::findBestMove(Board& board, Cell aiPlayer)
{
    std::vector<Move> moves =
        generateCandidateMoves(board);

    int bestScore =
        std::numeric_limits<int>::min();

    std::vector<Move> bestMoves;

    for (const Move& move : moves)
    {
        if (!board.isValidMove(move))
            continue;

        board.placeMove(move, aiPlayer);

        int score = minimax(
            board,
            SEARCH_DEPTH - 1,
            false,
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max(),
            aiPlayer
        );

        board.undoMove(move);

        if (score > bestScore)
        {
            bestScore = score;
            bestMoves.clear();
            bestMoves.push_back(move);
        }
        else if (score == bestScore)
        {
            bestMoves.push_back(move);
        }
    }

    return bestMoves[rand() % bestMoves.size()];
}

int AI::minimax(Board& board,
                int depth,
                bool maximizingPlayer,
                int alpha,
                int beta,
                Cell aiPlayer)
{
    if (depth == 0 || board.isFull())
    {
        return evaluateBoard(board, aiPlayer);
    }

    std::vector<Move> moves =
        generateCandidateMoves(board);

    if (maximizingPlayer)
    {
        int maxEval =
            std::numeric_limits<int>::min();

        for (const Move& move : moves)
        {
            if (!board.isValidMove(move))
                continue;

            board.placeMove(move, aiPlayer);

            if (board.isWin(move, aiPlayer))
            {
                board.undoMove(move);
                return 10000000;
            }

            int eval = minimax(
                board,
                depth - 1,
                false,
                alpha,
                beta,
                aiPlayer
            );

            board.undoMove(move);

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if (beta <= alpha)
                break;
        }

        return maxEval;
    }
    else
    {
        int minEval =
            std::numeric_limits<int>::max();

        Cell enemy = opponent(aiPlayer);

        for (const Move& move : moves)
        {
            if (!board.isValidMove(move))
                continue;

            board.placeMove(move, enemy);

            if (board.isWin(move, enemy))
            {
                board.undoMove(move);
                return -10000000;
            }

            int eval = minimax(
                board,
                depth - 1,
                true,
                alpha,
                beta,
                aiPlayer
            );

            board.undoMove(move);

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            if (beta <= alpha)
                break;
        }

        return minEval;
    }
}

int AI::evaluateBoard(Board& board, Cell aiPlayer)
{
    int score = 0;

    std::vector<Move> moves =
        generateCandidateMoves(board);

    for (const Move& move : moves)
    {
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

    return score;
}

int AI::evaluatePosition(Board& board,
                         Move move,
                         Cell player)
{
    static const int dirs[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1}
    };

    int score = 0;

    for (auto& d : dirs)
    {
        score += evaluateDirection(
            board,
            move,
            d[0],
            d[1],
            player
        );
    }

    return score;
}

int AI::evaluateDirection(Board& board,
                          Move move,
                          int dRow,
                          int dCol,
                          Cell player)
{
    int consecutive = 1;
    int openEnds = 0;

    int row = move.row + dRow;
    int col = move.col + dCol;

    while (row >= 0 &&
           row < Board::SIZE &&
           col >= 0 &&
           col < Board::SIZE)
    {
        Move current{row, col};

        board.placeMove(move, player);

        if (!board.isValidMove(current))
        {
            board.undoMove(move);
            break;
        }

        board.undoMove(move);

        ++consecutive;

        row += dRow;
        col += dCol;
    }

    if (row >= 0 &&
        row < Board::SIZE &&
        col >= 0 &&
        col < Board::SIZE)
    {
        Move end{row, col};

        if (board.isValidMove(end))
            ++openEnds;
    }

    row = move.row - dRow;
    col = move.col - dCol;

    while (row >= 0 &&
           row < Board::SIZE &&
           col >= 0 &&
           col < Board::SIZE)
    {
        Move current{row, col};

        board.placeMove(move, player);

        if (!board.isValidMove(current))
        {
            board.undoMove(move);
            break;
        }

        board.undoMove(move);

        ++consecutive;

        row -= dRow;
        col -= dCol;
    }

    if (row >= 0 &&
        row < Board::SIZE &&
        col >= 0 &&
        col < Board::SIZE)
    {
        Move end{row, col};

        if (board.isValidMove(end))
            ++openEnds;
    }

    if (consecutive >= 5)
        return 1000000;

    if (consecutive == 4 && openEnds == 2)
        return 100000;

    if (consecutive == 4 && openEnds == 1)
        return 10000;

    if (consecutive == 3 && openEnds == 2)
        return 5000;

    if (consecutive == 3 && openEnds == 1)
        return 1000;

    if (consecutive == 2 && openEnds == 2)
        return 300;

    return 10;
}

std::vector<Move> AI::generateCandidateMoves(
    Board& board)
{
    std::vector<Move> moves;

    for (int row = 0;
         row < Board::SIZE;
         ++row)
    {
        for (int col = 0;
             col < Board::SIZE;
             ++col)
        {
            Move move{row, col};

            if (board.isValidMove(move))
            {
                moves.push_back(move);
            }
        }
    }

    return moves;
}