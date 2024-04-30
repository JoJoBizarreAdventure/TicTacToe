#include <iostream>
#include "anim.h"

bool Anim::execAnim(BLContext &ctx) {
    if (cnt < 0)
        return false;

    if (cnt < totalStep)
        cnt++;

    render(ctx);
    return cnt == totalStep;
}

void Anim::render(BLContext &ctx) const {
    auto color = colorConfig[END_ANIM_COLOR];
    BLPoint endPoint = {startPos.x + cnt * step.first, startPos.y + cnt * step.second};
    ctx.setStrokeWidth(lineWidth);
    ctx.strokeLine(startPos, endPoint, color);
}

void Anim::resetAnim(BLPoint pos, std::pair<double, double> posDiff) {
    cnt = 0;
    this->startPos = pos;
    this->step = {posDiff.first / totalStep, posDiff.second / totalStep};
}

void Anim::stopAnim() {
    cnt = -1;
}

Anim::Anim(double width) : UiBase(0, 0) {
    lineWidth = width;
}



