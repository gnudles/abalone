#ifndef IPLACER_H
#define IPLACER_H
#include <vector>

class IAbaloneBoard;
class IPlacer
{
public:
    virtual void Place(IAbaloneBoard* board, int num_players, int marbles) = 0;
    virtual std::vector<int> numberOfMarblesPossible(int num_players) = 0;
};

#endif // IPLACER_H
