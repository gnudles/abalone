#include "BelgianDaisyPlacer.h"
#include "IAbaloneBoard.h"
BelgianDaisyPlacer::BelgianDaisyPlacer()
{

}


void BelgianDaisyPlacer::Place(IAbaloneBoard *board, int num_players, int marbles)
{
    const static int places_black[7]={4,5,9,10,11,
                                16,17};


    if (num_players == 2)
    {
        if ((marbles == 14 ) && board->hexagonEdgeLength() == 5)
        {
            for (int i=0; i <7 ; ++i)
            {

                board->addMarble(places_black[i],IAbaloneBoard::BLACK);
                board->addMarble(board->rotatePosition(places_black[i],3),IAbaloneBoard::BLACK);
                board->addMarble(board->verticalMirrorPosition(places_black[i]),IAbaloneBoard::WHITE);
                board->addMarble(board->verticalMirrorPosition(board->rotatePosition(places_black[i],3)),IAbaloneBoard::WHITE);
            }
            board->setNumberOfPlayers(2);

        }
    }

}


