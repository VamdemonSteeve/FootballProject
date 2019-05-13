#include <iostream>
#include <algorithm>
#include <ctime>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <GLUT/GLUT.h>
#include "FOOt.h"
#include "KEYS.h"
#include "client.h"
using namespace std;
float winWidth = 840;
float winHeight = 420;
int clicked = 17;
int team;     //teams 0 and 2
int *a;
int*o;
int counter = 0;
void speedUp(){
    for(int i=0; i < 16; i++)
        deccelaration[i] = true;
}

int sign(int number){
    return team == 0 ? 1 : -1;
}

int begin(int number){
    return team == 0 ? 0 : 16;
}

double minimal(double a, double b){
    if(a < b)
        return a;
    else
        return b;
}

double maximal(double a, double b){
    if(a < b)
        return b;
    else
        return a;
}

void checkBall(){
    if(!ball->kicked && !ball->standBy && kicked == clicked){
        ball->setCoords(ball->coords[0] + mainField->getPlayer(kicked).speed * ballDirect[0], ball->coords[1] + mainField->getPlayer(kicked).speed * ballDirect[1]);
        ballSpeed = 0;
    }
}

void defaultBall(){
    speedUp();
    counter =0;
    ball->kicked = false;
    ball->setCoords(ball->coords[0] + ballSpeed * ballDirect[0], ball->coords[1] + ballSpeed * ballDirect[1]);
    ball->setOrientation(mainField->getPlayer(kicked).orientation);
    ballDirect = NULL;
    ball->stop();
    ballSpeed = 0;
}

void input(){
    srand(10007);
    a = new int [32];
    o = new int [16];
    bool flag = false;
    for(int i = 0; i <= 15; i++){
        //srand(time(0));
        //cout << "Enter the position x, y" << endl;
        speeds[i] = 0;
        moving[i] = true;
        boundaries[i] = false;
        numbers[i] = i;
        accelarator[i] = 1;
        deccelaration[i] = true;
        inertion = 0;
        while(!flag){
            flag = true;
            for(int j=0;j < 2; j++){
                a[i*2 + j] =  -150 + rand() % 300;
                //cin >> a[i*2 + j];
            }
            for(int k = 0; k < i; k++)
            {
                if(sqrt( pow(a[k*2] - a[i*2],2) + pow(a[k*2 + 1] - a[i*2 + 1],2) ) < min(20,20)){
                    //cout <<"current: "<< a[i*2] <<" "<<a[i*2 + 1]<<endl;
                    flag = false;
                }
            }
            if( i < 8 && a[i*2] > -50)
                flag = false;
            if( 8 <= i && a[i*2] < 50)
                flag = false;
        }
        flag = false;
        //cout << "Enter the direction" << endl;
        //cin >> o[i];
        o[i] = i % 8;
        if(!(0 <= o[i] < 8))
            error("Incorrect value!");
    }
    mainField = new footballField(a,o);
}

void glEnter2D(void) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-winWidth/2, winWidth/2, -winHeight/2, winHeight + 60);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
}

void glLeave2D(void) {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glEnable(GL_DEPTH_TEST);
}

void glWrite(float x, float y, int *font, char text[256], int kls, int regime = 0) {
    int i;
    glRasterPos2f(x, y);
    switch(regime){
        case 0:
            for (i=0; i<kls; i++)
                glutBitmapCharacter(font, int(text[i]));
            break;
        case 1:
            glutBitmapCharacter(font,scoreA); break;
        case 2:
            glutBitmapCharacter(font, (int)(char(scoreB))); break;
    }
}

