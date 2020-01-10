#ifndef HEXDEFAULTPLACER_H
#define HEXDEFAULTPLACER_H
#include "IPlacer.h"

class HexDefaultPlacer : IPlacer
{

public:
    HexDefaultPlacer();
    virtual void Place(IAbaloneBoard *board, int num_players, int marbles);
    virtual std::vector<int> numberOfMarblesPossible(int num_players){return std::vector<int>({12,14});}
private:

};

#endif // HEXDEFAULTPLACER_H
