#ifndef TICTACTOE_GRID_H
#define TICTACTOE_GRID_H

#include "uiBase.h"

enum GridState {
    Empty = 0,
    P1_MOUSE_ON = -1,
    P2_MOUSE_ON = -2,
    P1_SELECTED = 1,
    P2_SELECTED = 2
};

class Grid : public UiBase {
private:
    int length;
    GridState *state = nullptr;
public:
    Grid(int posY, int posX, int length);

    void linkState(GridState *ptr);

private:
    void renderX(BLContext &ctx, bool fade) const;

    void renderO(BLContext &ctx, bool fade) const;

public:
    void render(BLContext &ctx) const override;

    [[nodiscard]] int getX() const;

    [[nodiscard]] int getY() const;
};


#endif //TICTACTOE_GRID_H
