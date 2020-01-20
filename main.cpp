#include <iostream>
#include "BoardPrinterConsole.h"
#include "AbaloneBoard.h"
#include "terminal_colors.h"
#include "HexDefaultPlacer.h"
#include <HumanConsolePlayer.h>
#include <AIPlayer.h>
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



    printf("Please select option:\n1] play against AI\n2] two human players\n\n");
    unsigned int option;
    char *line_read = (char *)NULL;
    bool invalid_input = true;
    while (invalid_input)
    {
        line_read = readline (">>> ");
        if (line_read && *line_read)
        {

            int args = sscanf(line_read,"%d",&option);
            if (args == 1 && (option-1)<3 )
            {
                invalid_input = false;
            }
            else printf("invalid option. please type either 1 or 2...\n");
        }
        if (line_read)
        {
            free (line_read);
            line_read = (char *)NULL;
        }
        else
        { exit(0);}
    }
    AbalonePlayer * players[2];
    int turn = 0;
    if (option == 2)
    {
        players[0]= new HumanConsolePlayer();
        players[1]= new HumanConsolePlayer();
    }
    else if (option == 1)
    {
        players[0]= new HumanConsolePlayer();
        players[1]= new AIPlayer();
    }
    else if (option == 3)
    {
        players[0]= new AIPlayer();
        players[1]= new AIPlayer();
    }
    printer.Print(&abalone_board);
    int num_turns = 0;
    while(true)
    {


                IAbaloneBoard::move_record_t move = players[turn]->getMove(abalone_board);
                if (abalone_board.makeMove(move)==IAbaloneBoard::MOVE_OK)
                {
                    num_turns++;
                    if (num_turns>160)
                    {
                        printf("too much turns!\n");
                        printf("**********************************************\n");
                        printf("**********************************************\n");
                        printf("**********************************************\n");
                        printf("**********************************************\n");
                        printf("**********************************************\n");
                        printf("**********************************************\n");
                        printf("**********************************************\n");
                        printf("**********************************************\n");
                        players[0]->control("shuffle");//shuffle
                        //players[1]->control("shuffle");//shuffle
                        abalone_board.clearAllMarbles();
                        placer.Place(&abalone_board,2,14);
                        turn = 0;
                        num_turns = 0;
                        continue;
                    }
                    printer.Print(&abalone_board);
                    turn^=1;
                    if (abalone_board.takenDown(abalone_board.currentTurn())==6)
                    {
                        num_turns =0;
                        printf("game ended!\n");

                        players[turn^1]->control("save");//shuffle
                        players[turn]->control("load");//shuffle
                        abalone_board.clearAllMarbles();
                        placer.Place(&abalone_board,2,14);
                        turn = 0;
                        continue;
                    }
                    /*std::map<std::string,double> ai_vector;
                    abalone_board.ai_params(abalone_board.currentTurn(),ai_vector);
                    std::map<std::string,double>::const_iterator it =ai_vector.begin();
                    for (;it !=ai_vector.end();++it)
                    {
                        std::cout<< it->first << ":" << it->second << std::endl;
                    }
                    std::cout<< "total input vector length:" << ai_vector.size() << std::endl;*/


                }
                else
                {
                    printf("invalid move!\n");
                }


    }

    delete players[0];
    delete players[1];
    return 0;
}
