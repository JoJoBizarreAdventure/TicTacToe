#ifndef TICTACTOE_PLAYER_H
#define TICTACTOE_PLAYER_H

#include <utility>

#define PLAYER_ONE_SYMBOL (-1)
#define PLAYER_TWO_SYMBOL (-2)

struct PlayerStep {
    std::pair<int, int> index;
    bool clicked;
};

class Player {
private:
    int symbol;
public:
    explicit Player(int symbol) { this->symbol = symbol; };

    virtual ~Player() = default;

    [[nodiscard]] int GetSymbol() const { return symbol; }

private:
    int score = 0;

public:
    void addScore() { score++; };

    void clearScore() { score = 0; };

    [[nodiscard]] int getScore() const { return score; }

    virtual bool isHuman() = 0;

    virtual PlayerStep getStep() = 0;
};

#endif //TICTACTOE_PLAYER_H
