/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParentPlayer.cpp
 * Author: Mehrdad
 * 
 * Created on August 20, 2017, 8:13 PM
 */

#include "ParentPlayer.h"

ParentPlayer::ParentPlayer() {
    Init();
}

void ParentPlayer::Init(){
    /* Initialize the player's random number state using time */
  gettimeofday( &tv, NULL );
  init_genrand( &m_rng, tv.tv_usec );
  
}

ParentPlayer::ParentPlayer(const ParentPlayer& orig) {
}

ParentPlayer::~ParentPlayer() {
    
    
}

void ParentPlayer::ReadGame(char* gamePath){
    
  FILE* file = fopen( gamePath, "r" );
  if( file == NULL ) {

    fprintf( stderr, "ERROR: could not open game %s\n", gamePath );
    exit( EXIT_FAILURE );
  }
  m_pGame = readGame( file );
  if( m_pGame == NULL ) {

    fprintf( stderr, "ERROR: could not read game %s\n", gamePath );
    exit( EXIT_FAILURE );
  }
  fclose( file );
}

Action* ParentPlayer::ComputeNextAction(){

    int32_t min, max;

    double probs[ NUM_ACTION_TYPES ];
    double actionProbs[ NUM_ACTION_TYPES ];
    /* build the set of valid actions */
    double p = 0;
    for( int a = 0; a < NUM_ACTION_TYPES; ++a ) {
      actionProbs[ a ] = 0.0;
    }
    
    /* Define the probabilities of actions for the player */
    probs[ a_fold ] = 0.06;
    probs[ a_call ] = ( 1.0 - probs[ a_fold ] ) * 0.5;
    probs[ a_raise ] = ( 1.0 - probs[ a_fold ] ) * 0.5;

    /* consider fold */
    m_Action.type = a_fold;
    m_Action.size = 0;
    if( isValidAction( m_pGame, &m_MatchState.state, 0, &m_Action ) ) {

      actionProbs[ a_fold ] = probs[ a_fold ];
      p += probs[ a_fold ];
    }

    /* consider call */
    m_Action.type = a_call;
    m_Action.size = 0;
    actionProbs[ a_call ] = probs[ a_call ];
    p += probs[ a_call ];

    /* consider raise */
    if( raiseIsValid( m_pGame, &m_MatchState.state, &min, &max ) ) {

      actionProbs[ a_raise ] = probs[ a_raise ];
      p += probs[ a_raise ];
    }

    /* normalize the probabilities  */
    assert( p > 0.0 );
    for( int a = 0; a < NUM_ACTION_TYPES; ++a ) {
      actionProbs[ a ] /= p;
    }

    /* choose one of the valid actions at random */
    p = genrand_real2( &m_rng );
    int a;
    for(  a = 0; a < NUM_ACTION_TYPES - 1; ++a ) {
      if( p <= actionProbs[ a ] ) {
        break;
      }
      p -= actionProbs[ a ];
    }
    m_Action.type = (enum ActionType)a;
    if( a == a_raise ) {
      m_Action.size = min + genrand_int32( &m_rng ) % ( max - min + 1 );
    }

    return &m_Action;
}

void ParentPlayer:: DoAction(Action* action){
    
    assert( isValidAction( m_pGame, &m_MatchState.state, 1, action ) );
    int r = printAction( m_pGame, action, MAX_LINE_LEN - m_LineLen - 2,
		     &m_Line[ m_LineLen ] );
    
    if( r < 0 ) {
      fprintf( stderr, "ERROR: line too long after printing action\n" );
      exit( EXIT_FAILURE );
    }
    
    m_LineLen += r;
    m_Line[ m_LineLen ] = '\r';
    ++m_LineLen;
    m_Line[ m_LineLen ] = '\n';
    ++m_LineLen;
    
    if( fwrite( m_Line, 1, m_LineLen, m_pToServer ) != m_LineLen ) { 
        fprintf( stderr, "ERROR: could not get send response to server\n" );
        exit( EXIT_FAILURE );
    }
    fflush( m_pToServer );
}


void ParentPlayer::Connect2Dealer(char* ServerIP, char* PortNo){
    
    uint16_t port;
    int sock;
    
    if( sscanf( PortNo, "%" SCNu16, &port ) < 1 ) {
        fprintf( stderr, "ERROR: invalid port %s\n", PortNo );
        exit( EXIT_FAILURE );
    }
  
    sock = connectTo( ServerIP, port );
  
    if( sock < 0 ) {
        exit( EXIT_FAILURE );
    }
    m_pToServer = fdopen( sock, "w" );
    m_pFromServer = fdopen( sock, "r" );
    if( m_pToServer == NULL || m_pFromServer == NULL ) {
        fprintf( stderr, "ERROR: could not get socket streams\n" );
        exit( EXIT_FAILURE );
    }

}

void ParentPlayer::SendVersionStr2Dealer(){

 /* send version string to dealer */
  if( fprintf( m_pToServer, "VERSION:%" PRIu32 ".%" PRIu32 ".%" PRIu32 "\n",
	       VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION ) != 14 ) 
  {
    fprintf( stderr, "ERROR: could not get send version to server\n" );
    exit( EXIT_FAILURE );
  }
  fflush( m_pToServer );
  
}

bool ParentPlayer::ParseLine2MatchState(){
    
        /* ignore comments */
    if( m_Line[ 0 ] == '#' || m_Line[ 0 ] == ';' ) {
      return false;
    }

    m_LineLen = readMatchState( m_Line, m_pGame, &m_MatchState );
    if( m_LineLen < 0 ) {

      fprintf( stderr, "ERROR: could not read state %s", m_Line );
      exit( EXIT_FAILURE );
    }

    if( stateFinished( &m_MatchState.state ) ) {
      /* ignore the game over message */
      return false;
    }

    if( currentPlayer( m_pGame, &m_MatchState.state ) != m_MatchState.viewingPlayer ) {
      /* we're not acting */
      return false;
    }

    /* add a colon (guaranteed to fit because we read a new-m_Line in fgets) */
    m_Line[ m_LineLen ] = ':';
    ++m_LineLen;
    
    return true;

}


void ParentPlayer::PlayGame(){
    
  while( fgets( m_Line, MAX_LINE_LEN, m_pFromServer ) ) {

    if (ParseLine2MatchState()){
        Action* action = ComputeNextAction();
        /* do the action! */
        DoAction(action);    
    }
  }
    
}