#define SECRET_ABALONE_BOARD_CLASS
#include "AbaloneBoard.h"
IAbaloneBoard* abaloneBoardFactory(int size)
{
    if (size == 5)
    {
        return new AbaloneBoard<5>();
    }
    if (size == 6)
    {
        return new AbaloneBoard<6>();
    }
    if (size == 7)
    {
        return new AbaloneBoard<7>();
    }
    if (size == 8)
    {
        return new AbaloneBoard<8>();
    }
    return 0;
}
