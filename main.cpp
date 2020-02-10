#include <iostream>
#include "BoardPrinterConsole.h"
#include "IAbaloneBoard.h"
#include "BoardFactory.h"
#include "terminal_colors.h"
#include "DefaultPlacer.h"
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

    IAbaloneBoard* abalone_board = abaloneBoardFactory(5);




    printf("Please select option:\n1] play against AI\n2] two human players\n");
    printf("3] ai vs ai \n4] ai vs rank \n5] rank vs rank \n6] play against rank\n7] 3 players\n");
    unsigned int option;
    char *line_read = (char *)NULL;
    bool invalid_input = true;
    while (invalid_input)
    {
        line_read = readline (">>> ");
        if (line_read && *line_read)
        {

            int args = sscanf(line_read,"%d",&option);
            if (args == 1 && (option-1)<7 )
            {
                invalid_input = false;
            }
            else printf("invalid option. please select number in range 1-7...\n");
        }
        if (line_read)
        {
            free (line_read);
            line_read = (char *)NULL;
        }
        else
        { exit(0);}
    }
    AbalonePlayer * players[MAX_PLAYERS]={0,0,0};


    int turn = 0;
    if (option == 2)
    {
        players[0]= new HumanConsolePlayer();
        players[1]= new HumanConsolePlayer();
    }
#ifdef ENABLE_AI
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
        players[1]= new AIPlayer(true,1);
    }
    else if (option == 6)
    {
        players[0]= new HumanConsolePlayer();
        players[1]= new AIPlayer(true,0);
    }
#endif
    else if (option == 7)
    {
        players[0]= new HumanConsolePlayer();
        players[1]= new HumanConsolePlayer();
        players[2]= new HumanConsolePlayer();
        /*players[0]= new AIPlayer(false,1);
        players[1]= new AIPlayer(true,0);
        players[2]= new AIPlayer(false,1);*/

    }
    else
    {
        exit(0);
    }

    DefaultPlacer placer;
    #ifdef ENABLE_AI
    AIPlayer * trainee =0;
#endif
    if(option == 7)
        placer.Place(abalone_board,3,11);
    else
    {
        placer.Place(abalone_board,2,14);
#ifdef ENABLE_AI
         trainee = new AIPlayer(false,1);
#endif
    }
    MoveRecorder recorder;
    //recorder.toSVG("/home/orr/.abalone/game1.svg");
    //exit(0);
    recorder.recordInitialPosition(*abalone_board);
    recorder.toSVG("/home/orr/.abalone/game.svg");

    printer.Print(*abalone_board);
    int num_turns = 0;
    while(true)
    {


                IAbaloneBoard::move_record_t move = players[abalone_board->currentTurn()]->getMove(*abalone_board);
                if (abalone_board->makeMove(move)==IAbaloneBoard::MOVE_OK)
                {

                    /*std::map<std::string,float> ai_vector;
                    std::vector<IAbaloneBoard::move_record_t > moves;
                    abalone_board->ai_params(abalone_board->currentTurn(),ai_vector,moves,true,1);
                    std::map<std::string,float>::const_iterator it =ai_vector.begin();
                    for (;it !=ai_vector.end();++it)
                    {
                        std::cout<< it->first << ":" << it->second << std::endl;
                    }
                    std::cout<< "total input vector length:" << ai_vector.size() << std::endl;
*/
                    recorder.addMove(move);
                    num_turns++;
                    if (num_turns>120)
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
                        recorder.toSVG("/home/orr/.abalone/game.svg");
                        //players[0]->control("shuffle",0);//shuffle
                        //players[1]->control("shuffle",0);//shuffle
                        //players[1]->control("train",(void*)&recorder);
                        //players[1]->control("save",0);//winner
                        #ifdef ENABLE_AI
                        if (option==5)
                        {
                        trainee->control("collect",(void*)&recorder);//
                        trainee->control("save",0);//winner
                        }
#endif
                        abalone_board->clearAllMarbles();
                        placer.Place(abalone_board,2,14);
                        recorder.clear();
                        recorder.recordInitialPosition(*abalone_board);
                        num_turns = 0;
                        continue;
                    }
                    printer.Print(*abalone_board);
                    if (abalone_board->dropCount(abalone_board->currentTurn())==6)
                    {
                        num_turns =0;
                        printf("game ended!\n");
                        recorder.toSVG("/home/orr/.abalone/game.svg");
                        #ifdef ENABLE_AI
                        trainee->control("collect",(void*)&recorder);//
                        trainee->control("save",0);//winner
#endif
                        players[0]->control("load",0);//ai


                        //players[turn]->control("save",0);//looser
                        abalone_board->clearAllMarbles();
                        placer.Place(abalone_board,2,14);

                        recorder.clear();
                        recorder.recordInitialPosition(*abalone_board);
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
    if (players[2]!=0)
        delete players[2];
#ifdef ENABLE_AI
    if (trainee)
        delete trainee;
#endif
    delete abalone_board;
    return 0;
}
