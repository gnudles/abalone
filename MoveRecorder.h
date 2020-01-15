#ifndef MOVERECORDER_H
#define MOVERECORDER_H

#include "IAbaloneBoard.h"
class MoveRecorder
{
public:
    MoveRecorder();
    std::vector<IAbaloneBoard::move_record_t> move_recorder;
};

#endif // MOVERECORDER_H
