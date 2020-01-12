#ifndef IABALONEBOARD_H
#define IABALONEBOARD_H

#include <vector>

class IAbaloneBoard
{
public:
    typedef enum {BLACK = 0, WHITE, RED, GREEN, BLUE, YELLOW, NO_MARBLE} MarbleColor;
    typedef enum {UP_LEFT = 0, UP_RIGHT, RIGHT, DOWN_RIGHT,DOWN_LEFT,LEFT} Direction;
    typedef enum {MOVE_OK = 0, OPPONENT_MARBLES, INVALID_MOVE, NO_MARBLES_TO_PUSH, SUICIDAL,CANT_PUSH_IT, DONT_PUSH_YOURSELF, POINTS_TOO_FAR} MoveCode;
    /*
     *    0 \   / 1
     *  5 <-     -> 2
     *    4 /   \ 3
     */
    virtual int numberOfRows() const = 0;
    virtual int numberOfCells() const = 0;
    virtual int numberOfPlayers() const = 0;
    virtual int hexagonEdgeLength() const = 0;
    virtual std::vector<int> cellsInRow() const = 0;
    virtual MarbleColor getMarbleAt(int position) const = 0;
    virtual void addMarble(int position, MarbleColor marble) = 0;
    virtual int rotatePosition(int position, int rotation) const = 0;
    virtual MoveCode makeMove(MarbleColor color, int first_marble_pos, int last_marble_pos, Direction d, bool activate) = 0;
    virtual void nextTurn() = 0;
    virtual MarbleColor currentTurn() const = 0;
    virtual int takenDown(MarbleColor color) const = 0;
};

#endif // IABALONEBOARD_H
