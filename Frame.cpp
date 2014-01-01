/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "Frame.h"

#include "exceptions.h"

#include "LedAnimatorActionBase.h"
#include "Animation.h"
#include "ColourValue.h"
#include "FunctionValue.h"
#include "LinkedValue.h"
#include "NoValue.h"

using namespace AnimatorModel;
using namespace UndoCommands;
using namespace Exception;

Frame::Frame(QObject *parent,
             Animation& animation,
             int number,
             FrameValue& value,
             Frame *previous,
             QUndoStack &undoStack) :
    Selectable(parent, animation, number),
    iUndoStack(undoStack),
    iValue(&value),
    iPrevious(previous),
    iNext(NULL) {
}

Frame::Frame(QObject *parent,
             Animation &animation,
             int number,
             Frame *previous,
             QUndoStack& undoStack) :
    Selectable(parent, animation, number),
    iUndoStack(undoStack),
    iValue(new NoValue(parent)),
    iPrevious(previous),
    iNext(NULL) {
}

void Frame::setValue(const FrameValue& value) {
    iUndoStack.push(new SetFrameColourCommand(iAnimation, *this, value));
    //doSetValue(value);
}

const Function Frame::function() const {
    switch(value().type()) {
    case kNoValue:
    case kColour:
    case kLinked:
        return Function();
    case kFunction: {
        /*const FunctionValue& functionValue = static_cast<const FunctionValue&>(value());
        float combinedRedIncrement = 0;
        float combinedGreenIncrement = 0;
        float combinedBlueIncrement = 0;

        if(number() < 0) {  // zero value TODO AS!!!
            if(next() != NULL) {
                const Frame* nextFrame = next();
                int counter = 1;

                while(nextFrame->value().type() == kFunction) {
                    const FunctionValue& functionValue = static_cast<const FunctionValue&>(nextFrame->value());

                    combinedRedIncrement -= functionValue.function().redIncrement();
                    combinedGreenIncrement -= functionValue.function().greenIncrement();
                    combinedBlueIncrement -= functionValue.function().blueIncrement();

                    nextFrame = nextFrame->next();
                    counter++;
                }
            }
        }

        if(number() > 0) {// zero value TODO AS!!!
            if(previous() != NULL) {
                const Frame* nextFrame = previous();
                int counter = 1;

                while(nextFrame->value().type() == kFunction) {
                    const FunctionValue& functionValue = static_cast<const FunctionValue&>(nextFrame->value());

                    combinedRedIncrement -= functionValue.function().redIncrement();
                    combinedGreenIncrement -= functionValue.function().greenIncrement();
                    combinedBlueIncrement -= functionValue.function().blueIncrement();

                    nextFrame = nextFrame->previous();
                    counter++;
                }
            }
        }

        return Function(combinedRedIncrement, combinedGreenIncrement, combinedBlueIncrement);*/

        const Frame* nextFrame = NULL;

        Function combinedFunction;

        const FunctionValue& functionValue = static_cast<const FunctionValue&>(value());

        combinedFunction += functionValue.function();

        if(number() < 0) {
            if(next() == NULL) {
                throw InvalidFrameException("Next frame should not be null");
            }

            nextFrame = next();

            while(nextFrame->value().type() == kFunction) {
                const FunctionValue& functionValue = static_cast<const FunctionValue&>(nextFrame->value());

                combinedFunction += functionValue.function();

                nextFrame = nextFrame->next();
            }
        }

        if(number() > 0) {
            if(previous() == NULL) {
                throw InvalidFrameException("Previous frame should not be null");
            }

            nextFrame = previous();

            while(nextFrame->value().type() == kFunction) {
                const FunctionValue& functionValue = static_cast<const FunctionValue&>(nextFrame->value());

                combinedFunction += functionValue.function();

                nextFrame = nextFrame->previous();
            }
        }

        if(nextFrame->value().type() == kNoValue) {
            throw InvalidFrameException("Frame has NoValue");
        }

        return combinedFunction;
    }
    }
}

