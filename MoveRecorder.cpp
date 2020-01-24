#include "MoveRecorder.h"
#include "AbaloneBoard.h"
#include <libxml++/libxml++.h>
#define SVG_SCALE(X) ((X)*200)
#define SVG_SCALE_MOVE(X) (SVG_SCALE(X)+1000)
MoveRecorder::MoveRecorder()
{

}
void MoveRecorder::toSVG(const std::string & file)
{
    xmlpp::Document d;
    xmlpp::Element *svg_node;
    using standard_abalone  = AbaloneBoard<5,2>;


    svg_node = d.create_root_node("svg","http://www.w3.org/2000/svg");
    svg_node->set_attribute("width","2020");
    svg_node->set_attribute("height","2020");
    {
        xmlpp::Element * svg_defs = svg_node->add_child("defs");
        {
            xmlpp::Element * scg = svg_defs->add_child("linearGradient");
            scg->set_attribute("id","scg");
            scg->set_attribute("x1","0");
            scg->set_attribute("x2","1");
            scg->set_attribute("y1","1");
            scg->set_attribute("y2","0");
            {
                xmlpp::Element * stop = scg->add_child("stop");
                stop->set_attribute("offset","0%");
                stop->set_attribute("stop-color","rgb(60,60,60)");
            }
            {
                xmlpp::Element * stop = scg->add_child("stop");
                stop->set_attribute("offset","100%");
                stop->set_attribute("stop-color","rgb(90,90,90)");
            }


        }

        {
            xmlpp::Element * background = svg_node->add_child("rect");
            background->set_attribute("x","0");
            background->set_attribute("y","0");
            background->set_attribute("width","100%");
            background->set_attribute("height","100%");
            background->set_attribute("fill","rgba(170,110,160,0.7)");
        }

        xmlpp::Element * board = svg_node->add_child("g");
        board->set_attribute("transform","translate(10,10) scale(0.4)");
        {
            xmlpp::Element * masking = board->add_child("defs")->add_child("mask");
            masking->set_attribute("id","brd_mask");
            {
                xmlpp::Element * hexagon = masking->add_child("polygon");
                hexagon->set_attribute("points","0,1000 500,134 1500,134 2000,1000 1500,1866 500,1866");
                hexagon->set_attribute("fill","white");
                for (int i = 0; i< standard_abalone::cells ; ++i)
                {
                    float x=standard_abalone::cartesian_coordinates[i][0];
                    float y=standard_abalone::cartesian_coordinates[i][1];
                    x=SVG_SCALE_MOVE(x);
                    y=SVG_SCALE_MOVE(y);

                    xmlpp::Element * board_circle = masking->add_child("circle");
                    board_circle->set_attribute("cx",std::to_string((int)x));
                    board_circle->set_attribute("cy",std::to_string((int)y));
                    board_circle->set_attribute("r","67");
                    board_circle->set_attribute("fill","black");

                }
            }

            {
                xmlpp::Element * hexagon = board->add_child("polygon");
                hexagon->set_attribute("points","0,1000 500,134 1500,134 2000,1000 1500,1866 500,1866");
                hexagon->set_attribute("fill","rgb(80,80,80)");
                hexagon->set_attribute("stroke","black");
                hexagon->set_attribute("mask","url(#brd_mask)");
            }
            {
                float start_x=-2;
                float start_y=TRIANGLE_HEIGHT*2/3.0+4*TRIANGLE_HEIGHT;
                int items = 5;
                for (int i =0; i<10;++i)
                {

                    float y=SVG_SCALE_MOVE(start_y);
                    for (int j =0;j<items;++j)
                    {
                        float x=SVG_SCALE_MOVE(start_x+j*1.0);
                        {
                        xmlpp::Element * circle = board->add_child("circle");
                        circle->set_attribute("cx",std::to_string((int)x));
                        circle->set_attribute("cy",std::to_string((int)y));
                        circle->set_attribute("r","45");
                        circle->set_attribute("fill","url(#scg)");
        //                circle->set_attribute("fill","black");


                        xmlpp::Element * triangle = board->add_child("polygon");
                        triangle->set_attribute("points","-0.5,0.28867 0,-0.57735 0.5,0.28867");
                        triangle->set_attribute("fill","rgb(90,90,90)");
                        triangle->set_attribute("transform"," translate("+std::to_string((int)x) +"," +std::to_string((int)y) +")  scale(72) ");
                        }
                        y=2000-y;
                        {
                            xmlpp::Element * circle = board->add_child("circle");
                            circle->set_attribute("cx",std::to_string((int)x));
                            circle->set_attribute("cy",std::to_string((int)y));
                            circle->set_attribute("r","45");
                            circle->set_attribute("fill","url(#scg)");
            //                circle->set_attribute("fill","black");


                            xmlpp::Element * triangle = board->add_child("polygon");
                            triangle->set_attribute("points","-0.5,-0.28867 0.5,-0.28867 0,0.57735");
                            triangle->set_attribute("fill","rgb(90,90,90)");
                            triangle->set_attribute("transform"," translate("+std::to_string((int)x) +"," +std::to_string((int)y) +")  scale(72) ");
                        }
                        y=2000-y;
                    }
                    start_y-=TRIANGLE_HEIGHT;
                    if (i<5)
                    {
                        start_x-=0.5;
                        items++;
                    }
                    else
                    {
                        start_x+=0.5;
                        items--;
                    }
                }


            }


/*
            xmlpp::Element * circle = board->add_child("circle");
            circle->set_attribute("cx","1800");
            circle->set_attribute("cy","1000");
            circle->set_attribute("r","80");
            circle->set_attribute("fill","white");
            circle->set_attribute("stroke","black");
            circle->set_attribute("stroke-width","2.5");
            {
                xmlpp::Element *anim=circle->add_child("animate");
                anim->set_attribute("id","m1");
                anim->set_attribute("fill","freeze");
                anim->set_attribute("attributeName","cx");
                anim->set_attribute("from","1800");
                anim->set_attribute("to","1900");
                anim->set_attribute("dur","0.8s");
            }
            {
                xmlpp::Element *anim=circle->add_child("animate");
                anim->set_attribute("fill","freeze");
                anim->set_attribute("attributeName","cy");
                anim->set_attribute("from","1000");
                anim->set_attribute("to","1173");
                anim->set_attribute("dur","0.8s");
            }
            {
                xmlpp::Element *anim=circle->add_child("animate");
                anim->set_attribute("fill","freeze");
                anim->set_attribute("attributeName","opacity");
                anim->set_attribute("from","1.0");
                anim->set_attribute("to","0");
                anim->set_attribute("dur","0.8s");
                anim->set_attribute("begin","m1.end");
            }
            {
                xmlpp::Element *anim=circle->add_child("animate");
                anim->set_attribute("fill","freeze");
                anim->set_attribute("attributeName","r");
                anim->set_attribute("from","80");
                anim->set_attribute("to","10");
                anim->set_attribute("dur","0.8s");
                anim->set_attribute("begin","m1.end");
            }
            */
            /*
             * <animate id="a1" attributeName="fill" from="red" to="blue" dur="3s" fill="freeze" />
    <animate id="a4" attributeName="x" from="10" to="50" dur="3s" fill="freeze" />
    <animate id="a5" attributeName="x" from="50" to="10" dur="3s" begin="a3.end" fill="freeze" />
    <animate id="a6" attributeName="fill" from="blue" to="red" dur="3s" begin="a3.end" fill="freeze" />
             */

            if (move_recorder.size()>0){
            standard_abalone abalone_board;
            const float add_by_direction[6][2]=
            {{-0.5,-TRIANGLE_HEIGHT},{0.5,-TRIANGLE_HEIGHT},{1.0,0.0},{0.5,TRIANGLE_HEIGHT},{-0.5,TRIANGLE_HEIGHT},{-1.0,0.0}};
            Place(&abalone_board);
            const char* svg_color[2] = {"black","white"};
            std::vector<xmlpp::Element*> marble_xml[2];
            for (int c=0; c<2; ++c)
            {
                for (int i =0 ; i<abalone_board.marbles_by_player[c].size();++i)
                {
                    int pos = abalone_board.marbles_by_player[c][i];
                    if (pos>=0)
                    {
                        xmlpp::Element * circle = board->add_child("circle");
                        float x,y;
                        x= SVG_SCALE_MOVE(abalone_board.cartesian_coordinates[pos][0]);
                        y= SVG_SCALE_MOVE(abalone_board.cartesian_coordinates[pos][1]);
                        circle->set_attribute("cx",std::to_string((int)x));
                        circle->set_attribute("cy",std::to_string((int)y));
                        circle->set_attribute("r","80");
                        circle->set_attribute("fill",svg_color[c]);
                        circle->set_attribute("stroke","black");
                        circle->set_attribute("stroke-width","5");
                        marble_xml[c].push_back(circle);
                    }
                }
            }
            int anim_count = 0;
            bool kill_anim=false;
            for (int i=0;i<move_recorder.size();++i)
            {
                const IAbaloneBoard::move_record_t &move = move_recorder[i];

                {
                    const struct {int mine_count; int push_count;} helper_array[IAbaloneBoard::NO_MOVE]=
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
                        pivot = abalone_board.cell_neighbors[move.last_m][move.direction];
                    }
                    else if (push_count == 2)
                    {
                        pivot = abalone_board.cell_far_neighbors[move.last_m][move.direction];
                    }
                    int adv_dir = abalone_board.oppositeDirection(move.ftol_dir);
                    for (int i=push_count+mine_count ;i > 0; --i)
                    {
                        //moveSingleMarble(pivot,cell_neighbors[pivot][move.direction]);
                        xmlpp::Element * circle = marble_xml[abalone_board.cell_marble[pivot].first]
                                [abalone_board.cell_marble[pivot].second];
                        float from_x, from_y,to_x,to_y;
                        from_x = SVG_SCALE_MOVE(abalone_board.cartesian_coordinates[pivot][0]);
                        from_y = SVG_SCALE_MOVE(abalone_board.cartesian_coordinates[pivot][1]);
                        to_x = from_x+SVG_SCALE(add_by_direction[move.direction][0]);
                        to_y = from_y+SVG_SCALE(add_by_direction[move.direction][1]);
                        {
                            xmlpp::Element *anim=circle->add_child("animate");

                            anim->set_attribute("fill","freeze");
                            anim->set_attribute("attributeName","cx");
                            anim->set_attribute("from",std::to_string((int)from_x));
                            anim->set_attribute("to",std::to_string((int)to_x));
                            anim->set_attribute("dur","0.8s");
                            if (anim_count>0)
                                anim->set_attribute("begin","m"+std::to_string(anim_count-1)+".end");
                        }
                        {
                            xmlpp::Element *anim=circle->add_child("animate");
                            anim->set_attribute("fill","freeze");
                            anim->set_attribute("attributeName","cy");
                            anim->set_attribute("from",std::to_string((int)from_y));
                            anim->set_attribute("to",std::to_string((int)to_y));
                            anim->set_attribute("dur","0.8s");
                            if (anim_count>0)
                                anim->set_attribute("begin","m"+std::to_string(anim_count-1)+".end");
                            if (i == 1)
                            {

                                anim->set_attribute("id","m"+std::to_string(anim_count));
                                anim_count++;
                            }
                        }
                        if (abalone_board.cell_neighbors[pivot][move.direction]<0)//kill scene
                        {
                            {
                                xmlpp::Element *anim=circle->add_child("animate");
                                anim->set_attribute("fill","freeze");
                                anim->set_attribute("attributeName","opacity");
                                anim->set_attribute("from","1.0");
                                anim->set_attribute("to","0");
                                anim->set_attribute("dur","1s");
                                if (anim_count>0)
                                    anim->set_attribute("begin","m"+std::to_string(anim_count-1)+".end");
                            }
                            {
                                xmlpp::Element *anim=circle->add_child("animate");
                                anim->set_attribute("fill","freeze");
                                anim->set_attribute("attributeName","r");
                                anim->set_attribute("from","80");
                                anim->set_attribute("to","30");
                                anim->set_attribute("dur","1s");
                                if (anim_count>0)
                                    anim->set_attribute("begin","m"+std::to_string(anim_count-1)+".end");



                            }
                        }
                        pivot = abalone_board.cell_neighbors [pivot][adv_dir];
                    }
                }
                abalone_board.executeMove(move);
            }
            }

        }
    }



    d.write_to_file(file);
}
