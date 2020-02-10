#ifndef ABALONEPLAYER_H
#define ABALONEPLAYER_H
#include "IAbaloneBoard.h"
class AbalonePlayer
{
public:
    virtual ~AbalonePlayer(){}
    virtual IAbaloneBoard::move_record_t getMove(const IAbaloneBoard &board) = 0;
    virtual void control(const std::string &cmd, void* data){}
};
#endif // ABALONEPLAYER_H
