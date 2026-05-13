#pragma once

#include "AI.h"
#include "Board.h"


class Game {
public:
    void run();

private:
    void processTurn();
    void handleHumanMove();
    void handleAIMove();

    bool gameOver = false;
    Cell currentPlayer;

    Board board;
    AI ai;
};