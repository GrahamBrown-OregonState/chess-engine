#include "GameState.hpp"
#include <algorithm>

GameState::GameState() : currentTurn(PieceColor::White) {}

void GameState::reset() {
    boardState = ChessBoard();
    currentTurn = PieceColor::White;
}

PieceColor GameState::getCurrentTurn() const {
    return currentTurn;
}

Piece GameState::getPieceAt(int col, int row) const {
    return boardState.getPiece(col, row);
}

std::vector<Move> GameState::generateAllMoves(PieceColor color) {
    std::vector<Move> moves;

    // col = file (a-h = 0-7), row = rank (1-8 = 0-7)
    // getPiece(col, row) => board[col][row]
    for (int col = 0; col < 8; col++) {
        for (int row = 0; row < 8; row++) {
            Piece piece = boardState.getPiece(col, row);
            if (piece.pieceType == PieceType::NP) continue;
            if (piece.pieceColor != color) continue;

            switch (piece.pieceType) {
                // generate pawn moves
                case PieceType::P: {
                    // White pawns at row 1 advance toward row 7 (dir=+1)
                    // Black pawns at row 6 advance toward row 0 (dir=-1)
                    int dir = (color == PieceColor::White) ? 1 : -1;
                    int startRow = (color == PieceColor::White) ? 1 : 6;

                    // forward one (row changes, col stays)
                    int nr = row + dir;
                    if (nr >= 0 && nr < 8 && boardState.getPiece(col, nr).pieceType == PieceType::NP) {
                        moves.push_back({col, row, col, nr});

                        // forward two from start
                        int nr2 = row + 2 * dir;
                        if (row == startRow && boardState.getPiece(col, nr2).pieceType == PieceType::NP) {
                            moves.push_back({col, row, col, nr2});
                        }
                    }

                    // diagonal captures (col changes by +-1, row changes by dir)
                    for (int dc : {-1, 1}) {
                        int nc = col + dc;
                        int nr2 = row + dir;
                        if (nc >= 0 && nc < 8 && nr2 >= 0 && nr2 < 8) {
                            Piece target = boardState.getPiece(nc, nr2);
                            if (target.pieceType != PieceType::NP && target.pieceColor != color) {
                                moves.push_back({col, row, nc, nr2, target});
                            }
                        }
                    }
                    break;
                }
                // generate knight moves
                case PieceType::N: {
                    int offsets[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
                    for (auto& o : offsets) {
                        int nc = col + o[0], nr = row + o[1];
                        if (nc >= 0 && nc < 8 && nr >= 0 && nr < 8) {
                            Piece target = boardState.getPiece(nc, nr);
                            if (target.pieceType == PieceType::NP || target.pieceColor != color) {
                                moves.push_back({col, row, nc, nr, target});
                            }
                        }
                    }
                    break;
                }
                // generate bishop moves
                case PieceType::B: {
                    int dirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
                    for (auto& d : dirs) {
                        for (int i = 1; i < 8; i++) {
                            int nc = col + d[0]*i, nr = row + d[1]*i;
                            if (nc < 0 || nc >= 8 || nr < 0 || nr >= 8) break;
                            Piece target = boardState.getPiece(nc, nr);
                            if (target.pieceType == PieceType::NP) {
                                moves.push_back({col, row, nc, nr});
                            } else {
                                if (target.pieceColor != color) {
                                    moves.push_back({col, row, nc, nr, target});
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                // generate rook moves
                case PieceType::R: {
                    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
                    for (auto& d : dirs) {
                        for (int i = 1; i < 8; i++) {
                            int nc = col + d[0]*i, nr = row + d[1]*i;
                            if (nc < 0 || nc >= 8 || nr < 0 || nr >= 8) break;
                            Piece target = boardState.getPiece(nc, nr);
                            if (target.pieceType == PieceType::NP) {
                                moves.push_back({col, row, nc, nr});
                            } else {
                                if (target.pieceColor != color) {
                                    moves.push_back({col, row, nc, nr, target});
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                // generate queen moves
                case PieceType::Q: {
                    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
                    for (auto& d : dirs) {
                        for (int i = 1; i < 8; i++) {
                            int nc = col + d[0]*i, nr = row + d[1]*i;
                            if (nc < 0 || nc >= 8 || nr < 0 || nr >= 8) break;
                            Piece target = boardState.getPiece(nc, nr);
                            if (target.pieceType == PieceType::NP) {
                                moves.push_back({col, row, nc, nr});
                            } else {
                                if (target.pieceColor != color) {
                                    moves.push_back({col, row, nc, nr, target});
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                // generate knight moves
                case PieceType::K: {
                    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
                    for (auto& d : dirs) {
                        int nc = col + d[0], nr = row + d[1];
                        if (nc >= 0 && nc < 8 && nr >= 0 && nr < 8) {
                            Piece target = boardState.getPiece(nc, nr);
                            if (target.pieceType == PieceType::NP || target.pieceColor != color) {
                                moves.push_back({col, row, nc, nr, target});
                            }
                        }
                    }
                    break;
                }
                case PieceType::NP:
                    break;
            }
        }
    }

    return moves;
}

void GameState::makeMove(Move& m) {
    Piece piece = boardState.getPiece(m.fromCol, m.fromRow);
    m.captured = boardState.getPiece(m.toCol, m.toRow);

    // Handle pawn promotion
    if (piece.pieceType == PieceType::P && m.promotion != PieceType::NP) {
        boardState.setPiece(m.toCol, m.toRow, {m.promotion, piece.pieceColor});
    } else {
        boardState.setPiece(m.toCol, m.toRow, piece);
    }
    boardState.setPiece(m.fromCol, m.fromRow, {PieceType::NP, PieceColor::NP});

    currentTurn = (currentTurn == PieceColor::White) 
        ? PieceColor::Black : PieceColor::White;
}

void GameState::undoMove(Move& m) {
    Piece piece = boardState.getPiece(m.toCol, m.toRow);

    // If this was a promotion, restore the pawn
    if (m.promotion != PieceType::NP) {
        boardState.setPiece(m.fromCol, m.fromRow, {PieceType::P, piece.pieceColor});
    } else {
        boardState.setPiece(m.fromCol, m.fromRow, piece);
    }
    boardState.setPiece(m.toCol, m.toRow, m.captured);

    currentTurn = (currentTurn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
}

int GameState::evaluate() {
    // simple material count
    int score = 0;
    int values[] = {0, 100, 320, 330, 500, 900, 20000};  // NP, P, N, B, R, Q, K

    for (int col = 0; col < 8; col++) {
        for (int row = 0; row < 8; row++) {
            Piece p = boardState.getPiece(col, row);
            if (p.pieceType == PieceType::NP) continue;

            int val = values[static_cast<int>(p.pieceType)];
            score += (p.pieceColor == PieceColor::White) ? val : -val;
        }
    }

    return score;
}

int GameState::minimax(int depth, bool isMaximizing, int alpha, int beta) {
    if (depth == 0) {
        return evaluate();
    }

    PieceColor color = isMaximizing ? PieceColor::White : PieceColor::Black;
    std::vector<Move> moves = generateAllMoves(color);

    if (moves.empty()) {
        return isMaximizing ? -100000 : 100000;
    }

    if (isMaximizing) {
        int maxEval = -999999;
        for (Move& m : moves) {
            makeMove(m);
            int eval = minimax(depth - 1, false, alpha, beta);
            undoMove(m);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = 999999;
        for (Move& m : moves) {
            makeMove(m);
            int eval = minimax(depth - 1, true, alpha, beta);
            undoMove(m);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

Move GameState::findBestMove(int depth) {
    std::vector<Move> moves = generateAllMoves(currentTurn);
    Move bestMove;
    bool isMaximizing = (currentTurn == PieceColor::White);
    int bestEval = isMaximizing ? -999999 : 999999;

    for (Move& m : moves) {
        makeMove(m);
        int eval = minimax(depth - 1, !isMaximizing, -999999, 999999);
        undoMove(m);

        if (isMaximizing && eval > bestEval) {
            bestEval = eval;
            bestMove = m;
        } else if (!isMaximizing && eval < bestEval) {
            bestEval = eval;
            bestMove = m;
        }
    }

    return bestMove;
}
