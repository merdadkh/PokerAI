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
#include "../Include/ProbEval.h"
#include <iostream>

CPlayer::CPlayer() {
    this->Init();
    m_pPreFlopProb = CProbEval::LoadPreFlopProb_fromFile();
}

CPlayer::CPlayer(const CPlayer& orig) {
}

CPlayer::~CPlayer() {
}

Action* CPlayer::ComputeNextAction(){
    
    
    ofstream os;
    os.open("DebugFile.txt", ios::app);
    
    char boardStr[9];
    char handStr[6];
        
    uint8_t * handCards = new uint8_t[2];
    for (size_t i = 0; i < 2; i++)
        handCards[i] = m_MatchState.state.holeCards[m_MatchState.viewingPlayer][i];

    int numCard = printCards( m_pGame->numHoleCards, handCards, 6, handStr );
    os << "Cards: " << handStr << "\t";

    unsigned long long hand1Mask = CHandEval::ParseHand(handStr);
    size_t hand_index = CHandIterator::PocketMask2IndexMap.find(hand1Mask)->second;

    double sum = 0, WinProb = -1;
    int count = 0;
    if (m_MatchState.state.round == 0){
        for (size_t i = 0; i < POCKET_HAND_COUNT; i++) {
            if (m_pPreFlopProb->at(hand_index).at(i) >= 0)
            {
                sum += m_pPreFlopProb->at(hand_index).at(i);
                count++;    
            }	
        }
        WinProb = sum / count;
        os << "WinProb: " << WinProb << "\t";

    }
    else if (m_MatchState.state.round == 1){
        
        uint8_t * boardCards = new uint8_t[m_pGame->numBoardCards[1]];
        for (size_t i = 0; i < m_pGame->numBoardCards[1]; i++)
            boardCards[i] = m_MatchState.state.boardCards[i];
        

        numCard = printCards( m_pGame->numBoardCards[1], boardCards, 9, boardStr );
    

        unsigned long long flopMask = CHandEval::ParseHand(boardStr);
	vector<vector<double> >* vec = CProbEval::LoadFlopProb_fromFile(flopMask);
        for (size_t i = 0; i < POCKET_HAND_COUNT; i++) {
            if (vec->at(hand_index).at(i) >= 0)
            {
                sum += vec->at(hand_index).at(i);
                count++;    
            }	
        }
        WinProb = sum / count;
        
        os << "Cards: " << handStr << ":" << boardStr << endl;
        os << "WinProb: " << WinProb << "\t";

    }
    
  
    
    if ((WinProb < 0) || (WinProb > 0.4 && WinProb < 0.6)){
        m_Action.type = a_call;
        m_Action.size = 0;
    }
    else if (WinProb >= 0.6) {
        int32_t min, max;
        m_Action.type = a_raise;
        if( raiseIsValid( m_pGame, &m_MatchState.state, &min, &max ) ) {
            m_Action.size = min + genrand_int32( &m_rng ) % ( max - min + 1 );
        }
    }
    else{
        m_Action.type = a_fold;
        m_Action.size = 0;
    }
    
    if( !isValidAction( m_pGame, &m_MatchState.state, 0, &m_Action ) ){
        m_Action.type = a_call;
        m_Action.size = 0;
    }
    
    os.close();
    
    return &m_Action;
}
