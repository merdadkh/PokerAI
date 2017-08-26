/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParentPlayer.h
 * Author: Mehrdad
 *
 * Created on August 20, 2017, 8:13 PM
 */

#ifndef PLAYERPARENT_H
#define PLAYERPARENT_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <getopt.h>

extern "C" {
#include "net.h"
#include "game.h"
#include "rng.h"
}

class ParentPlayer {
    
public:
    ParentPlayer();
    ParentPlayer(const ParentPlayer& orig);
    virtual ~ParentPlayer();
    
    void Init();
    void ReadGame(char* gamePath);
    void DoAction(Action* action);
    virtual Action* ComputeNextAction();

    void Connect2Dealer(char* ServerIP, char* PortNo);
    void SendVersionStr2Dealer();
    
    bool ParseLine2MatchState();
    void PlayGame();
    
    
public:
    
    Game* m_pGame;
    MatchState m_MatchState;
    Action m_Action;
    char m_Line[ MAX_LINE_LEN ];
    int m_LineLen;
    
    FILE *m_pToServer, *m_pFromServer;

    rng_state_t m_rng;
    struct timeval tv;

};

#endif /* PLAYERPARENT_H */

