//
//  basescene.h
//  Dr_MARI0
//
//  Created by Nakamura Kouki on 2018/01/01.
//
#pragma once
#include "ofMain.h"

// 全てのシーンの雛形
class BaseScene {
    
public:
    //仮想関数 (virtual) として定義する
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
};