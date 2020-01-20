#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <fann.h>
#include "AbalonePlayer.h"

class AIPlayer : public AbalonePlayer
{
public:
    AIPlayer();
    virtual ~AIPlayer();
    virtual IAbaloneBoard::move_record_t getMove(const IAbaloneBoard &abalone_board);
    virtual void control(const std::string &cmd);
private:
    struct fann *ann;
};

#endif // AIPLAYER_H
