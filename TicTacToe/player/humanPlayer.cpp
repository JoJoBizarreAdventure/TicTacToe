#include "humanPlayer.h"

HumanPlayer::HumanPlayer(int symbol, GLFWwindow *window) : Player(symbol) {
    this->window = window;
}

HumanPlayer::~HumanPlayer() = default;


PlayerStep HumanPlayer::getStep() {
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    bool click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    return {{static_cast<int>(yPos), static_cast<int>(xPos)}, click};
}

bool HumanPlayer::isHuman() {
    return true;
}

