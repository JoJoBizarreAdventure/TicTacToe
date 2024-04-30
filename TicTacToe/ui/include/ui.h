#ifndef TICTACTOE_UI_H
#define TICTACTOE_UI_H

#include <vector>
#include "grid.h"
#include "chessboard.h"
#include "sidebar.h"
#include "player.h"
#include "anim.h"

class Ui : public UiBase {
private:
    BLImage renderTarget;
public:
    void *rawData();

private:
    int width, height;
    Chessboard *chessboard;
    std::vector<std::vector<Grid>> grids = {};
    Sidebar *sidebar;
    Anim *endAnim;

    void initGrids(int startY, int startX);

public:
    explicit Ui(int w, int h);

    ~Ui();

    void linkGridStates(GridState states[3][3]);

    void bindPlayers(Player *p1, Player *p2);

    [[nodiscard]] std::pair<int, int> mousePosition2Index(int pos_y, int pos_x) const;

private:
    void renderGrids(BLContext &ctx) const;

public:
    void renderUI();

    void refreshUI();

private:
    void render(BLContext &ctx) const override;

public:
    void setEndAnim(int startX, int startY, int endX, int endY);
};


#endif //TICTACTOE_UI_H
