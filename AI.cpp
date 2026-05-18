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

// p p p p p (five in a row)
int AI::fiveInARow(int x, int y, Cell p, const Board& b) const
{
    static const int dx[4]={1,0,1,1}, dy[4]={0,1,1,-1};
    int count = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = -4; j <= 0; ++j)
        {
            // pattern: p, p, p, p, p  (5 cells)
            bool ok = true;
            for (int k = 0; k <= 4; ++k)
                if (!isPiece(b, x+(j+k)*dx[i], y+(j+k)*dy[i], p)) { ok=false; break; }
            if (ok) { ++count; break; }
        }
    }
    return count;
}

// _ p p p p _ (live four)
int AI::liveFour(int x, int y, Cell p, const Board& b) const
{
    static const int dx[4]={1,0,1,1}, dy[4]={0,1,1,-1};
    int count = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = -5; j <= 0; ++j)
        {
            // pattern: empty, p, p, p, p, empty  (6 cells)
            int r0=x+(j)*dx[i],   c0=y+(j)*dy[i];
            int r5=x+(j+5)*dx[i], c5=y+(j+5)*dy[i];
            if (!isEmpty(b,r0,c0)) continue;
            if (!isEmpty(b,r5,c5)) continue;
            bool ok = true;
            for (int k = 1; k <= 4; ++k)
                if (!isPiece(b, x+(j+k)*dx[i], y+(j+k)*dy[i], p)) { ok=false; break; }
            if (ok) { ++count; break; }
        }
    }
    return count;
}

// x p p p p _ or _ p p p p x (dead four)
int AI::deadFour(int x, int y, Cell p, const Board& b) const
{
    static const int dx[4]={1,0,1,1}, dy[4]={0,1,1,-1};
    int count = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = -5; j <= 0; ++j)
        {
            // x p p p p _
            {
                bool ok = isOther(b, x+j*dx[i], y+j*dy[i], p);
                for (int k=1;k<=4&&ok;++k)
                    ok = isPiece(b, x+(j+k)*dx[i], y+(j+k)*dy[i], p);
                ok = ok && isEmpty(b, x+(j+5)*dx[i], y+(j+5)*dy[i]);
                if (ok) { ++count; break; }
            }
            // _ p p p p x
            {
                bool ok = isEmpty(b, x+j*dx[i], y+j*dy[i]);
                for (int k=1;k<=4&&ok;++k)
                    ok = isPiece(b, x+(j+k)*dx[i], y+(j+k)*dy[i], p);
                ok = ok && isOther(b, x+(j+5)*dx[i], y+(j+5)*dy[i], p);
                if (ok) { ++count; break; }
            }
        }
    }
    return count;
}

// _ p p p _ (live three)
int AI::liveThree(int x, int y, Cell p, const Board& b) const
{
    static const int dx[4]={1,0,1,1}, dy[4]={0,1,1,-1};
    int count = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = -4; j <= 0; ++j)
        {
            bool ok = isEmpty(b, x+j*dx[i], y+j*dy[i]);
            for (int k=1;k<=3&&ok;++k)
                ok = isPiece(b, x+(j+k)*dx[i], y+(j+k)*dy[i], p);
            ok = ok && isEmpty(b, x+(j+4)*dx[i], y+(j+4)*dy[i]);
            if (ok) { ++count; break; }
        }
    }
    return count;
}

