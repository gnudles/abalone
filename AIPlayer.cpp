#include "AIPlayer.h"
#include <map>
#include <string>
#include <AbaloneBoard.h>

#define NET_INPUT_LAYER 170
AIPlayer::AIPlayer()
{
    ann = fann_create_from_file(
        "/tmp/abalone.net"
    );
    if (ann == 0)
    {
        printf("creating empty net\n");
     ann = fann_create_standard(4, NET_INPUT_LAYER , 30, 8, 1);
    }


}
AIPlayer::~AIPlayer()
{

    fann_destroy(ann);
}
static double my_rand()
{
    static int32_t rand_num = 2344251;
    rand_num*=7;
    rand_num = rand_num ^ ((rand_num+5156)>> (rand_num&15));
    rand_num = rand_num ^ ((rand_num-36161)<< (rand_num&15));
    rand_num = rand_num ^ ((rand_num+0x10101010)>> (rand_num&15));
    rand_num = rand_num ^ ((rand_num-36161)<< (rand_num&15));
    return ((double)rand_num)/((unsigned int)0xffffffff);
}
IAbaloneBoard::move_record_t AIPlayer::getMove(const IAbaloneBoard &abalone_board)
{
    using standard_abalone  = AbaloneBoard<5,2>;
    IAbaloneBoard::MarbleColor my_color = abalone_board.currentTurn();
            //return {IAbaloneBoard::WHITE,0,0,IAbaloneBoard::NO_DIRECTION,IAbaloneBoard::NO_DIRECTION,IAbaloneBoard::NO_MOVE};
    std::map<std::string,double> ai_current_input;
    std::map<std::string,double> ai_next_input;
    std::vector<IAbaloneBoard::move_record_t> moves;
    std::vector<IAbaloneBoard::move_record_t> next_moves;
    abalone_board.ai_params(abalone_board.currentTurn(),ai_current_input, moves);
    fann_type net_input[NET_INPUT_LAYER];
    int net_inp_ind=0;
    std::map<std::string,double>::const_iterator it =ai_current_input.begin();
    for (;it !=ai_current_input.end();++it)
    {
        net_input[net_inp_ind] = it->second;
        net_inp_ind++;
    }
    for (int i = net_inp_ind; i< NET_INPUT_LAYER; ++i)
    {
        net_input[i] = 0;
    }
    int max_i = 0;
    fann_type max_res = -10000.0;
    for (int i = 0; i< moves.size(); ++i)
    {
        next_moves.clear();
        ai_next_input.clear();
        int net_next_inp_ind = net_inp_ind;
        standard_abalone new_board = (const standard_abalone&)abalone_board;
        new_board.makeMove(moves[i]);
        new_board.ai_params(my_color,ai_next_input,next_moves);
        it =ai_next_input.begin();
        for (;it !=ai_next_input.end();++it)
        {
            net_input[net_next_inp_ind] = it->second;
            net_next_inp_ind++;
        }
        const unsigned int kill_mask = (1<<IAbaloneBoard::TWO_KILL_ONE) |(1<<IAbaloneBoard::THREE_KILL_ONE)|(1<<IAbaloneBoard::THREE_PUSH_ONE_KILL_ONE);
        const unsigned int push_mask = (1<<IAbaloneBoard::TWO_PUSH_ONE) |(1<<IAbaloneBoard::THREE_PUSH_ONE)|(1<<IAbaloneBoard::THREE_PUSH_TWO);
        fann_type train_result = 0;
        if (((1<<moves[i].mt)&  kill_mask) !=0 )
        {
            train_result +=10;



        }
        if (((1<<moves[i].mt)&  push_mask) !=0 )
        {
            train_result +=2;
        }

            if ( ai_next_input["my-deviation"] < ai_current_input["my-deviation"])
            {
                 train_result += (4+my_rand())*(ai_current_input["my-deviation"]- ai_next_input["my-deviation"]);

            }

        if (ai_next_input["distance-between-me-rival"]<ai_current_input["distance-between-me-rival"])
                     train_result +=(5+my_rand()) * (ai_current_input["distance-between-me-rival"]-ai_next_input["distance-between-me-rival"]);

        //if (train_result>0)
        {
        fann_train(	ann,
                net_input,
                &train_result	);
        }
        fann_type result =  *fann_run(ann,
                net_input	);
        if (result>max_res)
        {
            max_i = i;
            max_res = result;
        }
    }
    printf("!!!!!!!!! %f\n",(float)max_res);

    return moves[max_i];

}

void AIPlayer::control(const std::string &cmd)
{
    if (cmd=="shuffle")
    {
    fann_randomize_weights(ann,
            -0.4,
            0.4);
    }
    if (cmd == "save")
    {
        fann_save(	ann,
            "/tmp/abalone.net"	);
    }
    if (cmd == "load")
    {
        fann_destroy(ann);
        ann = fann_create_from_file(
            "/tmp/abalone.net"
        );
    }
}
