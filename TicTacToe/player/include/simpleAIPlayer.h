#ifndef TICTACTOE_SIMPLEAIPLAYER_H
#define TICTACTOE_SIMPLEAIPLAYER_H

#include "aiPlayer.h"
#include<random>

class SimpleAIPlayer : public AIPlayer {
public:
    SimpleAIPlayer(int symbol, GridState (&chessboard)[3][3]);

    PlayerStep getStep() override;
};


#endif //TICTACTOE_SIMPLEAIPLAYER_H
