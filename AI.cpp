#include "AI.h"
#include <ctime>
#include <vector>
#include <algorithm>

Cell AI::opponent(Cell player) const
{
    return (player == Cell::X) ? Cell::O : Cell::X;
}

// Returns the cell at (r,c), or a sentinel if out of bounds
static Cell getAt(const Board& board, int r, int c)
{
    if (r < 0 || r >= Board::SIZE ||
        c < 0 || c >= Board::SIZE)
        return Cell::X; // treat out-of-bounds as blocked (non-empty, non-opponent-specific)
    return board.getCell(r, c);
}

static bool isEmpty (const Board& b, int r, int c) { return getAt(b,r,c) == Cell::Empty; }
static bool isPiece (const Board& b, int r, int c, Cell p) { return getAt(b,r,c) == p; }
static bool isOther (const Board& b, int r, int c, Cell p)
{
    Cell v = getAt(b,r,c);
    return v != Cell::Empty && v != p;
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

int AI::scoreCell(int x, int y, Cell ai, const Board& board) const
{
    Cell opp = opponent(ai);
    int score = 0;

    // Cast away const to temporarily place pieces for pattern evaluation
    Board& b = const_cast<Board&>(board);

    // --- Defend: score opponent's threats if THEY played here ---
    b.placeMove({x, y}, opp);
    score += 60000 * liveFour(x, y, opp, b);
    int oppCombo = deadFour(x,y,opp,b) + liveThree(x,y,opp,b) + oneTwo(x,y,opp,b);
    if (oppCombo >= 2 && deadFour(x,y,opp,b) > 0) score += 20000;
    if (oppCombo >= 2 && deadFour(x,y,opp,b) == 0) score += 10000;
    score += 550 * deadFour(x,y,opp,b);
    score += 500 * (liveThree(x,y,opp,b) + oneTwo(x,y,opp,b));
    score += 100 * (deadThree(x,y,opp,b) + liveTwo(x,y,opp,b) + oneOne(x,y,opp,b));
    b.undoMove({x, y});

    // --- Attack: score AI's gain if WE play here ---
    b.placeMove({x, y}, ai);
    if (liveFour(x,y,ai,b) >= 1) score += 100000;
    int aiCombo = deadFour(x,y,ai,b) + liveThree(x,y,ai,b) + oneTwo(x,y,ai,b);
    if (aiCombo >= 2 && deadFour(x,y,ai,b) > 0) score += 80000;
    if (aiCombo >= 2 && deadFour(x,y,ai,b) == 0) score += 50000;
    score += 1000 * (deadThree(x,y,ai,b) + liveTwo(x,y,ai,b) + oneOne(x,y,ai,b));
    b.undoMove({x, y});

    return score;
}

Move AI::findBestMove(Board& board, Cell aiPlayer)
{
    int bestScore = -1;
    std::vector<Move> bestMoves;

    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {
            if (!board.isValidMove({r, c}))
                continue;

            // Immediate win check
            board.placeMove({r,c}, aiPlayer);
            if (board.isWin({r,c}, aiPlayer))
            {
                board.undoMove({r,c});
                return {r, c};
            }
            board.undoMove({r,c});

            int score = scoreCell(r, c, aiPlayer, board);

            if (score > bestScore)
            {
                bestScore = score;
                bestMoves.clear();
                bestMoves.push_back({r, c});
            }
            else if (score == bestScore)
            {
                bestMoves.push_back({r, c});
            }
        }
    }

    if (bestMoves.empty())
        return {Board::SIZE / 2, Board::SIZE / 2};

    srand((unsigned)time(nullptr));
    return bestMoves[rand() % bestMoves.size()];
}