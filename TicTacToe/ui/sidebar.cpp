#include "sidebar.h"

#include <sstream>
#include <filesystem>

Sidebar::Sidebar(int posY, int posX, int width, int height) : UiBase(posY, posX) {
    this->width = width;
    this->height = height;

    std::string fontPath = std::filesystem::current_path().append("font/menuFont.ttf").generic_string();

    BLFontFace face;
    BLResult result = face.createFromFile(fontPath.c_str());
    if (result != BL_SUCCESS) {
        printf("Failed to load a font (err=%u)\n", result);
        return;
    }
    font.createFromFace(face, 24);

    renderCache = BLImage(width, height, BL_FORMAT_PRGB32);
}

void Sidebar::renderSplitLine(BLContext &ctx) const {
    ctx.setStrokeWidth(width / 40.0);

    double gap = height * 0.02;
    double unitLen = (height - gap) / 10.0;
    // begin
    ctx.setStrokeStartCap(BL_STROKE_CAP_ROUND);
    ctx.setStrokeEndCap(BL_STROKE_CAP_BUTT);
    auto width_double = static_cast<double>(width);
    ctx.strokeLine(
            {width_double, gap},
            {width_double, unitLen},
            colorConfig[SIDEBAR_SPLIT_LINE_COLOR]);
    // end
    ctx.setStrokeStartCap(BL_STROKE_CAP_BUTT);
    ctx.setStrokeEndCap(BL_STROKE_CAP_ROUND);
    ctx.strokeLine(
            {width_double, height - unitLen},
            {width_double, height - gap},
            colorConfig[SIDEBAR_SPLIT_LINE_COLOR]);
    // other
    ctx.setStrokeStartCap(BL_STROKE_CAP_BUTT);
    ctx.setStrokeEndCap(BL_STROKE_CAP_BUTT);
    for (int i = 1; i <= 8; i++) {
        ctx.strokeLine(
                {width_double, unitLen * i + gap},
                {width_double, unitLen * (i + 1)},
                colorConfig[SIDEBAR_SPLIT_LINE_COLOR]);
    }
}

float Sidebar::textDesireWidth2Fontsize(const std::string &text, float desireWidth) {
    return 2.0f * desireWidth / static_cast<float >(text.length());
}


void Sidebar::renderMenu(BLContext &ctx) const {
    if (!font.isValid())
        return;

    BLFont fontCopy(font);

    double offset = 0;

    double gap = width * 0.02;

    double lineGap = height / 40.0;

    // Title
    std::string titleText = "Tic Tac Toe";
    const double titleWidth = width * 0.9;
    fontCopy.setSize(textDesireWidth2Fontsize(titleText, static_cast<float>(titleWidth)));
    ctx.setFillStyle(colorConfig[SIDEBAR_MENU_COLOR]);
    ctx.fillUtf8Text(BLPoint(width * 0.05, offset + fontCopy.size()), fontCopy, titleText.c_str());

    ctx.setStrokeWidth(gap / 2);
    ctx.strokeBox(gap, gap, width - 2 * gap, fontCopy.size() + gap, colorConfig[SIDEBAR_MENU_COLOR]);
    offset += fontCopy.size() + lineGap;

    // score
    std::string p1Text = "P1:";
    const double p1Width = width * 0.2;
    fontCopy.setSize(textDesireWidth2Fontsize(p1Text, static_cast<float>(p1Width)));

    ctx.fillUtf8Text(
            BLPoint(width * 0.1, offset + fontCopy.size()),
            fontCopy, p1Text.c_str(), p1Text.length(),
            colorConfig[GRID_PLAYER_ONE_COLOR]);
    std::string p1ScoreText = std::to_string(p1Score());
    ctx.fillUtf8Text(BLPoint(width * 0.3, offset + fontCopy.size()),
                     fontCopy, p1ScoreText.c_str());
    ctx.strokeRoundRect(2 * gap, offset + gap, width - 5 * gap, 2 * fontCopy.size(), gap, gap,
                        colorConfig[SIDEBAR_MENU_COLOR]);
    offset += fontCopy.size();
    ctx.strokeLine(2 * gap, offset + lineGap / 2, width - 3 * gap, offset + lineGap / 2,
                   colorConfig[SIDEBAR_MENU_COLOR]);

    std::string p2Text = "P2:";
    ctx.fillUtf8Text(
            BLPoint(width * 0.1, offset + fontCopy.size()),
            fontCopy, p2Text.c_str(), p2Text.length(),
            colorConfig[GRID_PLAYER_TWO_COLOR]);
    std::string p2ScoreText = std::to_string(p2Score());
    ctx.fillUtf8Text(BLPoint(width * 0.3, offset + fontCopy.size()),
                     fontCopy, p2ScoreText.c_str());
    offset += fontCopy.size() + lineGap;

    // help part
    ctx.strokeRoundRect(2 * gap, offset, width - 5 * gap, height - offset - 3 * gap, gap, gap,
                        colorConfig[SIDEBAR_MENU_COLOR]);
    const std::string helpTitle = "Shortcut";
    double helpTextWidth = width * 0.6;
    fontCopy.setSize(textDesireWidth2Fontsize(helpTitle, static_cast<float>(helpTextWidth)));
    ctx.fillUtf8Text(BLPoint(width * 0.2, offset + fontCopy.size()), fontCopy, helpTitle.c_str());
    offset += fontCopy.size() + lineGap;

    fontCopy.setSize(fontCopy.size() * 0.45f);
    const std::vector<std::string> helpContent = {
            "Press \"R\" for restart",
            "Press \"C\" for clear score",
            "Press \"Esc\" for quit game"
    };
    for (const auto &line: helpContent) {
        ctx.fillUtf8Text(BLPoint(width * 0.06, offset + fontCopy.size()), fontCopy, line.c_str());
        offset += fontCopy.size() + lineGap * 0.8;
    }

    if (!endGame)
        return;

    offset += fontCopy.size() + lineGap * 0.8;
    std::string nextGameText = "Press \"N\" for next game";
    ctx.fillUtf8Text(BLPoint(width * 0.06, offset + fontCopy.size()), fontCopy, nextGameText.c_str(),
                     nextGameText.length(), colorConfig[SIDEBAR_HINT_TEXT_COLOR]);

}

void Sidebar::render(BLContext &ctx) const {
    ctx.fillBox(0, 0, width, height, renderBuffer);
}

void Sidebar::bindScores(Sidebar::getScoreFunc p1, Sidebar::getScoreFunc p2) {
    p1Score = std::move(p1);
    p2Score = std::move(p2);
}

void Sidebar::refresh(bool force) {
    if (!force && lastP1Score == p1Score() && lastP2Score == p2Score())
        return;

    BLContext ctx(renderCache);
    ctx.clearAll();
    renderSplitLine(ctx);
    renderMenu(ctx);
    ctx.end();
    renderBuffer = BLPattern(renderCache);

    lastP1Score = p1Score();
    lastP2Score = p2Score();
}

void Sidebar::setEndGame(bool value) {
    if (endGame == value)
        return;

    endGame = value;
    refresh(true);
}






