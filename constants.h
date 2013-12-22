/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QPair>

#include <QMetaType>

const int DEFAULT_FRAME_FREQUENCY = 500;
const int DEFAULT_NUM_ROWS = 4;
const int DEFAULT_NUM_COLUMNS = 4;
const int DEFAULT_NUM_FRAMES = 100;

const int MAX_ROWS = 250;
const int MAX_COLUMNS = 250;
const int MAX_FRAMES = 999;

const int INVALID = -1;
const int INITIAL_LED = 1;

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

const QString COMPANY_NAME = "BewareTheBadMouse";
const QString APP_NAME = "Led Animator";
const QString COMPANY_DOMAIN = "com.onebadmouse";

const QString SETTINGS_USER_CURRENT_ANIM = "user/currentAnim";
const QString SETTINGS_MAIN_WINDOW = "MainWindow";

const QString LED_MIME_TYPE = "ledanimator/x-leditemdata";
const QString FRAME_MIME_TYPE = "ledanimator/x-frameitemdata";

const static int kNoValue = 0;
const static int kColour = 1;
const static int kFunction = 2;

namespace AnimatorModel {

class Position : public QPair<int, int> {
public:
    inline Position(int row, int column) : QPair<int, int> (row, column) {}
    inline Position() : QPair<int, int> (INVALID, INVALID) {}
    inline Position(const Position& position) : QPair<int, int> (position.first, position.second) {}

    inline int row() const { return first; }
    inline int column() const { return second; }
    inline bool isValid() const { return first > INVALID && second > INVALID; }
    inline bool operator==(const Position& position) { return first == position.row() && second == position.column(); }
    inline bool operator<(const Position& position) { return (first < position.row() && second == position.column()) ||
                                                        (first == position.row() && second < position.column()); }
    inline Position& operator=(const Position& position) { first = position.first; second = position.second; return *this; }
};
}

using namespace AnimatorModel;

Q_DECLARE_METATYPE ( Position )



#endif // CONSTANTS_H