bool controllCollision(int i, int distance,bool isBall = false){
    bool flag = true;
    int *direct = direction(mainField->getPlayer(i).orientation);
    if(ballDirect == NULL)
    {
        ballDirect = direction(8);
    }
    if(!isBall){
        for(int k = 0; k < 16; k++)
        {
            int*directk = direction(mainField->getPlayer(k).orientation);
            if(moving[k] && k != i && sqrt( pow(mainField->getPlayer(k).coords[0] + (mainField->getPlayer(k).speed + mainField->getPlayer(k).accelerator + 7) * directk[0] - ( mainField->getPlayer(i).coords[0] + (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator + 10) * direct[0]) ,2) + pow(mainField->getPlayer(k).coords[1] + (mainField->getPlayer(k).speed + mainField->getPlayer(k).accelerator + 7) * directk[1] - ( mainField->getPlayer(i).coords[1] + (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator + 7) * direct[1]),2)) < min(distance,distance))
                flag = false;
            if((!moving[k] || boundaries[k]) && k != i && sqrt( pow(mainField->getPlayer(k).coords[0] + (mainField->getPlayer(k).speed) * directk[0] - ( mainField->getPlayer(i).coords[0] + (mainField->getPlayer(i).speed + 5) * direct[0]) ,2) + pow(mainField->getPlayer(k).coords[1] + (mainField->getPlayer(k).speed) * directk[1] - ( mainField->getPlayer(i).coords[1] + (mainField->getPlayer(i).speed + 5) * direct[1]),2)) < min(distance,distance))
                flag = false;
        }
        moving[i] = flag;
    }
    else{
        for(int k = 0; k < 16; k++)
        {
            int*directk = direction(mainField->getPlayer(k).orientation);
            if(sqrt( pow(mainField->getPlayer(k).coords[0] + (mainField->getPlayer(k).speed + 7) * directk[0] - ( ball->coords[0] + (ballSpeed + 5) * ballDirect[0]) ,2) + pow(mainField->getPlayer(k).coords[1] + (mainField->getPlayer(k).speed) * directk[1] - ( ball->coords[1] + (ballSpeed) * direct[1]),2)) < min(distance,distance))
                flag = false;
        }
        if(!flag){
            ball->setCoords(ball->coords[0] + mainField->getPlayer(kicked).speed * ballDirect[0], ball->coords[1] + mainField->getPlayer(kicked).speed * ballDirect[1]);
            ballSpeed = 0;
        }
    }
    return flag;
}

