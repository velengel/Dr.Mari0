//title

#include "title.h"
#include "ofApp.h"

#define cap(i,j) for(int i=0;i<2;++i)for(int j=0;j<2;++j)


void title::setup(){
    keyboard[1].load("Phosphate.ttc",80);
    keyboard[1].setSpaceSize(40.0);
    sounds[0].load("sounds/rot.wav");
    sounds[1].load("sounds/set.wav");
    sounds[2].load("sounds/move.wav");
    sounds[3].load("sounds/erase.wav");
    sounds[4].load("sounds/drop.wav");
}

void title::update(){
    
}

void title::draw(){
    ofBackground(0, 0, 0);
    
    
    for ( int i = 0; i < ofGetWidth(); i+=step ){
        for ( int j=0; j < ofGetHeight(); j+=step){
            ofColor c;
            c.setHsb( hue, ofMap(i, ofGetWidth(),-ofGetWidth()/2, 0,128), ofMap(j, -ofGetHeight()/4,ofGetHeight()/4, 0,128 ) );
            ofSetColor( c );
            ofRect( i, j, step-1, step-1 );
        }
    }
    keyboard[1].drawString("Dr.MARI0",100,200);
    keyboard[1].drawString("PRESS 2 key to play",10,400);
}

void title::keyPressed(int key){
    //if(key=='z')currentScene=1;
}

void title::keyReleased(int key){
    
}

//--------------------------------------------------------------------------------------------------
//play


/*void play::ChangeScene(int s){
    b.ChangeScene(s);
}*/

void play::retC(int C) {
    if (C%10 == 1)ofSetColor(255,0,0);
    else if (C%10 == 2)ofSetColor(255, 255, 0);
    else if (C%10 == 3)ofSetColor(0, 0, 255);
    else ofSetColor(255, 255, 255);
}

void play::DrawBlocks(int x, int y, int ablock[][2]) {
    cap(i,j){
        retC(ablock[i][j]);
        if(ablock[i][j]!=0)ofDrawRectangle(x+i*cell, y+j*cell, cell, cell);
    }
}

//rotation block
void play::rotB() {
    rotblock[0][1] = nowblock[0][0];
    rotblock[1][1] = nowblock[0][1];
    rotblock[1][0] = nowblock[1][1];
    rotblock[0][0] = nowblock[1][0];
    cap(i,j)nowblock[i][j] = rotblock[i][j];
}

bool play::isblock(int x, int y) {
    if (field[(x - cell*2) / cell][y / cell+1] != 0)return true;
    return false;
}

void play::DrawField() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 24; ++j) {
            if (field[i][j] != 0) {
                retC(field[i][j]);
                if(field[i][j]<10){
                    
                    ofDrawRectangle(cell*2 + i * cell, j * cell, cell, cell);
                }else if(field[i][j]<20){
                    //ofDrawCircle(cell*2 + i * cell+cell/2, j * cell+cell/2,cell/2);
                    viruses[field[i][j]%10-1].draw(cell*2 + i * cell,j * cell,cell,cell);
                }else{
                    if (tim<0 && field[i][j] >= 90)viruses[3].draw(cell*2 + i * cell, j * cell, cell, cell);
                }
                
            }else{
               // viruses[3].draw(cell*2 + i * cell, j * cell, cell, cell);
            }
        }
    }
}



void play::eraseblock(){
    while (1) {
        f2 = true;
        for (int i = 0; i < 9; ++i) {
            for (int j = 23; j > 0; --j) {
                if (field[i][j] >= 90)field[i][j] = 0;
            }
        }
        for (int i = 0; i < 9; ++i) {
            for (int j = 23; j > 0; --j) {
                if (field[i][j] == 0 && field[i][j - 1] != 0 && field[i][j - 1] < 10) {
                    //field[i][j] = 0;
                    field[i][j] = field[i][j - 1];
                    field[i][j - 1] = 0;
                    f2 = false;
                }
                
            }
        }
        sounds[3].play();
        if (f2)break;
    }
}

