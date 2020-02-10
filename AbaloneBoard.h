#ifndef ABALONEBOARD_H
#define ABALONEBOARD_H
#define SECRET_ABALONE_BOARD_CLASS
#ifdef  SECRET_ABALONE_BOARD_CLASS
#include <array>
#include <assert.h>
#include <map>
#include <math.h>

#include "IAbaloneBoard.h"


template<typename T>
inline void swap_value(T& a, T& b)
{
    a^=b;
    b^=a;
    a^=b;
}
template <int L>
class AbaloneBoard : public IAbaloneBoard
{
    //static_assert(_NPlyr == 2 || _NPlyr == 3, "Number of players must be either 2 or 3!");
    static_assert(L >=5 && L<=8, "Currently only support edges of 5-8");
public:

    static constexpr int edge_length = L;
    static constexpr int rows = edge_length*2-1;
    static constexpr int cells = (edge_length+rows)*edge_length-rows;
    virtual int numberOfRows() const {return rows;}
    virtual int numberOfCells() const {return cells;}
    virtual int hexagonEdgeLength() const {return edge_length;}
    virtual int numberOfPlayers() const {return _nPlyr;}
    virtual std::vector<int> cellsInRow() const { std::vector<int> res; res.assign(cells_in_row.cbegin(),cells_in_row.cend()); return res;}


    virtual IAbaloneBoard::MarbleColor getMarbleAt(int position) const {
        assert(position>=0 && position<cells);
        return (IAbaloneBoard::MarbleColor)cell_marble[position].first;
    }
private:
    using rows_array_t = std::array<int , rows>;
    using cells_array_t = std::array<int , cells>;
    using cells_coord_array_t = std::array<std::array<float,2> , cells>;
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
    static constexpr inline int32_t const_abs(int32_t x) noexcept
    {
        if (x==0x80000000)
        {
            return 0x7fffffff;
        }
        return (x<0)?-x:x;
    }
    static constexpr cells_coord_array_t initializeCartCoords() //center piece is (0,0)
    {
        cells_coord_array_t coords=cells_coord_array_t();
        for (int i =0 ;i< cells;++i)
        {
            int r = row_of_cell[i];
            int clmn = pos_in_row_of_cell[i];
            float x=-(edge_length-1) + clmn + const_abs(r-(edge_length-1))*0.5;
            float y=(r-(edge_length-1))*TRIANGLE_HEIGHT;
            coords[i][0]=x;
            coords[i][1]=y;
        }
        return coords;

    }
    static constexpr cells_coord_array_t cartesian_coordinates{initializeCartCoords()};
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
    static constexpr cells_neigh_array_t initializeCellRhombNeighbors()
    {


        cells_neigh_array_t rhomb_neigh = cells_neigh_array_t();
        for (int i=0; i < cells ; i++)
        {
            for (int j = 0; j < 6; ++j)
            {
                rhomb_neigh[i][j]=cell_neighbors[i][j];
                if (rhomb_neigh[i][j] != -1 )
                {
                    rhomb_neigh[i][j] = cell_neighbors[rhomb_neigh[i][j]][(j+1)%6];
                }
            }
        }
        return rhomb_neigh;

    }
    static constexpr cells_neigh_array_t cell_rhomb_neighbors{initializeCellRhombNeighbors()};
    static constexpr cells_array_t initializeCellRotation()
    {
        cells_array_t conv_map = cells_array_t();
        int pos = edge_length - 1;
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


    }

public:
    //use this only after running testMove
    void _executeMove(const move_record_t &move)
    {
        const struct {int mine_count; int push_count;} helper_array[NO_MOVE]=
        {
        {1,0}, //ONE_MOVE
        {2,0}, //TWO_SLIDE
        {2,0}, //TWO_MOVE
        {2,1}, //TWO_PUSH_ONE,
        {2,1}, //TWO_KILL_ONE
        {3,0}, //THREE_SLIDE
        {3,0}, //THREE_MOVE
        {3,1}, //THREE_PUSH_ONE
        {3,1}, //THREE_KILL_ONE
        {3,2}, //THREE_PUSH_TWO
        {3,2} //THREE_PUSH_ONE_KILL_ONE
        };
        int pivot = move.last_m;

        int push_count = helper_array[move.mt].push_count;
        int mine_count = helper_array[move.mt].mine_count;
        if (push_count == 1)
        {
            pivot = cell_neighbors[move.last_m][move.direction];
        }
        else if (push_count == 2)
        {
            pivot = cell_far_neighbors[move.last_m][move.direction];
        }
        int adv_dir = oppositeDirection(move.ftol_dir);
        if (move.mt == TWO_KILL_ONE || move.mt == THREE_KILL_ONE || move.mt == THREE_PUSH_ONE_KILL_ONE)
        {
            lost_marbles[marbleAt(pivot)]++;
            I_dropped[marbleAt(move.frst_m)]++;
        }
        for (int i=push_count+mine_count ;i > 0; --i)
        {
            moveSingleMarble(pivot,cell_neighbors[pivot][move.direction]);
            pivot = cell_neighbors [pivot][adv_dir];
        }

    }


