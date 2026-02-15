#include "ChessBoard.hpp"

ChessBoard::ChessBoard() {
    // set white row of pawns
    Piece whitePawn = {PieceType::P, PieceColor::White};
    for (int i = 0; i < 8; i++) {
        setPiece(i, 1, whitePawn);
    }

    // set black row of pawns
    Piece blackPawn = {PieceType::P, PieceColor::Black};
    for (int i = 0; i < 8; i++) {
        setPiece(i, 6, blackPawn);
    }

    // set white rooks
    Piece whiteRook = {PieceType::R, PieceColor::White};
    this->board[0][0] = whiteRook;
    this->board[7][0] = whiteRook;

    // set black rooks
    Piece blackRook = {PieceType::R, PieceColor::Black};
    this->board[0][7] = blackRook;
    this->board[7][7] = blackRook;

    // set white knights
    Piece whiteKnight = {PieceType::N, PieceColor::White};
    this->board[1][0] = whiteKnight;
    this->board[6][0] = whiteKnight;

    // set black knights
    Piece blackKnight = {PieceType::N, PieceColor::Black};
    this->board[1][7] = blackKnight;
    this->board[6][7] = blackKnight;

    // set white bishops
    Piece whiteBishop = {PieceType::B, PieceColor::White};
    this->board[2][0] = whiteBishop;
    this->board[5][0] = whiteBishop;

    // set black bishops
    Piece blackBishop = {PieceType::B, PieceColor::Black};
    this->board[2][7] = blackBishop;
    this->board[5][7] = blackBishop;

    // set white queen
    Piece whiteQueen = {PieceType::Q, PieceColor::White};
    this->board[3][0] = whiteQueen;

    // set black queen
    Piece blackQueen = {PieceType::Q, PieceColor::Black};
    this->board[3][7] = blackQueen;

    // set white king
    Piece whiteKing = {PieceType::K, PieceColor::White};
    this->board[4][0] = whiteKing;

    // set black king
    Piece blackKing = {PieceType::K, PieceColor::Black};
    this->board[4][7] = blackKing;


    Piece noPiece = {PieceType::NP, PieceColor::NP};
    for (int i = 0; i < 8; i++) {
        for (int j = 2; j < 6; j++) {
            setPiece(i, j, noPiece);
        }
    }
}

void ChessBoard::setPiece(int col, int row, Piece piece) {
    this->board[col][row] = piece;
}

Piece ChessBoard::getPiece(int col, int row) {
    return this->board[col][row];
}
