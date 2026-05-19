#include "AI.h"
#include <ctime>
#include <vector>
#include <algorithm>

Cell AI::opponent(Cell player) const {
    return (player == Cell::X) ? Cell::O : Cell::X;
}

static bool isEmpty (const Board& b, int r, int c) {
    if (r < 0 || r >= Board::SIZE || c < 0 || c >= Board::SIZE)
        return false;
    return b.getCell(r, c) == Cell::Empty;
}

static bool isPiece (const Board& b, int r, int c, Cell p) {
    if (r < 0 || r >= Board::SIZE || c < 0 || c >= Board::SIZE)
        return false;
    return b.getCell(r, c) == p;
}

static bool isOther (const Board& b, int r, int c, Cell p) {
    if (r < 0 || r >= Board::SIZE || c < 0 || c >= Board::SIZE)
        return true;
    Cell v = b.getCell(r, c);
    return v != Cell::Empty && v != p;
}

int checkShape(int x, int y, Cell p, const Board& b, const std::vector<Cell>& cells) {
    static const int dx[4]={1,0,1,1}, dy[4]={0,1,1,-1};
    int count = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = -(int)cells.size() + 1; j <= 0; ++j) {
            bool ok = true;
            for (int k = 0; k < (int)cells.size(); ++k) {
                Cell c = cells[k];
                if (c == p) {
                    ok = ok && isPiece(b, x+(j+k)*dx[i], y+(j+k)*dy[i], p);
                } else if (c == Cell::Empty) {
                    ok = ok && isEmpty(b, x+(j+k)*dx[i], y+(j+k)*dy[i]);
                } else {
                    ok = ok && isOther(b, x+(j+k)*dx[i], y+(j+k)*dy[i], p);
                }
            }
            if (ok) {
                ++count;
                break;
            }
        }
    }
    return count;
}

// p p p p p (five in a row)
int AI::fiveInARow(int x, int y, Cell p, const Board& b) const {
    return checkShape(x, y, p, b, {p, p, p, p, p});
}

// _ p p p p _ (live four)
int AI::liveFour(int x, int y, Cell p, const Board& b) const {
    return checkShape(x, y, p, b, {Cell::Empty, p, p, p, p, Cell::Empty});
}

// x p p p p _ or _ p p p p x (dead four)
int AI::deadFour(int x, int y, Cell p, const Board& b) const {
    return checkShape(x, y, p, b, {opponent(p), p, p, p, p, Cell::Empty}) +
           checkShape(x, y, p, b, {Cell::Empty, p, p, p, p, opponent(p)});
}

// _ p p p _ (live three)
int AI::liveThree(int x, int y, Cell p, const Board& b) const {
    return checkShape(x, y, p, b, {Cell::Empty, p, p, p, Cell::Empty});
}

// x p p p _ _ or _ _ p p p x (dead three)
int AI::deadThree(int x, int y, Cell p, const Board& b) const {
    return checkShape(x, y, p, b, {opponent(p), p, p, p, Cell::Empty, Cell::Empty}) +
           checkShape(x, y, p, b, {Cell::Empty, Cell::Empty, p, p, p, opponent(p)});
}

// _ _ p p _ _ (live two)
int AI::liveTwo(int x, int y, Cell p, const Board& b) const {
    return checkShape(x, y, p, b, {Cell::Empty, Cell::Empty, p, p, Cell::Empty, Cell::Empty});
}

// _ p _ p _ (one-one)
int AI::oneOne(int x, int y, Cell p, const Board& b) const {
    return checkShape(x, y, p, b, {Cell::Empty, p, Cell::Empty, p, Cell::Empty});
}

// _ p _ p p _ or _ p p _ p _ (one-two)
int AI::oneTwo(int x, int y, Cell p, const Board& b) const {
    return checkShape(x, y, p, b, {Cell::Empty, p, Cell::Empty, p, p, Cell::Empty}) +
           checkShape(x, y, p, b, {Cell::Empty, p, p, Cell::Empty, p, Cell::Empty});
}

int AI::scoreCell(int x, int y, Cell ai, const Board& board) const {
    Cell opp = opponent(ai);

    // Cast away const to temporarily place pieces for pattern evaluation
    Board& b = const_cast<Board&>(board);

    b.placeMove({x, y}, ai);
    if (fiveInARow(x,y,ai,b)) {
        b.undoMove({x, y});
        return 20000000;
    }

    b.placeMove({x, y}, opp);
    if (fiveInARow(x,y,opp,b)) {
        b.undoMove({x, y});
        return 10000000;
    }
    
    b.placeMove({x, y}, ai);
    if (liveFour(x,y,ai,b) >= 1) {
        b.undoMove({x, y});
        return 5000000;
    }
    
    int ORIGINAL_SCORE[8] = {16, 44, 65, 80, 90, 96, 99, 100};
    int index = std::min(std::min(x, 14 - x), std::min(y, 14 - y));
    int score = ORIGINAL_SCORE[index];
    
    b.placeMove({x, y}, opp);
    if (liveFour(x,y,opp,b) >= 1) score += 500000;
    b.undoMove({x, y});
    
    b.placeMove({x, y}, ai);
    int aiCombo = deadFour(x,y,ai,b) + liveThree(x,y,ai,b) + oneTwo(x,y,ai,b);
    if (aiCombo >= 2) {
        if (deadFour(x,y,ai,b) > 0) score += 1000000;
        else score += 300000;
    } else {
        score += 500000 * deadFour(x,y,ai,b);
        score += 150000 * liveThree(x,y,ai,b);
        score += 100000 * oneTwo(x,y,ai,b);
    }
    b.undoMove({x, y});
    
    b.placeMove({x, y}, opp);
    int oppCombo = deadFour(x,y,opp,b) + liveThree(x,y,opp,b) + oneTwo(x,y,opp,b);
    if (oppCombo >= 2) {
        if (deadFour(x,y,ai,b) > 0) score += 100000;
        else score += 300000;
    } else {
        score += 50000 * deadFour(x,y,opp,b);
        score += 15000 * liveThree(x,y,opp,b);
        score += 10000 * oneTwo(x,y,opp,b);
    }
    b.undoMove({x, y});

    b.placeMove({x, y}, ai);
    score += 1000 * liveTwo(x,y,ai,b);
    score += 900 * deadThree(x,y,ai,b);
    score += 800 * oneOne(x,y,ai,b);
    b.undoMove({x, y});

    b.placeMove({x, y}, opp);
    score += 500 * liveTwo(x,y,opp,b);
    score += 450 * deadThree(x,y,opp,b);
    score += 400 * oneOne(x,y,opp,b);
    b.undoMove({x, y});

    return score;
}

Move AI::findBestMove(Board& board, Cell aiPlayer) {
    int bestScore = -1;
    std::vector<Move> bestMoves;

    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            if (!board.isValidMove({r, c})) continue;

            // Immediate win check
            board.placeMove({r,c}, aiPlayer);
            if (board.isWin({r,c}, aiPlayer)) {
                board.undoMove({r,c});
                return {r, c};
            }
            board.undoMove({r,c});

            int score = scoreCell(r, c, aiPlayer, board);

            if (score > bestScore) {
                bestScore = score;
                bestMoves.clear();
                bestMoves.push_back({r, c});
            } else if (score == bestScore) {
                bestMoves.push_back({r, c});
            }
        }
    }

    if (bestMoves.empty())
        return {Board::SIZE / 2, Board::SIZE / 2};

    srand((unsigned)time(nullptr));
    return bestMoves[rand() % bestMoves.size()];
}