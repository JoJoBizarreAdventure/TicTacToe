#ifndef TICTACTOE_HUMAN_PLAYER_H
#define TICTACTOE_HUMAN_PLAYER_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "player.h"

class HumanPlayer : public Player {
private:
    GLFWwindow *window;
public:
    explicit HumanPlayer(int symbol, GLFWwindow *window);

    ~HumanPlayer() override;

    bool isHuman() override;

private:
    PlayerStep getStep() override;
};


#endif //TICTACTOE_HUMAN_PLAYER_H
