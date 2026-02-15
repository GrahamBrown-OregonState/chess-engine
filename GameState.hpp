#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP
#include "ChessBoard.hpp"

struct Move {
    int fromR, fromC;
    int toR, toC;
    Piece captured;  // to undo
    PieceType promotion = PieceType::NP;
    bool isCastle = false;
    bool isEnPassant = false;
};

class GameState {
    private:
        ChessBoard boardState;
        bool threatened[8][8] = {false};
        PieceColor currentTurn; 
    public:
        std::vector<Move> generateAllMoves(PieceColor color);
        void makeMove(Move& m);
        void undoMove(Move& m);
        int evaluate();
        int minimax(int depth, bool isMaximizing, int alpha, int beta);
        Move findBestMove(int depth);

};

#endif