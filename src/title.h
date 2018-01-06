
#pragma once

#include "ofMain.h"
#include "basescene.h"
#include <array>
// BaseSceneを継承したSceneA
class title : public BaseScene {
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
};

class play : public BaseScene {
    static const int cell=30;
    int accel;
    int bx, by;
    int vx, vy;
    double tim, etim;
    int cnttim;
    int Nvirus ;
    int pflag, eflag, gflag, tflag;
    int fibl;
    int field[30][30], efield[30][30];//width=8, height=16
    int nowblock[2][2],nextblock[2][2],rotblock[2][2];
    int score,cnt, cl, sco;
    std::array<std::array<std::array<int,2>,2>,6> Blocks =
    {{
        {{
            { 1,1 },
            { 0,0 }
        }},
        {{
            { 1,2 },
            { 0,0 }
        }},
        {{
            { 1,3 },
            { 0,0 }
        }},
        {{
            { 2,2 },
            { 0,0 }
        }},
        {{
            { 2,3 },
            { 0,0 }
        }},
        {{
            { 3,3 },
            { 0,0 }
        }}
    }};
    bool f, f2, f3;
    int scur;//*/
    int virusnum[3];
    
public:
    void retC(int C);
    void DrawBlocks(int x, int y, int ablock[][2]);
    void rotB();
    bool isblock(int x, int y);
    void DrawField();
    void eraseblock();
    void iseraseblock();
    void createblock();
    void cntdispvirus();
    void init();
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
};

class clear : public BaseScene {
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
};
