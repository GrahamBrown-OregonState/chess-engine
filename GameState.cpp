#include "GameState.hpp"

std::vector<Move> GameState::generateAllMoves(PieceColor color) {
    std::vector<Move> moves;
    
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece piece = boardState.getPiece(r, c);
            if (piece.pieceType == PieceType::NP) continue;
            if (piece.pieceColor != color) continue;
            
            // similar to markThreats but adds Move to vector
            switch (piece.pieceType) {
                case PieceType::P: {
                    int dir = (color == PieceColor::White) ? -1 : 1;
                    int startRow = (color == PieceColor::White) ? 6 : 1;
                    
                    // forward one
                    if (boardState.getPiece(r + dir, c).pieceType == PieceType::NP) {
                        moves.push_back({r, c, r + dir, c});
                        
                        // forward two from start
                        if (r == startRow && boardState.getPiece(r + 2*dir, c).pieceType == PieceType::NP) {
                            moves.push_back({r, c, r + 2*dir, c});
                        }
                    }
                    
                    // captures
                    for (int dc : {-1, 1}) {
                        int nr = r + dir, nc = c + dc;
                        if (nc >= 0 && nc < 8 && nr >= 0 && nr < 8) {
                            Piece target = boardState.getPiece(nr, nc);
                            if (target.pieceType != PieceType::NP && target.pieceColor != color) {
                                moves.push_back({r, c, nr, nc, target});
                            }
                        }
                    }
                    break;
                }
                case PieceType::N: {
                    int offsets[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
                    for (auto& o : offsets) {
                        int nr = r + o[0], nc = c + o[1];
                        if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                            Piece target = boardState.getPiece(nr, nc);
                            if (target.pieceType == PieceType::NP || target.pieceColor != color) {
                                moves.push_back({r, c, nr, nc, target});
                            }
                        }
                    }
                    break;
                }
                case PieceType::B: {
                    int dirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
                    for (auto& d : dirs) {
                        for (int i = 1; i < 8; i++) {
                            int nr = r + d[0]*i, nc = c + d[1]*i;
                            if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) break;
                            Piece target = boardState.getPiece(nr, nc);
                            if (target.pieceType == PieceType::NP) {
                                moves.push_back({r, c, nr, nc});
                            } else {
                                if (target.pieceColor != color) {
                                    moves.push_back({r, c, nr, nc, target});
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                case PieceType::R: {
                    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
                    for (auto& d : dirs) {
                        for (int i = 1; i < 8; i++) {
                            int nr = r + d[0]*i, nc = c + d[1]*i;
                            if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) break;
                            Piece target = boardState.getPiece(nr, nc);
                            if (target.pieceType == PieceType::NP) {
                                moves.push_back({r, c, nr, nc});
                            } else {
                                if (target.pieceColor != color) {
                                    moves.push_back({r, c, nr, nc, target});
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                case PieceType::Q: {
                    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
                    for (auto& d : dirs) {
                        for (int i = 1; i < 8; i++) {
                            int nr = r + d[0]*i, nc = c + d[1]*i;
                            if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) break;
                            Piece target = boardState.getPiece(nr, nc);
                            if (target.pieceType == PieceType::NP) {
                                moves.push_back({r, c, nr, nc});
                            } else {
                                if (target.pieceColor != color) {
                                    moves.push_back({r, c, nr, nc, target});
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                case PieceType::K: {
                    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
                    for (auto& d : dirs) {
                        int nr = r + d[0], nc = c + d[1];
                        if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                            Piece target = boardState.getPiece(nr, nc);
                            if (target.pieceType == PieceType::NP || target.pieceColor != color) {
                                moves.push_back({r, c, nr, nc, target});
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
    Piece piece = boardState.getPiece(m.fromR, m.fromC);
    m.captured = boardState.getPiece(m.toR, m.toC);
    
    boardState.setPiece(m.toR, m.toC, piece);
    boardState.setPiece(m.fromR, m.fromC, {PieceType::NP, PieceColor::White});
    
    currentTurn = (currentTurn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
}

void GameState::undoMove(Move& m) {
    Piece piece = boardState.getPiece(m.toR, m.toC);
    
    boardState.setPiece(m.fromR, m.fromC, piece);
    boardState.setPiece(m.toR, m.toC, m.captured);
    
    currentTurn = (currentTurn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
}

int GameState::evaluate() {
    // simple material count
    int score = 0;
    int values[] = {0, 100, 320, 330, 500, 900, 20000};  // NP, P, N, B, R, Q, K
    
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece p = boardState.getPiece(r, c);
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
        // checkmate or stalemate - you'd check for check here
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
            if (beta <= alpha) break;  // prune
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
            if (beta <= alpha) break;  // prune
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