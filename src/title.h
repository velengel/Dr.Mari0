//
//  title.h
//  Dr_MARI0
//
//  Created by Nakamura Kouki on 2018/01/01.
//

#pragma once

#include "ofMain.h"
#include "basescene.h"

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
    
public:
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
