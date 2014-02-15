/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

const int DEFAULT_FRAME_RATE = 500;
const int DEFAULT_NUM_ROWS = 4;
const int DEFAULT_NUM_COLUMNS = 4;
const int DEFAULT_NUM_FRAMES = 100;

const int VERSION_HIGH = 3;
const int VERSION_LOW = 0;

const int MAX_ROWS = 250;
const int MAX_COLUMNS = 250;
const int MAX_FRAMES = 999;

const int INVALID = -1;
const int INITIAL_LED = 1;

const int MIN_FRAME_RATE = 10;
const int MAX_FRAME_RATE = 10000;

const int LED_RADIUS = 15;
const int LED_OFFSET_X = LED_RADIUS*2 + 40;
const int LED_OFFSET_Y = LED_OFFSET_X;

const int SLIDER_HEIGHT = 40;
const int SLIDER_OFFSET = 2;

const int BORDER = 12;

const int HEADER_BYTE = 0x56;
const int TERMINATING_BYTE = 0x45;
const int ESCAPE_BYTE = 0x02;
const int XOR_BYTE = 0x20;

const QString COMPANY_NAME = "Gigantic Sheep Ltd.";
const QString APP_NAME = "Led Animator";
const QString COMPANY_DOMAIN = "com.giganticsheep";

const QString SETTINGS_USER_CURRENT_ANIM = "user/currentAnim";
const QString SETTINGS_USER_EXPORT_PATH = "user/exportPath";
const QString SETTINGS_USER_SAVE_PATH = "user/savePath";
const QString SETTINGS_USER_OPEN_PATH = "user/openPath";
const QString SETTINGS_MAIN_WINDOW = "MainWindow";

const QString LED_MIME_TYPE = "ledanimator/x-leditemdata";
const QString FRAME_MIME_TYPE = "ledanimator/x-frameitemdata";

const int kNoValue = 0;
const int kColour = 1;
const int kFunction = 2;
const int kLinked = 3;

const int kTimeAxis = 0;
const int kValueAxis = 1;

const int kTimeAxisNum = -1;

const int kPriorityLow = -100;
const int kPriorityMed = -50;
const int kPriorityHigh = -10;


#endif // CONSTANTS_H
