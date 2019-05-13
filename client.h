//
//  client.h
//  GameProj
//
//  Created by Sharapov Yevgenii on 5/4/19.
//  Copyright © 2019 Sharapov Yevgenii. All rights reserved.
//

#ifndef client_h
#define client_h
#include <iostream>
#include <fstream>
using namespace std;
int portno, socketfd, nn, enemySocket;
char buffer1[255];
struct sockaddr_in serv_addr;
struct hostent *server;
int number = 17;
char scoreA = '0', scoreB = '0';
int* direction(int number){
    int* a = new int[2];
    switch(number){
        case 0: a[0] = 1; a[1] = 0; break;
        case 1: a[0] = 1; a[1] = 1; break;
        case 2: a[0] = 0; a[1] = 1; break;
        case 3: a[0] = -1; a[1] = 1; break;
        case 4: a[0] = -1; a[1] = 0; break;
        case 5: a[0] = -1; a[1] = -1; break;
        case 6: a[0] = 0; a[1] = -1; break;
        case 7: a[0] = 1; a[1] = -1; break;
        default: a[0] = 0; a[1] = 0; break;
    }
    return a;
}

void error(const char*error)
{
    perror(error);
    exit(1);
}

void init(int argc, char*argv[]){
    if (argc < 4)
    {
        printf("port number not provided!\n");
        exit(1);
    }
    
    portno = atoi(argv[2]);
    socketfd = socket(AF_INET,SOCK_STREAM,0);
    if(socketfd < 0){
        error("Error opening socket");
    }
    
    server = gethostbyname(argv[1]);
    if(server == NULL){
        printf("Error no host %s", argv[1]);
    }
    bzero((char *) &serv_addr,sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if(connect(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("Connection failed");
    }
}

void clientFunctionSend(int &clicked){
    int speed1,speed2, bspeed1, bspeed2;
    int * direct = direction(mainField->getPlayer(clicked).orientation);
    //cout << " ----------SEND!" <<endl;
    if(ballDirect == NULL){
        ballDirect = direction(8);
    }
    speed1 = mainField->getPlayer(clicked).coords[0] + mainField->getPlayer(clicked).speed * direct[0];
    speed2 = mainField->getPlayer(clicked).coords[1] + mainField->getPlayer(clicked).speed * direct[1];
    bspeed1 = ball->coords[0] + ballDirect[0] * ballSpeed;
    bspeed2 = ball->coords[1] + ballDirect[1] * ballSpeed;
    write(socketfd, &clicked, sizeof(clicked));
    write(socketfd, &mainField->getPlayer(clicked).orientation, sizeof(mainField->getPlayer(clicked).orientation));
    //cout << "sending: " << "number: " << clicked <<"  "<< speed1 << "   " << speed2;
    write(socketfd, &mainField->getPlayer(clicked).accelerator, sizeof(mainField->getPlayer(clicked).accelerator));
    write(socketfd, &speed1, sizeof(speed1));
    write(socketfd, &speed2, sizeof(speed2));
    write(socketfd, &ball->orientation, sizeof(int));
    write(socketfd, &bspeed1, sizeof(int));
    write(socketfd, &bspeed2, sizeof(int));
    write(socketfd, &ball->kicked, sizeof(bool));
    write(socketfd, &ball->standBy, sizeof(bool));
    write(socketfd,&inertion,sizeof(int));
    //ballSpeed = 0;
    cout << "sending: " << ball->orientation << "  " << bspeed1 << "  "<<bspeed2 <<  endl;
    //cout << "sending " << inertion << endl;
    int i = strncmp("Bye", buffer1, 3);
    if(i == 0)
    {
        close(socketfd);
    }
}

void clientFunctionGet(int &clicked){
    number = 17;
    //cout << " ----------GET!" <<endl;
    int coord1=0, coord2=0, speed, orientation = 0, accelarator = 0, borientation,bspeed1 = 0,bspeed2 = 0, binertion = 0;
    bool bkicked = false, bstandBy = false;
    read(socketfd, &number, sizeof(int));
    //cout << "number is " << number << endl;
    read(socketfd, &orientation,sizeof(orientation));
    read(socketfd, &accelarator, sizeof(accelarator));
    read(socketfd, &coord1, sizeof(coord1));
    read(socketfd, &coord2, sizeof(coord2));
    read(socketfd, &borientation, sizeof(borientation));
    read(socketfd, &bspeed1, sizeof(int));
    read(socketfd, &bspeed2, sizeof(int));
    read(socketfd, &bkicked, sizeof(bool));
    read(socketfd, &bstandBy, sizeof(bool));
    read(socketfd, &binertion, sizeof(int));
    //cout << "getting: " << "number: " << number <<"  "<< coord1 << "   " << coord2;
    //cout << "getting: " << binertion<< endl;
    if(number < 16 && number >= 0){
        mainField->getPlayer(number).setSpeed(0);
        mainField->getPlayer(number).setOrientation(orientation);
        mainField->getPlayer(number).setCoords(coord1, coord2);
        mainField->getPlayer(number).setAccelerator(accelarator);
    }
    if(borientation < 16 && borientation >= 0){
        ball->kicked = bkicked;
        ball->standBy = bstandBy;
        inertion = binertion;
        //cout << "getting:  " << borientation << "  " << bspeed1 << "  " << bspeed2 << endl;
        //cout << "  kicked = "<<bkicked << "  standBy = " << bstandBy << endl;
        ballSpeed = 0;
        ball->setCoords(bspeed1, bspeed2);
        ball->setOrientation(borientation);
    }
    int i = strncmp("Bye", buffer1, 3);
    if(i == 0)
    {
        close(socketfd);
    }
}

void establishConnection(){
    write(socketfd,&socketfd,sizeof(int));
    read(socketfd, &enemySocket, sizeof(int));
    printf("I am %d and my enemy is: %d \n",socketfd,enemySocket);
}

#endif /* client_h */
