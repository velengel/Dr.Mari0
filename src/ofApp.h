#pragma once

#include "ofMain.h"
//#include "basescene.h"
#include "title.h"

static ofImage viruses[5];
static ofTrueTypeFont keyboard[5];
static ofSoundPlayer sounds[10];
//extern int currentScene;

//ウイルスを時間で増やす
class BaseScene;

class ofApp : public ofBaseApp{
    //BaseScene a;
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
        int currentScene;
        vector<BaseScene *> scenes;
        void ChangeScene(int a);
    //static ofImage viruses[3];
		
};
