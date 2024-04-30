#include "grid.h"

Grid::Grid(int posY, int posX, int length) : UiBase(posY, posX) {
    this->length = length;
}

void Grid::linkState(GridState *ptr) {
    state = ptr;
}

// length <===> | gap | x | gap |
void Grid::renderX(BLContext &ctx, bool fade) const {
    double gap = length / 5.0;
    auto color = fade ? colorConfig[GRID_PLAYER_ONE_COLOR_FADE] : colorConfig[GRID_PLAYER_ONE_COLOR];
    ctx.strokeLine({startX + gap, startY + gap}, {startX + length - gap, startY + length - gap}, color);
    ctx.strokeLine({startX + gap, startY + length - gap}, {startX + length - gap, startY + gap}, color);
}

// set r equal to half x <===> r = half of ( length - 2 gap )
void Grid::renderO(BLContext &ctx, bool fade) const {
    double r = length / 5.0 * 3.0 / 2.0;
    auto color = fade ? colorConfig[GRID_PLAYER_TWO_COLOR_FADE] : colorConfig[GRID_PLAYER_TWO_COLOR];
    ctx.strokeArc(startX + length / 2.0, startY + length / 2.0, r, r, 0, 6.29, color);
}

void Grid::render(BLContext &ctx) const {
    ctx.setStrokeWidth(length / 10.0);
    switch (*state) {
        case P1_MOUSE_ON:
            renderX(ctx, true);
            break;
        case P2_MOUSE_ON:
            renderO(ctx, true);
            break;
        case P1_SELECTED:
            renderX(ctx, false);
            break;
        case P2_SELECTED:
            renderO(ctx, false);
        case Empty:
        default:
            return;
    }
}

int Grid::getX() const { return startX; }

int Grid::getY() const { return startY; }