void controllBorders(int i, int distance, bool isBall = false){
    bool flag = false;
    int *direct = direction(mainField->getPlayer(i).orientation);
    int var = mainField->getPlayer(i).orientation;
    if(!isBall){
        switch(var){
            case 0: if(fabs(winWidth/2 - (mainField->getPlayer(i).coords[0] + direct[0] * (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator))) < distance){ flag = true; } break;
            case 1: if(minimal(sqrt(2)*(winWidth/2 - (mainField->getPlayer(i).coords[0] + direct[0] * (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator))),sqrt(2)*(winHeight/2 - mainField->getPlayer(i).coords[1] - direct[1] * (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator))) < distance){ flag = true; } break;
            case 2: if(fabs(winHeight/2 - mainField->getPlayer(i).coords[1] - direct[1] * (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator)) < distance){ flag = true; } break;
            case 3: if(minimal(sqrt(2)*(winWidth/2 + (mainField->getPlayer(i).coords[0] + direct[0] * (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator))),sqrt(2)*(winHeight/2 - mainField->getPlayer(i).coords[1] - direct[1] * (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator))) < distance){ flag = true; } break;
            case 4: if(fabs(winWidth/2 + (mainField->getPlayer(i).coords[0] + direct[0] * (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator))) < distance){ flag = true; } break;
            case 5: if(minimal(sqrt(2)*(winWidth/2 + (mainField->getPlayer(i).coords[0] + direct[0] * (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator))),sqrt(2)*(winHeight/2 + mainField->getPlayer(i).coords[1] + direct[1] * (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator))) < distance){ flag = true; } break;
            case 6: if(fabs(winHeight/2 + mainField->getPlayer(i).coords[1] + direct[1] * (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator)) < distance){ flag = true; } break;
            case 7: if(minimal(sqrt(2)*(winWidth/2 - (mainField->getPlayer(i).coords[0] + direct[0] * (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator))),sqrt(2)*(winHeight/2 + mainField->getPlayer(i).coords[1] + direct[1] * (mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator))) < distance){ flag = true; } break;
        }
        boundaries[i] = flag;
    }
    else{
        int varBall = ball->orientation;
        switch(varBall){
            case 0: if(fabs(winWidth/2 - (ball->coords[0] + ballDirect[0] * (ballSpeed + 4))) < distance){
                ball->setCoords(ball->coords[0] + ballSpeed * ballDirect[0], ball->coords[1] + ballSpeed *ballDirect[1]);ballSpeed = 0; ballDirect = direction(4); /*kicked = 4*/; ball->setOrientation(4);} break;
            case 1: if(minimal(sqrt(2)*fabs((winWidth/2 - (ball->coords[0] + ballDirect[0] * (ballSpeed)))),sqrt(2)*fabs((winHeight/2 - ball->coords[1] - ballDirect[1] * (ballSpeed)))) < distance * sqrt(2)){

                ball->setCoords(ball->coords[0] + ballSpeed * ballDirect[0], ball->coords[1] + ballSpeed *ballDirect[1]);
                ballSpeed = 0;
                if(sqrt(2)*fabs((winWidth/2 - (ball->coords[0]))) < sqrt(2)*fabs((winHeight/2 - ball->coords[1]))){
                    ballDirect = direction(3);
                    //kicked = 3;
                    ball->setOrientation(3);
                }
                else{
                    ballDirect = direction(7);
                    //kicked = 7;
                    ball->setOrientation(7);
                }
            } break;
            case 2: if(fabs(winHeight/2 - (ball->coords[1] + ballDirect[1] * (ballSpeed))) < distance){
                ball->setCoords(ball->coords[0] + ballSpeed * ballDirect[0], ball->coords[1] + ballSpeed *ballDirect[1]);ballSpeed = 0; ballDirect = direction(6); ball->setOrientation(6);} break;
            case 3: if(minimal(sqrt(2)*fabs((winWidth/2 + (ball->coords[0] + ballDirect[0] * (ballSpeed)))),sqrt(2)*fabs((winHeight/2 - ball->coords[1] - ballDirect[1] * (ballSpeed)))) < sqrt(2)*distance){
                
                ball->setCoords(ball->coords[0] + ballSpeed * ballDirect[0], ball->coords[1] + ballSpeed *ballDirect[1]);
                ballSpeed = 0;
                if(sqrt(2)*fabs((winWidth/2 + ball->coords[0] + ballDirect[0] * (ballSpeed))) < sqrt(2)*fabs((winHeight/2 - ball->coords[1] - ballDirect[1] * (ballSpeed)))){
                    ballDirect = direction(1);
                    //kicked = 1;
                    ball->setOrientation(1);
                }
                else{
                    ballDirect = direction(5);
                    //kicked = 5;
                    ball->setOrientation(5);
                }
            }break;
            case 4: if(fabs(winWidth/2 + (ball->coords[0] + ballDirect[0] * (ballSpeed))) < distance){
                ball->setCoords(ball->coords[0] + ballSpeed * ballDirect[0], ball->coords[1] + ballSpeed *ballDirect[1]);ballSpeed = 0; ballDirect = direction(0); /*kicked = 0;*/ ball->setOrientation(0); }break;
            case 5: if(minimal(sqrt(2)*(winWidth/2 + (ball->coords[0] + ballDirect[0] * (ballSpeed))),sqrt(2)*(winHeight/2 + ball->coords[1] + ballDirect[1] * (ballSpeed))) < distance){
                ball->setCoords(ball->coords[0] + ballSpeed * ballDirect[0], ball->coords[1] + ballSpeed *ballDirect[1]);
                ballSpeed = 0;
                if(sqrt(2)*(winWidth/2 + (ball->coords[0] + ballDirect[0] * (ballSpeed))) < sqrt(2)*(winHeight/2 + ball->coords[1] + ballDirect[1] * (ballSpeed))){
                    ballDirect = direction(7);
                    //kicked = 7;
                    ball->setOrientation(7);
                }
                else{
                    ballDirect = direction(3);
                    //kicked = 3;
                    ball->setOrientation(3);
                }
            }break;
            case 6: if(fabs(winHeight/2 + (ball->coords[1] + ballDirect[1] * (ballSpeed))) < distance){
                ball->setCoords(ball->coords[0] + ballSpeed * ballDirect[0], ball->coords[1] + ballSpeed *ballDirect[1]);ballSpeed = 0; ballDirect = direction(2); ball->setOrientation(2); }break;
            case 7: if(minimal(sqrt(2)*(winWidth/2 - (ball->coords[0] + ballDirect[0] * (ballSpeed))),sqrt(2)*(winHeight/2 + ball->coords[1] + ballDirect[1] * (ballSpeed))) < distance){
                ball->setCoords(ball->coords[0] + ballSpeed * ballDirect[0], ball->coords[1] + ballSpeed *ballDirect[1]);
                ballSpeed = 0;
                if(sqrt(2)*(winWidth/2 - (ball->coords[0] + ballDirect[0] * (ballSpeed))) < sqrt(2)*(winHeight/2 + ball->coords[1] + ballDirect[1] * (ballSpeed))){
                    ballDirect = direction(5);
                    //kicked = 5;
                    ball->setOrientation(5);
                }
                else{
                    ballDirect = direction(1);
                    //kicked = 1;
                    ball->setOrientation(1);
                }
            }
                break;
        }
    }
    
}

void drawRectangle(double vx, double vy, double width, double height, bool fill, int color = 0){
    glPolygonMode(GL_FRONT, GL_FILL);
    switch(color){
        case 0: glColor3f(1,1,1); break;
        case 1: glColor3f(1,1,0); break;
    }
    if(fill){
        glBegin(GL_POLYGON);
    }
    else{
        glBegin(GL_LINES);
    }
    glVertex2d(vx,vy);
    glVertex2d(vx + width, vy);
    glVertex2d(vx + width, vy - height);
    glVertex2d(vx, vy - height);
    glEnd();
}

