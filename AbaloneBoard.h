#ifndef ABALONEBOARD_H
#define ABALONEBOARD_H
#include <array>
#include <assert.h>
#include <unordered_set>

#include "IAbaloneBoard.h"
template<typename T>
inline void swap_value(T& a, T& b)
{
    a^=b;
    b^=a;
    a^=b;
}
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
    virtual int numberOfPlayers() const {return _NPlyr;}
    virtual std::vector<int> cellsInRow() const { std::vector<int> res; res.assign(cells_in_row.cbegin(),cells_in_row.cend()); return res;}


    virtual IAbaloneBoard::MarbleColor getMarbleAt(int position) const {
        assert(position>=0 && position<cells);
        return (IAbaloneBoard::MarbleColor)cell_marble[position].first;
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
    static constexpr cells_neigh_array_t initializeCellFarNeighbors()
    {


        cells_neigh_array_t far_neigh = cells_neigh_array_t();
        for (int i=0; i < cells ; i++)
        {
            for (int j = 0; j < 6; ++j)
            {
                far_neigh[i][j]=cell_neighbors[i][j];
                if (far_neigh[i][j] != -1 )
                {
                    far_neigh[i][j] = cell_neighbors[far_neigh[i][j]][j];
                }
            }
        }
        return far_neigh;

    }
    static constexpr cells_neigh_array_t cell_far_neighbors{initializeCellFarNeighbors()};
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
        return conv_map;
    }
    static constexpr cells_array_t rotated_cells{initializeCellRotation()};

    inline void moveSingleMarble(int from, int to)
    {
        assert(to>= -1 && to < cells);
        assert(from>= 0 && to < cells);
        MarbleColor color = cell_marble[from].first;
        cell_marble[from].first = NO_MARBLE;
        int index = cell_marble[from].second;
        cell_marble[from].second = -1;
        marbles_by_player[(int)color][index] = to;
        if (to != -1)
        {

            cell_marble[to].first = color;
            cell_marble[to].second = index;
        }
        else
        {
            marbles_dropped[color]++;
        }

    }

    MoveCode __makeMove(MarbleColor color, int first_marble_pos, int last_marble_pos, Direction d, bool activate)
    {
        assert(color!= NO_MARBLE);
        assert(first_marble_pos >= 0);
        assert(last_marble_pos >= 0);
        assert(first_marble_pos < cells);
        assert(last_marble_pos < cells);

        if (cell_marble[first_marble_pos].first != color)
        {
            return (cell_marble[first_marble_pos].first == NO_MARBLE)?NO_MARBLES_TO_PUSH:OPPONENT_MARBLES;
        }
        int num_marbles = 0;
        if (first_marble_pos == last_marble_pos) // moving 1 marble
        {
            num_marbles = 1;
            int destination = cell_neighbors[first_marble_pos][d];
            if (destination == -1)
                return SUICIDAL;
            if (cell_marble[destination].first == color)
                return DONT_PUSH_YOURSELF;
            if (cell_marble[destination].first != NO_MARBLE)
                return CANT_PUSH_IT;
            if (activate)
            {
                moveSingleMarble(first_marble_pos, destination);
            }
            return MOVE_OK;
        }
        //moving 2 or more marbles...
        for (int i=0;i<6;++i)
        {
            if (cell_neighbors[first_marble_pos][i] == last_marble_pos)
            {
                //moving 2 adjacent marbles.
                bool sliding = true;
                if (i == (int)d) //moving on their axis
                {
                    sliding = false;
                }
                else if (((i+3)%6) == (int)d) //moving on their axis
                {
                    sliding = false;
                    //swap last and first
                    swap_value(first_marble_pos,last_marble_pos);
                }
                int first_destination = cell_neighbors[first_marble_pos][d];
                int last_destination = cell_neighbors[last_marble_pos][d];
                if (sliding)
                {

                    if (first_destination == -1 || last_destination == -1)
                        return SUICIDAL;
                    if (cell_marble[first_destination].first == color || cell_marble[last_destination].first == color)
                        return DONT_PUSH_YOURSELF;
                    if (cell_marble[first_destination].first != NO_MARBLE || cell_marble[last_destination].first != NO_MARBLE)
                        return CANT_PUSH_IT;
                    if (activate)
                    {
                        moveSingleMarble(first_marble_pos, first_destination);
                        moveSingleMarble(last_marble_pos, last_destination);
                    }
                    return MOVE_OK;
                }
                else
                {
                    if (last_destination == -1)
                        return SUICIDAL;
                    MarbleColor opponent_color = cell_marble[last_destination].first;
                    if (opponent_color == color)
                        return DONT_PUSH_YOURSELF;
                    if (opponent_color != NO_MARBLE)
                    {
                        //pushing opponent's marble
                        int after_dest = cell_neighbors[last_destination][d];
                        bool push_one_to_water = (after_dest == -1);

                        bool push_one = push_one_to_water;
                        if (push_one_to_water == false)
                        {
                            push_one = (cell_marble[after_dest].first == NO_MARBLE);
                        }
                        if (push_one)
                        {
                            if (activate)
                            {
                                moveSingleMarble(last_destination, after_dest);
                                moveSingleMarble(last_marble_pos, last_destination);
                                moveSingleMarble(first_marble_pos, first_destination);
                            }
                            return MOVE_OK;
                        }
                        else
                        {
                            return CANT_PUSH_IT;
                        }

                    }
                    else // moving to empty space
                    {
                        if (activate)
                        {
                            moveSingleMarble(last_marble_pos, last_destination);
                            moveSingleMarble(first_marble_pos, first_destination);
                        }
                        return MOVE_OK;
                    }
                }
                assert(true == false);
                return INVALID_MOVE; // should never reach here actually.
            }
        }

        for (int i=0;i<6;++i) //try to look for 3 marbles
        {
            if (cell_far_neighbors[first_marble_pos][i] == last_marble_pos)
            {
                int middle_marble_pos = cell_neighbors[first_marble_pos][i];
                if (cell_marble[middle_marble_pos].first == color)
                {
                    bool sliding = true;
                    if (i == (int)d) //moving on their axis
                    {
                        sliding = false;
                    }
                    else if (((i+3)%6) == (int)d) //moving on their axis
                    {
                        sliding = false;
                        //swap last and first
                        swap_value(first_marble_pos,last_marble_pos);
                    }
                    int first_destination = cell_neighbors[first_marble_pos][d];
                    int middle_destination = cell_neighbors[middle_marble_pos][d];
                    int last_destination = cell_neighbors[last_marble_pos][d];
                    if (sliding)
                    {

                        if (first_destination == -1 || middle_destination == -1 || last_destination == -1) //although middle == -1 without the others being equal to -1 is possible only in concave board
                            return SUICIDAL;
                        if (cell_marble[first_destination].first == color || cell_marble[middle_destination].first == color || cell_marble[last_destination].first == color)
                            return DONT_PUSH_YOURSELF;
                        if (cell_marble[first_destination].first != NO_MARBLE || cell_marble[middle_destination].first != NO_MARBLE || cell_marble[last_destination].first != NO_MARBLE)
                            return CANT_PUSH_IT;
                        if (activate)
                        {
                            moveSingleMarble(first_marble_pos, first_destination);
                            moveSingleMarble(middle_marble_pos, middle_destination);
                            moveSingleMarble(last_marble_pos, last_destination);
                        }
                        return MOVE_OK;
                    }
                    else
                    {
                        if (last_destination == -1)
                            return SUICIDAL;
                        MarbleColor opponent_color = cell_marble[last_destination].first;
                        if (opponent_color == color)
                            return DONT_PUSH_YOURSELF;
                        if (opponent_color != NO_MARBLE)
                        {
                            int after_dest = cell_neighbors[last_destination][d];
                            bool push_one_to_water = (after_dest == -1);

                            bool push_one = push_one_to_water;
                            if (push_one_to_water == false)
                            {
                                push_one = (cell_marble[after_dest].first == NO_MARBLE);
                            }
                            if (push_one)
                            {
                                if (activate)
                                {
                                    moveSingleMarble(last_destination, after_dest);//opponent marble
                                    moveSingleMarble(last_marble_pos, last_destination);
                                    moveSingleMarble(middle_marble_pos, middle_destination);
                                    moveSingleMarble(first_marble_pos, first_destination);
                                }
                                return MOVE_OK;
                            }
                            else
                            {
                                if (cell_marble[after_dest].first == color) // try to push one but afterward there is my marble.
                                {
                                    return INVALID_MOVE;
                                }
#ifdef MULTIPLAYER_REQUIRE_PUSH_WITH_SAME_COLOR
                                if (cell_marble[after_dest].first != opponent_color) // try to push one but afterward there is other one's marble.
                                {
                                    return INVALID_MOVE;
                                }
#endif
                                int after_after_dest = cell_neighbors[after_dest][d];
                                bool push_two_to_water = (after_after_dest == -1);

                                bool push_two = push_two_to_water;
                                if (push_two_to_water == false)
                                {
                                    push_two = (cell_marble[after_after_dest].first == NO_MARBLE);
                                }
                                if (push_two)
                                {
                                    if (activate)
                                    {
                                        moveSingleMarble(after_dest, after_after_dest);//opponent marble
                                        moveSingleMarble(last_destination, after_dest);//opponent marble
                                        moveSingleMarble(last_marble_pos, last_destination);
                                        moveSingleMarble(middle_marble_pos, middle_destination);
                                        moveSingleMarble(first_marble_pos, first_destination);
                                    }
                                    return MOVE_OK;
                                }
                                else
                                {
                                    return INVALID_MOVE;
                                }
                            }
                        }
                        else //NOT PUSHING, JUST MOVING
                        {
                            if (activate)
                            {
                                moveSingleMarble(last_marble_pos, last_destination);
                                moveSingleMarble(middle_marble_pos, middle_destination);
                                moveSingleMarble(first_marble_pos, first_destination);
                            }
                            return MOVE_OK;
                        }
                    }
                }
                return INVALID_MOVE;
            }
        }

        return INVALID_MOVE;

        //int first_marble_row = row_of_cell()
    }


