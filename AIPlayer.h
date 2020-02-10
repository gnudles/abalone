#ifndef AIPLAYER_H
#define AIPLAYER_H
#ifdef ENABLE_AI
#include <fann.h>
#include "AbalonePlayer.h"
const int NET_INPUT_LAYER[2]={160,230};
#define MAX_NET_INPUT 256
class AIPlayer : public AbalonePlayer
{
public:
    AIPlayer(bool rank_player,int revision);
    virtual ~AIPlayer();
    virtual IAbaloneBoard::move_record_t getMove(const IAbaloneBoard &abalone_board);
    virtual void control(const std::string &cmd, void* data);
private:
    struct fann *ann;
    std::vector <std::pair<std::array<float,MAX_NET_INPUT>,float> > training_data;
    int _revision;
};
#endif //ENABLE_AI
#endif // AIPLAYER_H
