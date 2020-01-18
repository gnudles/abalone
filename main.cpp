#include <iostream>
#include "BoardPrinterConsole.h"
#include "AbaloneBoard.h"
#include "terminal_colors.h"
#include "HexDefaultPlacer.h"
#include <readline/readline.h>
#include <iostream>
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
        std::vector<IAbaloneBoard::move_record_t> moves;
        abalone_board.possibleMoves(abalone_board.currentTurn(),moves);
        printf("possible moves for you:\n");
        for (int i=0;i<moves.size();++i)
        {
            printf("%d] %d-%d%s\n",i+1,moves[i].frst_m+1,moves[i].last_m+1,IAbaloneBoard::directionLiterals[moves[i].direction]);
        }

        line_read = readline (">>> ");
        if (line_read && *line_read)
        {
            int valid_input = 0;
            IAbaloneBoard::Direction d;
            int from,to;
            bool d_is_set=false;
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
                else if (args == 1)
                {
                    int move_id = from-1;
                    if (move_id >=0 && move_id<moves.size())
                    {
                        valid_input = 1;
                        from = moves[move_id].frst_m+1;
                        to = moves[move_id].last_m+1;
                        d = moves[move_id].direction;
                        d_is_set = true;
                    }

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
            if (valid_input && d_is_set == false)
            {
                for (int i = 0; i< 6; ++i)
                {
                    if (strcasecmp(strbuf,IAbaloneBoard::directionLiterals[i]) == 0)
                    {
                        d = (standard_abalone::Direction)i;
                        d_is_set = true;
                    }
                }

            }

            if (valid_input && d_is_set)
            {
                IAbaloneBoard::move_record_t move = {abalone_board.currentTurn(), from - 1, to - 1, d,IAbaloneBoard::NO_DIRECTION, IAbaloneBoard::NO_MOVE};
                if (abalone_board.makeMove(move)==IAbaloneBoard::MOVE_OK)
                {
                    printer.Print(&abalone_board);
                    std::map<std::string,double> ai_vector;
                    abalone_board.ai_params(abalone_board.currentTurn(),ai_vector);
                    std::map<std::string,double>::const_iterator it =ai_vector.begin();
                    for (;it !=ai_vector.end();++it)
                    {
                        std::cout<< it->first << ":" << it->second << std::endl;
                    }


                }
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
