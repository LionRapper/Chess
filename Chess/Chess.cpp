#include <iostream>
#include <cstdlib>

using namespace std;

struct Piece {
    int type;
    bool isWhite;
    int x;
    int y;
    bool justMovedTwo;
    bool hasMoved;
};

enum PieceType { NONE, PAWN, KNIGHT, ROOK, BISHOP, QUEEN, KING };

Piece board[8][8];

void initializeBoard(Piece board[8][8]);
Piece createPiece(PieceType type, bool isWhite, int x, int y);

void ShowBoard(Piece board[8][8]);
void OutPiece(Piece piece);

void MovePiece(Piece board[8][8],int movePiece);
bool IsLegal(Piece board[8][8],int movePiece,int round);

bool Pawn(Piece board[8][8], int x1, int y1, int x2, int y2);
bool Knight(Piece board[8][8], int x1, int y1, int x2, int y2);
bool Rook(Piece board[8][8], int x1, int y1, int x2, int y2);
bool Bishop(Piece board[8][8], int x1, int y1, int x2, int y2);
bool Queen(Piece board[8][8], int x1, int y1, int x2, int y2);
bool King(Piece board[8][8], int x1, int y1, int x2, int y2);

bool IsInCheck(Piece king, Piece board[8][8]);
Piece findKing(bool isWhite, Piece board[8][8]);

int main() {
    initializeBoard(board);

    int round = 1;
    do {
        int movePiece;

        cout << "round: " << round << endl;

        ShowBoard(board);

        Piece king = findKing(round % 2 == 1, board);
        if (IsInCheck(king, board)) {
            cout << "King is in check" << endl;
        }

        if (round % 2 == 1)
            cout << endl << "White to Move";
        else
            cout << endl << "Black to Move";

        bool legal = true;
        do {
            cout << endl << "xyxy Move from to: ";
            cin >> movePiece;
            if (IsLegal(board, movePiece, round) == false) {
                legal = false;
                cout << "ILLEGAL MOVE!";
            }
            else {
                legal = true;
            }
        } while (legal == false);
        MovePiece(board, movePiece);

        round++;
        system("CLS");
    } while (true);

    return 0;
}

void initializeBoard(Piece board[8][8]) {
    for (int i = 2; i < 6; i++) {
        for (int j = 0; i < 8; i++) {
            board[i][j] = createPiece(NONE, false, j, i);
        }
    }
    for (int i = 0; i < 8; i++) {
        board[1][i] = createPiece(PAWN, false, i, 1); // black 
        board[6][i] = createPiece(PAWN, true, i, 6); // white
    }
    board[0][0] = createPiece(ROOK, false, 0, 0); // black
    board[0][7] = createPiece(ROOK, false, 7, 0); 
    board[0][1] = createPiece(KNIGHT, false, 1, 0); 
    board[0][6] = createPiece(KNIGHT, false, 6, 0); 
    board[0][2] = createPiece(BISHOP, false, 2, 0); 
    board[0][5] = createPiece(BISHOP, false, 5, 0); 
    board[0][3] = createPiece(QUEEN, false, 3, 0); 
    board[0][4] = createPiece(KING, false, 4, 0); 
    board[7][0] = createPiece(ROOK, true, 0, 7); // white
    board[7][7] = createPiece(ROOK, true, 7, 7); 
    board[7][1] = createPiece(KNIGHT, true, 1, 7); 
    board[7][6] = createPiece(KNIGHT, true, 6, 7); 
    board[7][2] = createPiece(BISHOP, true, 2, 7); 
    board[7][5] = createPiece(BISHOP, true, 5, 7); 
    board[7][3] = createPiece(QUEEN, true, 3, 7); 
    board[7][4] = createPiece(KING, true, 4, 7); 
}

Piece createPiece(PieceType type, bool isWhite, int x, int y) {
    Piece piece;
    piece.type = type;
    piece.isWhite = isWhite;
    piece.x = x;
    piece.y = y;
    piece.justMovedTwo = false;
    piece.hasMoved = false;
    return piece;
}


void ShowBoard(Piece board[8][8]) {
    for (int x = 0; x < 8; x++) {
        cout << 8 - x << "\t";
        for (int y = 0; y < 8; y++) {
            OutPiece(board[x][y]);
            cout << "\t";
        }
        cout << endl << endl;
    }
    for (int y = 0; y < 8; y++) {
        cout << "\t" << y + 1;
    }
}

