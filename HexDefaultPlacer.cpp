#include "HexDefaultPlacer.h"
#include "IAbaloneBoard.h"
HexDefaultPlacer::HexDefaultPlacer()
{

}

static int32_t my_rand()
{
    static int32_t rand_num = 2344251;
    rand_num*=7;
    rand_num = rand_num ^ ((rand_num+5156)>> (rand_num&15));
    rand_num = rand_num + ((rand_num-36161)<< (rand_num&15));
    rand_num = rand_num ^ ((rand_num+0x10101010)>> (rand_num&15));
    rand_num = rand_num + ((rand_num-36161)<< (rand_num&15));
    return rand_num;
}
void HexDefaultPlacer::Place(IAbaloneBoard *board, int num_players, int marbles)
{
    const static int places_black[14]={0,1,2,3,4,
                                6,7,8,9,
                                13,14,15,5,10};

    int rot =  ((unsigned int)my_rand())%6;
    if (num_players == 2)
    {
        if ((marbles == 14 || marbles == 12) && board->hexagonEdgeLength() == 5)
        {
            for (int i=0; i <marbles ; ++i)
            {

                board->addMarble(board->rotatePosition(places_black[i],rot),IAbaloneBoard::BLACK);
                board->addMarble(board->rotatePosition(places_black[i],(rot+3)%6),IAbaloneBoard::WHITE);
            }

        }
    }
    if (num_players == 3)
    {
        if ((marbles == 14 || marbles == 12) && board->hexagonEdgeLength() == 5)
        {
            for (int i=0; i <marbles ; ++i)
            {

                board->addMarble(places_black[i],IAbaloneBoard::BLACK);
                board->addMarble(board->rotatePosition(places_black[i],2),IAbaloneBoard::WHITE);
                board->addMarble(board->rotatePosition(places_black[i],4),IAbaloneBoard::RED);
            }

        }
    }
}


