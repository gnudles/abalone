#include <iostream>
#include "BoardPrinterConsole.h"
#include "AbaloneBoard.h"
#include "terminal_colors.h"
#include "HexDefaultPlacer.h"
#include <readline/readline.h>
using namespace std;




int main()
{
    BoardPrinterConsole printer;
    using standard_abalone  = AbaloneBoard<5,2>;
    standard_abalone abalone_board;

    HexDefaultPlacer placer;
    placer.Place(&abalone_board,2,14);
    printer.Print(&abalone_board);

    char *line_read = (char *)NULL;
    while(true)
    {
        line_read = readline (">>> ");
        if (line_read && *line_read)
        {
            int valid_input = 0;
            standard_abalone::Direction d;
            int from,to;

            char strbuf[8];
            int args = sscanf(line_read,"%d-%d%2s",&from,&to,strbuf);
            if (args == 1)
            {
                args = sscanf(line_read,"%d%2s",&from,strbuf);
                if (args == 2)
                {
                    to = from;
                    valid_input = 1;
                }
            }
            if (args == 3)
            {
                valid_input = 1;
            }
            if (from < 1 || to < 1 || from > abalone_board.numberOfCells() || to > abalone_board.numberOfCells())
            {
                valid_input = 0;
            }
            if (valid_input)
            {
                valid_input = 0;

                const char* direction_literals[6] = {"ul","ur","r","dr","dl","l"};
                for (int i = 0; i< 6; ++i)
                {
                    if (strcasecmp(strbuf,direction_literals[i]) == 0)
                    {
                        d = (standard_abalone::Direction)i;
                        valid_input = 1;
                    }
                }

            }

            if (valid_input)
            {
                if (abalone_board.makeMove(IAbaloneBoard::BLACK, from - 1, to - 1, d, true)==IAbaloneBoard::MOVE_OK)
                    printer.Print(&abalone_board);
                else
                {
                    printf("invalid move!\n");
                }
            }
            else
            {
                printf("invalid input!\n");
            }
        }
        if (line_read)
        {
            free (line_read);
            line_read = (char *)NULL;
        }
        else
        { break;}

    }


    return 0;
}