const QColor Frame::colour() const {
    switch(value().type()) {
    case kNoValue:
        return QColor();
    case kColour:
    case kLinked:
        return static_cast<const ColourValue&>(value()).colour();
    case kFunction: {
        const Frame* nextFrame = NULL;

        Function combinedFunction;
        QColor colour;

        const FunctionValue& functionValue = static_cast<const FunctionValue&>(value());

        combinedFunction += functionValue.function();

        if(number() < 0) {
            if(next() == NULL) {
                throw InvalidFrameException("Next frame should not be null");
            }

            nextFrame = next();

            while(nextFrame->value().type() == kFunction) {
                const FunctionValue& functionValue = static_cast<const FunctionValue&>(nextFrame->value());

                combinedFunction += functionValue.function();

                nextFrame = nextFrame->next();
            }
        }

        if(number() > 0) {
            if(previous() == NULL) {
                throw InvalidFrameException("Previous frame should not be null");
            }

            nextFrame = previous();

            while(nextFrame->value().type() == kFunction) {
                const FunctionValue& functionValue = static_cast<const FunctionValue&>(nextFrame->value());

                combinedFunction += functionValue.function();

                nextFrame = nextFrame->previous();
            }
        }

        //Function combinedFunction = function();

        //if(nextFrame->value().type() == kNoValue) {
        //    throw InvalidFrameException("Frame has NoValue");
       // }

        const ColourValue& colourValue = static_cast<const ColourValue&>(nextFrame->value());
        float currentRedValue = colourValue.colour().redF();
        float currentGreenValue = colourValue.colour().greenF();
        float currentBlueValue = colourValue.colour().blueF();

        currentRedValue += combinedFunction.redIncrement();
        currentGreenValue += combinedFunction.greenIncrement();
        currentBlueValue += combinedFunction.blueIncrement();

        if(currentRedValue > 1) {
            currentRedValue = 1;
        }

        if(currentBlueValue > 1) {
            currentBlueValue = 1;
        }

        if(currentGreenValue > 1) {
            currentGreenValue = 1;
        }

        if(currentRedValue < 0) {
            currentRedValue = 0;
        }

        if(currentBlueValue < 0) {
            currentBlueValue = 0;
        }

        if(currentGreenValue < 0) {
            currentGreenValue = 0;
        }


       colour = QColor::fromRgbF(currentRedValue, currentGreenValue, currentBlueValue);

       return colour;

     /* if(number() > 0 && previous() != NULL) {
          const FunctionValue& functionValue = static_cast<const FunctionValue&>(value());
          const Frame* previousFrame = previous();
          int counter = 1;

          if(!previousFrame) {
              throw InvalidFrameException("No previous frame");
          }

          while(previousFrame->value().type() == kFunction) {
              previousFrame = previousFrame->previous();
              counter++;
          }

          const ColourValue& colourValue = static_cast<const ColourValue&>(previousFrame->value());
          float currentRedValue = colourValue.colour().redF();
          float currentBlueValue = colourValue.colour().blueF();
          float currentGreenValue = colourValue.colour().greenF();

          for(int i = 0; i < counter; i++) {
              currentRedValue -= functionValue.function().redIncrement();
              currentBlueValue -= functionValue.function().greenIncrement();
              currentGreenValue -= functionValue.function().blueIncrement();

              //qDebug("red : %f green : %f blue %f", iCurrentRedValue, iCurrentGreenValue, iCurrentBlueValue);

              if(currentRedValue > 1) {
                  currentRedValue = 1;
              }

              if(currentBlueValue > 1) {
                  currentBlueValue = 1;
              }

              if(currentGreenValue > 1) {
                  currentGreenValue = 1;
              }

              if(currentRedValue < 0) {
                  currentRedValue = 0;
              }

              if(currentBlueValue < 0) {
                  currentBlueValue = 0;
              }

              if(currentGreenValue < 0) {
                  currentGreenValue = 0;
              }

          }

         colour = QColor::fromRgbF(currentRedValue, currentGreenValue, currentBlueValue);

          return colour;

        }

      if(number() < 0 && next() != NULL) {
          const FunctionValue& functionValue = static_cast<const FunctionValue&>(value());
          const Frame* nextFrame = next();
          int counter = 1;

          if(!nextFrame) {
              throw InvalidFrameException("No next frame");
          }

          while(nextFrame->value().type() == kFunction) {
              nextFrame = nextFrame->next();
              counter++;
          }

          const ColourValue& colourValue = static_cast<const ColourValue&>(nextFrame->value());
          float currentRedValue = colourValue.colour().redF();
          float currentBlueValue = colourValue.colour().blueF();
          float currentGreenValue = colourValue.colour().greenF();

          for(int i = 0; i < counter; i++) {
              currentRedValue -= functionValue.function().redIncrement();
              currentBlueValue -= functionValue.function().greenIncrement();
              currentGreenValue -= functionValue.function().blueIncrement();

           //   qDebug("red : %f green : %f blue %f", currentRedValue, currentGreenValue, currentBlueValue);

              if(currentRedValue > 1) {
                  currentRedValue = 1;
              }

              if(currentBlueValue > 1) {
                  currentBlueValue = 1;
              }

              if(currentGreenValue > 1) {
                  currentGreenValue = 1;
              }

              if(currentRedValue < 0) {
                  currentRedValue = 0;
              }

              if(currentBlueValue < 0) {
                  currentBlueValue = 0;
              }

              if(currentGreenValue < 0) {
                  currentGreenValue = 0;
              }

          }

          colour = QColor::fromRgbF(currentRedValue, currentGreenValue, currentBlueValue);

          return colour;

        }*/

    }
    }
}
