#ifndef TICTACTOE_SIDEBAR_H
#define TICTACTOE_SIDEBAR_H

#include "uiBase.h"
#include <string>
#include <functional>

class Sidebar : public UiBase {
private:
    int width, height;
    BLFont font;
public:
    explicit Sidebar(int posY, int posX, int width, int height);

private:
    void renderSplitLine(BLContext &ctx) const;

    static float textDesireWidth2Fontsize(const std::string &text, float desireWidth);

    void renderMenu(BLContext &ctx) const;

public:
    void render(BLContext &ctx) const override;

private:
    typedef std::function<int(void)> getScoreFunc;
    getScoreFunc p1Score = nullptr;
    getScoreFunc p2Score = nullptr;
public:
    void bindScores(getScoreFunc p1, getScoreFunc p2);

private:
    BLImage renderCache;
    BLPattern renderBuffer;
    int lastP1Score = -1;
    int lastP2Score = -1;
public:
    void refresh(bool force = false);

private:
    bool endGame = false;
public:
    void setEndGame(bool value);
};


#endif //TICTACTOE_SIDEBAR_H