// x _ p p p _ x style (dead three) — blocked on one side
int AI::deadThree(int x, int y, Cell p, const Board& b) const
{
    static const int dx[4]={1,0,1,1}, dy[4]={0,1,1,-1};
    int count = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = -5; j <= 0; ++j)
        {
            // x _ _ p p p
            {
                bool ok = isOther(b,x+j*dx[i],y+j*dy[i],p)
                       && isEmpty(b,x+(j+1)*dx[i],y+(j+1)*dy[i])
                       && isEmpty(b,x+(j+2)*dx[i],y+(j+2)*dy[i]);
                for (int k=3;k<=5&&ok;++k)
                    ok = isPiece(b,x+(j+k)*dx[i],y+(j+k)*dy[i],p);
                if (ok) { ++count; break; }
            }
            // p p p _ _ x
            {
                bool ok = true;
                for (int k=0;k<=2&&ok;++k)
                    ok = isPiece(b,x+(j+k)*dx[i],y+(j+k)*dy[i],p);
                ok = ok && isEmpty(b,x+(j+3)*dx[i],y+(j+3)*dy[i])
                        && isEmpty(b,x+(j+4)*dx[i],y+(j+4)*dy[i])
                        && isOther(b,x+(j+5)*dx[i],y+(j+5)*dy[i],p);
                if (ok) { ++count; break; }
            }
        }
    }
    return count;
}

// _ _ p p _ _ (live two)
int AI::liveTwo(int x, int y, Cell p, const Board& b) const
{
    static const int dx[4]={1,0,1,1}, dy[4]={0,1,1,-1};
    int count = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = -5; j <= 0; ++j)
        {
            bool ok = isEmpty(b,x+j*dx[i],y+j*dy[i])
                   && isEmpty(b,x+(j+1)*dx[i],y+(j+1)*dy[i])
                   && isPiece(b,x+(j+2)*dx[i],y+(j+2)*dy[i],p)
                   && isPiece(b,x+(j+3)*dx[i],y+(j+3)*dy[i],p)
                   && isEmpty(b,x+(j+4)*dx[i],y+(j+4)*dy[i])
                   && isEmpty(b,x+(j+5)*dx[i],y+(j+5)*dy[i]);
            if (ok) { ++count; break; }
        }
    }
    return count;
}

// _ p _ p _ (one-one)
int AI::oneOne(int x, int y, Cell p, const Board& b) const
{
    static const int dx[4]={1,0,1,1}, dy[4]={0,1,1,-1};
    int count = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = -4; j <= 0; ++j)
        {
            bool ok = isEmpty(b,x+j*dx[i],y+j*dy[i])
                   && isPiece(b,x+(j+1)*dx[i],y+(j+1)*dy[i],p)
                   && isEmpty(b,x+(j+2)*dx[i],y+(j+2)*dy[i])
                   && isPiece(b,x+(j+3)*dx[i],y+(j+3)*dy[i],p)
                   && isEmpty(b,x+(j+4)*dx[i],y+(j+4)*dy[i]);
            if (ok) { ++count; break; }
        }
    }
    return count;
}

// _ p _ p p _ or _ p p _ p _ (one-two)
int AI::oneTwo(int x, int y, Cell p, const Board& b) const
{
    static const int dx[4]={1,0,1,1}, dy[4]={0,1,1,-1};
    int count = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = -5; j <= 0; ++j)
        {
            // _ p _ p p _
            {
                bool ok = isEmpty(b,x+j*dx[i],y+j*dy[i])
                       && isPiece(b,x+(j+1)*dx[i],y+(j+1)*dy[i],p)
                       && isEmpty(b,x+(j+2)*dx[i],y+(j+2)*dy[i])
                       && isPiece(b,x+(j+3)*dx[i],y+(j+3)*dy[i],p)
                       && isPiece(b,x+(j+4)*dx[i],y+(j+4)*dy[i],p)
                       && isEmpty(b,x+(j+5)*dx[i],y+(j+5)*dy[i]);
                if (ok) { ++count; break; }
            }
            // _ p p _ p _
            {
                bool ok = isEmpty(b,x+j*dx[i],y+j*dy[i])
                       && isPiece(b,x+(j+1)*dx[i],y+(j+1)*dy[i],p)
                       && isPiece(b,x+(j+2)*dx[i],y+(j+2)*dy[i],p)
                       && isEmpty(b,x+(j+3)*dx[i],y+(j+3)*dy[i])
                       && isPiece(b,x+(j+4)*dx[i],y+(j+4)*dy[i],p)
                       && isEmpty(b,x+(j+5)*dx[i],y+(j+5)*dy[i]);
                if (ok) { ++count; break; }
            }
        }
    }
    return count;
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