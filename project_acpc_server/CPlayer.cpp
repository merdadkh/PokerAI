/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CPlayer.cpp
 * Author: Mehrdad
 * 
 * Created on August 25, 2017, 8:43 PM
 */

#include "CPlayer.h"

CPlayer::CPlayer() {
    this->Init();
}

CPlayer::CPlayer(const CPlayer& orig) {
}

CPlayer::~CPlayer() {
}

Action* CPlayer::ComputeNextAction(){
    
    m_Action.type = a_call;
    m_Action.size = 0;
    
    return &m_Action;
}
