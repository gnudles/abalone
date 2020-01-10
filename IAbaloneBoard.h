#ifndef IABALONEBOARD_H
#define IABALONEBOARD_H

#include <vector>

class IAbaloneBoard
{
public:
    typedef enum {WHITE = 0, BLACK, RED, NONE} MarbleColor;
    typedef enum {UP_LEFT = 0, UP_RIGHT, RIGHT, DOWN_RIGHT,DOWN_LEFT,LEFT} Directions;
    /*
     *    0 \   / 1
     *  5 <-     -> 2
     *    4 /   \ 3
     */
    virtual int numberOfRows() const = 0;
    virtual int numberOfCells() const = 0;
    virtual int hexagonEdgeLength() const = 0;
    virtual std::vector<int> cellsInRow() const = 0;
    virtual MarbleColor getMarbleAt(int position) const = 0;
    virtual void addMarble(int position, MarbleColor marble) = 0;
    virtual int rotatePosition(int position, int rotation) const = 0;
};

#endif // IABALONEBOARD_H
