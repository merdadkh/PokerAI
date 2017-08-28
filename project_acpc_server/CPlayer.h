/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CPlayer.h
 * Author: Mehrdad
 *
 * Created on August 25, 2017, 8:43 PM
 */

#ifndef CPLAYER_H
#define CPLAYER_H

#include "ParentPlayer.h"
#include <vector>

class CPlayer : public ParentPlayer {
public:
    CPlayer();
    CPlayer(const CPlayer& orig);
    virtual ~CPlayer();
    
    Action* ComputeNextAction();

    
    std::vector<std::vector<double> >* m_pPreFlopProb;
private:

};

#endif /* CPLAYER_H */