void play::iseraseblock() {
    f = false;
    //height
    for (int i = 0; i < 9; ++i) {
        cnt = 1;
        cl = field[i][0] % 10;
        for (int j = 1; j < 24; ++j) {
            
            if (field[i][j] % 10 == cl && field[i][j]  ) {
                cnt++;
            }
            else {
                if (cnt >= 4) {
                    sco=cnt;
                    for (int k = j - cnt; k < j; k++){
                        if(field[i][k]>10)sco+=9;
                        field[i][k] = 90+cl;
                        efield[i][k]=1;
                    }
                    score += 10*sco;
                    f = true;
                }
                cnt = 1;
                cl = field[i][j] % 10;
                
            }
        }
    }
    
    //width
    for (int j = 0; j <23; ++j){
        cnt = 1;
        cl = field[0][j] % 10;
        for (int i = 1; i < 10; ++i) {
            
            if (field[i][j] % 10 == cl && field[i][j] ) {
                cnt++;
            }
            else {
                if (cnt >= 4) {
                    sco=cnt;
                    for (int k = i - cnt; k <i; k++){
                        if(field[k][j]>10)sco+=9;
                        field[k][j] = 90+cl;
                        efield[k][j]=1;
                    }
                    score += 10*sco;
                    f = true;
                }
                cnt = 1;
                cl = field[i][j] % 10;
                
            }
        }
    }
    if (f){
        //eflag=1;
        //etim=-10000;
        //while(etim<0)etim+=0.001;
        eraseblock();
        tim=-10;
        etim=10;
        //eflag=0;
    }
}

void play::createblock(){
    fibl = rand() % 6;
    cap(i,j){
        nowblock[i][j]=nextblock[i][j];
    }
    
    cap(i,j){
        nextblock[i][j] = Blocks[fibl][i][j];
    }
    if(field[4][0]+field[4][1]>0)gflag=1;
    sounds[1].play();
}

void play::init(){
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 24; ++j) {
            efield[i][j]=0;
        }
    }
    cnttim=0;
    sounds[1].play();
    score = 0;
    bx = cell*2;
    by = 0;
    tim = 0;
    Nvirus = 2;
    pflag = 0,gflag=0,eflag=0;
    fibl = rand() % 6;
    accel=1;
    for(int i=0;i<30;++i)for(int j=0;j<30;++j)field[i][j]=0;
    for (int i = 0; i < 9; ++i)field[i][23] = 4;
    
    cap(i,j)nowblock[i][j] = Blocks[fibl][i][j];
    fibl = rand() % 6;
    cap(i,j)nextblock[i][j]=Blocks[fibl][i][j];
    //virus
    for (int i = 0; i < Nvirus; ++i) {
        vx = rand() % 9, vy = rand() % 13+9;
        field[vx][vy] = 11 + i % 3;
    }
}

void play::cntdispvirus(){
    for(int i=0;i<3;++i)virusnum[i]=0;
    f=true;
    for (int i = 0; i < 9; ++i) {
        for (int j = 23; j > 0; --j) {
            if(field[i][j]>10){
                virusnum[field[i][j]%10-1]++;
                f=false;
            }
        }
    }
    if(f){
        //currentScene=2;
       // *this->ChangeScene(2);
        b.ChangeScene(2);
        //keyboard[0].drawString(ofToString(currentScene), 700+50,250);
    }
    for(int i=0;i<3;++i){
        retC(i+1);
        //ofSetColor(255,122,255);
        keyboard[0].drawString(ofToString(virusnum[i]), 400+50,250+i*50);
        viruses[i].draw(400,222+i*50,cell,cell);
    }
}

//--------------------------------------------------------------
void play::setup(){
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false);
    ofBackground(0, 0, 0);
    ofSetWindowShape(1024, 768);
    ofSetWindowPosition(150, 100);
    
    init();
    viruses[0].load("images/rvirus.png");
    viruses[1].load("images/yvirus.png");
    viruses[2].load("images/bvirus.png");
    viruses[3].load("images/erase.png");
    ofTrueTypeFont::setGlobalDpi(72);
    keyboard[0].load("Tahoma.ttf",36);
    keyboard[0].setSpaceSize(20.0);
}

