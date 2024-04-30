#include <iostream>
#include "minMaxAIPlayer.h"

std::unordered_map<uint32_t, std::pair<int, int>> MinMaxAIPlayer::cache{};

MinMaxAIPlayer::MinMaxAIPlayer(int symbol, GridState (&chessboard)[3][3]) :
        AIPlayer(symbol, chessboard) {
    if (cache.empty())
        initCache();
}

PlayerStep MinMaxAIPlayer::getStep() {
    return {queryCache(), true};
}

std::pair<int, int> MinMaxAIPlayer::queryCache() {
    uint16_t p1Steps = 0, p2Steps = 0;
    int p1Cnt = 0, p2Cnt = 0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (*chessboardRef[y][x] == P1_SELECTED) {
                p1Steps |= gridBitRepresent[y][x];
                p1Cnt++;
            } else if (*chessboardRef[y][x] == P2_SELECTED) {
                p2Steps |= gridBitRepresent[y][x];
                p2Cnt++;
            }
        }
    }
    uint32_t key;
    if (p1Cnt > p2Cnt || (p1Cnt == p2Cnt && GetSymbol() == PLAYER_ONE_SYMBOL)) {
        key = (p1Steps << 9) | p2Steps;
    } else {
        key = (p2Steps << 9) | p1Steps;
    }
    return cache.at(key);
}

void MinMaxAIPlayer::initCache() {
    Node *root = new Node();
    std::unordered_map<uint32_t, int> statusCache;
    generateTree(root, 0, 0, 0, true, statusCache);
    destroyTree(root);
}

void MinMaxAIPlayer::generateTree(Node *node, uint16_t p1, uint16_t p2, int cnt, bool turn,
                                  std::unordered_map<uint32_t, int> &statusCache) {
    if (cnt == 9) {
        node->status = 0;
        return;
    }

    uint32_t key = (p1 << 9) | p2;
    if (cache.find(key) != cache.end()) {
        node->status = statusCache[key];
        return;
    }


    uint16_t *steps;
    if (turn) {
        steps = &p1;
    } else {
        steps = &p2;
    }

    uint16_t chessboard = p1 | p2;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            uint16_t choice = 1 << (y * 3 + x);
            if (chessboard & choice)
                continue;
            *steps |= choice;

            Node *child = new Node(y, x);
            node->children.push_back(child);

            if (cnt >= 4 && checkIfWin(*steps, y, x)) {
                child->status = 1;
                *steps -= choice;
                continue;
            }

            generateTree(child, p1, p2, cnt + 1, !turn, statusCache);
            *steps -= choice;
        }
    }
    std::vector<Node *> win, lose, draw;
    for (const auto &child: node->children) {
        if (child->status > 0)
            win.push_back(child);
        else if (child->status < 0)
            lose.push_back(child);
        else
            draw.push_back(child);
    }
    if (!win.empty()) {
        statusCache[key] = node->status = -static_cast<int>(win.size());
        cache[key] = win[0]->choice;
        int maxValue = win[0]->status;
        for (int i = 1; i < win.size(); i++) {
            if (win[i]->status > maxValue) {
                cache[key] = win[i]->choice;
                maxValue = win[i]->status;
            }
        }
        return;
    }
    if (!draw.empty()) {
        statusCache[key] = node->status = 0;
        cache[key] = draw[0]->choice;
        return;
    }
    statusCache[key] = node->status = static_cast<int>(lose.size());
    cache[key] = lose[0]->choice;
    int maxValue = lose[0]->status;
    for (int i = 1; i < lose.size(); i++) {
        if (lose[i]->status > maxValue) {
            cache[key] = lose[i]->choice;
            maxValue = lose[i]->status;
        }
    }
}

bool MinMaxAIPlayer::checkIfWin(uint16_t steps, int y, int x) {
    const int xAxisCheck[3] = {0b000'000'111, 0b000'111'000, 0b111'000'000};
    const int yAxisCheck[3] = {0b001'001'001, 0b010'010'010, 0b100'100'100};
    const int diagonalCheck = 0b100'010'001, antiDiagonalCheck = 0b001'010'100;
    if ((xAxisCheck[y] & steps) == xAxisCheck[y]) {
        return true;
    }
    if ((yAxisCheck[x] & steps) == yAxisCheck[x]) {
        return true;
    }
    if (x == y && (diagonalCheck & steps) == diagonalCheck) {
        return true;
    }
    if (x + y == 2 && (antiDiagonalCheck & steps) == antiDiagonalCheck) {
        return true;
    }
    return false;
}

void MinMaxAIPlayer::destroyTree(Node *node) {
    for (auto child: node->children) {
        destroyTree(child);
    }
    delete node;
}







