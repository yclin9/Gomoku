#include "Board.h"

Board::Board() {
    for (auto& row : grid) {
        row.fill(Cell::Empty);
    }
}

bool Board::inBounds(int row, int col) const {
    return row >= 0 && row < SIZE &&
           col >= 0 && col < SIZE;
}

bool Board::isValidMove(const Move& move) const {
    return inBounds(move.row, move.col) &&
           grid[move.row][move.col] == Cell::Empty;
}

void Board::placeMove(const Move& move, Cell player) {
    grid[move.row][move.col] = player;
}

void Board::undoMove(const Move& move) {
    grid[move.row][move.col] = Cell::Empty;
}

int Board::countDirection(int row, int col, int dRow, int dCol, Cell player) const {
    int count = 0;

    while (inBounds(row, col) && grid[row][col] == player) {
        ++count;
        row += dRow;
        col += dCol;
    }

    return count;
}

bool Board::isWin(const Move& move, Cell player) const {
    static const int dirs[4][2] = {
        {1, 0},   // vertical
        {0, 1},   // horizontal
        {1, 1},   // diagonal ul -> dr
        {1, -1}   // diagonal ur -> dl
    };

    for (auto& d : dirs) {
        int dRow = d[0];
        int dCol = d[1];

        int total =
            1 +
            countDirection(move.row + dRow,
                           move.col + dCol,
                           dRow, dCol, player)
            +
            countDirection(move.row - dRow,
                           move.col - dCol,
                           -dRow, -dCol, player);

        if (total >= 5) return true;
    }

    return false;
}

bool Board::isFull() const {
    for (const auto& row : grid) {
        for (Cell c : row) {
            if (c == Cell::Empty)
                return false;
        }
    }
    return true;
}

void Board::print() const {
    std::cout << "\n   ";

    for (int c = 0; c < SIZE; ++c) {
        std::cout << static_cast<char>('A' + c) << ' ';
    }

    std::cout << "\n";

    for (int r = 0; r < SIZE; ++r) {
        int displayRow = SIZE - r;

        if (displayRow < 10)
            std::cout << ' ';

        std::cout << displayRow << ' ';

        for (int c = 0; c < SIZE; ++c) {
            char symbol = '.';

            if (grid[r][c] == Cell::X) {
                symbol = 'X';
            } else if (grid[r][c] == Cell::O){
                symbol = 'O';
            }

            std::cout << symbol << ' ';
        }

        std::cout << "\n";
    }

    std::cout << "   ";

    for (int c = 0; c < SIZE; ++c) {
        std::cout << static_cast<char>('A' + c) << ' ';
    }

    std::cout << '\n';
}

Cell Board::getCell(int row, int col) const {
    return grid[row][col];
}