/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <QString>

const int DEFAULT_FRAME_FREQUENCY = 500;
const int DEFAULT_NUM_ROWS = 7;
const int DEFAULT_NUM_COLUMNS = 11;
const int DEFAULT_NUM_FRAMES = 100;

const int MAX_ROWS = 20;
const int MAX_COLUMNS = 20;
const int MAX_FRAMES = 1000;

const int INVALID = -1;
const int INITIAL_FRAME = 1;

const int LED_RADIUS = 15;
const int LED_OFFSET_X = LED_RADIUS*2 + 40;
const int LED_OFFSET_Y = LED_OFFSET_X;

const int SLIDER_HEIGHT = 40;

const int BORDER = 20;

const int HEADER_BYTE = 0x56;
const int TERMINATING_BYTE = 0x45;
const int ESCAPE_BYTE = 0x02;
const int XOR_BYTE = 0x20;

const QString COMPANY_NAME = "BewareTheBadMouse";
const QString APP_NAME = "Led Animator";
const QString COMPANY_DOMAIN = "com.onebadmouse";

const QString SETTINGS_USER_CURRENT_ANIM = "user/currentAnim";
const QString SETTINGS_MAIN_WINDOW = "MainWindow";

#endif // DEFAULTS_H
