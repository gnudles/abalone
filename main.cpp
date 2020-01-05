#include <iostream>
#include "BoardPrinterConsole.h"
#include "AbaloneBoard.h"
#include "terminal_colors.h"
using namespace std;


int main()
{
    BoardPrinterConsole printer;
    using standard_abalone  = AbaloneBoard<5,2> ;
    standard_abalone abalone_board;
    abalone_board.addMarble(0,IAbaloneBoard::BLACK);
    abalone_board.addMarble(1,IAbaloneBoard::WHITE);
    printer.Print(&abalone_board);
    return 0;
}
