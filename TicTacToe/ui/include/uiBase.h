#ifndef TICTACTOE_UI_BASE_H
#define TICTACTOE_UI_BASE_H

#include "blend2d.h"
#include <unordered_map>

class UiBase {
protected:
    int startY;
    int startX;
public:
    explicit UiBase(int posY, int posX) {
        startY = posY;
        startX = posX;
    }

    virtual void render(BLContext &ctx) const = 0;

protected:
    static std::unordered_map<int, BLRgba32> colorConfig;
};
// chessboard
#define CHESSBOARD_BACKGROUND_COLOR 0
#define CHESSBOARD_LINE_COLOR 1
// player one
#define GRID_PLAYER_ONE_COLOR 2
#define GRID_PLAYER_ONE_COLOR_FADE 3
// player two
#define GRID_PLAYER_TWO_COLOR 4
#define GRID_PLAYER_TWO_COLOR_FADE 5
// sidebar
#define SIDEBAR_SPLIT_LINE_COLOR 6
#define SIDEBAR_MENU_COLOR 7
#define SIDEBAR_HINT_TEXT_COLOR 8
// endAnim
#define END_ANIM_COLOR 9

#endif //TICTACTOE_UI_BASE_H