void OutPiece(Piece piece) {
    switch (piece.type) {
    case 0:
        cout << "x ";
        break;
    case 1:
        cout << "P";
        break;
    case 2:
        cout << "N";
        break;
    case 3:
        cout << "R";
        break;
    case 4:
        cout << "B";
        break;
    case 5:
        cout << "Q";
        break;
    case 6:
        cout << "K";
        break;
    }
    // white or black
    if (piece.isWhite)
        cout << "w";
    else
        cout << "b";
}

void MovePiece(Piece board[8][8], int movePiece) {
    int x1 = 8 - (movePiece / 1000);
    int y1 = ((movePiece / 100) % 10) - 1;
    int x2 = 8 - ((movePiece % 100) / 10);
    int y2 = (movePiece % 10) - 1;

    // reset justMovedTwo for all pawns
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].type == 1) {
                board[i][j].justMovedTwo = false;
            }
        }
    }

    // set justMovedTwo for the moved pawn
    if (board[x1][y1].type == 1 && abs(x1 - x2) == 2) {
        board[x2][y2].justMovedTwo = true;
    }

    board[x2][y2].type = board[x1][y1].type;
    //board[x1][y1].type = NONE;
}


bool IsLegal(Piece board[8][8], int movePiece, int round) {
    int x1 = 8 - (movePiece / 1000);
    int y1 = ((movePiece / 100) % 10) - 1;
    int x2 = 8 - ((movePiece % 100) / 10);
    int y2 = (movePiece % 10) - 1;

    // check if the selected piece is the correct color
    if (board[x1][y1].isWhite != (round % 2 == 1)) {
        return false;
    }
    cout << "Is correct collor, ";

    // check if the destination is a valid location
    if (x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7) {
        return false;
    }

    cout << "dest is in board, ";

    // check if the destination is occupied by a piece of the same color
    if (board[x1][y1].isWhite == board[x2][y2].isWhite && board[x2][y2].type != 0) {
        return false;
    }

    cout << "dest in is not occ by same team piece, ";

    // check if the move is legal based on the type of piece
    switch (board[x1][y1].type) {
    case 1:
        cout << "starting pawn check, ";
        if (!Pawn(board, x1, y1, x2, y2)) {
            return false;
        }
        break;
    case 2:
        if (!Knight(board, x1, y1, x2, y2)) {
            return false;
        }
        break;
    case 3:
        if (!Rook(board, x1, y1, x2, y2)) {
            return false;
        }
        break;
    case 4:
        if (!Bishop(board, x1, y1, x2, y2)) {
            return false;
        }
        break;
    case 5:
        if (!Queen(board, x1, y1, x2, y2)) {
            return false;
        }
        break;
    case 6:
        if (!King(board, x1, y1, x2, y2)) {
            return false;
        }
        break;
    }
    // check if the move will put the current player's king in check
    Piece king = findKing(round % 2 == 1, board);
    
    board[x2][y2] = board[x1][y1];
    board[x1][y1].type = 0;

    if (IsInCheck(king, board)) {
        return false;
    }

    return true;
}

bool Pawn(Piece board[8][8], int x1, int y1, int x2, int y2) {
    // check if the move is within the pawn's normal range
    if (board[x1][y1].isWhite) {
        if (x1 == 6 && (x2 == 4 || x2 == 5) && y1 == y2) {
            return true;
        }
        if (x1 - x2 == 1 && y1 == y2) {
            return true;
        }
        if (x1 - x2 == 1 && abs(y1 - y2) == 1 && board[x2][y2].type != 0) {
            return true;
        }
        // en passant
        if (x1 == 3 && x2 == 2 && abs(y1 - y2) == 1 && board[x2][y1].type == 1 && board[x2][y1].justMovedTwo) {
            return true;
        }
    }
    else {
        if (x1 == 1 && (x2 == 2 || x2 == 3) && y1 == y2) {
            return true;
        }
        if (x2 - x1 == 1 && y1 == y2) {
            return true;
        }
        if (x2 - x1 == 1 && abs(y1 - y2) == 1 && board[x2][y2].type != 0) {
            return true;
        }
        // en passant
        if (x1 == 4 && x2 == 5 && abs(y1 - y2) == 1 && board[x2][y1].type == 1 && board[x2][y1].justMovedTwo) {
            return true;
        }
    }
    return false;
}


