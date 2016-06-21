/* ======================
   Date: 21/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "uicomponent.h"

class UIBoolean: public UIComponent
{
public:

    UIBoolean(const bool left);

    void update() override;

    void render() override;

    bool getOption() const;

private:

    static const int32 UIBOOL_LEFT_X = 0;
    static const int32 UIBOOL_LEFT_Y = 56;
    static const int32 UIBOOL_RIGHT_X = 112;
    static const int32 UIBOOL_RIGHT_Y = 56;
    static const int32 UIBOOL_ARROW_X_OFFSET = 8;
    static const int32 UIBOOL_ARROW_Y_OFFSET = 8;
    static const int32 UIBOOL_OPTION_DIST = 16;

private:

    // Yes or No (yes is true)
    bool m_option;
   
};