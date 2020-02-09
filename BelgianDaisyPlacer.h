#ifndef HEXDEFAULTPLACER_H
#define HEXDEFAULTPLACER_H
#include "IPlacer.h"

class BelgianDaisyPlacer : IPlacer
{

public:
    BelgianDaisyPlacer();
    virtual void Place(IAbaloneBoard *board, int num_players, int marbles);
    virtual std::vector<int> numberOfMarblesPossible(int num_players){
        if (num_players==2){ return std::vector<int>({14});}

        return std::vector<int>();}
    virtual std::vector<int> numberOfPlayersPossible(){return std::vector<int>({2,3});}
private:

};

#endif // HEXDEFAULTPLACER_H
