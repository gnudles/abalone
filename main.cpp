#include <iostream>
#include "BoardPrinterConsole.h"
#include "AbaloneBoard.h"
#include "terminal_colors.h"
#include "HexDefaultPlacer.h"
#include <HumanConsolePlayer.h>
#include <AIPlayer.h>
#include <readline/readline.h>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

#include "MoveRecorder.h"
using namespace std;


std::string mk_private_dir(const char * dir)
{
    struct passwd *pw = getpwuid(getuid());

    std::string homedir = pw->pw_dir;
    if (homedir.back() != '/')
        homedir+="/";
    homedir +=dir;

    int err = mkdir(homedir.c_str(), 0777);
    if (err == -1) {perror("mkdir"); homedir = "";}
    return homedir;
}

int main()
{
    mk_private_dir(".abalone");

    BoardPrinterConsole printer;
    using standard_abalone  = AbaloneBoard<5,2>;
    standard_abalone abalone_board;

    HexDefaultPlacer placer;
    placer.Place(&abalone_board,2,14);
    MoveRecorder recorder;
    //recorder.toSVG("/home/orr/.abalone/game.svg");

    recorder.recordInitialPosition(&abalone_board);


    printf("Please select option:\n1] play against AI\n2] two human players\n");
    printf("3] ai vs ai \n4] ai vs rank \n5] rank vs rank \n6] play against rank\n");
    unsigned int option;
    char *line_read = (char *)NULL;
    bool invalid_input = true;
    while (invalid_input)
    {
        line_read = readline (">>> ");
        if (line_read && *line_read)
        {

            int args = sscanf(line_read,"%d",&option);
            if (args == 1 && (option-1)<6 )
            {
                invalid_input = false;
            }
            else printf("invalid option. please type 1 or 2 or 3 or 4 or 5 or 6...\n");
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
        players[1]= new AIPlayer(false,0);
    }
    else if (option == 3)
    {
        players[0]= new AIPlayer(false,0);
        players[1]= new AIPlayer(false,1);
    }
    else if (option == 4)
    {
        players[0]= new AIPlayer(false,1);
        players[1]= new AIPlayer(true,0);
    }
    else if (option == 5)
    {
        players[0]= new AIPlayer(true,0);
        players[1]= new AIPlayer(true,0);
    }
    else if (option == 6)
    {
        players[0]= new HumanConsolePlayer();
        players[1]= new AIPlayer(true,0);
    }
    else
    {
        exit(0);
    }
    printer.Print(&abalone_board);
    int num_turns = 0;
    while(true)
    {


                IAbaloneBoard::move_record_t move = players[turn]->getMove(abalone_board);
                if (abalone_board.makeMove(move)==IAbaloneBoard::MOVE_OK)
                {

                    std::map<std::string,float> ai_vector;
                    std::vector<IAbaloneBoard::move_record_t > moves;
                    abalone_board.ai_params(abalone_board.currentTurn(),ai_vector,moves,true,1);
                    std::map<std::string,float>::const_iterator it =ai_vector.begin();
                    for (;it !=ai_vector.end();++it)
                    {
                        std::cout<< it->first << ":" << it->second << std::endl;
                    }
                    std::cout<< "total input vector length:" << ai_vector.size() << std::endl;

                    recorder.addMove(move);
                    num_turns++;
                    if (num_turns>90)
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
                        //players[0]->control("shuffle",0);//shuffle
                        //players[1]->control("shuffle",0);//shuffle
                        //players[1]->control("train",(void*)&recorder);
                        //players[1]->control("save",0);//winner
                        abalone_board.clearAllMarbles();
                        placer.Place(&abalone_board,2,14);
                        recorder.clear();
                        recorder.recordInitialPosition(&abalone_board);
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
                        players[turn^1]->control("collect",(void*)&recorder);//
                        //players[turn^1]->control("save",0);//winner
                        //players[turn]->control("shuffle",(void*)&recorder);//looser
                        players[turn]->control("collect",(void*)&recorder);//looser

                        //players[turn]->control("save",0);//looser
                        abalone_board.clearAllMarbles();
                        placer.Place(&abalone_board,2,14);
                        recorder.toSVG("/home/orr/.abalone/game.svg");
                        recorder.clear();
                        recorder.recordInitialPosition(&abalone_board);
                        turn = 0;
                        continue;
                    }



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
