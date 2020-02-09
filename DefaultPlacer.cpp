#include "DefaultPlacer.h"
#include "IAbaloneBoard.h"
DefaultPlacer::DefaultPlacer()
{

}


void DefaultPlacer::Place(IAbaloneBoard *board, int num_players, int marbles)
{
    const static int places_black[14]={0,1,2,3,4,
                                6,7,8,9,
                                13,14,15,5,10};


    //int rot =  ((unsigned int)my_rand())%6;
    if (num_players == 2)
    {
        if ((marbles == 14 || marbles == 12) && board->hexagonEdgeLength() == 5)
        {
            for (int i=0; i <marbles ; ++i)
            {

                board->addMarble(places_black[i],IAbaloneBoard::BLACK);
                board->addMarble(board->rotatePosition(places_black[i],3),IAbaloneBoard::WHITE);
            }
            board->setNumberOfPlayers(2);

        }
    }
    if (num_players == 3)
    {
        if ((marbles == 11) && board->hexagonEdgeLength() == 5)
        {
            for (int i=0; i <marbles ; ++i)
            {

                board->addMarble(i,IAbaloneBoard::BLACK);
                board->addMarble(board->rotatePosition(i,2),IAbaloneBoard::WHITE);
                board->addMarble(board->rotatePosition(i,4),IAbaloneBoard::RED);
            }
            board->setNumberOfPlayers(3);

        }
    }
}


