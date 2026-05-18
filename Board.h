#pragma once

#include <array>
#include <iostream>

enum class Cell {
    Empty,
    X,
    O
};

struct Move {
    int row;
    int col;
};

class Board {
public:
    static constexpr int SIZE = 15;

    Board();

    bool isValidMove(const Move& move) const;
    void placeMove(const Move& move, Cell player);
    void undoMove(const Move& move);

    bool isWin(const Move& move, Cell player) const;
    bool isFull() const;

    void print() const;

    Cell getCell(int row, int col) const;

private:
    std::array<std::array<Cell, SIZE>, SIZE> grid;

    bool inBounds(int row, int col) const;

    int countDirection(int row, int col, int dRow, int dCol, Cell player) const;
};