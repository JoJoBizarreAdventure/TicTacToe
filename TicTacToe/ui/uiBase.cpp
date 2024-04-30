#include "uiBase.h"

std::unordered_map<int, BLRgba32> UiBase::colorConfig = {
        // chessboard
        {CHESSBOARD_BACKGROUND_COLOR, BLRgba32(0xFFC3E2C2)},
        {CHESSBOARD_LINE_COLOR,       BLRgba32(0xFFEAECCC)},

        // player one
        {GRID_PLAYER_ONE_COLOR,       BLRgba32(0xFFDBCC95)},
        {GRID_PLAYER_ONE_COLOR_FADE,  BLRgba32(0x7FDBCC95)},

        // player two
        {GRID_PLAYER_TWO_COLOR,       BLRgba32(0xFFCD8D7A)},
        {GRID_PLAYER_TWO_COLOR_FADE,  BLRgba32(0x7FCD8D7A)},

        // sidebar
        {SIDEBAR_SPLIT_LINE_COLOR,    BLRgba32(0xFF828282)},
        {SIDEBAR_MENU_COLOR,          BLRgba32(0xFF828282)},
        {SIDEBAR_HINT_TEXT_COLOR,     BLRgba32(0xFF70649A)},

        // end anim
        {END_ANIM_COLOR,              BLRgba32(0x9F828282)},
};