void drawCircle(double cx, double cy, double radius, int segments,int color, bool fill = true, double part = 1){
    double angle = 2* 3.1415 / (segments * part);
    glPolygonMode( GL_FRONT, GL_FILL );
    switch(color){
        case 0: glColor3f(0, 0, 1 ); break;
        case 1: glColor3f(1, 0, 0 ); break;
        default: glColor3f(1, 1, 1 );break;
    }
    if(fill){
        glBegin(GL_POLYGON);
    }
    else{
        glLineWidth(100);
        glBegin(GL_LINES);
    }
    double angle1 = 0;
    for(int i = 0; i < segments; i++){
        glVertex2d(cx + radius * cos(angle1),cy + radius *sin(angle1));
        angle1 += angle ;
    }
    glEnd();
}

void angles(int cx, int cy, int radius,int orientation, int type){
    glPolygonMode(GL_FRONT, GL_FILL);
    switch(type){
        case 0: glColor3f(0, 0, 1 ); break;
        case 1: glColor3f(1, 0, 0 ); break;
    }
    glBegin(GL_POLYGON);
    switch(orientation){
        case 0: glVertex2f(cx + radius/sqrt(2), cy - radius/sqrt(2)); glVertex2f(cx + sqrt(2)*radius, cy); glVertex2f(cx + radius/sqrt(2), cy + radius/sqrt(2)); break;
        case 1: glVertex2f(cx + radius, cy); glVertex2f(cx + radius, cy + radius); glVertex2f(cx, cy + radius); break;
        case 2: glVertex2f(cx + radius/sqrt(2), cy + radius/sqrt(2)); glVertex2f(cx,sqrt(2)*radius +  cy);glVertex2f(cx - radius/sqrt(2), cy + radius/sqrt(2)); break;
        case 3: glVertex2f(cx, cy + radius); glVertex2f(cx - radius, cy + radius); glVertex2f(cx - radius, cy); break;
        case 4: glVertex2f(cx - radius/sqrt(2), cy + radius/sqrt(2)); glVertex2f(cx - sqrt(2)*radius, cy); glVertex2f(cx - radius/sqrt(2), cy - radius/sqrt(2)); break;
        case 5: glVertex2f(cx - radius, cy); glVertex2f(cx - radius, cy - radius); glVertex2f(cx, cy - radius); break;
        case 6: glVertex2f(cx - radius/sqrt(2), cy - radius/sqrt(2)); glVertex2f(cx, - sqrt(2)*radius +  cy);glVertex2f(cx + radius/sqrt(2), cy - radius/sqrt(2)); break;
        case 7: glVertex2f(cx, cy - radius); glVertex2f(cx + radius, cy - radius); glVertex2f(cx + radius, cy); break;
            
        default: break;
    }
    glEnd();
}

void drawPatterns(){
    
    glPointSize(50);
    glLineWidth(100);
    
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(-winWidth/2, winHeight/2);
    glVertex2f(winWidth/2, winHeight/2);
    glVertex2f(winWidth/2, -winHeight/2);
    glVertex2f(-winWidth/2, -winHeight/2);
    glEnd();
    
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(0, winHeight/2);
    glVertex2f(0, -winHeight/2);
    glEnd();
    
    drawCircle(0, 0, 75, 100, -1, false);
    
    drawRectangle(-winWidth/2, 100, 100, 200, false);
    
    drawRectangle(-winWidth/2, winHeight/2 + 60, winWidth, 60, true, 1);
    
    glEnter2D();
    glColor3f(1,1,0);
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(0,0,0);
    string str = to_string(scoreA);
    glWrite(60, 430, (int*)GLUT_BITMAP_8_BY_13, (char*)"0", 6,1);
    glWrite(-60, 430, (int*)GLUT_BITMAP_8_BY_13, (char*)"0", 6,2);
    glPopAttrib();
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(1,0,0);
    glWrite(28, 450, (int*)GLUT_BITMAP_8_BY_13, (char*)"TEAM RED", 8);
    glColor3f(0,0,1);
    glWrite(-93, 450, (int*)GLUT_BITMAP_8_BY_13, (char*)"TEAM BLUE", 9);
    glPopAttrib();
    glLeave2D();
    
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2d(-winWidth/2 + 100, 100);
    glVertex2d(-winWidth/2 + 100, -100);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex2d(winWidth/2 - 100, 100);
    glVertex2d(winWidth/2 - 100, -100);
    glEnd();
    
    drawRectangle(winWidth/2-100, 100, 100, 200, false);
    glPushMatrix();
    // glRotatef(90, 0, 0, 1)(2, 2,1);//(90, 1, 0, 0);
    //drawCircle(-winWidth/2 + 100, 0, 50, 100, -1, false, 2);
    glPopMatrix();
}

