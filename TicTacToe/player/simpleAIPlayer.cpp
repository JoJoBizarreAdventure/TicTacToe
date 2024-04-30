#include "simpleAIPlayer.h"

SimpleAIPlayer::SimpleAIPlayer(int symbol, GridState (&chessboard)[3][3]) : AIPlayer(symbol, chessboard) {

}

PlayerStep SimpleAIPlayer::getStep() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::pair<int, int>> valid = {};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (*chessboardRef[i][j] == Empty) {
                valid.emplace_back(i, j);
            }
        }
    }
    std::uniform_int_distribution<> distrib(0, static_cast<int>(valid.size()) - 1);

    return {valid[distrib(gen)], true};
}

