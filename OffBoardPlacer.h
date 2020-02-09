#ifndef OFFBOARDPLACER_H
#define OFFBOARDPLACER_H

#include "IPlacer.h"

class OffBoardPlacer : IPlacer
{
public:
    OffBoardPlacer();
    virtual void Place(IAbaloneBoard *board, int num_players, int marbles);
    virtual std::vector<int> numberOfMarblesPossible(int num_players){
        if (num_players==2){ return std::vector<int>({12});}

        return std::vector<int>();
        }

    virtual std::vector<int> numberOfPlayersPossible(){return std::vector<int>({2});}
};

#endif // OFFBOARDPLACER_H
