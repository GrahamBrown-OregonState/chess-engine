#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP
#include <vector>
#include <string>
#include <optional>

using namespace std;

enum class PieceType {NP, P, N, B, R, Q, K};
enum class PieceColor {NP, Black, White};

struct Piece {
    PieceType pieceType;
    PieceColor pieceColor;
};

class ChessBoard {
    private:
        Piece board[8][8];
    public:
        ChessBoard();
        void setPiece(int col, int row, Piece piece);
        Piece getPiece(int col, int row);
        
};
#endif