public:
    AbaloneBoard():cell_marble({std::pair<MarbleColor,int>(NO_MARBLE,-1)})
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
            cell_marble[i]=std::pair<MarbleColor,int>(NO_MARBLE,-1);
        }
        for (int i = 0; i< _NPlyr; ++i)
        {
            marbles_by_player[i].clear();
            marbles_dropped[i] = 0;
        }
        current_turn = BLACK;


    }
    void addMarble(int position, MarbleColor marble){
        assert(position>=0 && position<cells);
        assert(cell_marble[position].first == NO_MARBLE);
        assert(marble != NO_MARBLE);

        int inserted_position = marbles_by_player[(int)marble].size();
        marbles_by_player[(int)marble].push_back(position);
        cell_marble[position]=std::pair<MarbleColor,int>(marble,inserted_position);
    }
virtual MoveCode makeMove(MarbleColor color, int first_marble_pos, int last_marble_pos, Direction d, bool activate)
{
        MoveCode result = __makeMove(color, first_marble_pos, last_marble_pos, d, activate);
        if (result == MOVE_OK && activate)
        {
            move_recorder.push_back({first_marble_pos,last_marble_pos,d});
            nextTurn();
        }
}
    virtual void nextTurn()
    {
        current_turn = (MarbleColor) ((current_turn+1)%_NPlyr);
    }
    virtual MarbleColor currentTurn() const
    {
        return current_turn;
    }
    virtual int takenDown(MarbleColor color) const
    {
        return marbles_dropped[color];
    }
private:

    std::array<std::pair<MarbleColor,int>, cells> cell_marble;
    std::vector<int> marbles_by_player[_NPlyr];
    typedef struct {int f_m; int l_m; Direction dir;} move_record_t;
    std::vector<move_record_t> move_recorder;
    int marbles_dropped[_NPlyr];
    MarbleColor current_turn;

};

#endif // ABALONEBOARD_H