    AbaloneBoard():cell_marble({std::pair<MarbleColor,int>(NO_MARBLE,-1)})
    {
        clearAllMarbles();
    }
    virtual ~AbaloneBoard(){}
    virtual void setNumberOfPlayers(int nPlyr)
    {
        assert (nPlyr>=2 && nPlyr <=3);
        _nPlyr = nPlyr;
    }
    virtual int rotatePosition(int position, int rotation) const
    {
        if (rotation<0)
            rotation += 6;
        for (int i =0; i<rotation;++i)
            position = rotated_cells[position];
        return position;
    }
    virtual int verticalMirrorPosition(int position) const
    {

        return row_column_to_pos(rows-1-row_of_cell[position],pos_in_row_of_cell[position]);

    }
    virtual int horizontalMirrorPosition(int position) const
    {
        int row = row_of_cell[position];
        return row_column_to_pos(row,cells_in_row[row] -1 - pos_in_row_of_cell[position]);

    }

    virtual int neighbor(int position, Direction d) const
    {
        assert (((int)d)>=0 && ((int)d)<6);
        assert (position>=0 && position<cells);
        return cell_neighbors[position][d];
    }
    virtual int farNeighbor(int position, Direction d) const
    {
        assert (((int)d)>=0 && ((int)d)<6);
        assert (position>=0 && position<cells);
        return cell_far_neighbors[position][d];
    }
    virtual int playerMarblePosition(int player, int marble_id) const
    {
        assert ( player >=0 && player < _nPlyr);
        assert (((unsigned int)marble_id) < marbles_by_player[player].size());
        return marbles_by_player[player][marble_id];
    }
    virtual int playerNumberOfMarbles(int player) const
    {
        assert ( player >=0 && player < _nPlyr);
        return marbles_by_player[player].size();
    }

