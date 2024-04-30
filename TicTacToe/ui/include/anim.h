#ifndef TICTACTOE_ANIM_H
#define TICTACTOE_ANIM_H

#include "task.h"
#include "uiBase.h"

class Anim : private UiBase {
private:
    int cnt = 240;
    const int totalStep = 240;
    BLPoint startPos{};
    std::pair<double, double> step; // to = from + step * cnt

    double lineWidth;
public:
    explicit Anim(double width);

    bool execAnim(BLContext &ctx);

private:
    void render(BLContext &ctx) const override;

public:
    void resetAnim(BLPoint pos, std::pair<double, double> posDiff);

    void stopAnim();
};


#endif //TICTACTOE_ANIM_H
