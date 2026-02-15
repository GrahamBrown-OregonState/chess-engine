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