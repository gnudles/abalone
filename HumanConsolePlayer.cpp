#include "HumanConsolePlayer.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <readline/readline.h>
IAbaloneBoard::move_record_t HumanConsolePlayer::getMove(const IAbaloneBoard &abalone_board)
{

    char *line_read = (char *)NULL;
    std::vector<IAbaloneBoard::move_record_t> moves;
    abalone_board.possibleMoves(abalone_board.currentTurn(),moves);
    printf("possible moves for you:\n");
    for (int i=0;i<moves.size();++i)
    {
        printf("%d] %d-%d%s\n",i+1,moves[i].frst_m+1,moves[i].last_m+1,IAbaloneBoard::directionLiterals[moves[i].direction]);
    }
    IAbaloneBoard::move_record_t move;
    bool move_ready = false;
    while (!move_ready)
    {

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
                        d = (IAbaloneBoard::Direction)i;
                        d_is_set = true;
                    }
                }

            }

            if (valid_input && d_is_set)
            {
                move = {abalone_board.currentTurn(), from - 1, to - 1, d,IAbaloneBoard::NO_DIRECTION, IAbaloneBoard::NO_MOVE};
                move_ready = true;
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
        { exit(0);}
    }
    return move;
}