bool Knight(Piece board[8][8], int x1, int y1, int x2, int y2) {
    if ((abs(x1 - x2) == 2 && abs(y1 - y2) == 1) || (abs(x1 - x2) == 1 && abs(y1 - y2) == 2)) {
        if (board[x2][y2].isWhite != board[x1][y1].isWhite || board[x2][y2].type == 0) {
            return true;
        }
    }
    return false;
}

bool Rook(Piece board[8][8], int x1, int y1, int x2, int y2) {
    if (x1 != x2 && y1 != y2) { // move is not along a rank or file
        return false;
    }
    if (x1 == x2) { // move is along a file
        int y_dir = (y1 < y2) ? 1 : -1;
        for (int y = y1 + y_dir; y != y2; y += y_dir) {
            if (board[x1][y].type != 0) {
                return false;
            }
        }
    }
    else { // move is along a rank
        int x_dir = (x1 < x2) ? 1 : -1;
        for (int x = x1 + x_dir; x != x2; x += x_dir) {
            if (board[x][y1].type != 0) {
                return false;
            }
        }
    }
    if (board[x2][y2].isWhite == board[x1][y1].isWhite && board[x2][y2].type != 0) {
        return false;
    }
    return true;
}


bool Bishop(Piece board[8][8], int x1, int y1, int x2, int y2) {
    if (abs(x1 - x2) != abs(y1 - y2)) { // move is not diagonal
        return false;
    }
    int x_dir = (x1 < x2) ? 1 : -1;
    int y_dir = (y1 < y2) ? 1 : -1;
    for (int x = x1 + x_dir, y = y1 + y_dir; x != x2; x += x_dir, y += y_dir) {
        if (board[x][y].type != 0) {
            return false;
        }
    }
    if (board[x2][y2].isWhite == board[x1][y1].isWhite && board[x2][y2].type != 0) {
        return false;
    }
    return true;
}

bool Queen(Piece board[8][8], int x1, int y1, int x2, int y2) {
    if (Rook(board, x1, y1, x2, y2) || Bishop(board, x1, y1, x2, y2)) {
        return true;
    }
    return false;
}

bool King(Piece board[8][8], int x1, int y1, int x2, int y2) {
    if (abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1) { // check if the king is moving to a neighboring square
        // check if the king is in check before making the move
        Piece king = findKing(board[x1][y1].isWhite, board);
        if (IsInCheck(king, board)) {
            // check if the move will put the king in check
            Piece temp = board[x2][y2];
            board[x2][y2] = board[x1][y1];
            board[x1][y1] = createPiece(NONE, false, x1, y1);
            if (IsInCheck(king, board)) {
                board[x1][y1] = board[x2][y2];
                board[x2][y2] = temp;
                return false;
            }
            board[x1][y1] = board[x2][y2];
            board[x2][y2] = temp;
        }
        return true;
    }
    else if (abs(x1 - x2) == 2 && abs(y1 - y2) == 0 && !board[x1][y1].hasMoved) {
        // check if the move is a castling move
        if (x1 < x2) { // check if castling kingside
            if (board[7][7].hasMoved || board[7][5].type != 0 || board[7][6].type != 0) {
                return false;
            }
        }
        else { // check if castling queenside
            if (board[7][0].hasMoved || board[7][1].type != 0 || board[7][2].type != 0 || board[7][3].type != 0) {
                return false;
            }
        }
        // check if the king is in check before making the move
        Piece king = findKing(board[x1][y1].isWhite, board);
        if (IsInCheck(king, board)) {
            return false;
        }
        return true;
    }
    else {
        return false;
    }
}



Piece findKing(bool isWhite, Piece board[8][8]) {
    Piece king;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].isWhite == isWhite && board[i][j].type == 6) {
                king.x = i;
                king.y = j;
                king.isWhite = isWhite;
                king.type = 6;
                return king;
            }
        }
    }
}