void controllBall(){
    int *direct;
    random_shuffle(&numbers[0], &numbers[15]);
    if(ballDirect == NULL)
        ballDirect = direction(8);
    if(ball->coords[0] + ballSpeed * ballDirect[0] < -winWidth/2 + 25 && -40 < ball->coords[1] + ballSpeed * ballDirect[1] && ball->coords[1] + ballSpeed * ballDirect[1] < 40)
    {
        scoreA ++;
        ball->setCoords(0, 0);
        ball->setOrientation(8);
        ballSpeed = 0;
        ball->kicked = false;
        ballDirect = direction(8);
        return;
    }
    if(ball->coords[0] + ballSpeed * ballDirect[0] > winWidth/2 - 25 && -40 < ball->coords[1] + ballSpeed * ballDirect[1] && ball->coords[1] + ballSpeed * ballDirect[1] < 40)
    {
        scoreB ++;
        ball->setCoords(0, 0);
        ball->setOrientation(8);
        ballSpeed = 0;
        ball->kicked = false;
        ballDirect = direction(8);
        return;
    }
    for(int i : numbers){
        int * angles = new int[2];
        direct = direction(mainField->getPlayer(i).orientation);
        angles[0] = mainField->getPlayer(i).coords[0] + direct[0] * mainField->getPlayer(i).actionRadius * sqrt(2);
        angles[1] = mainField->getPlayer(i).coords[1] + direct[1] * mainField->getPlayer(i).actionRadius * sqrt(2);
        if(ball->kicked && sqrt(pow((mainField->getPlayer(i).coords[0] + (mainField->getPlayer(i).speed) * direct[0]) - (ball->coords[0] + ballSpeed * ballDirect[0]),2) + pow((mainField->getPlayer(i).coords[1] + (mainField->getPlayer(i).speed) * direct[1]) - (ball->coords[1] + ballSpeed * ballDirect[1]),2)) <1.5 * mainField->getPlayer(i).actionRadius){
            ball->move();
            kicked = i;
            inertion = mainField->getPlayer(kicked).accelerator;
            speedUp();
            counter =0;
            if(mainField->getPlayer(kicked).kicking){
                ball->setCoords(ball->coords[0] + ballSpeed * ballDirect[0], ball->coords[1] + ballSpeed *ballDirect[1]);
                ball->kicked = true;
                speedUp();
            }
            else{
                ball->setCoords(mainField->getPlayer(kicked).coords[0], mainField->getPlayer(kicked).coords[1]);
                ball->kicked=false;
            }
            ballSpeed = 0;
            ball->setOrientation(mainField->getPlayer(kicked).orientation);
            ballDirect = direction(mainField->getPlayer(i).orientation);
            break;
        }
        cout << "distance from " << i << " is " << sqrt(pow((angles[0] + (mainField->getPlayer(i).speed) * direct[0]) - (ball->coords[0] + mainField->getPlayer(i).speed * ballDirect[0]),2) + pow((angles[1] + (mainField->getPlayer(i).speed) * direct[1]) - (ball->coords[1] + mainField->getPlayer(i).speed * ballDirect[1]),2)) << endl;
        if(!ball->kicked && sqrt(pow((angles[0] + (mainField->getPlayer(i).speed) * direct[0]) - (ball->coords[0] + mainField->getPlayer(i).speed * ballDirect[0]),2) + pow((angles[1] + (mainField->getPlayer(i).speed) * direct[1]) - (ball->coords[1] + mainField->getPlayer(i).speed * ballDirect[1]),2)) < 1.5* mainField->getPlayer(i).actionRadius)
        {
            cout << " I was kicked!" << endl;
            ball->move();
            kicked = i;
            inertion = mainField->getPlayer(kicked).accelerator;
            speedUp();
            counter =0;
            cout << "kicked! : " << kicked << endl;
            if(mainField->getPlayer(kicked).kicking){
                speedUp();
                ball->setCoords(ball->coords[0] + mainField->getPlayer(i).speed * ballDirect[0], ball->coords[1] + mainField->getPlayer(i).speed *ballDirect[1]);
                ball->kicked = true;
            }
            else{
                ball->setCoords(mainField->getPlayer(kicked).coords[0], mainField->getPlayer(kicked).coords[1]);
                ball->kicked=false;
            }
            ballSpeed = 0;
            ball->setOrientation(mainField->getPlayer(kicked).orientation);
            ballDirect = direction(mainField->getPlayer(i).orientation);
            break;
        }
    }
    //cout << "ball's coords are : " << ball->orientation << "---"<<ball->coords[0] + ballSpeed * ballDirect[0] << "  " << ball->coords[1] + ballSpeed * ballDirect[1] << "---" << ball->kicked << endl;
    controllBorders(kicked, 13, true);
    if(ball->kicked && !ball->standBy){
        cout << "ball's coords are : " << ball->orientation << "---"<<ball->coords[0] + ballSpeed * ballDirect[0] << "  " << ball->coords[1] + ballSpeed * ballDirect[1] << "---" << ball->kicked << endl;
        drawCircle(ball->coords[0] + ballSpeed * ballDirect[0] ,ball->coords[1] + ballSpeed *ballDirect[1] , 5, 50,-1);

        return;
    }
    if(!ball->kicked && !ball->standBy){
        cout << "ball's coords are : " << ball->orientation << "---"<<ball->coords[0] + ballSpeed * ballDirect[0] << "  " << ball->coords[1] + ballSpeed * ballDirect[1] << "---" << ball->kicked << endl;
        drawCircle(ball->coords[0] + mainField->getPlayer(kicked).speed * ballDirect[0] ,ball->coords[1] + mainField->getPlayer(kicked).speed *ballDirect[1] , 5, 50,-1);
        //clientFunctionSend(clicked);
        //clientFunctionGet(clicked);
        return;
    }
    if(ball->standBy){
        //ball->move();
        drawCircle(ball->coords[0] /*+ ballSpeed * ballDirect[0]*/,ball->coords[1] /*+ ballSpeed *ballDirect[1]*/, 5, 50,-1);
        return;
    }
}

