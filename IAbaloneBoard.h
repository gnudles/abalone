#ifndef IABALONEBOARD_H
#define IABALONEBOARD_H

#include <vector>
#include <map>
#include <string>

class IAbaloneBoard
{
public:
    static constexpr const char* directionLiterals[6] = {"ul","ur","r","dr","dl","l"};
    typedef enum {BLACK = 0, WHITE, RED, GREEN, BLUE, YELLOW, NO_MARBLE} MarbleColor;

    typedef enum {NO_DIRECTION = 0, UP_LEFT = 0, UP_RIGHT, RIGHT, DOWN_RIGHT,DOWN_LEFT,LEFT} Direction;
    inline Direction oppositeDirection(Direction d)
    {
        return (Direction)((d+3)%6);
    }
    typedef enum {MOVE_OK = 0, OPPONENT_MARBLES, INVALID_MOVE, NO_MARBLES_TO_PUSH, SUICIDAL,CANT_PUSH_IT, DONT_PUSH_YOURSELF, POINTS_TOO_FAR} MoveCode;
    typedef enum {ONE_MOVE, TWO_SLIDE, TWO_MOVE, TWO_PUSH_ONE, TWO_KILL_ONE, THREE_SLIDE, THREE_MOVE, THREE_PUSH_ONE, THREE_KILL_ONE, THREE_PUSH_TWO, THREE_PUSH_ONE_KILL_ONE, NO_MOVE,MT_COUNT=NO_MOVE} MoveType;
    typedef struct {MarbleColor color; int frst_m; int last_m; Direction direction; Direction ftol_dir; MoveType mt;} move_record_t;
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
    virtual MoveCode makeMove(move_record_t & move) = 0;
    virtual MoveCode testMove( move_record_t & move) const = 0;
    virtual void nextTurn() = 0;
    virtual MarbleColor currentTurn() const = 0;
    virtual int takenDown(MarbleColor color) const = 0;
    virtual void possibleMoves(MarbleColor color, std::vector<move_record_t > &result) const = 0;
    virtual const move_record_t& getLastMove() const = 0;
    virtual void ai_params(MarbleColor my_color, std::map<std::string,float>& ai_vec, std::vector<move_record_t > &moves,bool before,int revision) const = 0;
};

#endif // IABALONEBOARD_H
