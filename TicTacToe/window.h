#ifndef TICTACTOE_WINDOW_H
#define TICTACTOE_WINDOW_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "ui.h"
#include "player.h"

class Window {
private:
    int width = 1, height = 1;
    GLFWwindow *window;
    unsigned int screenColor = -1;
    unsigned int quadVAO = -1, quadVBO = -1;
    unsigned int shader = -1;
public:
    explicit Window(int width = -1, int height = -1, bool p1AI = false, bool p2AI = false,
                    bool simpleAI = true);

    ~Window();

private:
    GridState chessboardState[3][3]{};
    std::pair<int, int> lastPointTo = POINT_TO_NOTHING; // (y,x) , record position where mouse doesn't click

    static bool changeState(GridState &state, GridState target);

    void ResetChessboard();

    bool inputEnable = true;

    void mouseEventHandle(bool click, std::pair<int, int> index);

    void keyboardEventHandle();

    void *dataSource = nullptr;

    void applyData();

public:
    void loop();

private:
    Ui *ui;
    bool whoIsFirst = true;
    Player *player1, *player2;
    Player *currentPlayer;

    void shiftPlayer();

    [[nodiscard]] bool checkIfWin(int y, int x); // input is the last step
    uint8_t stepCnt = 0;

    [[nodiscard]] bool checkIfDraw() const;
};


#endif //TICTACTOE_WINDOW_H
