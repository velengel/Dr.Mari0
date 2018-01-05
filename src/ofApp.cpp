#include "ofApp.h"
int currentScene;
const int cell=30;
int accel=1;
int bx;
int by=0;
int vx,vy;
double tim;
double etim=0;
int Nvirus ;
int pflag=1 ;
int eflag=0;
int fibl;
int field[30][30];//width=8, height=16
int efield[30][30];
int nowblock[2][2];
int score;
//int Blocks[6][2][2];
int Blocks[6][2][2] = {
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
};

int gflag=0;
bool f;
bool f2;
bool f3;
int cnt;
int cl;
int nextblock[2][2];
int rotblock[2][2];
int scur=0;
ofImage viruses[5];
ofTrueTypeFont keyboard[5];
ofSoundPlayer sounds[10];
extern int currentScene;
int cnttim=0;

void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofBackground(0);
    BaseScene *stitle = new title();
    stitle->setup();
    scenes.push_back(stitle);
    
    BaseScene *splay = new play();
    splay->setup();
    scenes.push_back(splay);
    
    BaseScene *sclear = new clear();
    sclear->setup();
    scenes.push_back(sclear);
    
    currentScene = 0;
    
   
}
void ofApp::update(){
    scenes[currentScene]->update();
}
void ofApp::draw(){
    // 現在表示しているシーンを描画
    scenes[currentScene]->draw();
}
void ofApp::keyPressed(int key){
    // キー入力でシーンを変更
    switch (key) {
            
        case '1':
            currentScene = 0;
            break;
            
        case '2':
            currentScene = 1;
            break;
            
        case '3':
            currentScene = 2;
            break;
    }
    scenes[currentScene]->keyPressed(key);
    
    
}

void ofApp::keyReleased(int key){
    scenes[currentScene]->keyReleased(key);
}
