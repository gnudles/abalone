#include "OffBoardPlacer.h"
#include "IAbaloneBoard.h"
OffBoardPlacer::OffBoardPlacer()
{

}

void OffBoardPlacer::Place(IAbaloneBoard *board, int num_players, int marbles)
{
    const static int places_black[6]={6,12,13,19,20,21};

    if (num_players == 2)
    {
        if ((marbles == 12) && board->hexagonEdgeLength() == 5)
        {
            for (int i=0; i <6 ; ++i)
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
