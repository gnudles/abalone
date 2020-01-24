#ifndef MOVERECORDER_H
#define MOVERECORDER_H

#include "IAbaloneBoard.h"
class MoveRecorder
{
public:
    MoveRecorder();
    void toSVG(const std::string & path);
    void Place(IAbaloneBoard* board) const
    {
        int cell = 0;
        for (; cell< board->numberOfCells(); ++cell)
        {
            if (initial_position[cell]!= IAbaloneBoard::NO_MARBLE)
                board->addMarble(cell, initial_position[cell]);
        }
    }
    void recordInitialPosition(const IAbaloneBoard * board)
    {
        int cell = 0;
        for (; cell< board->numberOfCells(); ++cell)
        {
            initial_position.push_back(board->getMarbleAt(cell));
        }
    }
    void addMove(const IAbaloneBoard::move_record_t& move)
    {
        move_recorder.push_back(move);
    }
    void undoMove()
    {
        move_recorder.pop_back();
    }
    void clear()
    {
        move_recorder.clear();
        initial_position.clear();
    }
    IAbaloneBoard::MarbleColor winner() const { return move_recorder.back().color; }
    std::vector<IAbaloneBoard::move_record_t>& records()
    {
        return move_recorder;
    }

private:
    std::vector<IAbaloneBoard::move_record_t> move_recorder;
    std::vector<IAbaloneBoard::MarbleColor> initial_position;
};

#endif // MOVERECORDER_H
