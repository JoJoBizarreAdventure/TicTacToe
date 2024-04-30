#include "chessboard.h"

Chessboard::Chessboard(int posY, int posX, int length) : UiBase(posY, posX) {
    this->length = length;
    intervalLen = length / 5 / 4;
    gridLen = length * 4 / 5 / 3;
}

void Chessboard::render(BLContext &ctx) const {
    ctx.setStrokeWidth(intervalLen / 2.0);
    ctx.setStrokeStartCap(BL_STROKE_CAP_ROUND);
    ctx.setStrokeEndCap(BL_STROKE_CAP_ROUND);

    double from = length / 20.0, to = length - from;
    // horizontal line above & vertical line left
    double low = gridLen + intervalLen * 1.5;
    double high = length - low;
    auto color = colorConfig[CHESSBOARD_LINE_COLOR];
    ctx.strokeLine({startX + from, startY + low}, {startX + to, startY + low}, color);
    ctx.strokeLine({startX + low, startY + from}, {startX + low, startY + to}, color);
    // horizontal line below & vertical line right
    ctx.strokeLine({startX + from, startY + high}, {startX + to, startY + high}, color);
    ctx.strokeLine({startX + high, startY + from}, {startX + high, startY + to}, color);
}

std::pair<int, int> Chessboard::mousePosition2Index(int pos_y, int pos_x) const {
    pos_y -= startY;
    pos_x -= startX;

    int totalLen = intervalLen + gridLen;
    int y = pos_y / totalLen;
    int x = pos_x / totalLen;
    int rest_y = pos_y - y * totalLen;
    int rest_x = pos_x - x * totalLen;

    if (x < 0 || x > 2 || y < 0 || y > 2)
        return POINT_TO_NOTHING;

    if (rest_x < intervalLen || rest_y < intervalLen) {
        return POINT_TO_NOTHING;
    }

    return {y, x};
}



