#include <Arduino.h>
#include "Board.h"

Board board;

void setup()
{
    board.init();
}

void loop()
{
    board.rgb().red();
    delay(500);

    board.rgb().off();
    delay(500);
}