    virtual MarbleColor marbleAt(int position) const
    {
        assert (position>=0 && position<cells);
        return cell_marble[position].first;
    }
    virtual int marbleIdAt(int position) const
    {
        assert (position>=0 && position<cells);
        return cell_marble[position].second;
    }
    void clearAllMarbles(){
        for (int i = 0; i< cells; ++i)
        {
            cell_marble[i]=std::pair<MarbleColor,int>(NO_MARBLE,-1);
        }
        for (int i = 0; i< MAX_PLAYERS; ++i)
        {
            marbles_by_player[i].clear();
            lost_marbles[i] = 0;
        }
        current_turn = BLACK;
        last_move ={WHITE,0,0,NO_DIRECTION,NO_DIRECTION,NO_MOVE};
    }
    void addMarble(int position, MarbleColor marble){
        assert(position>=0 && position<cells);
        assert(cell_marble[position].first == NO_MARBLE);
        assert(marble != NO_MARBLE);

        int inserted_position = marbles_by_player[(int)marble].size();
        marbles_by_player[(int)marble].push_back(position);
        cell_marble[position]=std::pair<MarbleColor,int>(marble,inserted_position);
    }
    virtual MoveCode makeMove(move_record_t& move)
    {
            MoveCode result = testMove(move);
            if (result == MOVE_OK)
            {
                _executeMove(move);
                nextTurn();
                last_move = move;
            }

            return result;
    }
    MoveCode testMove(move_record_t & move) const
    {
        assert(move.color!= NO_MARBLE);
        assert(move.frst_m >= 0);
        assert(move.last_m >= 0);
        assert(move.frst_m < cells);
        assert(move.last_m < cells);
        move.mt = NO_MOVE;

        if (cell_marble[move.frst_m].first != move.color)
        {
            return (cell_marble[move.frst_m].first == NO_MARBLE)?NO_MARBLES_TO_PUSH:OPPONENT_MARBLES;
        }

        if (move.frst_m == move.last_m) // moving 1 marble
        {
            int destination = cell_neighbors[move.frst_m][move.direction];
            if (destination == -1)
                return SUICIDAL;
            if (cell_marble[destination].first == move.color)
                return DONT_PUSH_YOURSELF;
            if (cell_marble[destination].first != NO_MARBLE)
                return CANT_PUSH_IT;
            move.mt = ONE_MOVE;
            return MOVE_OK;
        }
        //moving 2 or more marbles...
        for (int i=0;i<6;++i)
        {
            if (cell_neighbors[move.frst_m][i] == move.last_m)
            {
                //moving 2 adjacent marbles.
                bool sliding = true;
                move.ftol_dir = (Direction)i;
                if (i == (int)move.direction) //moving on their axis
                {
                    sliding = false;
                }
                else if (((i+3)%6) == (int)move.direction) //moving on their axis
                {
                    sliding = false;
                    //swap last and first
                    swap_value(move.frst_m,move.last_m);
                    move.ftol_dir = move.direction;
                }
                int first_destination = cell_neighbors[move.frst_m][move.direction];
                int last_destination = cell_neighbors[move.last_m][move.direction];
                if (sliding)
                {

                    if (first_destination == -1 || last_destination == -1)
                        return SUICIDAL;
                    if (cell_marble[first_destination].first == move.color || cell_marble[last_destination].first == move.color)
                        return DONT_PUSH_YOURSELF;
                    if (cell_marble[first_destination].first != NO_MARBLE || cell_marble[last_destination].first != NO_MARBLE)
                        return CANT_PUSH_IT;

                    move.mt = TWO_SLIDE;
                    return MOVE_OK;
                }
                else
                {
                    if (last_destination == -1)
                        return SUICIDAL;
                    MarbleColor opponent_color = cell_marble[last_destination].first;
                    if (opponent_color == move.color)
                        return DONT_PUSH_YOURSELF;
                    if (opponent_color != NO_MARBLE)
                    {
                        //pushing opponent's marble
                        int after_dest = cell_neighbors[last_destination][move.direction];
                        bool push_one_to_water = (after_dest == -1);

                        bool push_one = push_one_to_water;
                        if (push_one_to_water == false)
                        {
                            push_one = (cell_marble[after_dest].first == NO_MARBLE);
                        }
                        if (push_one)
                        {
                            move.mt = push_one_to_water?TWO_KILL_ONE:TWO_PUSH_ONE;
                            return MOVE_OK;
                        }
                        else
                        {
                            return CANT_PUSH_IT;
                        }

                    }
                    else // moving to empty space
                    {
                        move.mt = TWO_MOVE;
                        return MOVE_OK;
                    }
                }
                assert(true == false);
                return INVALID_MOVE; // should never reach here actually.
            }
        }

        for (int i=0;i<6;++i) //try to look for 3 marbles
        {
            if (cell_far_neighbors[move.frst_m][i] == move.last_m)
            {
                int middle_marble_pos = cell_neighbors[move.frst_m][i];
                if (cell_marble[middle_marble_pos].first == move.color)
                {
                    bool sliding = true;
                    move.ftol_dir = (Direction)i;
                    if (i == (int)move.direction) //moving on their axis
                    {
                        sliding = false;
                    }
                    else if (((i+3)%6) == (int)move.direction) //moving on their axis
                    {
                        sliding = false;
                        //swap last and first
                        swap_value(move.frst_m,move.last_m);
                        move.ftol_dir = move.direction;
                    }
                    int first_destination = cell_neighbors[move.frst_m][move.direction];
                    int middle_destination = cell_neighbors[middle_marble_pos][move.direction];
                    int last_destination = cell_neighbors[move.last_m][move.direction];
                    if (sliding)
                    {

                        if (first_destination == -1 || middle_destination == -1 || last_destination == -1) //although middle == -1 without the others being equal to -1 is possible only in concave board
                            return SUICIDAL;
                        if (cell_marble[first_destination].first == move.color || cell_marble[middle_destination].first == move.color || cell_marble[last_destination].first == move.color)
                            return DONT_PUSH_YOURSELF;
                        if (cell_marble[first_destination].first != NO_MARBLE || cell_marble[middle_destination].first != NO_MARBLE || cell_marble[last_destination].first != NO_MARBLE)
                            return CANT_PUSH_IT;
                        move.mt = THREE_SLIDE;
                        return MOVE_OK;
                    }
                    else
                    {
                        if (last_destination == -1)
                            return SUICIDAL;
                        MarbleColor opponent_color = cell_marble[last_destination].first;
                        if (opponent_color == move.color)
                            return DONT_PUSH_YOURSELF;
                        if (opponent_color != NO_MARBLE)
                        {
                            int after_dest = cell_neighbors[last_destination][move.direction];
                            bool push_one_to_water = (after_dest == -1);

                            bool push_one = push_one_to_water;
                            if (push_one_to_water == false)
                            {
                                push_one = (cell_marble[after_dest].first == NO_MARBLE);
                            }
                            if (push_one)
                            {
                                move.mt = push_one_to_water?THREE_KILL_ONE:THREE_PUSH_ONE;
                                return MOVE_OK;
                            }
                            else
                            {
                                if (cell_marble[after_dest].first == move.color) // try to push one but afterward there is my marble.
                                {
                                    return INVALID_MOVE;
                                }
#ifdef MULTIPLAYER_REQUIRE_PUSH_WITH_SAME_COLOR
                                if (cell_marble[after_dest].first != opponent_color) // try to push one but afterward there is other one's marble.
                                {
                                    return INVALID_MOVE;
                                }
#endif
                                int after_after_dest = cell_neighbors[after_dest][move.direction];
                                bool push_two_to_water = (after_after_dest == -1);

                                bool push_two = push_two_to_water;
                                if (push_two_to_water == false)
                                {
                                    push_two = (cell_marble[after_after_dest].first == NO_MARBLE);
                                }
                                if (push_two)
                                {
                                    move.mt = push_two_to_water?THREE_PUSH_ONE_KILL_ONE:THREE_PUSH_TWO;
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
                            move.mt = THREE_MOVE;
                            return MOVE_OK;
                        }
                    }
                }
                return INVALID_MOVE;
            }
        }

        return INVALID_MOVE;


    }
    virtual void possibleMoves(MarbleColor color, std::vector<move_record_t> &result) const
    {
        assert(result.empty());
        assert(color<_nPlyr);
        std::vector<int>::const_iterator it = marbles_by_player[color].begin();
        std::vector<int>::const_iterator it_end = marbles_by_player[color].end();
        for (;it != it_end; ++it)
        {
            int cell = *it;
            if (cell < 0)
                continue;
            int d = 0;
            for (;d<6;++d)
            {
                int neigh_pos = cell_neighbors[cell][d];
                if(neigh_pos>=0)
                {
                    if (cell_marble[neigh_pos].first == NO_MARBLE)
                    {
                        result.push_back({color,cell,cell,(Direction)d,NO_DIRECTION,ONE_MOVE});
                    }
                    else if (d<3) // only on first three axis, in order to skip duplicates
                    {
                        if (cell_marble[neigh_pos].first == color)
                        {
                            for (int nd = 0 ;nd<6;++nd)
                            {
                                move_record_t move = {color,cell,neigh_pos,(Direction)nd,NO_DIRECTION,NO_MOVE};
                                if (testMove(move) == MOVE_OK)
                                {
                                    result.push_back((const move_record_t&)move);
                                }
                            }
                            int far_neigh = cell_far_neighbors[cell][d];
                            if(far_neigh>=0)
                            {
                                if (cell_marble[far_neigh].first == color)//we got three in the same color
                                {
                                    for (int nd = 0 ; nd<6 ;++nd)
                                    {
                                        move_record_t move = {color,cell,far_neigh,(Direction)nd,NO_DIRECTION,NO_MOVE};
                                        if (testMove(move) == MOVE_OK)
                                        {
                                            result.push_back((const move_record_t&)move);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }
    }
    virtual void nextTurn()
    {
        current_turn = (MarbleColor) ((current_turn+1)%_nPlyr);
    }
    virtual MarbleColor currentTurn() const
    {
        return current_turn;
    }

    virtual int lostMarbles(MarbleColor color) const
    {
        return lost_marbles[color];
    }
    virtual int dropCount(MarbleColor color) const
    {
        return I_dropped[color];
    }
    void getClusters(MarbleColor color,std::vector<std::vector<int> > & clusters) const
    {
        assert(clusters.empty());
        std::vector<int> m_in_g;
        std::vector<int> g_of_m;

        std::vector<int>::const_iterator it;
        std::vector<int>::const_iterator it_end = marbles_by_player[color].end();
        int g = 0;
        for (it = marbles_by_player[color].begin(); it != it_end;++it)
        {
            bool exists = (*it>=0);
            g_of_m.push_back(g);
            m_in_g.push_back(exists?1:0);
            g++;
        }
        int i = 0;
        for (it = marbles_by_player[color].begin(); it != it_end;++it,++i)
        {
            if (*it>=0)
            for (int d =0 ;d< 3 ;++d)
            {
                int neigh = cell_neighbors[*it][d];
                if (neigh>=0)
                {
                    if (cell_marble[neigh].first == color)
                    {
                        int neigh_m = cell_marble[neigh].second;
                        int neigh_g = g_of_m[neigh_m];
                        int my_g = g_of_m[i];
                        if (my_g!=neigh_g)
                        {

                            if (m_in_g[neigh_g] == 1)
                            {
                                g_of_m[neigh_m] = my_g;
                                m_in_g[neigh_g]--;
                                m_in_g[my_g]++;
                            }
                            else
                            {
                                if (m_in_g[my_g] == 1)
                                {
                                    g_of_m[i] = neigh_g;
                                    m_in_g[my_g]--;
                                    m_in_g[neigh_g]++;
                                }
                                int from_g;
                                int to_g;
                                if (m_in_g[my_g]>=m_in_g[neigh_g])
                                {
                                    from_g = neigh_g;
                                    to_g = my_g;
                                }
                                else
                                {
                                    from_g = my_g;
                                    to_g = neigh_g;
                                }
                                for (int m = 0; m< g_of_m.size();m++)
                                {
                                    if (g_of_m[m] == from_g)
                                    {
                                        g_of_m[m] = to_g;
                                        m_in_g[from_g]--;
                                        m_in_g[to_g]++;
                                        if (m_in_g[from_g] == 0)
                                            break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        std::map<int,std::vector<int> > clusters_map;
        std::map<int,std::vector<int> >::iterator map_iter;
        for (int m =0; m < g_of_m.size(); ++m)
        {
            if (marbles_by_player[color][m]>=0)
            {
                if (m_in_g[g_of_m[m]]>0)
                {
                    map_iter = clusters_map.find(g_of_m[m]);
                    if (map_iter== clusters_map.end())
                    {
                        clusters_map[ g_of_m[m] ] = std::vector<int>();
                    }
                    clusters_map[ g_of_m[m] ].push_back(marbles_by_player[color][m]);
                }
            }
        }
        for (map_iter = clusters_map.begin(); map_iter!= clusters_map.end() ;++map_iter)
        {
            clusters.push_back((*map_iter).second);

        }
    }
    virtual float cellCoordX(int position) const
    {
        assert (position>=0 && position< cells);
        return cartesian_coordinates[position][0];
    }
    virtual float cellCoordY(int position) const
    {
        assert (position>=0 && position< cells);
        return cartesian_coordinates[position][1];
    }
    void center_of_mass(const std::vector<int> & positions, float &cx, float &cy, float &R, float &deviation) const
    {
        cx = 0;
        cy = 0;
        std::vector<int>::const_iterator it;
        std::vector<int>::const_iterator it_end = positions.end();
        int count = 0;
        for (it = positions.begin(); it != it_end;++it)
        {
            int pos = *it;
            if (pos>=0)
            {
                count ++;
                cx+=cartesian_coordinates[pos][0];
                cy+=cartesian_coordinates[pos][1];
            }
        }
        if (count>0)
        {
            cx/=count;
            cy/=count;
        }
        R = sqrt(cx*cx+cy*cy);
        deviation = 0;
        for (it = positions.begin(); it != it_end;++it)
        {
            int pos = *it;
            if (pos>=0)
            {
                float diffx=cx-cartesian_coordinates[pos][0];
                float diffy=cy-cartesian_coordinates[pos][1];
                deviation+=diffx*diffx+diffy*diffy;
            }
        }
        if (count>0)
        {
            deviation/=count;
        }
        deviation = sqrtf(deviation);
    }
    virtual const move_record_t& getLastMove() const
    {
        return last_move;
    }
private:
    void ai_params_layers(MarbleColor my_color,MarbleColor rival_color, std::map<std::string,float>& out, int revision) const
    {
        int pivot = 0;
        int dir;
        float count[2][edge_length];
        for (int i = edge_length; i>0;--i)
        {
            int total_in_ring = 0;
            count[0][i-1] = 0;
            count[1][i-1] = 0;
            dir = RIGHT;
            if (i == 1)
            {
                if (cell_marble[pivot].first !=NO_MARBLE)
                {
                    count[cell_marble[pivot].first][i-1]++;
                }
                total_in_ring++;
            }
            else
            {
                for (int j=0;j<6;++j)
                {
                    for(int k=0;k<i-1;++k)
                    {
                        if (cell_marble[pivot].first !=NO_MARBLE)
                        {
                            count[cell_marble[pivot].first][i-1]++;
                        }
                        total_in_ring++;
                        pivot=cell_neighbors[pivot][dir];
                    }
                    dir=(dir+1)%6;
                }
                pivot = cell_neighbors[pivot][DOWN_RIGHT];
            }
            if (revision>=1)
            {
                count[my_color][i-1]/=(float)total_in_ring;
                count[rival_color][i-1]/=(float)total_in_ring;
            }
            out[std::string("my-ring")+std::to_string(i)+"-count"] = count[my_color][i-1];
            out[std::string("rival-ring")+std::to_string(i)+"-count"] = count[rival_color][i-1];
        }
    }

    void ai_params_thirds(MarbleColor my_color,MarbleColor rival_color, std::map<std::string,float>& out, int revision) const
    {

        int dp=0;
        int pivot;
        int rot_pivot;
        float count[2][6]={{0},{0}};

        for (int i=0; i < edge_length; ++i)
        {
            pivot = dp;
            for (int j=0; j < edge_length; ++j)
            {
                rot_pivot=pivot;

                for (int k = 0; k < 6;++k)
                {
                    if (cell_marble[rot_pivot].first !=NO_MARBLE)
                        count[cell_marble[rot_pivot].first][k]++;
                    rot_pivot = rotatePosition(rot_pivot,1);
                }


                pivot = cell_neighbors[pivot][RIGHT];
            }
            dp = cell_neighbors[dp][DOWN_RIGHT];
        }
        for (int i = 0; i < 6; ++i)
        {
            if (revision>=1)
            {
                count[my_color][i]/=16.0;
                count[rival_color][i]/=16.0;
            }
            out[std::string("my-third")+std::to_string(i)+"-count"] = count[my_color][i];
            out[std::string("rival-third")+std::to_string(i)+"-count"] = count[rival_color][i];
        }
    }
    void ai_params_min_row(MarbleColor color, const std::string& id, std::map<std::string,float>& out, int revision) const
    {
        float axis_min[6]={rows,rows,rows};
        float axis_max[6]={rows,rows,rows};
        std::vector<int>::const_iterator it;
        std::vector<int>::const_iterator it_end = marbles_by_player[color].end();
        for (it = marbles_by_player[color].begin(); it != it_end;++it)
        {
            int pos =*it;
            for (int i=0; i< 3; ++i)
            {
                int r = row_of_cell[pos];
                if (r<axis_min[i])
                {
                    axis_min[i] = r;
                }
                if ( rows-1-r < axis_max[i])
                {
                    axis_max[i]=rows-1-r;
                }
                pos = rotatePosition(pos,1);
            }
        }
        if (revision>=1)
        {
            axis_min[0]/=16.0;
            axis_max[0]/=16.0;
            axis_min[1]/=16.0;
            axis_max[1]/=16.0;
            axis_min[2]/=16.0;
            axis_max[2]/=16.0;
        }
        out[id+"-axis-1-min"]=axis_min[0];
        out[id+"-axis-1-max"]=axis_max[0];
        out[id+"-axis-2-min"]=axis_min[1];
        out[id+"-axis-2-max"]=axis_max[1];
        out[id+"-axis-3-min"]=axis_min[2];
        out[id+"-axis-3-max"]=axis_max[2];
    }
    void ai_params_move_count(MarbleColor color, const std::string& id, std::map<std::string,float>& out, std::vector<move_record_t > &moves, int revision) const
    {
        possibleMoves(color,moves);
        //out["number-of-my-moves"]=my_moves.size();
        float move_types_count[MT_COUNT] = {0};
        float adder;
        if (revision==0)
            adder = 1.0;
        else
            adder = 0.125;
        for(int i=0; i< moves.size(); ++i)
        {
            move_types_count[moves[i].mt]+=adder;
        }
        out[id+"-one-move-count"]=move_types_count[ONE_MOVE];
        out[id+"-two-slide-count"]=move_types_count[TWO_SLIDE];
        out[id+"-two-move-count"]=move_types_count[TWO_MOVE];
        out[id+"-two-push-count"]=move_types_count[TWO_PUSH_ONE];
        out[id+"-two-kill-count"]=move_types_count[TWO_KILL_ONE];
        out[id+"-three-slide-count"]=move_types_count[THREE_SLIDE];
        out[id+"-three-move-count"]=move_types_count[THREE_MOVE];
        out[id+"-three-push1-count"]=move_types_count[THREE_PUSH_ONE];
        out[id+"-three-kill1-count"]=move_types_count[THREE_KILL_ONE];
        out[id+"-three-push2-count"]=move_types_count[THREE_PUSH_TWO];
        out[id+"-three-kill2-count"]=move_types_count[THREE_PUSH_ONE_KILL_ONE];
        if (id == "rival")
        {
            int max_kill=0;
            int total_kill=0;
            const unsigned int kill_mask = (1<<TWO_KILL_ONE) |(1<<THREE_KILL_ONE)|(1<<THREE_PUSH_ONE_KILL_ONE);
            for(int i=0;i<moves.size();++i)
            {
                AbaloneBoard<L> x= *this;
                x._executeMove(moves[i]);
                std::vector<move_record_t> next_moves;
                x.possibleMoves(color,next_moves);


                int kill_count = 0;
                for (int nm=0;nm<next_moves.size();++nm)
                {

                    if ((kill_mask&(1<<next_moves[nm].mt))!=0)
                    {
                        kill_count++;
                    }
                }
                if (kill_count > max_kill)
                    max_kill = kill_count;
                if (kill_count>0)
                    total_kill++;
            }
            out["rival-next-max-kill"]=max_kill;
            if (revision>=1)
            {
                out["rival-next-max-kill"]/=4.0;
                out["rival-next-total-kill"]=(float)total_kill/32.0;
            }
        }
    }

    void ai_clusters_count(MarbleColor my_color,MarbleColor rival_color, std::map<std::string,float>& out) const
    {
        std::vector<std::vector<int>> my_clusters;
        std::vector<std::vector<int>> rival_clusters;
        getClusters(my_color,my_clusters);
        getClusters(rival_color,rival_clusters);

        out["my-number-of-clusters"] = (float)my_clusters.size()/4.0;

        out["rival-number-of-clusters"] = (float)rival_clusters.size()/4.0;

/*

        float my_cx,my_cy,my_R,my_deviation;
        center_of_mass(marbles_by_player[my_color], my_cx,my_cy,my_R,my_deviation);
        float rival_cx,rival_cy,rival_R,rival_deviation;
        center_of_mass(marbles_by_player[rival_color], rival_cx,rival_cy,rival_R,rival_deviation);

        float distance_me_rival = sqrtf((my_cx - rival_cx)*(my_cx - rival_cx) + (my_cy-rival_cy)*(my_cy-rival_cy));
        ai_vec["my-distance-from-center"]=my_R;
        ai_vec["my-deviation"]=my_deviation;
        ai_vec["rival-distance-from-center"]=rival_R;
        ai_vec["rival-deviation"]=rival_deviation;
*/
    }
    void ai_neigh_count(MarbleColor my_color,MarbleColor rival_color, std::map<std::string,float>& out, int revision) const
    {
        std::vector<int>::const_iterator it;
        std::vector<int>::const_iterator it_end = marbles_by_player[my_color].end();
        float near_count = 0;
        float far_count = 0;
        float rhomb_count = 0;
        float my_triple_count[3] = {0};
        float me_rival_near_count = 0;
        float me_rival_far_count = 0;
        float me_rival_rhomb_count = 0;
        float rival_near_count = 0;
        float rival_far_count = 0;
        float rival_rhomb_count = 0;
        float rival_triple_count[3] = {0};
        float count_friendly_neigh[7]={0};
        float count_foe_neigh[7]={0};
        float rival_count_friendly_neigh[7]={0};
        float rival_count_foe_neigh[7]={0};
        for (it = marbles_by_player[my_color].begin(); it != it_end;++it)
        {
            int pos = *it;
            if (pos>=0)
            {
                int my_neigh_single_marble_count_friendly = 0;
                int my_neigh_single_marble_count_foe = 0;
                for (int i = 0; i<6; ++i)
                {

                    int near_pos = cell_neighbors[pos][i];
                    int far_pos= cell_far_neighbors[pos][i];
                    int rhomb_pos = cell_rhomb_neighbors[pos][i];
                    bool had_near =false;
                    bool had_far =false;

                    if (near_pos>=0)
                    {
                        if (cell_marble[near_pos].first==my_color)
                        {
                            my_neigh_single_marble_count_friendly++;
                            if (i<3)
                            {
                                near_count++;
                                had_near =true;
                            }
                        }
                        else
                            if(cell_marble[near_pos].first!=NO_MARBLE)
                            {
                                my_neigh_single_marble_count_foe++;
                                me_rival_near_count++;
                            }
                    }
                    if (far_pos >= 0)
                    {
                        if (cell_marble[far_pos].first == my_color)
                        {
                            if (i<3)
                            {
                                far_count++;
                                had_far = true;
                            }
                        }
                        else
                            if(cell_marble[far_pos].first!=NO_MARBLE)
                            {
                                me_rival_far_count++;
                            }
                    }
                    if (had_near && had_far)//triple
                    {
                        my_triple_count[i]++;
                    }
                    if (rhomb_pos>=0)
                    {
                        if (cell_marble[rhomb_pos].first==my_color)
                        {
                            if (i<3)
                            {
                                rhomb_count++;
                            }
                        }
                        else
                            if(cell_marble[rhomb_pos].first!=NO_MARBLE)
                            {
                                me_rival_rhomb_count++;
                            }
                    }
                }
                count_friendly_neigh[my_neigh_single_marble_count_friendly]++;
                count_foe_neigh[my_neigh_single_marble_count_foe]++;
            }
        }
        it_end = marbles_by_player[rival_color].end();
        for (it = marbles_by_player[rival_color].begin(); it != it_end;++it)
        {
            int pos = *it;
            if (pos>=0)
            {
                int rival_neigh_single_marble_count_friendly = 0;
                int rival_neigh_single_marble_count_foe = 0;
                for (int i = 0; i<6; ++i)
                {
                    int near_pos = cell_neighbors[pos][i];
                    int far_pos= cell_far_neighbors[pos][i];
                    int rhomb_pos = cell_rhomb_neighbors[pos][i];
                    bool had_near =false;
                    bool had_far =false;

                    if (near_pos>=0)
                    {

                        if (cell_marble[near_pos].first==rival_color)
                        {
                            if (i<3)
                            {
                                rival_near_count++;
                                had_near = true;
                            }
                            rival_neigh_single_marble_count_friendly++;
                        }
                        else
                            if(cell_marble[near_pos].first!=NO_MARBLE)
                            {
                                rival_neigh_single_marble_count_foe++;
                            }

                    }
                    if (far_pos >= 0)
                    {

                        if (cell_marble[far_pos].first == rival_color)
                        {
                            if (i<3)
                            {
                                rival_far_count++;
                                had_far = true;
                            }
                        }
                    }
                    if (had_near && had_far)
                    {
                        rival_triple_count[i]++;
                    }
                    if (rhomb_pos >= 0)
                    {

                        if (cell_marble[rhomb_pos].first == rival_color)
                        {
                            if (i<3)
                                rival_rhomb_count++;
                        }
                    }
                }
                rival_count_foe_neigh[rival_neigh_single_marble_count_foe]++;
                rival_count_friendly_neigh[rival_neigh_single_marble_count_friendly]++;
            }
        }

        if (revision>=1)
        {
            for (int i=0 ; i<7; ++i)
            {
                out["my-marbles-with-"+std::to_string(i)+"-friendly-neighbors"]= count_friendly_neigh[i]/8.0;
                out["my-marbles-with-"+std::to_string(i)+"-foe-neighbors"]= count_foe_neigh[i]/8.0;
                out["rival-marbles-with-"+std::to_string(i)+"-friendly-neighbors"]= rival_count_friendly_neigh[i]/8.0;
                out["rival-marbles-with-"+std::to_string(i)+"-foe-neighbors"]= rival_count_foe_neigh[i]/8.0;
            }
            near_count/=32.0;
            far_count/=16.0;
            rhomb_count/=16.0;
            rival_near_count/=32.0;
            rival_far_count/=16.0;
            rival_rhomb_count/=16.0;
            my_triple_count[0]/=8.0;
            my_triple_count[1]/=8.0;
            my_triple_count[2]/=8.0;
            rival_triple_count[0]/=8.0;
            rival_triple_count[1]/=8.0;
            rival_triple_count[2]/=8.0;
            me_rival_near_count/=8.0;
            me_rival_rhomb_count/=8.0;
            me_rival_far_count/=8.0;
        }
        out["my-total-neighbors"]= near_count;
        out["my-total-far-neighbors"]= far_count;
        out["my-total-rhomb-neighbors"]= rhomb_count;
        out["my-triples-count-axis1"]= my_triple_count[0];
        out["my-triples-count-axis2"]= my_triple_count[1];
        out["my-triples-count-axis3"]= my_triple_count[2];
        out["cross-total-neighbors"]= me_rival_near_count;
        out["cross-total-far-neighbors"]= me_rival_far_count;
        out["cross-total-rhomb-neighbors"]= me_rival_rhomb_count;
        out["rival-total-neighbors"]= rival_near_count;
        out["rival-total-far-neighbors"]= rival_far_count;
        out["rival-triples-count-axis1"]= rival_triple_count[0];
        out["rival-triples-count-axis2"]= rival_triple_count[1];
        out["rival-triples-count-axis3"]= rival_triple_count[2];
        out["rival-total-rhomb-neighbors"]= rival_rhomb_count;
    }
public:
    void ai_params(MarbleColor my_color, std::map<std::string,float>& ai_vec, std::vector<move_record_t > &moves, bool before, int revision) const
    {
        assert(_nPlyr == 2);
        MarbleColor rival_color=(my_color==BLACK)?WHITE:BLACK;
        float my_cx,my_cy,my_R,my_deviation;
        center_of_mass(marbles_by_player[my_color], my_cx,my_cy,my_R,my_deviation);
        float rival_cx,rival_cy,rival_R,rival_deviation;
        center_of_mass(marbles_by_player[rival_color], rival_cx,rival_cy,rival_R,rival_deviation);
        float distance_me_rival = sqrtf((my_cx - rival_cx)*(my_cx - rival_cx) + (my_cy-rival_cy)*(my_cy-rival_cy));
        if (revision>=1)
        {
            my_R/=8.0;
            my_deviation/=8.0;
            rival_R/=8.0;
            rival_deviation/=8.0;
            distance_me_rival/=8.0;
        }
        ai_vec["my-distance-from-center"]=my_R;
        ai_vec["my-deviation"]=my_deviation;
        ai_vec["rival-distance-from-center"]=rival_R;
        ai_vec["rival-deviation"]=rival_deviation;
        //if (rival)
        ai_vec["distance-between-me-rival"]=distance_me_rival;
        if (revision == 0
                || (revision>=1 && before))
            ai_vec["my-lost-dears"]=lost_marbles[my_color];
        ai_vec["rival-lost-dears"]=lost_marbles[rival_color];
        if (revision>=1)
        {
            if (before)
                ai_vec["my-lost-dears"]/=8.0;
            ai_vec["rival-lost-dears"]/=8.0;
        }
        ai_params_move_count(my_color, "my", ai_vec, moves,revision);
        std::vector<move_record_t > rival_moves;
        ai_params_move_count(rival_color, "rival", ai_vec,rival_moves,revision);
        ai_params_thirds(my_color,rival_color,ai_vec,revision);
        ai_params_layers(my_color,rival_color,ai_vec,revision);
        ai_neigh_count( my_color, rival_color,  ai_vec, revision);
        ai_params_min_row(my_color, "my", ai_vec,revision);
        ai_params_min_row(rival_color, "rival", ai_vec,revision);


        if (revision>0)
        {
            ai_clusters_count(my_color,rival_color,ai_vec);
            const unsigned int kill_mask = (1<<IAbaloneBoard::TWO_KILL_ONE) |(1<<IAbaloneBoard::THREE_KILL_ONE)|(1<<IAbaloneBoard::THREE_PUSH_ONE_KILL_ONE);
            const unsigned int push_mask = (1<<IAbaloneBoard::TWO_PUSH_ONE) |(1<<IAbaloneBoard::THREE_PUSH_ONE)|(1<<IAbaloneBoard::THREE_PUSH_TWO);
            ai_vec["last-move-kill"]=(((1<<last_move.mt)&  kill_mask) !=0 )?1:0;
            ai_vec["last-move-push"]=(((1<<last_move.mt)&  push_mask) !=0 )?1:0;
        }

    }
private:

    std::array<std::pair<MarbleColor,int>, cells> cell_marble;
    std::vector<int> marbles_by_player[MAX_PLAYERS];
    move_record_t last_move;
    int _nPlyr;
    int lost_marbles[MAX_PLAYERS];
    int I_dropped[MAX_PLAYERS];
    MarbleColor current_turn;



};


#endif //SECRET_ABALONE_BOARD_CLASS
#endif // ABALONEBOARD_H