void Draw(){
    int *direct;    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    direct = direction(mainField->getPlayer(clicked).orientation);
    cout << "real coords: " <<mainField->getPlayer(clicked).coords[0] + mainField->getPlayer(clicked).speed * direct[0] << "  " << mainField->getPlayer(clicked).coords[1] + mainField->getPlayer(clicked).speed * direct[1] << endl;
    //cout << "ball's orientation : " << ball->orientation;
   // scoresSend();
    //scoresGet();
    drawPatterns();
    for(int i = 0; i < 16; i++){
        direct = direction(mainField->getPlayer(i).orientation);
        //if(!mainField->getPlayer(i).standBy)
        //{
        controllBorders(i, 16);
        controllCollision(i,16);
        //}
        if(i < 8) {drawCircle(mainField->getPlayer(i).coords[0] + (mainField->getPlayer(i).speed) * direct[0], mainField->getPlayer(i).coords[1] + (mainField->getPlayer(i).speed) * direct[1], mainField->getPlayer(i).actionRadius, 50, 0);
            if(team == 0)
                clientFunctionSend(i);
            else
                clientFunctionGet(i);
            if(clicked == i)
                drawCircle(mainField->getPlayer(i).coords[0] + (mainField->getPlayer(i).speed) * direct[0], mainField->getPlayer(i).coords[1] + (mainField->getPlayer(i).speed) * direct[1], 2 * mainField->getPlayer(i).actionRadius, 50, 0, false);
            angles(mainField->getPlayer(i).coords[0] + (mainField->getPlayer(i).speed) * direct[0], mainField->getPlayer(i).coords[1] + (mainField->getPlayer(i).speed) * direct[1],  mainField->getPlayer(i).actionRadius, mainField->getPlayer(i).orientation, 0);
        }
        else{drawCircle(mainField->getPlayer(i).coords[0] + (mainField->getPlayer(i).speed) * direct[0], mainField->getPlayer(i).coords[1] + (mainField->getPlayer(i).speed) * direct[1], mainField->getPlayer(i).actionRadius, 50, 1);
            if(team == 1)
                clientFunctionSend(i);
            else
                clientFunctionGet(i);
            if(clicked == i)
                drawCircle(mainField->getPlayer(i).coords[0] + (mainField->getPlayer(i).speed) * direct[0], mainField->getPlayer(i).coords[1] + (mainField->getPlayer(i).speed) * direct[1], 2 * mainField->getPlayer(i).actionRadius, 50, 1, false);
            angles(mainField->getPlayer(i).coords[0] + (mainField->getPlayer(i).speed) * direct[0], mainField->getPlayer(i).coords[1] + (mainField->getPlayer(i).speed) * direct[1], mainField->getPlayer(i).actionRadius, mainField->getPlayer(i).orientation, 1);
        }
        controllBall();
    }
    for(int i=0; i < 16; i++){
        controllCollision(i,16);
        controllBorders(i, 16);
    }
    drawRectangle(-winWidth/2, 50, 10, 100, true);
    drawRectangle(winWidth/2-10, 50, 10, 100, true);
    glEnd();
    glutSwapBuffers();
}

