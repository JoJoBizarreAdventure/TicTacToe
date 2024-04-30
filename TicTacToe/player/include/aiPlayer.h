#ifndef TICTACTOE_AIPLAYER_H
#define TICTACTOE_AIPLAYER_H

#include "player.h"
#include "grid.h"

class AIPlayer : public Player {
protected:
    GridState *chessboardRef[3][3]{};
public:
    explicit AIPlayer(int symbol, GridState (&chessboard)[3][3]);

    bool isHuman() override;
};


#endif //TICTACTOE_AIPLAYER_H
