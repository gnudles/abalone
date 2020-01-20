#ifndef ABALONEPLAYER_H
#define ABALONEPLAYER_H
#include <IAbaloneBoard.h>
class AbalonePlayer
{
public:
    virtual IAbaloneBoard::move_record_t getMove(const IAbaloneBoard &board) = 0;
    virtual void control(const std::string &cmd){}
};
#endif // ABALONEPLAYER_H
