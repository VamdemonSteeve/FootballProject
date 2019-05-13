//
//  FOOt.h
//  GameProj
//
//  Created by Sharapov Yevgenii on 4/29/19.
//  Copyright © 2019 Sharapov Yevgenii. All rights reserved.
//

#ifndef FOOt_h
#define FOOt_h

//#include "player.cpp"
//#ifndef football_cpp
#include <math.h>
#include <iostream>
int speeds[16];
int accelarator[16];
bool deccelaration[16];
int ballSpeed = 0;
bool moving[16];
bool boundaries[16];
int numbers[16];
int inertion;
int kicked = 0;
using namespace std;
class Ball{
public:
    bool kicked;
    bool standBy;
    int coords[2];
    int orientation;
    int speed;
    Ball(int coord1, int coord2){
        coords[0] = coord1;
        coords[1] = coord2;
        kicked = false;
        standBy = false;
    }
    void stop(){
        standBy = true;
    }
    void move(){
        standBy = false;
    }
    void setOrientation(int orientation){
        this->orientation = orientation;
    }
    void setCoords(int coord1, int coord2){
        this->coords[0] = coord1;
        this->coords[1] = coord2;
    }
    void kick(){
        while(kicked){
            for (int i = coords[0]; i < 120; i++) {
                for(int j = coords[1]; j < 240; j++){
                    switch(orientation){
                        case 1: coords[1] ++; break;
                        case 2: coords[0] --; coords[1] ++; break;
                        case 3: coords[0] --; break;
                        case 4: coords[0] --; coords[1] --; break;
                        case 5: coords[1] --; break;
                        case 6: coords[0] ++; coords[1] --; break;
                        case 7: coords[0] ++; break;
                        case 8: coords[0] ++; coords[1] ++; break;
                    }
                }
            }
        }
    }
private:
    
};

class Player{
public:
    int speed;
    int coords[2];
    int orientation;
    bool standBy;
    int team;
    int actionRadius;
    int accelerator;
    bool kicking;                                                          //kick the ball or take it;
    Player(int coord1,int coord2,int orientation,int team, int actionRadius,int speed,int accelerator){
        this->coords[0] = coord1;
        this->coords[1] = coord2;
        this->orientation = orientation;
        this->team = team;
        this->speed = speed;
        this->actionRadius = actionRadius;
        this->accelerator = accelerator;
        standBy = false;
        kicking = true;
    }
    /*Player(const Player &player){
        this->coords[0] = player.coords[0];
        this->coords[1] = player.coords[1];
        this->orientation = player.orientation;
        this->team = player.team;
        this->actionRadius = player.actionRadius;
    }*/
    Player(){}
    void kick(){
        kicking = true;
    }
    void hold(){
        kicking = false;
    }
    void stop(){
        standBy = true;
    }
    void move(){
        standBy = false;
    }
    void speedUp(int s = 1){
        accelerator += s;
    }
    void speedDown(int s = 1){
        accelerator -= s;
    }
    void setSpeed(int speed){
        this->speed = speed;
    }
    void setCoords(int coord1, int coord2){
        this->coords[0] = coord1;
        this->coords[1] = coord2;
    }
    void setOrientation(int orientation){
        this->orientation = orientation;
    }
    void setAccelerator(int accelerator){
        this->accelerator = accelerator;
    }
    bool checkRadius(Ball ball){
        if(sqrt( (coords[0] - ball.coords[0])*(coords[0] - ball.coords[0]) + (coords[1] - ball.coords[1])*(coords[1] - ball.coords[1])) < actionRadius)
            return true;
        else
            return false;
    }
};


class footballField{
public:
    Player players[16];
    footballField(int *coords, int *orientation){
        for(int i=0; i < 16; i++){
            if(i < 8){
                players[i] = Player(coords[i*2], coords[i*2+1], orientation[i], 0, 8, speeds[i],accelarator[i]);
                //players[i].hold();
                players[i].kick();
            }
            else{
                players[i] = Player(coords[i*2], coords[i*2+1], orientation[i], 1, 8, speeds[i],accelarator[i]);
                players[i].kick();
            }
        }
    }
    Player& getPlayer(int i){
        return players[i];
    }
    footballField(){}
};
footballField *mainField;
Ball *ball = new Ball(0,0);
int*ballDirect = NULL;
#endif /* FOOt_h */
