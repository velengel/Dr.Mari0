
#pragma once
#include "ofMain.h"
//#include "ofApp.h"
// 全てのシーンの雛形
class ofApp;

class BaseScene {
    
    
public:
    
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
    int killednum=0;
    int scur;
    ofApp &b;
    //BaseScene& p;
    //仮想関数 (virtual) として定義する
    BaseScene(ofApp& a):b(a){};
    virtual void setup(){};
    virtual void update(){};
    virtual void draw(){};
    virtual void keyPressed (int key){};
    virtual void keyReleased(int key){};
    //static ofImage viruses[3];
    /*virtual void mouseMoved(int x, int y ){};
    virtual void mouseDragged(int x, int y, int button){};
    virtual void mousePressed(int x, int y, int button){};
    virtual void mouseReleased(int x, int y, int button){};
    virtual void mouseEntered(int x, int y){};
    virtual void mouseExited(int x, int y){};
    virtual void windowResized(int w, int h){};
    virtual void gotMessage(ofMessage msg){};
    virtual void dragEvent(ofDragInfo dragInfo){};*/
    int step = 30;
    float hue = fmodf(ofGetElapsedTimef()*200,255);
    //void ChangeScene(int a);
    virtual void init(){};
};

