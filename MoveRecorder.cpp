#include "MoveRecorder.h"
#include "IAbaloneBoard.h"
#include "BoardFactory.h"
#include "tinyxml2.h"
using namespace tinyxml2;
#define SVG_SCALE(X) ((X)*190)
#define SVG_SCALE_MOVE(X) (SVG_SCALE(X)+1000)
MoveRecorder::MoveRecorder()
{

}


void MoveRecorder::toSVG(const std::string & file)
{

    XMLDocument d;


    IAbaloneBoard * abalone_board = abaloneBoardFactory(board_size);
    //using standard_abalone  = AbaloneBoard<5>;

    XMLElement *svg_node = d.InsertChildElement("svg");
    svg_node->SetName("svg","http://www.w3.org/2000/svg");
    svg_node->SetAttribute("xmlns","http://www.w3.org/2000/svg");
    svg_node->SetAttribute("width","2020");
    svg_node->SetAttribute("height","2120");
    {
        XMLElement * svg_defs = svg_node->InsertChildElement("defs");
        {
            XMLElement * scg = svg_defs->InsertChildElement("linearGradient");
            scg->SetAttribute("id","scg");
            scg->SetAttribute("x1","1");
            scg->SetAttribute("x2","0");
            scg->SetAttribute("y1","1");
            scg->SetAttribute("y2","0");
            {
                XMLElement * stop = scg->InsertChildElement("stop");
                stop->SetAttribute("offset","0%");
                stop->SetAttribute("stop-color","rgba(30,30,30,0.4)");
            }
            {
                XMLElement * stop = scg->InsertChildElement("stop");
                stop->SetAttribute("offset","100%");
                stop->SetAttribute("stop-color","rgba(90,90,90,0.2)");
            }
            XMLElement * wg = svg_defs->InsertChildElement("radialGradient");
            wg->SetAttribute("id","white_grad");
            wg->SetAttribute("cx","0.35");
            wg->SetAttribute("cy","0.37");

            {
                XMLElement * stop = wg->InsertChildElement("stop");
                stop->SetAttribute("offset","0");
                stop->SetAttribute("stop-color","#ffffff");
            }
            {
                XMLElement * stop = wg->InsertChildElement("stop");
                stop->SetAttribute("offset","0.3");
                stop->SetAttribute("stop-color","#f2f2f2");
            }
            {
                XMLElement * stop = wg->InsertChildElement("stop");
                stop->SetAttribute("offset","0.366");
                stop->SetAttribute("stop-color","#e9e9e9");
            }
            {
                XMLElement * stop = wg->InsertChildElement("stop");
                stop->SetAttribute("offset","0.64");
                stop->SetAttribute("stop-color","#aaaaaa");
            }
            {
                XMLElement * stop = wg->InsertChildElement("stop");
                stop->SetAttribute("offset","1");
                stop->SetAttribute("stop-color","#4e4e4e");
            }
            XMLElement * bg = svg_defs->InsertChildElement("radialGradient");
            bg->SetAttribute("id","black_grad");
            bg->SetAttribute("cx","0.35");
            bg->SetAttribute("cy","0.37");

            {
                XMLElement * stop = bg->InsertChildElement("stop");
                stop->SetAttribute("offset","0");
                stop->SetAttribute("stop-color","#333333");
            }
            {
                XMLElement * stop = bg->InsertChildElement("stop");
                stop->SetAttribute("offset","0.3");
                stop->SetAttribute("stop-color","#222222");
            }
            {
                XMLElement * stop = bg->InsertChildElement("stop");
                stop->SetAttribute("offset","0.366");
                stop->SetAttribute("stop-color","#111111");
            }
            {
                XMLElement * stop = bg->InsertChildElement("stop");
                stop->SetAttribute("offset","0.64");
                stop->SetAttribute("stop-color","#080808");
            }
            {
                XMLElement * stop = bg->InsertChildElement("stop");
                stop->SetAttribute("offset","1");
                stop->SetAttribute("stop-color","#000000");
            }
            XMLElement * rg = svg_defs->InsertChildElement("radialGradient");
            rg->SetAttribute("id","red_grad");
            rg->SetAttribute("cx","0.35");
            rg->SetAttribute("cy","0.37");

            {
                XMLElement * stop = rg->InsertChildElement("stop");
                stop->SetAttribute("offset","0");
                stop->SetAttribute("stop-color","#ffaaaa");
            }
            {
                XMLElement * stop = rg->InsertChildElement("stop");
                stop->SetAttribute("offset","0.3");
                stop->SetAttribute("stop-color","#992222");
            }
            {
                XMLElement * stop = rg->InsertChildElement("stop");
                stop->SetAttribute("offset","0.366");
                stop->SetAttribute("stop-color","#881111");
            }
            {
                XMLElement * stop = rg->InsertChildElement("stop");
                stop->SetAttribute("offset","0.64");
                stop->SetAttribute("stop-color","#770808");
            }
            {
                XMLElement * stop = rg->InsertChildElement("stop");
                stop->SetAttribute("offset","1");
                stop->SetAttribute("stop-color","#220a0a");
            }
            {
                XMLElement * filter = svg_defs->InsertChildElement("filter");
                filter->SetAttribute("id","fe_bump");
                {
                    XMLElement * flood = filter->InsertChildElement("feFlood");
                    flood->SetAttribute("flood-color","#111");
                    flood->SetAttribute("result","COLOR-outline");

                }
                {
                    XMLElement * morph = filter->InsertChildElement("feMorphology");
                    morph->SetAttribute("operator","dilate");
                    morph->SetAttribute("radius","2");
                    morph->SetAttribute("in","SourceAlpha");
                    morph->SetAttribute("result","OUTLINE_10");

                }
                {
                    XMLElement * composite = filter->InsertChildElement("feComposite");
                    composite->SetAttribute("operator","in");
                    composite->SetAttribute("in","COLOR-outline");
                    composite->SetAttribute("in2","OUTLINE_10");
                    composite->SetAttribute("result","OUTLINE_20");

                }
                {
                    XMLElement * fe = filter->InsertChildElement("feGaussianBlur");
                    fe->SetAttribute("stdDeviation","9");
                    fe->SetAttribute("in","SourceAlpha");
                    fe->SetAttribute("result","LIGHTING-EFFECTS_10");
                }
                {
                    XMLElement * fe = filter->InsertChildElement("feSpecularLighting");
                    fe->SetAttribute("surfaceScale","6");
                    fe->SetAttribute("specularConstant","0.5");
                    fe->SetAttribute("specularExponent","1");
                    fe->SetAttribute("lighting-color","#777");
                    fe->SetAttribute("in","LIGHTING-EFFECTS_10");
                    fe->SetAttribute("result","LIGHTING-EFFECTS_20");
                    {
                        XMLElement * pl = fe->InsertChildElement("fePointLight");
                        pl->SetAttribute("x","200");
                        pl->SetAttribute("y","300");
                        pl->SetAttribute("z","700");

                    }
                }
                {
                    XMLElement * composite = filter->InsertChildElement("feComposite");
                    composite->SetAttribute("operator","in");
                    composite->SetAttribute("in","LIGHTING-EFFECTS_20");
                    composite->SetAttribute("in2","SourceAlpha");
                    composite->SetAttribute("result","LIGHTING-EFFECTS_30");

                }
                {
                    XMLElement * composite = filter->InsertChildElement("feComposite");
                    composite->SetAttribute("operator","arithmetic");
                    composite->SetAttribute("k1","0");
                    composite->SetAttribute("k2","1");
                    composite->SetAttribute("k3","1");
                    composite->SetAttribute("k4","0");
                    composite->SetAttribute("in","SourceGraphic");
                    composite->SetAttribute("in2","LIGHTING-EFFECTS_30");
                    composite->SetAttribute("result","LIGHTING-EFFECTS_40");

                }
                {
                    XMLElement * fe = filter->InsertChildElement("feComponentTransfer");

                    fe->SetAttribute("in","LIGHTING-EFFECTS_40");
                    fe->SetAttribute("result","COLOR-EFFECTS_10");
                    {
                        XMLElement * ff = fe->InsertChildElement("feFuncR");
                        ff->SetAttribute("type","gamma");
                        ff->SetAttribute("offset","0.07");
                        ff->SetAttribute("amplitude","12");
                        ff->SetAttribute("exponent","4.8");

                    }
                    {
                        XMLElement * ff = fe->InsertChildElement("feFuncG");
                        ff->SetAttribute("type","gamma");
                        ff->SetAttribute("offset","0.07");
                        ff->SetAttribute("amplitude","12");
                        ff->SetAttribute("exponent","4.8");

                    }
                    {
                        XMLElement * ff = fe->InsertChildElement("feFuncB");
                        ff->SetAttribute("type","gamma");
                        ff->SetAttribute("offset","0.07");
                        ff->SetAttribute("amplitude","12");
                        ff->SetAttribute("exponent","4.8");

                    }
                }
                {
                    XMLElement * fe = filter->InsertChildElement("feMerge");
                    {
                        XMLElement * mn = fe->InsertChildElement("feMergeNode");
                        mn->SetAttribute("in","OUTLINE_20");
                    }
                    {
                        XMLElement * mn = fe->InsertChildElement("feMergeNode");
                        mn->SetAttribute("in","COLOR-EFFECTS_10");
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
            /*XMLElement * rwg = svg_defs->InsertChildElement("radialGradient");
            rwg->SetAttribute("xlink:href","#white_grad");
            wg->SetAttribute("id","w_rad_grad");*/

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
        XMLElement * svg_style = svg_node->InsertChildElement("style");
        {
            svg_style->SetText(".letters { font: bold 40px sans-serif; }");
        }

        {
            XMLElement * background = svg_node->InsertChildElement("rect");
            background->SetAttribute("x","0");
            background->SetAttribute("y","0");
            background->SetAttribute("width","100%");
            background->SetAttribute("height","100%");
            background->SetAttribute("fill","rgba(100,110,160,0.7)");
        }

        XMLElement * board = svg_node->InsertChildElement("g");
        board->SetAttribute("transform","scale(1.0) translate(10,200) ");
        {
            XMLElement * masking = board->InsertChildElement("defs")->InsertChildElement("mask");
            masking->SetAttribute("id","brd_mask");
            {
                XMLElement * hexagon = masking->InsertChildElement("polygon");
                hexagon->SetAttribute("points","0,1000 500,134 1500,134 2000,1000 1500,1866 500,1866");
                hexagon->SetAttribute("fill","white");
                for (int i = 0; i< abalone_board->numberOfCells() ; ++i)
                {
                    float x=abalone_board->cellCoordX(i);
                    float y=abalone_board->cellCoordY(i);
                    x=SVG_SCALE_MOVE(x);
                    y=SVG_SCALE_MOVE(y);

                    XMLElement * board_circle = masking->InsertChildElement("circle");
                    board_circle->SetAttribute("cx",std::to_string((int)x).c_str());
                    board_circle->SetAttribute("cy",std::to_string((int)y).c_str());
                    board_circle->SetAttribute("r","65");
                    board_circle->SetAttribute("fill","black");

                }
            }

            {
                XMLElement * hexagon_g = board->InsertChildElement("g");
                XMLElement * hexagon = hexagon_g->InsertChildElement("polygon");
                hexagon->SetAttribute("points","0,1000 500,134 1500,134 2000,1000 1500,1866 500,1866");
                hexagon->SetAttribute("fill","rgb(80,80,80)");
                hexagon->SetAttribute("stroke","black");
                hexagon->SetAttribute("mask","url(#brd_mask)");
                hexagon_g->SetAttribute("filter","url(#fe_bump)");

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
                        XMLElement * circle = board->InsertChildElement("circle");
                        circle->SetAttribute("cx",std::to_string((int)x).c_str());
                        circle->SetAttribute("cy",std::to_string((int)y).c_str());
                        circle->SetAttribute("r","40");
                        circle->SetAttribute("fill","url(#scg)");
        //                circle->SetAttribute("fill","black");


                        XMLElement * triangle = board->InsertChildElement("polygon");
                        triangle->SetAttribute("points","-0.5,0.28867 0,-0.57735 0.5,0.28867");
                        triangle->SetAttribute("fill","rgb(90,90,90)");
                        std::string transformation = "translate("+std::to_string((int)x) +"," +std::to_string((int)y) +")  scale(66)";
                        triangle->SetAttribute("transform",transformation.c_str());
                        }
                        y=2000-y;
                        {
                            XMLElement * circle = board->InsertChildElement("circle");
                            circle->SetAttribute("cx",std::to_string((int)x).c_str());
                            circle->SetAttribute("cy",std::to_string((int)y).c_str());
                            circle->SetAttribute("r","40");
                            circle->SetAttribute("fill","url(#scg)");
            //                circle->SetAttribute("fill","black");


                            XMLElement * triangle = board->InsertChildElement("polygon");
                            triangle->SetAttribute("points","-0.5,-0.28867 0.5,-0.28867 0,0.57735");
                            triangle->SetAttribute("fill","rgb(90,90,90)");
                            std::string transformation = "translate("+std::to_string((int)x) +"," +std::to_string((int)y) +")  scale(66)";
                            triangle->SetAttribute("transform",transformation.c_str());
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
            float text_y_offset = 15;
            float text_x_offset = -8;
            {//alpha
                float start_y=(board_size-1)*TRIANGLE_HEIGHT;
                float start_x=-(board_size-1)*0.5 -1;
                for (int i =0; i< abalone_board->numberOfRows();++i)
                {
                    XMLElement * letter = board->InsertChildElement("text");
                    letter->SetAttribute("class","letters");
                    letter->SetAttribute("fill","#fac");
                    letter->SetAttribute("x",std::to_string(SVG_SCALE_MOVE(start_x)+text_x_offset).c_str());
                    letter->SetAttribute("y",std::to_string(SVG_SCALE_MOVE(start_y)+text_y_offset).c_str());
                    std::string l;
                    l+=(char)('A'+i);
                    letter->SetText(l.c_str());
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
                    XMLElement * letter = board->InsertChildElement("text");
                    letter->SetAttribute("class","letters");
                    letter->SetAttribute("fill","#afc");
                    letter->SetAttribute("x",std::to_string(SVG_SCALE_MOVE(start_x)+text_x_offset).c_str());
                    letter->SetAttribute("y",std::to_string(SVG_SCALE_MOVE(start_y)+text_y_offset).c_str());
                    letter->SetText(std::to_string(i+1).c_str());
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
            std::vector<XMLElement*> marble_xml[MAX_PLAYERS];
            for (int c=0; c<num_players; ++c)
            {
                for (int i =0 ; i<abalone_board->playerNumberOfMarbles(c);++i)
                {
                    int pos = abalone_board->playerMarblePosition(c,i);
                    if (pos>=0)
                    {
                        XMLElement * circle = board->InsertChildElement("circle");
                        float x,y;
                        x= SVG_SCALE_MOVE(abalone_board->cellCoordX(pos));
                        y= SVG_SCALE_MOVE(abalone_board->cellCoordY(pos));
                        circle->SetAttribute("cx",std::to_string((int)x).c_str());
                        circle->SetAttribute("cy",std::to_string((int)y).c_str());
                        circle->SetAttribute("r","76");
                        circle->SetAttribute("fill",svg_color[c]);
                        circle->SetAttribute("stroke","black");
                        circle->SetAttribute("stroke-width","7");
                        marble_xml[c].push_back(circle);
                    }
                }
            }
            std::vector<XMLElement*> killed_marble_xml[MAX_PLAYERS];
            {
                float x=80,y=-40;
            for (int c=0; c<num_players; ++c)
            {
                for (int i =0 ; i<6;++i)
                {
                        XMLElement * circle = board->InsertChildElement("circle");


                        circle->SetAttribute("cx",std::to_string((int)x).c_str());
                        circle->SetAttribute("cy",std::to_string((int)y).c_str());
                        circle->SetAttribute("r","20");
                        circle->SetAttribute("fill",svg_color[c]);
                        circle->SetAttribute("opacity","0.2");
                        circle->SetAttribute("stroke","black");
                        circle->SetAttribute("stroke-width","6");
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
                        XMLElement * circle = marble_xml[abalone_board->marbleAt(pivot)]
                                [abalone_board->marbleIdAt(pivot)];
                        float from_x, from_y,to_x,to_y;
                        from_x = SVG_SCALE_MOVE(abalone_board->cellCoordX(pivot));
                        from_y = SVG_SCALE_MOVE(abalone_board->cellCoordY(pivot));
                        to_x = from_x+SVG_SCALE(add_by_direction[move.direction][0]);
                        to_y = from_y+SVG_SCALE(add_by_direction[move.direction][1]);
                        { // x axis animation
                            XMLElement *anim=circle->InsertChildElement("animate");

                            anim->SetAttribute("fill","freeze");
                            anim->SetAttribute("attributeName","cx");
                            anim->SetAttribute("from",std::to_string((int)from_x).c_str());
                            anim->SetAttribute("to",std::to_string((int)to_x).c_str());
                            anim->SetAttribute("dur","0.8s");
                            if (anim_count>0)
                            {
                                std::string anim_node = "m"+std::to_string(anim_count - 1)+".end";
                                anim->SetAttribute("begin",anim_node.c_str());
                            }
                        }
                        { // y axis animation
                            XMLElement *anim=circle->InsertChildElement("animate");
                            anim->SetAttribute("fill","freeze");
                            anim->SetAttribute("attributeName","cy");
                            anim->SetAttribute("from",std::to_string((int)from_y).c_str());
                            anim->SetAttribute("to",std::to_string((int)to_y).c_str());
                            anim->SetAttribute("dur","0.8s");
                            if (anim_count>0)
                            {
                                std::string anim_node = "m"+std::to_string(anim_count - 1)+".end";
                                anim->SetAttribute("begin",anim_node.c_str());
                            }
                            if (i == 1)
                            {
                                std::string anim_node = "m"+std::to_string(anim_count);
                                anim->SetAttribute("id",anim_node.c_str());
                                anim_count++;
                            }
                        }
                        if (abalone_board->neighbor(pivot,move.direction)<0)//kill scene
                        {
                            {
                                XMLElement *anim=circle->InsertChildElement("animate");
                                anim->SetAttribute("fill","freeze");
                                anim->SetAttribute("attributeName","opacity");
                                anim->SetAttribute("from","1.0");
                                anim->SetAttribute("to","0");
                                anim->SetAttribute("dur","1s");
                                if (anim_count>0)
                                {
                                    std::string anim_node = "m"+std::to_string(anim_count - 1)+".end";
                                    anim->SetAttribute("begin",anim_node.c_str());
                                }
                            }
                            {
                                XMLElement *anim=circle->InsertChildElement("animate");
                                anim->SetAttribute("fill","freeze");
                                anim->SetAttribute("attributeName","r");
                                anim->SetAttribute("from","80");
                                anim->SetAttribute("to","30");
                                anim->SetAttribute("dur","1s");
                                if (anim_count>0)
                                {
                                    std::string anim_node = "m"+std::to_string(anim_count - 1)+".end";
                                    anim->SetAttribute("begin",anim_node.c_str());
                                }



                            }
                            {
                                IAbaloneBoard::MarbleColor foe =(IAbaloneBoard::MarbleColor)(abalone_board->currentTurn()^1);
                                XMLElement *circle=killed_marble_xml[foe][abalone_board->lostMarbles(foe)];
                                {
                                    XMLElement *anim=circle->InsertChildElement("animate");
                                    anim->SetAttribute("fill","freeze");
                                    anim->SetAttribute("attributeName","opacity");
                                    anim->SetAttribute("from","0.2");
                                    anim->SetAttribute("to","1.0");
                                    anim->SetAttribute("dur","1s");
                                    if (anim_count>0)
                                    {
                                        std::string anim_node = "m"+std::to_string(anim_count - 1)+".end";
                                        anim->SetAttribute("begin",anim_node.c_str());
                                    }
                                }
                                {
                                    XMLElement *anim=circle->InsertChildElement("animate");
                                    anim->SetAttribute("fill","freeze");
                                    anim->SetAttribute("attributeName","r");
                                    anim->SetAttribute("from","20");
                                    anim->SetAttribute("to","60");
                                    anim->SetAttribute("dur","1s");
                                    if (anim_count>0)
                                    {
                                        std::string anim_node = "m"+std::to_string(anim_count - 1)+".end";
                                        anim->SetAttribute("begin",anim_node.c_str());
                                    }
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



    d.SaveFile(file.c_str());
    delete abalone_board;

}
