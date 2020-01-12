#include "BoardPrinterConsole.h"
#include "IAbaloneBoard.h"
#include <stdio.h>
#include "terminal_colors.h"
void BoardPrinterConsole::Print(const IAbaloneBoard* board)
{
    int c = 0;
    for (int i=0; i< board->numberOfRows() ; i++)
    {

        for (int j = 0; j< board->numberOfRows() - board->cellsInRow()[i]; ++j)
        {
            printf("  ");
        }
        for (int j = 0; j< board->cellsInRow()[i]; ++j)
        {
            if (board->getMarbleAt(c)==IAbaloneBoard::WHITE)
                printf(TERM_YELLOW_COLOR);
            else if (board->getMarbleAt(c)==IAbaloneBoard::BLACK)
                printf(TERM_PURPLE_COLOR);
            else if (board->getMarbleAt(c)==IAbaloneBoard::RED)
                printf(TERM_RED_COLOR);
            printf("%02d  " TERM_RESET,c + 1);
            c++;
        }
        printf("\n\n");

    }
    printf("\n\n");
    /*for (int i =0; i< board->numberOfPlayers();++i)
    {
        printf("")
    }*/
    printf("whites off: %d      blacks off: %d\n\n",board->takenDown(IAbaloneBoard::WHITE),board->takenDown(IAbaloneBoard::BLACK));
    const char * str;
    if (board->currentTurn() == IAbaloneBoard::BLACK)
    {
        str="purple";

    }
    else if (board->currentTurn() == IAbaloneBoard::WHITE)
    {
        str="yellow";
    }
    else if (board->currentTurn() == IAbaloneBoard::RED)
    {
        str="red";
    }

    printf ("Player of %s marbles, what is your move?\n",str);


}