//--------------------------------------------------------------
void play::update(){
    if(!pflag && !eflag && !gflag)tim+=20*accel;
    if (tim > 1000) {
        tim -= 1000;
        f = false;
        cap(i, j) {
            if (nowblock[i][j] && isblock(bx + i * cell, by + j * cell )) {
                f = true;
                break;
            }
        }
        if(!f)by += cell;
        else {
            cap(i, j) {
                if (nowblock[i][j])field[(bx + i * cell - cell*2) / cell][(by + j * cell) / cell] = nowblock[i][j];
            }
            iseraseblock();
            by = 0;
            bx=cell*6;
            createblock();
        }
    }
    if (by > cell*23) {
        by = 0;
        bx = cell*6;
        createblock();
        iseraseblock();
    }
   
}

//static int tflag=0;
//--------------------------------------------------------------
void play::draw(){
    ofBackground(75, 75, 75);
    DrawField();
    DrawBlocks(bx, by, nowblock);
    DrawBlocks(400, 50, nextblock);
    ofNoFill();
    ofDrawRectangle(385,35,cell*3,cell*3);
    ofFill();
    ofSetColor(255, 255, 255);
    ofDrawLine(cell*11, 0, cell*11, cell*23);
    ofDrawLine(cell*2, 0, cell*2, cell*23);
    //ofDrawBitmapStringHighlight("score : %d", score, 300, 50);
    //ofDrawBitmapStringHighlight("Z : Left Rotation", 400, 100);
    keyboard[0].drawString("     Z     : Left Rotation", 400,100+300);
    keyboard[0].drawString("     X     : Right Rotation", 400,140+300);
    keyboard[0].drawString("     P     : Pause", 400,180+300);
    keyboard[0].drawString("Left Arrow  : Left Move", 400,220+300);
    keyboard[0].drawString("Right Arrow : Right Move", 400,260+300);
    keyboard[0].drawString("Up Arrow   : Hard Drop", 400,300+300);
    keyboard[0].drawString("Down Arrow : Soft Drop", 400,340+300);
    
    cntdispvirus();
    ofSetColor(255,122,255);
    keyboard[0].drawString("Score : "+ofToString(score), 400,190);
    //keyboard[0].drawString("tim : "+ofToString(tim), 700,290);
    if(scur>0){
        ofNoFill();
        ofSetColor(255, 255, 255);
        ofDrawRectangle(400,105+40*(scur-2)+300,500,40);
        ofFill();
    }
    //pause
    
    if(pflag){
        ofBackground(25, 50, 100);
        ofDrawBitmapStringHighlight("Pause", 305, 305);
    }
    /*if(eflag){
        etim=-100000;
        while(etim<0){
            etim+=0.001;
            for (int i = 0; i < 9; ++i) {
                for (int j = 0; j < 24; ++j) {
                    if (field[i][j] >= 90){
                        //viruses[3].draw(cell*2 + i * cell, j * cell, cell, cell);
                        //ofBackground(25, 50, 100,100);
                        ofSetColor(255, 255, 255);
                        ofDrawRectangle(cell*2+i*cell, j*cell, cell, cell);
                    }
                }
            }
        }
    }*/
    if(etim){
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 24; ++j) {
                if(efield[i][j])viruses[3].draw(cell*2 + i * cell, j * cell, cell, cell);
            }
        }
        etim--;
        
    }else{
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 24; ++j) {
                efield[i][j]=0;
            }
        }
    }
    if(gflag){
        ofBackground(175, 175, 175);
        keyboard[1].drawString("GAME OVER",100,200);
        keyboard[1].drawString("PRESS 2 key to caplay",10,400);
    }
    if(!tflag)cnttim++;
    keyboard[0].drawString("Time : "+ofToString(cnttim/60), 650,190);
    
    
}

