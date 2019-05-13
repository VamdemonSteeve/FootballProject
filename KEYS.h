//
//  KEYS.h
//  GameProj
//
//  Created by Sharapov Yevgenii on 5/1/19.
//  Copyright © 2019 Sharapov Yevgenii. All rights reserved.
//

#ifndef KEYS_h
#define KEYS_h

bool navigation[8] = {false, false, false, false, false, false, false, false};

int checkKeys(){
    if(navigation[0] && navigation[2])
        return 1;
    if(navigation[0] && navigation[6])
        return 7;
    if(navigation[2] && navigation[4])
        return 3;
    if(navigation[4] && navigation[6])
        return 5;
    if(navigation[0])
        return 0;
    if(navigation[2])
        return 2;
    if(navigation[4])
        return 4;
    if(navigation[6])
        return 6;
    
    return -1;
}

void dropKeys(int number = 9){
    for(int i = 0; i < 8; i++)
    {
        if(i != number){
            navigation[i] = false;
        }
    }
        
}

bool deceleration(){
    int count = 0;
    for(int i =0; i < 16; i++){
        if(navigation[i])
            count ++;
    }
    return false ? count > 1 : true;
}

void showKeys(){
    for(int i = 0; i < 8; i++)
        cout << navigation[i] << " ";
    cout<<endl;
}
/*
 if(ball->coords[1] > winHeight/2 && var == 2)
 ball->setCoords(ball->coords[0], winHeight/2 - 10);
 if(ball->coords[1] > winHeight/2 && var == 1)
 ball->setCoords(ball->coords[0] - ball->coords[1] + winHeight/2, winHeight/2 - 10);
 if(ball->coords[1] > winHeight/2 && var == 3)
 ball->setCoords(ball->coords[0] + ball->coords[1] - winHeight/2, winHeight/2 - 10);
 
 if(ball->coords[1] < -winHeight/2 && var == 5)
 ball->setCoords(ball->coords[0], -winHeight/2 + 10);
 if(ball->coords[1] > winHeight/2 && var == 1)
 ball->setCoords(ball->coords[0] + ball->coords[1] - winHeight/2, -winHeight/2 + 10);
 if(ball->coords[1] > winHeight/2 && var == 7)
 ball->setCoords(ball->coords[0] - ball->coords[1] + winHeight/2, -winHeight/2 + 10);
 
 if(ball->coords[0] < -winWidth/2 && var == 4)
 ball->setCoords(-winWidth/2 + 10, ball->coords[1]);
 if(ball->coords[1] > winHeight/2 && var == 1)
 ball->setCoords(winWidth/2 - 10,ball->coords[1] - ball->coords[0] + winWidth/2);
 if(ball->coords[1] > winHeight/2 && var == 3)
 ball->setCoords(winWidth/2 - 10,ball->coords[1] + ball->coords[0] - winWidth/2);
 
 if(ball->coords[0] > winWidth/2 && var == 0)
 ball->setCoords(winWidth/2 - 10, ball->coords[1]);
 if(ball->coords[1] > winHeight/2 && var == 1)
 ball->setCoords( -winWidth/2 + 10,ball->coords[1] + ball->coords[0] - winWidth/2);
 if(ball->coords[1] > winHeight/2 && var == 7)
 ball->setCoords( -winWidth/2 + 10,ball->coords[1] - ball->coords[0] + winWidth/2);*/

#endif /* KEYS_h */
