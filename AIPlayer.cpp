
#ifdef ENABLE_AI

#include "AIPlayer.h"
#include <map>
#include <string>
#include <AbaloneBoard.h>
#include <MoveRecorder.h>
#include <unistd.h>
const char* NET_PATH[2]={ "/home/orr/.abalone/abalone1.net","/home/orr/.abalone/abalone2.net"};
const char* TRAIN_PATH[2]={ "/home/orr/.abalone/abalone1.data","/home/orr/.abalone/abalone2.data"};

AIPlayer::AIPlayer(bool rank_player,int revision):_revision(revision)
{
    ann = 0;
    if (!rank_player)
    {
        ann = fann_create_from_file(
            NET_PATH[_revision]
        );
        if (ann == 0)
        {
            printf("creating empty net\n");
         ann = fann_create_standard(5, NET_INPUT_LAYER[revision] , 60, 50,7, 1);
         fann_randomize_weights(ann,
                 -2.0,
                 2.0);
        }
    }


}
AIPlayer::~AIPlayer()
{
    if (ann)
        fann_destroy(ann);
}
static float my_rand()
{
    static uint32_t rand_num = 2344251;
    rand_num*=7;
    rand_num = rand_num ^ ((rand_num+5156)>> (rand_num&15));
    rand_num = rand_num ^ ((rand_num-36161)<< (rand_num&15));
    rand_num = rand_num ^ ((rand_num+0x10101010)>> (rand_num&15));
    rand_num = rand_num ^ ((rand_num-36161)<< (rand_num&15));
    return ((float)rand_num)/((uint32_t)0xffffffff);
}
fann_type getRank(std::map<std::string,float>& ai_current_input, std::map<std::string,float>& ai_next_input, const IAbaloneBoard::move_record_t& move, float error_rate)
{

    const unsigned int kill_mask = (1<<IAbaloneBoard::TWO_KILL_ONE) |(1<<IAbaloneBoard::THREE_KILL_ONE)|(1<<IAbaloneBoard::THREE_PUSH_ONE_KILL_ONE);
    const unsigned int push_mask = (1<<IAbaloneBoard::TWO_PUSH_ONE) |(1<<IAbaloneBoard::THREE_PUSH_ONE)|(1<<IAbaloneBoard::THREE_PUSH_TWO);
    fann_type train_result = 0;
    if (ai_next_input["rival-lost-dears"] > ai_current_input["rival-lost-dears"])
    {

        train_result +=12.5;



    }
    if (ai_next_input["rival-lost-dears"] ==6)
        train_result +=90;
    if (((1<<move.mt)&  push_mask) !=0 )
    {
        train_result +=4;
    }


    train_result += 2.7*( 6/ai_next_input["my-deviation"] - 6/ai_current_input["my-deviation"]);
    //train_result += ( 2/ai_current_input["rival-deviation"] - 2/ai_next_input["rival-deviation"]);
    train_result += -18*( ai_next_input["distance-between-me-rival"] - ai_current_input["distance-between-me-rival"]);
    /*float factor = 6;
    if (ai_current_input["rival-distance-from-center"] < 0.9 && ai_current_input["rival-deviation"]>2.9 && (ai_current_input["rival-ring3-count"])<=1 && (ai_current_input["rival-ring2-count"])==0 &&(ai_current_input["rival-ring1-count"]) == 0 && ai_current_input["my-deviation"] <4.5)
    {
        float r03=ai_current_input["rival-third0-count"] - ai_current_input["rival-third3-count"];
    if (r03<0)
    {
        train_result +=factor*ai_next_input["my-third3-count"]*(-r03);
        float r14=ai_current_input["rival-third1-count"] - ai_current_input["rival-third4-count"];
        if (r14<0)
        {
            train_result +=factor*ai_next_input["my-third4-count"]*(-r14);
        }
    }
    else
    {
        train_result +=factor*ai_next_input["my-third3-count"]*(r03);
        float r14=ai_current_input["rival-third1-count"] - ai_current_input["rival-third4-count"];
        if (r14>0)
        {
            train_result +=factor*ai_next_input["my-third4-count"]*(r14);
        }
    }
    if (ai_current_input["rival-third2-count"]>=4)
    {
        train_result +=factor*ai_next_input["my-third2-count"];
    }



    }*/
    train_result +=0.6*ai_next_input["my-total-neighbors"];
    train_result +=0.5*ai_next_input["my-total-far-neighbors"];
    train_result +=0.3*ai_next_input["my-total-rhomb-neighbors"];
    train_result -=0.24*ai_next_input["rival-total-neighbors"];
    train_result -=0.1*ai_next_input["rival-total-far-neighbors"];
    train_result -=0.1*ai_next_input["rival-total-rhomb-neighbors"];
    train_result += 1.2/(ai_next_input["my-distance-from-center"]+1);
    train_result += 2*(ai_next_input["my-ring1-count"]);
    train_result += 0.5*(ai_next_input["my-ring2-count"]);
    train_result += 0.3*(ai_next_input["my-ring3-count"]);
    train_result -= 0.08*(ai_next_input["my-ring4-count"]);
    train_result -= 0.12*(ai_next_input["my-ring5-count"]);
    train_result += 3*(ai_next_input["rival-deviation"]);
    train_result -= 2*(ai_next_input["rival-next-max-kill"]);
    train_result -= 2*(ai_next_input["rival-three-kill1-count"]);
    train_result -= 2.3*(ai_next_input["rival-three-kill2-count"]);
    train_result -= 0.5*(ai_next_input["rival-three-push1-count"]);
    train_result -= 0.7*(ai_next_input["rival-three-push2-count"]);
    train_result -= 1.8*(ai_next_input["rival-two-kill-count"]);
    train_result -= 0.5*(ai_next_input["rival-two-push-count"]);
    train_result -= 0.2*(ai_next_input["rival-ring3-count"]);
    train_result += 0.4*(ai_next_input["rival-ring4-count"]);
    train_result += 0.41*(ai_next_input["rival-ring5-count"]);


    train_result +=error_rate*my_rand();

    /*
    cross-total-far-neighbors:4
    cross-total-neighbors:1
    cross-total-rhomb-neighbors:2
    distance-between-me-rival:4.51584
    my-axis-1-max:1
    my-axis-1-min:3
    my-axis-2-max:0
    my-axis-2-min:4
    my-axis-3-max:1
    my-axis-3-min:3
    my-deviation:1.4357
    my-distance-from-center:1.98463
    my-lost-dears:0
    my-one-move-count:23
    my-ring1-count:0
    my-ring2-count:2
    my-ring3-count:4
    my-ring4-count:4
    my-ring5-count:3
    my-third0-count:5
    my-third1-count:13
    my-third2-count:13
    my-third3-count:4
    my-third4-count:1
    my-third5-count:1
    my-three-kill1-count:0
    my-three-kill2-count:0
    my-three-move-count:16
    my-three-push1-count:0
    my-three-push2-count:0
    my-three-slide-count:3
    my-total-far-neighbors:16
    my-total-neighbors:27
    my-total-rhomb-neighbors:18
    my-triples-count:16
    my-two-kill-count:0
    my-two-move-count:16
    my-two-push-count:0
    my-two-slide-count:12
    rival-axis-1-max:3
    rival-axis-1-min:0
    rival-axis-2-max:5
    rival-axis-2-min:0
    rival-axis-3-max:3
    rival-axis-3-min:0
    rival-deviation:1.55347
    rival-distance-from-center:2.53848
    rival-lost-dears:0
    rival-next-max-kill:0
    rival-one-move-count:18
    rival-ring1-count:0
    rival-ring2-count:1
    rival-ring3-count:3
    rival-ring4-count:4
    rival-ring5-count:6
    rival-third0-count:4
    rival-third1-count:0
    rival-third2-count:0
    rival-third3-count:4
    rival-third4-count:13
    rival-third5-count:13
    rival-three-kill1-count:0
    rival-three-kill2-count:0
    rival-three-move-count:9
    rival-three-push1-count:0
    rival-three-push2-count:0
    rival-three-slide-count:1
    rival-total-far-neighbors:14
    rival-total-neighbors:26
    rival-total-rhomb-neighbors:18
    rival-triples-count:14
    rival-two-kill-count:0
    rival-two-move-count:12
    rival-two-push-count:0
    rival-two-slide-count:7
*/


    return train_result/32.0;

}
IAbaloneBoard::move_record_t AIPlayer::getMove(const IAbaloneBoard &abalone_board)
{
    using standard_abalone  = AbaloneBoard<5>;
    IAbaloneBoard::MarbleColor my_color = abalone_board.currentTurn();
            //return {IAbaloneBoard::WHITE,0,0,IAbaloneBoard::NO_DIRECTION,IAbaloneBoard::NO_DIRECTION,IAbaloneBoard::NO_MOVE};
    std::vector<IAbaloneBoard::move_record_t> moves;
    std::vector<IAbaloneBoard::move_record_t> next_moves;
    std::map<std::string,float> ai_current_input;
    std::map<std::string,float> ai_next_input;

    abalone_board.ai_params(abalone_board.currentTurn(),ai_current_input, moves,true, _revision);
    std::array<fann_type,MAX_NET_INPUT> net_input;
    int net_inp_ind=0;
    std::map<std::string,float>::const_iterator it =ai_current_input.begin();
    for (;it !=ai_current_input.end();++it)
    {
        net_input[net_inp_ind] = it->second;
        net_inp_ind++;
    }
    for (int i = net_inp_ind; i< MAX_NET_INPUT; ++i)
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
        new_board.ai_params(my_color,ai_next_input,next_moves,false,_revision);
        it =ai_next_input.begin();
        for (;it !=ai_next_input.end();++it)
        {
            net_input[net_next_inp_ind] = it->second;
            net_next_inp_ind++;
        }
        //net_input[net_next_inp_ind] = getRank( ai_current_input, ai_next_input,moves[i],0);
        //net_next_inp_ind++;

        fann_type train_result = getRank( ai_current_input, ai_next_input,moves[i],2);
        if( !ann && _revision==0)
            train_result+=my_rand()*0.9;
        //fann_type result = train_result;
/*        fann_train(	ann,
                net_input,
                &train_result	);*/
        fann_type result;
        if (ann) result = *fann_run(ann,
                net_input.data()	)+my_rand()*3;
        else result = train_result;

        if (result>max_res)
        {
            max_i = i;
            max_res = result;
        }
    }
    printf("!!!!!!!!! %f\n",(float)max_res);

    return moves[max_i];

}