void Timer(int value){
    for(int i=0; i < 16; i++){
        if(moving[i] && !boundaries[i])
            mainField->getPlayer(i).setSpeed(mainField->getPlayer(i).speed + mainField->getPlayer(i).accelerator);
        if(deccelaration[i] && ball->kicked){
            //cout << "Speeding up.....!!!!" << endl;
            ballSpeed ++;
            //controllBorders(kicked, 15,true);
        }
        //if(!moving[i])
        //accelarator[i] = 1;
    }
    if(deccelaration[15] == false){
        defaultBall();
    }
    //if(accelarator[kicked] != 0){
    counter = (counter + 1) % (16 * (inertion + 1));
    inertion = max(0,inertion --);
    deccelaration[counter/(inertion + 1)] = false;
    //}
    //else{
    // ball->stop();
    //}
    glutPostRedisplay();
    glutTimerFunc(0, Timer, 0);
}

void mouseClicked(int button, int state, int x, int y){
    dropKeys();
    for(int i=(team * 8); i < (team + 1) * 8; i++){
        int * direct = direction(mainField->getPlayer(i).orientation);
        if(sqrt( pow(x - winWidth/2 - mainField->getPlayer(i).coords[0] - direct[0] * mainField->getPlayer(i).speed,2) + pow( -y + winHeight/2 + 60 - mainField->getPlayer(i).coords[1] - direct[1] * mainField->getPlayer(i).speed,2)) < mainField->getPlayer(i).actionRadius)
        {
            clicked = i;
            break;
        }
    }
}

void mousePressedMove(int x, int y){
    
}

void keyBoard(unsigned char key, int x, int y){
    int * direct = direction(mainField->getPlayer(clicked).orientation);
    switch(key){
        case (char)13: if(mainField->getPlayer(clicked).kicking){mainField->getPlayer(clicked).hold();}
        else
            mainField->getPlayer(clicked).kick();
            checkBall();
            break;
        case (char)27: exit(1);
        case (char)32: mainField->getPlayer(clicked).setCoords(mainField->getPlayer(clicked).coords[0] + mainField->getPlayer(clicked).speed * direct[0], mainField->getPlayer(clicked).coords[1] + mainField->getPlayer(clicked).speed * direct[1]);
            checkBall();
            moving[clicked] = false;
            mainField->getPlayer(clicked).setSpeed(0);
            mainField->getPlayer(clicked).stop();
            dropKeys();
            mainField->getPlayer(clicked).setAccelerator(0);
            break;
    }
    
}

