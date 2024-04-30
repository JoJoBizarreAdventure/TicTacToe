#include "aiPlayer.h"

AIPlayer::AIPlayer(int symbol, GridState (&chessboard)[3][3]) : Player(symbol) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            chessboardRef[i][j] = &chessboard[i][j];
        }
    }
}

bool AIPlayer::isHuman() {
    return false;
}


