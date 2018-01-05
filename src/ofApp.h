#pragma once

#include "ofMain.h"
#include "basescene.h"
#include "title.h"

extern const int cell;
extern int accel;
extern int bx;
extern int by;
extern int vx,vy;
extern double tim;
extern double etim;
extern int Nvirus ;
extern int pflag ;
extern int eflag;
extern int fibl;
extern int field[30][30];//width=8, height=16
extern int efield[30][30];
extern int nowblock[2][2];
extern int score;
//extern int Blocks[6][2][2];
extern int Blocks[6][2][2];
extern int gflag;
extern bool f;
extern bool f2;
extern bool f3;
extern int cnt;
extern int cl;
extern int nextblock[2][2];
extern int rotblock[2][2];
extern int scur;
extern ofImage viruses[5];
extern ofTrueTypeFont keyboard[5];
extern ofSoundPlayer sounds[10];
extern int currentScene;
extern int cnttim;

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
    
    //extern ofImage viruses[3];
		
};
