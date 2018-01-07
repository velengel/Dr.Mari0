
#pragma once

#include "ofMain.h"
#include "basescene.h"
#include <array>
// BaseSceneを継承したSceneA
class title : public BaseScene {
    
    
public:
    //ofApp &a;
    title(ofApp& a):BaseScene(a){};
    
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
};

class play : public BaseScene {
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
    
    int virusnum[3];
    int chain[2];
    int cntturn=0;
    int gauge=0;
public:
    //ofApp &b;
    BaseScene* p;
    play(ofApp& a, BaseScene* q):BaseScene(a), p(q){}
   // void ChangeScene(int a);
    void nextcap();
    void retC(int C);
    void DrawBlocks(int x, int y, int ablock[][2]);
    void rotB();
    bool isblock(int x, int y);
    void DrawField();
    void eraseblock();
    bool iseraseblock();
    void createblock();
    void cntdispvirus();
    void DrawShadow();
    void init();
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
};

class clear : public BaseScene {
    
public:
    //ofApp &b;
    BaseScene* p;
    //clear(ofApp& a):BaseScene(a){}
    clear(ofApp& a, BaseScene* q):BaseScene(a),p(q){}
    
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
};
