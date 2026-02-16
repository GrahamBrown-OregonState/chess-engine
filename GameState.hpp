#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP
#include "ChessBoard.hpp"

struct Move {
    int fromCol, fromRow;
    int toCol, toRow;
    Piece captured;  // to undo
    PieceType promotion = PieceType::NP;
    bool isCastle = false;
    bool isEnPassant = false;
};

class GameState {
    private:
        ChessBoard boardState;
        PieceColor currentTurn;
    public:
        GameState();
        void reset();
        std::vector<Move> generateAllMoves(PieceColor color);
        void makeMove(Move& m);
        void undoMove(Move& m);
        int evaluate();
        int minimax(int depth, bool isMaximizing, int alpha, int beta);
        Move findBestMove(int depth);
        PieceColor getCurrentTurn() const;
        Piece getPieceAt(int col, int row) const;
};

#endif