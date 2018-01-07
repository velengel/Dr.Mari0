

#include "ofApp.h"
class play;
//int currentScene;
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofBackground(0);
    BaseScene *stitle =  new title(*this);
    stitle->setup();
    scenes.push_back(stitle);
    
    BaseScene *splay = new play(*this, stitle);
    splay->setup();
    scenes.push_back(splay);
    
    BaseScene *sclear = new clear(*this, splay);
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
            ChangeScene(0);
            //currentScene=0;
            break;
            
        case '2':
            scenes[currentScene]->init();
            ChangeScene(1);
            //currentScene=1;
            
            scenes[1]->init();
            break;
            
        case '3':
            ChangeScene(2);
            //currentScene=2;
            
            break;
    }
    scenes[currentScene]->keyPressed(key);
    
    
}

void ofApp::keyReleased(int key){
    scenes[currentScene]->keyReleased(key);
}


void ofApp::ChangeScene(int s){
    currentScene=s;
}
