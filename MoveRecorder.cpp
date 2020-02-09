#include "MoveRecorder.h"
#include "IAbaloneBoard.h"
#include "BoardFactory.h"
#include <libxml++/libxml++.h>
#define SVG_SCALE(X) ((X)*190)
#define SVG_SCALE_MOVE(X) (SVG_SCALE(X)+1000)
MoveRecorder::MoveRecorder()
{

}
void MoveRecorder::toSVG(const std::string & file)
{
    xmlpp::Document d;
    xmlpp::Element *svg_node;
    IAbaloneBoard * abalone_board = abaloneBoardFactory(board_size);
    //using standard_abalone  = AbaloneBoard<5>;


    svg_node = d.create_root_node("svg","http://www.w3.org/2000/svg");
    svg_node->set_attribute("width","2020");
    svg_node->set_attribute("height","2020");
    {
        xmlpp::Element * svg_defs = svg_node->add_child("defs");
        {
            xmlpp::Element * scg = svg_defs->add_child("linearGradient");
            scg->set_attribute("id","scg");
            scg->set_attribute("x1","1");
            scg->set_attribute("x2","0");
            scg->set_attribute("y1","1");
            scg->set_attribute("y2","0");
            {
                xmlpp::Element * stop = scg->add_child("stop");
                stop->set_attribute("offset","0%");
                stop->set_attribute("stop-color","rgba(30,30,30,0.4)");
            }
            {
                xmlpp::Element * stop = scg->add_child("stop");
                stop->set_attribute("offset","100%");
                stop->set_attribute("stop-color","rgba(90,90,90,0.2)");
            }
            xmlpp::Element * wg = svg_defs->add_child("radialGradient");
            wg->set_attribute("id","white_grad");
            wg->set_attribute("cx","0.35");
            wg->set_attribute("cy","0.37");

            {
                xmlpp::Element * stop = wg->add_child("stop");
                stop->set_attribute("offset","0");
                stop->set_attribute("stop-color","#ffffff");
            }
            {
                xmlpp::Element * stop = wg->add_child("stop");
                stop->set_attribute("offset","0.3");
                stop->set_attribute("stop-color","#f2f2f2");
            }
            {
                xmlpp::Element * stop = wg->add_child("stop");
                stop->set_attribute("offset","0.366");
                stop->set_attribute("stop-color","#e9e9e9");
            }
            {
                xmlpp::Element * stop = wg->add_child("stop");
                stop->set_attribute("offset","0.64");
                stop->set_attribute("stop-color","#aaaaaa");
            }
            {
                xmlpp::Element * stop = wg->add_child("stop");
                stop->set_attribute("offset","1");
                stop->set_attribute("stop-color","#4e4e4e");
            }
            xmlpp::Element * bg = svg_defs->add_child("radialGradient");
            bg->set_attribute("id","black_grad");
            bg->set_attribute("cx","0.35");
            bg->set_attribute("cy","0.37");

            {
                xmlpp::Element * stop = bg->add_child("stop");
                stop->set_attribute("offset","0");
                stop->set_attribute("stop-color","#333333");
            }
            {
                xmlpp::Element * stop = bg->add_child("stop");
                stop->set_attribute("offset","0.3");
                stop->set_attribute("stop-color","#222222");
            }
            {
                xmlpp::Element * stop = bg->add_child("stop");
                stop->set_attribute("offset","0.366");
                stop->set_attribute("stop-color","#111111");
            }
            {
                xmlpp::Element * stop = bg->add_child("stop");
                stop->set_attribute("offset","0.64");
                stop->set_attribute("stop-color","#080808");
            }
            {
                xmlpp::Element * stop = bg->add_child("stop");
                stop->set_attribute("offset","1");
                stop->set_attribute("stop-color","#000000");
            }
            xmlpp::Element * rg = svg_defs->add_child("radialGradient");
            rg->set_attribute("id","red_grad");
            rg->set_attribute("cx","0.35");
            rg->set_attribute("cy","0.37");

            {
                xmlpp::Element * stop = rg->add_child("stop");
                stop->set_attribute("offset","0");
                stop->set_attribute("stop-color","#ffaaaa");
            }
            {
                xmlpp::Element * stop = rg->add_child("stop");
                stop->set_attribute("offset","0.3");
                stop->set_attribute("stop-color","#992222");
            }
            {
                xmlpp::Element * stop = rg->add_child("stop");
                stop->set_attribute("offset","0.366");
                stop->set_attribute("stop-color","#881111");
            }
            {
                xmlpp::Element * stop = rg->add_child("stop");
                stop->set_attribute("offset","0.64");
                stop->set_attribute("stop-color","#770808");
            }
            {
                xmlpp::Element * stop = rg->add_child("stop");
                stop->set_attribute("offset","1");
                stop->set_attribute("stop-color","#220a0a");
            }
            {
                xmlpp::Element * filter = svg_defs->add_child("filter");
                filter->set_attribute("id","fe_bump");
                {
                    xmlpp::Element * flood = filter->add_child("feFlood");
                    flood->set_attribute("flood-color","#111");
                    flood->set_attribute("result","COLOR-outline");

                }
                {
                    xmlpp::Element * morph = filter->add_child("feMorphology");
                    morph->set_attribute("operator","dilate");
                    morph->set_attribute("radius","2");
                    morph->set_attribute("in","SourceAlpha");
                    morph->set_attribute("result","OUTLINE_10");

                }
                {
                    xmlpp::Element * composite = filter->add_child("feComposite");
                    composite->set_attribute("operator","in");
                    composite->set_attribute("in","COLOR-outline");
                    composite->set_attribute("in2","OUTLINE_10");
                    composite->set_attribute("result","OUTLINE_20");

                }
                {
                    xmlpp::Element * fe = filter->add_child("feGaussianBlur");
                    fe->set_attribute("stdDeviation","9");
                    fe->set_attribute("in","SourceAlpha");
                    fe->set_attribute("result","LIGHTING-EFFECTS_10");
                }
                {
                    xmlpp::Element * fe = filter->add_child("feSpecularLighting");
                    fe->set_attribute("surfaceScale","6");
                    fe->set_attribute("specularConstant","0.5");
                    fe->set_attribute("specularExponent","1");
                    fe->set_attribute("lighting-color","#777");
                    fe->set_attribute("in","LIGHTING-EFFECTS_10");
                    fe->set_attribute("result","LIGHTING-EFFECTS_20");
                    {
                        xmlpp::Element * pl = fe->add_child("fePointLight");
                        pl->set_attribute("x","200");
                        pl->set_attribute("y","300");
                        pl->set_attribute("z","700");

                    }
                }
                {
                    xmlpp::Element * composite = filter->add_child("feComposite");
                    composite->set_attribute("operator","in");
                    composite->set_attribute("in","LIGHTING-EFFECTS_20");
                    composite->set_attribute("in2","SourceAlpha");
                    composite->set_attribute("result","LIGHTING-EFFECTS_30");

                }
                {
                    xmlpp::Element * composite = filter->add_child("feComposite");
                    composite->set_attribute("operator","arithmetic");
                    composite->set_attribute("k1","0");
                    composite->set_attribute("k2","1");
                    composite->set_attribute("k3","1");
                    composite->set_attribute("k4","0");
                    composite->set_attribute("in","SourceGraphic");
                    composite->set_attribute("in2","LIGHTING-EFFECTS_30");
                    composite->set_attribute("result","LIGHTING-EFFECTS_40");

                }
                {
                    xmlpp::Element * fe = filter->add_child("feComponentTransfer");

                    fe->set_attribute("in","LIGHTING-EFFECTS_40");
                    fe->set_attribute("result","COLOR-EFFECTS_10");
                    {
                        xmlpp::Element * ff = fe->add_child("feFuncR");
                        ff->set_attribute("type","gamma");
                        ff->set_attribute("offset","0.07");
                        ff->set_attribute("amplitude","12");
                        ff->set_attribute("exponent","4.8");

                    }
                    {
                        xmlpp::Element * ff = fe->add_child("feFuncG");
                        ff->set_attribute("type","gamma");
                        ff->set_attribute("offset","0.07");
                        ff->set_attribute("amplitude","12");
                        ff->set_attribute("exponent","4.8");

                    }
                    {
                        xmlpp::Element * ff = fe->add_child("feFuncB");
                        ff->set_attribute("type","gamma");
                        ff->set_attribute("offset","0.07");
                        ff->set_attribute("amplitude","12");
                        ff->set_attribute("exponent","4.8");

                    }
                }
                {
                    xmlpp::Element * fe = filter->add_child("feMerge");
                    {
                        xmlpp::Element * mn = fe->add_child("feMergeNode");
                        mn->set_attribute("in","OUTLINE_20");
                    }
                    {
                        xmlpp::Element * mn = fe->add_child("feMergeNode");
                        mn->set_attribute("in","COLOR-EFFECTS_10");
                    }
                }

                /**
                 *
                 *


            <!-- OUTLINE END -->

            <!-- LIGHTING EFFECTS -->
            <feGaussianBlur stdDeviation="4" in="SourceAlpha" result="LIGHTING-EFFECTS_10"/>
            <feSpecularLighting surfaceScale="5" specularConstant="0.5" specularExponent="7.5" lighting-color="#white" in="LIGHTING-EFFECTS_10" result="LIGHTING-EFFECTS_20">
            <fePointLight x="-100" y="-300" z="300" />
            </feSpecularLighting>
            <feComposite in2="SourceAlpha" operator="in" in="LIGHTING-EFFECTS_20" result="LIGHTING-EFFECTS_30"/>
            <feComposite operator="arithmetic" k1="0" k2="1" k3="1" k4="0" in="SourceGraphic" in2="LIGHTING-EFFECTS_30" result="LIGHTING-EFFECTS_40"/>
            <!-- LIGHTING EFFECTS END -->

            <!-- COLOR EFFECTS -->
            <feComponentTransfer in="LIGHTING-EFFECTS_40" result="COLOR-EFFECTS_10">
              <feFuncR type="gamma" offset="0.0" amplitude="12" exponent="4.84"/>
              <feFuncB type="gamma" offset="0.0" amplitude="12.1" exponent="4.84"/>
              <feFuncG type="gamma" offset="0.0" amplitude="12.1" exponent="4.84"/>
            </feComponentTransfer>
            <!-- COLOR EFFECTS END -->

            <feMerge>
              <feMergeNode in="OUTLINE_20" />
              <feMergeNode in="COLOR-EFFECTS_10"/>
            </feMerge>
          </filter>
                 */
            }
            /*xmlpp::Element * rwg = svg_defs->add_child("radialGradient");
            rwg->set_attribute("xlink:href","#white_grad");
            wg->set_attribute("id","w_rad_grad");*/

            /*
<linearGradient
       id="white_grad"
       x1="0"
       x2="1"
       y1="1"
       y2="0">
      <stop
         offset="0%"
         stop-color="rgb(60,60,60)"
         id="stop2401"
         style="stop-color:#ffffff;stop-opacity:1" />
      <stop
         style="stop-color:#f2f2f2;stop-opacity:1"
         id="stop3936"
         stop-color="rgb(60,60,60)"
         offset="0.29902157" />
      <stop
         offset="0.36631212"
         stop-color="rgb(60,60,60)"
         id="stop3938"
         style="stop-color:#e9e9e9;stop-opacity:1" />
      <stop
         style="stop-color:#aaaaaa;stop-opacity:1"
         id="stop3940"
         stop-color="rgb(60,60,60)"
         offset="0.63958609" />
      <stop
         offset="100%"
         stop-color="rgb(90,90,90)"
         id="stop2403"
         style="stop-color:#4e4e4e;stop-opacity:1" />
    </linearGradient>

    <radialGradient
       inkscape:collect="always"
       xlink:href="#white_grad"
       id="radialGradient3910"
       cx="940.67944"
       cy="1002.4737"
       fx="940.67944"
       fy="1002.4737"
       r="33.400002"
       gradientUnits="userSpaceOnUse"
       gradientTransform="matrix(-0.79985919,-0.92349738,0.9304043,-0.80584137,737.6405,2664.0165)" />
             */


        }
        xmlpp::Element * svg_style = svg_node->add_child("style");
        {
            svg_style->set_child_text(".letters { font: bold 40px sans-serif; }");
        }

        {
            xmlpp::Element * background = svg_node->add_child("rect");
            background->set_attribute("x","0");
            background->set_attribute("y","0");
            background->set_attribute("width","100%");
            background->set_attribute("height","100%");
            background->set_attribute("fill","rgba(100,110,160,0.7)");
        }

        xmlpp::Element * board = svg_node->add_child("g");
        board->set_attribute("transform","scale(0.4) translate(10,200) ");
        {
            xmlpp::Element * masking = board->add_child("defs")->add_child("mask");
            masking->set_attribute("id","brd_mask");
            {
                xmlpp::Element * hexagon = masking->add_child("polygon");
                hexagon->set_attribute("points","0,1000 500,134 1500,134 2000,1000 1500,1866 500,1866");
                hexagon->set_attribute("fill","white");
                for (int i = 0; i< abalone_board->numberOfCells() ; ++i)
                {
                    float x=abalone_board->cellCoordX(i);
                    float y=abalone_board->cellCoordY(i);
                    x=SVG_SCALE_MOVE(x);
                    y=SVG_SCALE_MOVE(y);

                    xmlpp::Element * board_circle = masking->add_child("circle");
                    board_circle->set_attribute("cx",std::to_string((int)x));
                    board_circle->set_attribute("cy",std::to_string((int)y));
                    board_circle->set_attribute("r","65");
                    board_circle->set_attribute("fill","black");

                }
            }

            {
                xmlpp::Element * hexagon_g = board->add_child("g");
                xmlpp::Element * hexagon = hexagon_g->add_child("polygon");
                hexagon->set_attribute("points","0,1000 500,134 1500,134 2000,1000 1500,1866 500,1866");
                hexagon->set_attribute("fill","rgb(80,80,80)");
                hexagon->set_attribute("stroke","black");
                hexagon->set_attribute("mask","url(#brd_mask)");
                hexagon_g->set_attribute("filter","url(#fe_bump)");

            }
            {
                float start_x=-0.5* (board_size-1);
                float start_y=TRIANGLE_HEIGHT*2/3.0+(board_size-1)*TRIANGLE_HEIGHT;
                int items = board_size;
                for (int i =0; i<board_size*2;++i)
                {

                    float y=SVG_SCALE_MOVE(start_y);
                    for (int j =0;j<items;++j)
                    {
                        float x=SVG_SCALE_MOVE(start_x+j*1.0);
                        {
                        xmlpp::Element * circle = board->add_child("circle");
                        circle->set_attribute("cx",std::to_string((int)x));
                        circle->set_attribute("cy",std::to_string((int)y));
                        circle->set_attribute("r","40");
                        circle->set_attribute("fill","url(#scg)");
        //                circle->set_attribute("fill","black");


                        xmlpp::Element * triangle = board->add_child("polygon");
                        triangle->set_attribute("points","-0.5,0.28867 0,-0.57735 0.5,0.28867");
                        triangle->set_attribute("fill","rgb(90,90,90)");
                        triangle->set_attribute("transform"," translate("+std::to_string((int)x) +"," +std::to_string((int)y) +")  scale(66) ");
                        }
                        y=2000-y;
                        {
                            xmlpp::Element * circle = board->add_child("circle");
                            circle->set_attribute("cx",std::to_string((int)x));
                            circle->set_attribute("cy",std::to_string((int)y));
                            circle->set_attribute("r","40");
                            circle->set_attribute("fill","url(#scg)");
            //                circle->set_attribute("fill","black");


                            xmlpp::Element * triangle = board->add_child("polygon");
                            triangle->set_attribute("points","-0.5,-0.28867 0.5,-0.28867 0,0.57735");
                            triangle->set_attribute("fill","rgb(90,90,90)");
                            triangle->set_attribute("transform"," translate("+std::to_string((int)x) +"," +std::to_string((int)y) +")  scale(66) ");
                        }
                        y=2000-y;
                    }
                    start_y-=TRIANGLE_HEIGHT;
                    if (i<abalone_board->hexagonEdgeLength())
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
            {//alpha
                float start_y=(board_size-1)*TRIANGLE_HEIGHT;
                float start_x=-(board_size-1)*0.5 -1;
                for (int i =0; i< abalone_board->numberOfRows();++i)
                {
                    xmlpp::Element * letter = board->add_child("text");
                    letter->set_attribute("class","letters");
                    letter->set_attribute("fill","red");
                    letter->set_attribute("x",std::to_string(SVG_SCALE_MOVE(start_x)));
                    letter->set_attribute("y",std::to_string(SVG_SCALE_MOVE(start_y)));
                    Glib::ustring l;
                    l+=(char)('A'+i);
                    letter->set_child_text(l);
                    if (i>= abalone_board->hexagonEdgeLength()-1)
                    {
                        start_x+=0.5;
                    }
                    else
                    {
                        start_x-=0.5;
                    }
                    start_y-=TRIANGLE_HEIGHT;
                }

            }
            {//numeric
                float start_y=(board_size)*TRIANGLE_HEIGHT;
                float start_x=-(board_size-1)*0.5 +0.5;
                for (int i =0; i< abalone_board->numberOfRows();++i)
                {
                    xmlpp::Element * letter = board->add_child("text");
                    letter->set_attribute("class","letters");
                    letter->set_attribute("fill","blue");
                    letter->set_attribute("x",std::to_string(SVG_SCALE_MOVE(start_x)));
                    letter->set_attribute("y",std::to_string(SVG_SCALE_MOVE(start_y)));
                    letter->set_child_text(std::to_string(i+1));
                    if (i>= abalone_board->hexagonEdgeLength()-1)
                    {
                        start_x+=0.5;
                        start_y-=TRIANGLE_HEIGHT;
                    }
                    else
                    {
                        start_x+=1.0;
                    }

                }

            }




            if (move_recorder.size()>=0){
            const float add_by_direction[6][2]=
            {{-0.5,-TRIANGLE_HEIGHT},{0.5,-TRIANGLE_HEIGHT},{1.0,0.0},{0.5,TRIANGLE_HEIGHT},{-0.5,TRIANGLE_HEIGHT},{-1.0,0.0}};
            Place(abalone_board);
            const char* svg_color[MAX_PLAYERS] = {"url(#black_grad)","url(#white_grad)","url(#red_grad)"};
            std::vector<xmlpp::Element*> marble_xml[MAX_PLAYERS];
            for (int c=0; c<num_players; ++c)
            {
                for (int i =0 ; i<abalone_board->playerNumberOfMarbles(c);++i)
                {
                    int pos = abalone_board->playerMarblePosition(c,i);
                    if (pos>=0)
                    {
                        xmlpp::Element * circle = board->add_child("circle");
                        float x,y;
                        x= SVG_SCALE_MOVE(abalone_board->cellCoordX(pos));
                        y= SVG_SCALE_MOVE(abalone_board->cellCoordY(pos));
                        circle->set_attribute("cx",std::to_string((int)x));
                        circle->set_attribute("cy",std::to_string((int)y));
                        circle->set_attribute("r","76");
                        circle->set_attribute("fill",svg_color[c]);
                        circle->set_attribute("stroke","black");
                        circle->set_attribute("stroke-width","7");
                        marble_xml[c].push_back(circle);
                    }
                }
            }
            std::vector<xmlpp::Element*> killed_marble_xml[MAX_PLAYERS];
            {
                float x=80,y=-40;
            for (int c=0; c<num_players; ++c)
            {
                for (int i =0 ; i<6;++i)
                {
                        xmlpp::Element * circle = board->add_child("circle");


                        circle->set_attribute("cx",std::to_string((int)x));
                        circle->set_attribute("cy",std::to_string((int)y));
                        circle->set_attribute("r","20");
                        circle->set_attribute("fill",svg_color[c]);
                        circle->set_attribute("opacity","0.2");
                        circle->set_attribute("stroke","black");
                        circle->set_attribute("stroke-width","6");
                        killed_marble_xml[c].push_back(circle);
                        x+=140;

                }
                x+=200;
            }
            }
            int anim_count = 0;
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
                        pivot = abalone_board->neighbor(move.last_m,move.direction);
                    }
                    else if (push_count == 2)
                    {
                        pivot = abalone_board->farNeighbor(move.last_m,move.direction);
                    }
                    int adv_dir = abalone_board->oppositeDirection(move.ftol_dir);
                    for (int i=push_count+mine_count ;i > 0; --i)
                    {
                        //moveSingleMarble(pivot,neighbor(pivot,move.direction));
                        xmlpp::Element * circle = marble_xml[abalone_board->marbleAt(pivot)]
                                [abalone_board->marbleIdAt(pivot)];
                        float from_x, from_y,to_x,to_y;
                        from_x = SVG_SCALE_MOVE(abalone_board->cellCoordX(pivot));
                        from_y = SVG_SCALE_MOVE(abalone_board->cellCoordY(pivot));
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
                        if (abalone_board->neighbor(pivot,move.direction)<0)//kill scene
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
                            {
                                IAbaloneBoard::MarbleColor foe =(IAbaloneBoard::MarbleColor)(abalone_board->currentTurn()^1);
                                xmlpp::Element *circle=killed_marble_xml[foe][abalone_board->lostMarbles(foe)];
                                {
                                    xmlpp::Element *anim=circle->add_child("animate");
                                    anim->set_attribute("fill","freeze");
                                    anim->set_attribute("attributeName","opacity");
                                    anim->set_attribute("from","0.2");
                                    anim->set_attribute("to","1.0");
                                    anim->set_attribute("dur","1s");
                                    if (anim_count>0)
                                        anim->set_attribute("begin","m"+std::to_string(anim_count-1)+".end");
                                }
                                {
                                    xmlpp::Element *anim=circle->add_child("animate");
                                    anim->set_attribute("fill","freeze");
                                    anim->set_attribute("attributeName","r");
                                    anim->set_attribute("from","20");
                                    anim->set_attribute("to","60");
                                    anim->set_attribute("dur","1s");
                                    if (anim_count>0)
                                        anim->set_attribute("begin","m"+std::to_string(anim_count-1)+".end");
                                }

                            }
                        }
                        pivot = abalone_board->neighbor(pivot,(IAbaloneBoard::Direction)adv_dir);
                    }
                }
                abalone_board->_executeMove(move);
                abalone_board->nextTurn();
            }
            }

        }
    }



    d.write_to_file(file);
    delete abalone_board;
}