//--------------------------------------------------------------
void play::keyPressed(int key){
    if(key == 'z' && !pflag){
        scur=1;
        rotB();
        f3 = false;
        cap(i, j) {
            if (nowblock[i][j] && field[(bx + i * cell - cell*2) / cell][(by + j * cell) / cell])f3 = true;
        }
        if (f3)for (int i = 0; i < 3; ++i)rotB();
        else {
            f = false, f2 = false;
            cap(i, j) {
                if (nowblock[i][j] && bx + i * cell > cell*10)f = true;
            }
            cap(i, j) {
                if (nowblock[i][j] && bx + i * cell < cell*2)f2 = true;
            }
            if (f)bx -= cell;
            if (f2)bx += cell;
            sounds[0].play();
        }
        
    }
    if(key=='t'){
        tflag++;
        tflag%=2;
    }
    if(key == 'x' && !pflag){
        for(int i=0;i<3;++i)rotB();
        scur=2;
        f3 = false;
        cap(i, j) {
            if (nowblock[i][j] && field[(bx + i * cell - cell*2) / cell][(by + j * cell) / cell])f3 = true;
        }
        if (f3)rotB();
        else {
            f = false, f2 = false;
            cap(i, j) {
                if (nowblock[i][j] && bx + i * cell > cell*10)f = true;
            }
            cap(i, j) {
                if (nowblock[i][j] && bx + i * cell < cell*2)f2 = true;
            }
            if (f)bx -= cell;
            if (f2)bx += cell;
            sounds[0].play();
        }
    }
    if(key == 'p'){
        scur=3;
        pflag++;
        pflag %= 2;
    }
    if(key == OF_KEY_LEFT && !pflag){
        
        scur=4;
        f = false;
        cap(i, j) {
            if (nowblock[i][j]) {
                if (bx + i * cell - cell < cell*2)f = true;
                //0 < (bx + i * cell - cell*2) / cell - 1 ||
                if (field[(bx + i * cell - cell*2) / cell - 1][(by + j * cell) / cell])f = true;
            }
        }
        if (!f){
            bx -= cell;
            sounds[2].play();
        }
    }
    if(key == OF_KEY_RIGHT && !pflag){
        scur=5;
        f = false;
        cap(i, j) {
            if (nowblock[i][j]) {
                if (bx + i * cell + cell > cell*10)f = true;
                if (field[(bx + i * cell - cell*2) / cell + 1][(by + j * cell) / cell])f = true;
            }
        }
        if (!f){
            bx += cell;
            sounds[2].play();
        }
    }
    if(key == OF_KEY_DOWN && !pflag){
        accel=8;
        scur=7;
        sounds[4].play();
    }
    if(key == OF_KEY_UP && !pflag){
        scur=6;
        while (1) {
            f = false;
            cap(i, j) {
                if (nowblock[i][j] && isblock(bx + i * cell, by + j * cell)) {
                    f = true;
                    break;
                }
            }
            if (!f)by += cell;
            else break;
        }
        cap(i, j) {
            if (nowblock[i][j])field[(bx + i * cell - cell*2) / cell][(by + j * cell) / cell] = nowblock[i][j];
        }
        iseraseblock();
        by = 0;
        bx = cell*6;
        createblock();
    }
    if(key=='2' && gflag){
        gflag=0;
        init();
    }
}

void play::keyReleased(int key){
    scur=0;
    if(key == OF_KEY_DOWN && !pflag)accel=1;
    if(key == 'z'){
        //scur=0;
        
        //keyboard.drawString("     Z    : Left Rotation", 400,100);
    }
}

//clear-----


void clear::setup(){
    keyboard[1].load("Phosphate.ttc",80);
    keyboard[1].setSpaceSize(40.0);
}

void clear::update(){
    
}

void clear::draw(){
    ofBackground(124, 0, 50);
    float hue = fmodf(ofGetElapsedTimef()*200,255);
    int step = 30;
    for ( int i = 0; i < ofGetWidth(); i+=step ){
        for ( int j=0; j < ofGetHeight(); j+=step){
            ofColor c;
            c.setHsb( hue, ofMap(i, ofGetWidth(),-ofGetWidth()/2, 0,128), ofMap(j, -ofGetHeight()/4,ofGetHeight()/4, 0,128 ) );
            ofSetColor( c );
            ofRect( i, j, step-1, step-1 );
        }
    }
    keyboard[1].drawString("Clear",100,200);
}

void clear::keyPressed(int key){
    
}

void clear::keyReleased(int key){
    
}