void AIPlayer::control(const std::string &cmd, void* data)
{
    if (!ann)
        return;
    if (cmd=="shuffle")
    {
    fann_randomize_weights(ann,
            -0.4,
            0.4);
    }
    else if (cmd == "save")
    {
        fann_save(	ann,
            NET_PATH[_revision]	);
    }
    else if (cmd == "load")
    {
        fann_destroy(ann);
        ann = fann_create_from_file(
            NET_PATH[_revision]
        );
    }
    else if (cmd == "collect")
    {
        using standard_abalone  = AbaloneBoard<5>;
        standard_abalone abalone_board;
        ((const MoveRecorder*)data)->Place(&abalone_board);
        std::vector<IAbaloneBoard::move_record_t>& rec_vec= ((MoveRecorder*)data)->records();
        IAbaloneBoard::MarbleColor winner_color = ((const MoveRecorder*)data)->winner();
        for (int m = 0; m < rec_vec.size(); ++m)
        {
            if (rec_vec[m].color == winner_color)
            {
                std::vector<IAbaloneBoard::move_record_t> moves;
                std::vector<IAbaloneBoard::move_record_t> next_moves;
                std::map<std::string,float> ai_current_input;
                std::map<std::string,float> ai_next_input;

                abalone_board.ai_params(abalone_board.currentTurn(),ai_current_input, moves,true, _revision);
                std::array<fann_type,MAX_NET_INPUT> net_input;
                int net_inp_ind=0;
                std::map<std::string,float>::const_iterator it =ai_current_input.begin();
                for (;it !=ai_current_input.end();++it)
                {
                    net_input[net_inp_ind] = it->second;
                    net_inp_ind++;
                }
                for (int i = net_inp_ind; i< MAX_NET_INPUT; ++i)
                {
                    net_input[i] = 0;
                }
                for (int i = 0; i< moves.size(); ++i)
                {

                    int net_next_inp_ind = net_inp_ind;
                    standard_abalone new_board = (const standard_abalone&)abalone_board;
                    new_board.makeMove(moves[i]);
                    new_board.ai_params(winner_color,ai_next_input,next_moves,true, _revision);
                    it =ai_next_input.begin();
                    for (;it !=ai_next_input.end();++it)
                    {
                        net_input[net_next_inp_ind] = it->second;
                        net_next_inp_ind++;
                    }
                    //net_input[net_next_inp_ind] = getRank( ai_current_input, ai_next_input,moves[i],0);
                    //net_next_inp_ind++;


                    fann_type train_result = -0.6;//
                    if (moves[i].frst_m == rec_vec[m].frst_m && moves[i].last_m == rec_vec[m].last_m
                            && moves[i].direction == rec_vec[m].direction)
                    {
                        train_result= 0.6;

                    }
                    else
                    {
                        //train_result= getRank( ai_current_input, ai_next_input,moves[i],0.3)*0.3;
                    }
                    training_data.push_back(std::pair<std::array<float,MAX_NET_INPUT>,float>((const std::array<float,MAX_NET_INPUT> &)net_input,train_result));
                    next_moves.clear();
                    ai_next_input.clear();

                }
            }
            abalone_board.makeMove(rec_vec[m]);


        }
        if( training_data.size()>2000)
        {
            unlink(TRAIN_PATH[_revision]);

            FILE * f =fopen(TRAIN_PATH[_revision],"w");
            if (f)
            {
                fprintf(f,"%d %d %d\n",(int)training_data.size(),NET_INPUT_LAYER[_revision],1);
                for (int s =0;s< training_data.size(); ++s)
                {
                    for (int ii = 0 ; ii < NET_INPUT_LAYER[_revision] ; ++ii)
                    {
                        fprintf(f,"%f ",training_data[s].first[ii]);
                    }
                    fprintf(f,"\n%f\n",training_data[s].second);
                }
                fclose(f);
                training_data.clear();
                printf ("++++++++++++++++++++++++++++++++++++++++\n");
                printf ("++++++++++++++++++++++++++++++++++++++++\n");
                printf ("++++++++++++++++++++++++++++++++++++++++\n");
                printf ("++++++++++++++++++++++++++++++++++++++++\n");
                printf ("++++++++++++++++++++++++++++++++++++++++\n");
                printf ("++++++++++++++++++++++++++++++++++++++++\n");
                printf ("++++++++++++++++++++++++++++++++++++++++\n");
                printf ("++++++++++++++++++++++++++++++++++++++++\n");
                fann_set_training_algorithm(
                    ann,
                    FANN_TRAIN_RPROP
                );
                fann_set_learning_rate(ann,
                        0.3	);
                fann_train_on_file(ann, TRAIN_PATH[_revision], 200,
                        5, 0.3);
                fann_save(	ann,
                    NET_PATH[_revision]	);
                /*fann_randomize_weights(ann,
                        -5.8,
                        5.8);*/
            }
        }
    }
}
#endif
