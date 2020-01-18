#ifndef ABALONEBOARD_H
#define ABALONEBOARD_H
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

    static constexpr cells_coord_array_t initializeCartCoords() //center piece is (0,0)
    {
        cells_coord_array_t coords=cells_coord_array_t();
        for (int i =0 ;i< cells;++i)
        {
            int r = row_of_cell[i];
            int clmn = pos_in_row_of_cell[i];
            float x=-(edge_length-1) + clmn + fabs(r-(edge_length-1))*0.5;
            float y=(r-(edge_length-1))*0.86602540378443864676372317f;
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


    void executeMove(const move_record_t &move)
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
        for (int i=push_count+mine_count ;i > 0; --i)
        {
            moveSingleMarble(pivot,cell_neighbors[pivot][move.direction]);
            pivot = cell_neighbors [pivot][adv_dir];
        }
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
    virtual MoveCode makeMove(move_record_t& move)
    {
            MoveCode result = testMove(move);
            if ((result == MOVE_OK))
            {
                executeMove(move);
                nextTurn();
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
        assert(color<_NPlyr);
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
    void center_of_mass(MarbleColor color, double &cx,double &cy, double &R, double &deviation)
    {
        cx = 0;
        cy = 0;
        std::vector<int>::const_iterator it;
        std::vector<int>::const_iterator it_end = marbles_by_player[color].end();
        int count = 0;
        for (it = marbles_by_player[color].begin(); it != it_end;++it)
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
        for (it = marbles_by_player[color].begin(); it != it_end;++it)
        {
            int pos = *it;
            if (pos>=0)
            {
                double diffx=cx-cartesian_coordinates[pos][0];
                double diffy=cy-cartesian_coordinates[pos][1];
                deviation+=diffx*diffx+diffy*diffy;
            }
        }
        if (count>0)
        {
            deviation/=count;
        }
        deviation = sqrt(deviation);
    }
private:
    void ai_params_layers(MarbleColor my_color,MarbleColor rival_color, std::map<std::string,double>& out)
    {
        int pivot = 0;
        int dir;
        int count[2][edge_length];
        for (int i = edge_length; i>0;--i)
        {
            count[0][i-1] = 0;
            count[1][i-1] = 0;
            dir = RIGHT;
            for (int j=0;j<6;++j)
            {
                for(int k=0;k<i-1;++k)
                {
                    if (cell_marble[pivot].first !=NO_MARBLE)
                    {
                        count[cell_marble[pivot].first][i-1]++;
                    }
                    pivot=cell_neighbors[pivot][dir];
                }
                dir=(dir+1)%6;
            }
            pivot = cell_neighbors[pivot][DOWN_RIGHT];
            out[std::string("my-ring")+std::to_string(i)+"-count"] = count[my_color][i-1];
            out[std::string("rival-ring")+std::to_string(i)+"-count"] = count[rival_color][i-1];
        }
    }
    void ai_params_thirds(MarbleColor my_color,MarbleColor rival_color, std::map<std::string,double>& out)
    {

        int dp=0;
        int pivot;
        int rot_pivot;
        int count[2][6]={{0},{0}};

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
            out[std::string("my-third")+std::to_string(i)+"-count"] = count[my_color][i];
            out[std::string("rival-third")+std::to_string(i)+"-count"] = count[rival_color][i];
        }
    }
    void ai_params_move_count(MarbleColor color, const std::string& id, std::map<std::string,double>& out)
    {
        std::vector<move_record_t> moves;
        possibleMoves(color,moves);
        //out["number-of-my-moves"]=my_moves.size();
        int move_types_count[MT_COUNT] = {0};
        for(int i=0; i< moves.size(); ++i)
        {
            move_types_count[moves[i].mt]++;
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
    }
    void ai_neigh_count(MarbleColor my_color,MarbleColor rival_color, std::map<std::string,double>& out)
    {
        std::vector<int>::const_iterator it;
        std::vector<int>::const_iterator it_end = marbles_by_player[my_color].end();
        int near_count = 0;
        int far_count = 0;
        int me_rival_near_count = 0;
        int me_rival_far_count = 0;
        int rival_near_count = 0;
        int rival_far_count = 0;
        for (it = marbles_by_player[my_color].begin(); it != it_end;++it)
        {
            int pos = *it;
            if (pos>=0)
            {
                for (int i = 0; i<6; ++i)
                {
                    int near_pos = cell_neighbors[pos][i];
                    int far_pos= cell_far_neighbors[pos][i];

                    if (near_pos>=0)
                    {
                        if (cell_marble[near_pos].first==my_color)
                        {
                            if (i<3)
                                near_count++;
                        }
                        else
                            if(cell_marble[near_pos].first!=NO_MARBLE)
                            {
                                me_rival_near_count++;
                            }
                    }
                    if (far_pos >= 0)
                    {
                        if (cell_marble[far_pos].first == my_color)
                        {
                            if (i<3)
                                far_count++;
                        }
                        else
                            if(cell_marble[far_pos].first!=NO_MARBLE)
                            {
                                me_rival_far_count++;
                            }
                    }
                }
            }
        }
        it_end = marbles_by_player[rival_color].end();
        for (it = marbles_by_player[rival_color].begin(); it != it_end;++it)
        {
            int pos = *it;
            if (pos>=0)
            {
                for (int i = 0; i<3; ++i)
                {
                    int near_pos = cell_neighbors[pos][i];
                    int far_pos= cell_far_neighbors[pos][i];

                    if (near_pos>=0)
                    {
                        if (cell_marble[near_pos].first==rival_color)
                        {
                            rival_near_count++;
                        }

                    }
                    if (far_pos >= 0)
                    {
                        if (cell_marble[far_pos].first == rival_color)
                        {
                            rival_far_count++;
                        }
                    }
                }
            }
        }
        out["my-total-neighbors"]= near_count;
        out["my-total-far-neighbors"]= far_count;
        out["cross-total-neighbors"]= me_rival_near_count;
        out["cross-total-far-neighbors"]= me_rival_far_count;
        out["rival-total-neighbors"]= rival_near_count;
        out["rival-total-far-neighbors"]= rival_far_count;
    }
public:
    void ai_params(MarbleColor my_color, std::map<std::string,double>& out)
    {
        assert(_NPlyr == 2);
        MarbleColor rival_color=(my_color==BLACK)?WHITE:BLACK;
        double my_cx,my_cy,my_R,my_deviation;
        center_of_mass(my_color, my_cx,my_cy,my_R,my_deviation);
        double rival_cx,rival_cy,rival_R,rival_deviation;
        center_of_mass(rival_color, rival_cx,rival_cy,rival_R,rival_deviation);
        double distance_me_rival = sqrt((my_cx - rival_cx)*(my_cx - rival_cx) + (my_cy-rival_cy)*(my_cy-rival_cy));
        out["my-distance-from-center"]=my_R;
        out["my-deviation"]=my_deviation;
        out["rival-distance-from-center"]=rival_R;
        out["rival-deviation"]=rival_deviation;
        out["distance-between-me-rival"]=distance_me_rival;
        out["my-lost-dears"]=marbles_dropped[my_color];
        out["rival-lost-dears"]=marbles_dropped[rival_color];
        ai_params_move_count(my_color, "my", out);
        ai_params_move_count(rival_color, "rival", out);
        ai_params_thirds(my_color,rival_color,out);
        ai_params_layers(my_color,rival_color,out);
        ai_neigh_count( my_color, rival_color,  out);

    }
private:

    std::array<std::pair<MarbleColor,int>, cells> cell_marble;
    std::vector<int> marbles_by_player[_NPlyr];


    int marbles_dropped[_NPlyr];
    MarbleColor current_turn;

};

#endif // ABALONEBOARD_H
