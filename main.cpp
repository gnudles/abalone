#include <iostream>
#include "BoardPrinterConsole.h"
#include "AbaloneBoard.h"
#include "terminal_colors.h"
#include "HexDefaultPlacer.h"
using namespace std;


int main()
{
    BoardPrinterConsole printer;
    using standard_abalone  = AbaloneBoard<5,2>;
    standard_abalone abalone_board;

    HexDefaultPlacer placer;
    placer.Place(&abalone_board,3,14);
    printer.Print(&abalone_board);
    return 0;
}
