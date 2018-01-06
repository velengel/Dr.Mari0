#pragma once

#include "ofMain.h"
#include "basescene.h"
#include "title.h"

/*static int accel=1;
static int bx;
static int by=0;
static int vx,vy;
static double tim;
static double etim=0;
static int Nvirus ;
static int pflag=1 ;
static int eflag=0;
static int fibl;//
static int field[30][30];//width=8, height=16
static int efield[30][30];
static int nowblock[2][2];
static int score;//*/
//static int Blocks[6][2][2];
/*static int Blocks[6][2][2] = {
    {
        { 1,1 },
        { 0,0 }
    },
    {
        { 1,2 },
        { 0,0 }
    },
    {
        { 1,3 },
        { 0,0 }
    },
    {
        { 2,2 },
        { 0,0 }
    },
    {
        { 2,3 },
        { 0,0 }
    },
    {
        { 3,3 },
        { 0,0 }
    }
};*/

/*static int gflag=0;
static bool f;
static bool f2;
static bool f3;
static int cnt;
static int cl;
static int nextblock[2][2];
static int rotblock[2][2];
static int scur=0;*/
static ofImage viruses[5];
static ofTrueTypeFont keyboard[5];
static ofSoundPlayer sounds[10];
extern int currentScene;
//static int cnttim=0;//

//ウイルスを時間で増やす

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		/*void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);*/
    
        vector<BaseScene *> scenes;
    
    //static ofImage viruses[3];
		
};