void keyBoardSpecial(int key, int x, int y){
    int dir;
    int * direct = direction(mainField->getPlayer(clicked).orientation);
    switch(key){
        case GLUT_KEY_UP:
            if(mainField->getPlayer(clicked).coords[1] + mainField->getPlayer(clicked).speed * direct[1] > winHeight/2 - 26)
            {
                dropKeys();
                break;
            }
            mainField->getPlayer(clicked).move();
            moving[clicked] = true;
            mainField->getPlayer(clicked).setCoords(mainField->getPlayer(clicked).coords[0] + mainField->getPlayer(clicked).speed * direct[0], mainField->getPlayer(clicked).coords[1] + mainField->getPlayer(clicked).speed * direct[1]);
            mainField->getPlayer(clicked).setSpeed(0);
            checkBall();
            if(deceleration() && !navigation[6])
            {
                mainField->getPlayer(clicked).speedUp();
                mainField->getPlayer(clicked).setAccelerator(min( mainField->getPlayer(clicked).accelerator, 7));
            }
            else{
                dropKeys();
                navigation[6] = false;
                mainField->getPlayer(clicked).speedDown();
                if(mainField->getPlayer(clicked).accelerator < 0)
                    mainField->getPlayer(clicked).setAccelerator(0);
            }
            if(navigation[2]){
                dir = 2;
            }
            else
            {
                navigation[2] = true;
                dir = checkKeys() > 0 ? checkKeys() : 2;
            }
            controllCollision(clicked, 10 + mainField->getPlayer(clicked).actionRadius);
            controllBorders(clicked, 16);
            mainField->getPlayer(clicked).setOrientation(dir);
            dropKeys(2);
            break;
        case GLUT_KEY_DOWN:
            if(mainField->getPlayer(clicked).coords[1] + mainField->getPlayer(clicked).speed * direct[1] < - winHeight/2 + 26)
            {
                dropKeys();
                return;
            }
            mainField->getPlayer(clicked).move();
            moving[clicked] = true;
            mainField->getPlayer(clicked).setCoords(mainField->getPlayer(clicked).coords[0] + mainField->getPlayer(clicked).speed * direct[0], mainField->getPlayer(clicked).coords[1] + mainField->getPlayer(clicked).speed * direct[1]);
            mainField->getPlayer(clicked).setSpeed(0);
            checkBall();
            if(deceleration() && !navigation[2])
            {
                mainField->getPlayer(clicked).speedUp();
                mainField->getPlayer(clicked).setAccelerator(min(mainField->getPlayer(clicked).accelerator, 7));
            }
            else{
                dropKeys();
                navigation[2] = false;
                mainField->getPlayer(clicked).speedDown();
                if(mainField->getPlayer(clicked).accelerator < 0)
                    mainField->getPlayer(clicked).setAccelerator(0);
            }
            if(navigation[6]){
                dir = 6;
            }
            else
            {
                navigation[6] = true;
                dir = checkKeys() > 0 ? checkKeys() : 6;
            }
            controllCollision(clicked, 10 + mainField->getPlayer(clicked).actionRadius);
            controllBorders(clicked, 16);
            mainField->getPlayer(clicked).setOrientation(dir);
            dropKeys(6);
            break;
        case GLUT_KEY_RIGHT:
            if(mainField->getPlayer(clicked).coords[0] + mainField->getPlayer(clicked).speed * direct[0] > winWidth/2 - 26)
            {
                dropKeys();
                return;
            }
            mainField->getPlayer(clicked).move();
            moving[clicked] = true;
            mainField->getPlayer(clicked).setCoords(mainField->getPlayer(clicked).coords[0] + mainField->getPlayer(clicked).speed * direct[0], mainField->getPlayer(clicked).coords[1] + mainField->getPlayer(clicked).speed * direct[1]);
            mainField->getPlayer(clicked).setSpeed(0);
            checkBall();
            if(deceleration() && !navigation[4])
            {
                mainField->getPlayer(clicked).speedUp();
                mainField->getPlayer(clicked).setAccelerator(min(mainField->getPlayer(clicked).accelerator, 7));
            }
            else{
                dropKeys();
                navigation[4] = false;
                mainField->getPlayer(clicked).speedDown();
                if(mainField->getPlayer(clicked).accelerator < 0)
                    mainField->getPlayer(clicked).setAccelerator(0);
            }
            if(navigation[0]){
                dir = 0;
            }
            else
            {
                navigation[0] = true;
                dir = checkKeys() > 0 ? checkKeys() : 0;
            }
            controllCollision(clicked, 10 + mainField->getPlayer(clicked).actionRadius);
            controllBorders(clicked, 16);
            mainField->getPlayer(clicked).setOrientation(dir);
            dropKeys(0);
            break;
        case GLUT_KEY_LEFT:
            if(mainField->getPlayer(clicked).coords[0] + mainField->getPlayer(clicked).speed * direct[0] < - winWidth/2 + 16){
                dropKeys();
                return;
            }
            mainField->getPlayer(clicked).move();
            moving[clicked] = true;
            mainField->getPlayer(clicked).setCoords(mainField->getPlayer(clicked).coords[0] + mainField->getPlayer(clicked).speed * direct[0], mainField->getPlayer(clicked).coords[1] + mainField->getPlayer(clicked).speed * direct[1]);
            mainField->getPlayer(clicked).setSpeed(0);
            checkBall();
            if(deceleration() && !navigation[0])
            {
                mainField->getPlayer(clicked).speedUp();
                mainField->getPlayer(clicked).setAccelerator(min(mainField->getPlayer(clicked).accelerator, 7));
            }
            else{
                dropKeys();
                navigation[0] = false;
                mainField->getPlayer(clicked).speedDown();
                if(mainField->getPlayer(clicked).accelerator < 0)
                    mainField->getPlayer(clicked).setAccelerator(0);
            }
            if(navigation[4]){
                dir = 4;
            }
            else
            {
                navigation[4] = true;
                dir = checkKeys() > 0 ? checkKeys() : 4;
            }
            controllCollision(clicked, 10 + mainField->getPlayer(clicked).actionRadius);
            controllBorders(clicked, 16);
            mainField->getPlayer(clicked).setOrientation(dir);
            dropKeys(4);
            break;
            
    }
}
void Initialize(){
    glClearColor(0.8, 1.0, 0.0, 0.5);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-winWidth/2, winWidth/2, -winHeight/2, winHeight/2 + 60, -5.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    //glTranslatef(50, 50, 0);
    //glScalef(0.1,0.1,0.1);
    //glRotatef(90,90,0,0);
    
}

int main(int argc, char *argv[])
{
    team = atoi(argv[3]);
    input();
    init(argc,argv);
    //establishConnection();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight + 60);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(argv[3]);        glutDisplayFunc(Draw);//Рисование
    glutTimerFunc(50, Timer, 0);
    glutMouseFunc(mouseClicked);
    glutSpecialFunc(keyBoardSpecial);
    glutKeyboardFunc(keyBoard);
    Initialize();
    glutMainLoop();
    return 0;
}



