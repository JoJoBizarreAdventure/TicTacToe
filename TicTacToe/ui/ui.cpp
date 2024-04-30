#include "ui.h"
#include <iostream>

void *Ui::rawData() {
    BLImageData result{};
    renderTarget.getData(&result);
    return result.pixelData;
}

void Ui::initGrids(int startY, int startX) {
    grids.resize(3, {});

    int intervalLen = chessboard->GetIntervalLen(), gridLen = chessboard->GetGridLen();

    int total_len = intervalLen + gridLen;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            grids[y].emplace_back(
                    startY + total_len * y + intervalLen,
                    startX + total_len * x + intervalLen,
                    gridLen
            );
        }
    }
}


Ui::Ui(int w, int h) : UiBase(0, 0) {
    width = w;
    height = h;
    renderTarget = BLImage(width, height, BL_FORMAT_PRGB32);

    int chessboardStartY = 0, chessboardStartX = width - height;
    chessboard = new Chessboard(chessboardStartY, chessboardStartX, height);
    initGrids(chessboardStartY, chessboardStartX);
    sidebar = new Sidebar(0, 0, chessboardStartX, height);

    endAnim = new Anim(chessboard->GetIntervalLen() / 3.0);
}

Ui::~Ui() {
    delete chessboard;
}

void Ui::linkGridStates(GridState (*states)[3]) {
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            grids[y][x].linkState(&states[y][x]);
        }
    }
}

std::pair<int, int> Ui::mousePosition2Index(int pos_y, int pos_x) const {
    return chessboard->mousePosition2Index(pos_y, pos_x);
}

void Ui::renderGrids(BLContext &ctx) const {
    ctx.setFillStyle(BLRgba32(0xFF5FAFDF));
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            grids[y][x].render(ctx);
        }
    }
}

void Ui::renderUI() {
    BLContext ctx(renderTarget);

    render(ctx);

    ctx.end();
}

void Ui::refreshUI() {
    sidebar->refresh();
    endAnim->stopAnim();
}


void Ui::bindPlayers(Player *p1, Player *p2) {
    sidebar->bindScores(
            [p1]() {
                return p1->getScore();
            }, [p2]() {
                return p2->getScore();
            });
}

void Ui::render(BLContext &ctx) const {
    ctx.clearAll();
    ctx.fillAll(colorConfig[CHESSBOARD_BACKGROUND_COLOR]);

    chessboard->render(ctx);
    renderGrids(ctx);

    bool status = endAnim->execAnim(ctx);
    sidebar->setEndGame(status);
    sidebar->render(ctx);
}

void Ui::setEndAnim(int startX, int startY, int endX, int endY) {
    Grid &start = grids[startY][startX];
    int length = chessboard->GetGridLen();
    double diff = chessboard->GetGridLen() * 2.8 + chessboard->GetIntervalLen() * 2;
    double diffX, diffY;
    double offsetX, offsetY;
    if (startX == endX) {
        diffX = 0;
        diffY = diff;
        offsetX = 0.5 * length;
        offsetY = 0.1 * length;
    } else if (startY == endY) {
        diffX = diff;
        diffY = 0;
        offsetX = 0.1 * length;
        offsetY = 0.5 * length;
    } else if (startX == startY) {
        diffX = diff;
        diffY = diffX;
        offsetX = 0.1 * length;
        offsetY = 0.1 * length;
    } else {
        diffX = diff;
        diffY = -diffX;
        offsetX = 0.1 * length;
        offsetY = 0.9 * length;
    }

    endAnim->resetAnim(
            {start.getX() + offsetX, start.getY() + offsetY},
            {diffX, diffY});
}




