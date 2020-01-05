#ifndef ABALONEBOARD_H
#define ABALONEBOARD_H
#include <array>
#include <assert.h>

#include "IAbaloneBoard.h"

template <int L, int _NPlyr>
class AbaloneBoard : public IAbaloneBoard
{
    static_assert(_NPlyr == 2 || _NPlyr == 3, "Number of players must be either 2 or 3!");
    static_assert(L == 5, "Currently only support edges of 5.");
public:

    static constexpr int edge_length = L;
    static constexpr int rows = edge_length*2-1;
    static constexpr int cells = (edge_length+rows)*edge_length-rows;
    virtual int numberOfRows() const {return rows;}
    virtual int numberOfCells() const {return cells;}
    virtual int hexagonEdgeLength() const {return edge_length;}
    virtual std::vector<int> cellsInRow() const { std::vector<int> res; res.assign(cells_in_row.cbegin(),cells_in_row.cend()); return res;}
    virtual IAbaloneBoard::MarbleColor getMarbleAt(int position) const {
        assert(position>=0 && position<cells);
        return (IAbaloneBoard::MarbleColor)cell_marble[position];
    }
private:
    using cells_in_row_t = std::array<int , rows>;
    static constexpr int calc_cells_in_row(size_t ind1)
        {
            return (ind1 < edge_length)?  (edge_length+ind1):(rows+edge_length-ind1-1);
        }
    static constexpr cells_in_row_t initializeIndicesCellsInRow()
        {
            cells_in_row_t conv_map = cells_in_row_t();
            for (size_t i = 0; i < rows; ++i)
            {
                conv_map[i]=calc_cells_in_row(i);
            }
            return conv_map;
        }
public:
    AbaloneBoard():cell_marble({NONE})
    {
        //clearAllMarbles();
    }

    static constexpr cells_in_row_t cells_in_row{initializeIndicesCellsInRow()};



    void clearAllMarbles(){
        for (int i = 0; i< cells; ++i)
        {
            cell_marble[cells]=IAbaloneBoard::NONE;
        }
    }
    void addMarble(int position, MarbleColor marble){assert(position>=0 && position<cells); cell_marble[position]=marble;}
private:
    std::array<MarbleColor, cells> cell_marble;

};

#endif // ABALONEBOARD_H