bool IsInCheck(Piece king, Piece board[8][8]) {
    // check for threats from pawns
    if (king.isWhite) {
        if (king.x > 0 && king.y > 0 && board[king.x - 1][king.y - 1].type == 1 && !board[king.x - 1][king.y - 1].isWhite) {
            return true;
        }
        if (king.x > 0 && king.y < 7 && board[king.x - 1][king.y + 1].type == 1 && !board[king.x - 1][king.y + 1].isWhite) {
            return true;
        }
    }
    else {
        if (king.x < 7 && king.y > 0 && board[king.x + 1][king.y - 1].type == 1 && board[king.x + 1][king.y - 1].isWhite) {
            return true;
        }
        if (king.x < 7 && king.y < 7 && board[king.x + 1][king.y + 1].type == 1 && board[king.x + 1][king.y + 1].isWhite) {
            return true;
        }
    }
    // check for threats from knights
    if (king.x > 1 && king.y > 0 && board[king.x - 2][king.y - 1].type == 2 && board[king.x - 2][king.y - 1].isWhite != king.isWhite) {
        return true;
    }
    if (king.x > 1 && king.y < 7 && board[king.x - 2][king.y + 1].type == 2 && board[king.x - 2][king.y + 1].isWhite != king.isWhite) {
        return true;
    }
    if (king.x > 0 && king.y > 1 && board[king.x - 1][king.y - 2].type == 2 && board[king.x - 1][king.y - 2].isWhite != king.isWhite) {
        return true;
    }
    if (king.x > 0 && king.y < 6 && board[king.x - 1][king.y + 2].type == 2 && board[king.x - 1][king.y + 2].isWhite != king.isWhite) {
        return true;
    }
    if (king.x < 6 && king.y > 0 && board[king.x + 2][king.y - 1].type == 2 && board[king.x + 2][king.y - 1].isWhite != king.isWhite) {
        return true;
    }
    if (king.x < 6 && king.y < 7 && board[king.x + 2][king.y + 1].type == 2 && board[king.x + 2][king.y + 1].isWhite != king.isWhite) {
        return true;
    }
    if (king.x < 7 && king.y > 1 && board[king.x + 1][king.y - 2].type == 2 && board[king.x + 1][king.y - 2].isWhite != king.isWhite) {
        return true;
    }
    if (king.x < 7 && king.y < 6 && board[king.x + 1][king.y + 2].type == 2 && board[king.x + 1][king.y + 2].isWhite != king.isWhite) {
        return true;
    }
    // check for threats from rooks and queens
    for (int i = king.x + 1; i < 8; i++) {
        if (board[i][king.y].type == 3 || board[i][king.y].type == 5) {
            if (board[i][king.y].isWhite != king.isWhite) {
                return true;
            }
            break;
        }
        if (board[i][king.y].type != 0) {
            break;
        }
    }
    for (int i = king.x - 1; i >= 0; i--) {
        if (board[i][king.y].type == 3 || board[i][king.y].type == 5) {
            if (board[i][king.y].isWhite != king.isWhite) {
                return true;
            }
            break;
        }
        if (board[i][king.y].type != 0) {
            break;
        }
    }
    for (int j = king.y + 1; j < 8; j++) {
        if (board[king.x][j].type == 3 || board[king.x][j].type == 5) {
            if (board[king.x][j].isWhite != king.isWhite) {
                return true;
            }
            break;
        }
        if (board[king.x][j].type != 0) {
            break;
        }
    }
    for (int j = king.y - 1; j >= 0; j--) {
        if (board[king.x][j].type == 3 || board[king.x][j].type == 5) {
            if (board[king.x][j].isWhite != king.isWhite) {
                return true;
            }
            break;
        }
        if (board[king.x][j].type != 0) {
            break;
        }
    }
    // check for threats from bishops and queens
    for (int i = king.x + 1, j = king.y + 1; i < 8 && j < 8; i++, j++) {
        if (board[i][j].type == 4 || board[i][j].type == 5) {
            if (board[i][j].isWhite != king.isWhite) {
                return true;
            }
            break;
        }
        if (board[i][j].type != 0) {
            break;
        }
    }
    for (int i = king.x + 1, j = king.y - 1; i < 8 && j >= 0; i++, j--) {
        if (board[i][j].type == 4 || board[i][j].type == 5) {
            if (board[i][j].isWhite != king.isWhite) {
                return true;
            }
            break;
        }
        if (board[i][j].type != 0) {
            break;
        }
    }
    for (int i = king.x - 1, j = king.y + 1; i >= 0 && j < 8; i--, j++) {
        if (board[i][j].type == 4 || board[i][j].type == 5) {
            if (board[i][j].isWhite != king.isWhite) {
                return true;
            }
            break;
        }
        if (board[i][j].type != 0) {
            break;
        }
    }
    for (int i = king.x - 1, j = king.y - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j].type == 4 || board[i][j].type == 5) {
            if (board[i][j].isWhite != king.isWhite) {
                return true;
            }
            break;
        }
        if (board[i][j].type != 0) {
            break;
        }
    }
    // if none of the above conditions are true, the king is not in check
    return false;
}
