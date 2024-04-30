#ifndef TICTACTOE_CHESSBOARD_H
#define TICTACTOE_CHESSBOARD_H

#include "uiBase.h"

#define POINT_TO_NOTHING std::pair<int,int>{-1,-1}

class Chessboard : public UiBase {
private:
    int length;
    int gridLen, intervalLen;

public:
    explicit Chessboard(int posY, int posX, int length);

    void render(BLContext &ctx) const override;

    [[nodiscard]] std::pair<int, int> mousePosition2Index(int pos_y, int pos_x) const;

    [[nodiscard]] int GetGridLen() const { return gridLen; }

    [[nodiscard]] int GetIntervalLen() const { return intervalLen; }
};


#endif //TICTACTOE_CHESSBOARD_H
