#ifndef ABALONEBOARD_H
#define ABALONEBOARD_H
#include <array>
#include <assert.h>
#include <unordered_set>

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
    using rows_array_t = std::array<int , rows>;
    using cells_array_t = std::array<int , cells>;
    using cells_neigh_array_t = std::array<std::array<int,6> , cells>;
    /*
     *    0 \   / 1
     *  5 <-     -> 2
     *    4 /   \ 3
     */


    static constexpr cells_array_t initializeRowOfCell()
        {
            cells_array_t conv_map = cells_array_t();
            int index=0;
            for (size_t i = 0; i < rows; ++i)
            {
                int cr=calc_cells_in_row(i);
                for (size_t j=0; j< cr;++j)
                {
                    conv_map[index] = i;
                    index++;
                }
            }
            return conv_map;
        }
    static constexpr cells_array_t initializeCellPosInRow()
        {
            cells_array_t conv_map = cells_array_t();
            int index=0;
            for (size_t i = 0; i < rows; ++i)
            {
                int cr=calc_cells_in_row(i);
                for (size_t j=0; j< cr;++j)
                {
                    conv_map[index] = j;
                    index++;
                }
            }
            return conv_map;
        }
    static constexpr int calc_cells_in_row(size_t ind1)
        {
            return (ind1 < edge_length)?  (edge_length+ind1):(rows+edge_length-ind1-1);
        }
    static constexpr rows_array_t initializeCellsInRow()
        {
            rows_array_t conv_map = rows_array_t();
            for (size_t i = 0; i < rows; ++i)
            {
                conv_map[i]=calc_cells_in_row(i);
            }
            return conv_map;
        }
    static constexpr rows_array_t initializeRowBeginning()
        {
            rows_array_t conv_map = rows_array_t();
            int count = 0;
            for (size_t i = 0; i < rows; ++i)
            {
                conv_map[i]=count;
                count+=calc_cells_in_row(i);
            }
            return conv_map;
        }



    static constexpr rows_array_t cells_in_row{initializeCellsInRow()};
    static constexpr rows_array_t row_beginning{initializeRowBeginning()};
    static constexpr inline int row_column_to_pos(size_t row,size_t column) noexcept
        {
        if (row>=rows) return -1;
         if (column>=cells_in_row[row]) return -1;
         return row_beginning[row]+column;
        }
    static constexpr cells_array_t row_of_cell{initializeRowOfCell()};
    static constexpr cells_array_t pos_in_row_of_cell{initializeCellPosInRow()};
    static constexpr inline short retrieve_pixel(std::array<std::array<short,rows*2-1>,rows> & pixels, int x, int y)
    {
        if (x>=pixels[0].size() || x < 0 || y<0 || y>= pixels.size())
            return -1;
        return pixels[y][x];
    }
    static constexpr cells_neigh_array_t initializeCellNeighbors()
    {
        std::array<std::array<short,rows*2-1>,rows> pixels={-1};
        for (int i=0;i<pixels.size();++i)
            for (int j=0;j<pixels[i].size();++j)
                pixels[i][j]=-1;
        int c = 0;
        for (int y=0; y< rows ; y++)
        {
            int x = rows-cells_in_row[y];
            for (int j = 0; j< cells_in_row[y]; ++j)
            {
                pixels[y][x]=c;
                x+=2;
                c++;
            }
        }
        /*
         *    0 \   / 1
         *  5 <-     -> 2
         *    4 /   \ 3
         */
        cells_neigh_array_t neigh = cells_neigh_array_t();
        c = 0;
        for (int y=0; y < rows ; y++)
        {
            int x = rows - cells_in_row[y];
            for (int j = 0; j < cells_in_row[y]; ++j)
            {
                neigh[c][UP_LEFT]=retrieve_pixel(pixels,x-1,y-1);
                neigh[c][UP_RIGHT]=retrieve_pixel(pixels,x+1,y-1);
                neigh[c][RIGHT]=retrieve_pixel(pixels,x+2,y);
                neigh[c][DOWN_RIGHT]=retrieve_pixel(pixels,x+1,y+1);
                neigh[c][DOWN_LEFT]=retrieve_pixel(pixels,x-1,y+1);
                neigh[c][LEFT]=retrieve_pixel(pixels,x-2,y);
                x+=2;
                c++;
            }
        }
        return neigh;

    }
    static constexpr cells_neigh_array_t cell_neighbors{initializeCellNeighbors()};
    static constexpr cells_array_t initializeCellRotation()
    {
        cells_array_t conv_map = cells_array_t();
        int pos = 4;
        int c = 0;
        for (int i=0; i< rows ; i++)
        {
            int travel_pos=pos;
            for (int j = 0; j< cells_in_row[i]; ++j)
            {
                conv_map[c] = travel_pos;
                travel_pos = cell_neighbors[travel_pos][DOWN_RIGHT];
                c++;
            }
            if (cell_neighbors[pos][LEFT]!=-1)
            {
                pos = cell_neighbors[pos][LEFT];
            }
            else
                pos = cell_neighbors[pos][DOWN_LEFT];
        }
        /*
        for (int i=0; i< cells;++i)
        {
            int new_row =0;
            int new_column =0;
            if (row_of_cell[i]<= rows/2 && pos_in_row_of_cell[i] < edge_length)
            {
                new_row = pos_in_row_of_cell[i];
                new_column = cells_in_row[new_row] - row_of_cell[i] - 1;
            }
            else
            {
                if (pos_in_row_of_cell[i] > edge_length && row_of_cell[i]<= rows/2)
                {
                    new_row = row_of_cell[i]+ rows/2 -(cells_in_row[row_of_cell[i]] - 1 - pos_in_row_of_cell[i]);

                    new_column = cells_in_row[new_row]-1-(cells_in_row[row_of_cell[i]] - 1 - pos_in_row_of_cell[i]);
                }
                else
                {
                    //new_row = pos_in_row_of_cell[i] + row_of_cell[i] - rows/2;
                    //new_column = pos_in_row_of_cell[i] + rows - 1 - row_of_cell[i];
                }

            }
            conv_map[i] = row_column_to_pos(new_row, new_column);

        }*/
        return conv_map;
    }
    static constexpr cells_array_t rotated_cells{initializeCellRotation()};
public:
    AbaloneBoard():cell_marble({NONE})
    {
        clearAllMarbles();
    }
    virtual int rotatePosition(int position, int rotation) const
    {
        for (int i =0; i<rotation;++i)
            position = rotated_cells[position];
        return position;
    }

    void clearAllMarbles(){
        for (int i = 0; i< cells; ++i)
        {
            cell_marble[i]=NONE;
        }
        marbles_by_player[0].clear();
        marbles_by_player[1].clear();
        marbles_by_player[2].clear();
    }
    void addMarble(int position, MarbleColor marble){assert(position>=0 && position<cells); cell_marble[position]=marble; marbles_by_player[(int)marble].insert(position);}
private:
    std::array<MarbleColor, cells> cell_marble;
    std::unordered_set<int> marbles_by_player[3];

};

#endif // ABALONEBOARD_H
