#ifndef HUMANCONSOLEPLAYER_H
#define HUMANCONSOLEPLAYER_H

#include "AbalonePlayer.h"
class HumanConsolePlayer : public AbalonePlayer
{
public:
    virtual IAbaloneBoard::move_record_t getMove(const IAbaloneBoard &abalone_board);
};

#endif // HUMANCONSOLEPLAYER_H
