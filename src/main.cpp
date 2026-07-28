#include <Arduino.h>
#include "Board.h"

Board board;

void setup()
{
    board.init();
}

void loop()
{
    board.rgb_.red();
    delay(500);

    board.rgb_.off();
    delay(500);
}