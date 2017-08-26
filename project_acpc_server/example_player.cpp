/*
Copyright (C) 2011 by the Computer Poker Research Group, University of Alberta
*/

#include "ParentPlayer.h"

int main( int argc, char **argv )
{
  /* we make some assumptions about the actions - check them here */
  assert( NUM_ACTION_TYPES == 3 );

  if( argc < 4 ) {
    fprintf( stderr, "usage: player game server port\n" );
    exit( EXIT_FAILURE );
  }

  ParentPlayer examplePlayer;
  
  examplePlayer.ReadGame(argv[1]);
  examplePlayer.Connect2Dealer(argv[ 2 ], argv[ 3 ]);
  examplePlayer.SendVersionStr2Dealer();
  examplePlayer.PlayGame();

  return EXIT_SUCCESS;
}
