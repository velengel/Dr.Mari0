#include "ofApp.h"

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
