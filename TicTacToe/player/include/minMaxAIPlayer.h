#ifndef TICTACTOE_MINMAXAIPLAYER_H
#define TICTACTOE_MINMAXAIPLAYER_H

#include <vector>
#include "aiPlayer.h"

struct Node {
    std::pair<int, int> choice;
    std::vector<Node *> children;
    int status; // win 1 lose -1 draw 0

    explicit Node(int y = -1, int x = -1) {
        choice = {y, x};
        children = {};
        status = 0;
    }
};

class MinMaxAIPlayer : public AIPlayer {
private:
public:
    MinMaxAIPlayer(int symbol, GridState (&chessboard)[3][3]);

    PlayerStep getStep() override;


private:
    static std::unordered_map<uint32_t, std::pair<int, int>> cache;

    const uint16_t gridBitRepresent[3][3] = {
            {
                    0b1,
                    0b10,
                    0b100
            },
            {
                    0b1'000,
                    0b10'000,
                    0b100'000,
            },
            {
                    0b1'000'000,
                    0b10'000'000,
                    0b100'000'000,
            }
    };

    std::pair<int, int> queryCache();

    // precompute
    static void initCache();

    static void generateTree(Node *node, uint16_t p1, uint16_t p2, int cnt, bool turn,
                             std::unordered_map<uint32_t, int> &statusCache);

    static bool checkIfWin(uint16_t steps, int y, int x);

    static void destroyTree(Node *node);
};


#endif //TICTACTOE_MINMAXAIPLAYER_H
