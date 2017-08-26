/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Dealer.h
 * Author: Mehrdad
 *
 * Created on August 25, 2017, 10:23 AM
 */


#ifndef DEALER_H
#define DEALER_H

#include "../project_acpc_server/game.h"

#define DEFAULT_MAX_INVALID_ACTIONS UINT32_MAX
#define DEFAULT_MAX_RESPONSE_MICROS 600000000
#define DEFAULT_MAX_USED_HAND_MICROS 600000000
#define DEFAULT_MAX_USED_PER_HAND_MICROS 7000000


typedef struct {
  uint32_t maxInvalidActions;
  uint64_t maxResponseMicros;
  uint64_t maxUsedHandMicros;
  uint64_t maxUsedMatchMicros;

  uint32_t numInvalidActions[ MAX_PLAYERS ];
  uint64_t usedHandMicros[ MAX_PLAYERS ];
  uint64_t usedMatchMicros[ MAX_PLAYERS ];
} ErrorInfo;


class Dealer {
public:
    Dealer();
    Dealer(const Dealer& orig);
    virtual ~Dealer();
    
    static void printUsage( FILE *file, int verbose );
    static void initErrorInfo( const uint32_t maxInvalidActions, const uint64_t maxResponseMicros,
                               const uint64_t maxUsedHandMicros, const uint64_t maxUsedMatchMicros, ErrorInfo *info );
    static int checkErrorInvalidAction( const uint8_t seat, ErrorInfo *info );
    static int checkErrorTimes( const uint8_t seat, const struct timeval *sendTime,
                                const struct timeval *recvTime, ErrorInfo *info );
    static int checkErrorNewHand( const Game *game, ErrorInfo *info );
    static uint8_t seatToPlayer( const Game *game, const uint8_t player0Seat, const uint8_t seat );
    static uint8_t playerToSeat( const Game *game, const uint8_t player0Seat, const uint8_t player );
    static int sendPlayerMessage( const Game *game, const MatchState *state,
                                  const int quiet, const uint8_t seat,
                                  const int seatFD, struct timeval *sendTime );
    static int readPlayerResponse( const Game *game, const MatchState *state, const int quiet,
                                   const uint8_t seat, const struct timeval *sendTime, ErrorInfo *errorInfo,
                                   ReadBuf *readBuf, Action *action, struct timeval *recvTime );
    static int setUpNewHand( const Game *game, const uint8_t fixedSeats, uint32_t *handId, uint8_t *player0Seat,
                             rng_state_t *rng, ErrorInfo *errorInfo, State *state );
    static int processTransactionFile( const Game *game, const int fixedSeats, uint32_t *handId, uint8_t *player0Seat,
                                       rng_state_t *rng, ErrorInfo *errorInfo, double totalValue[ MAX_PLAYERS ],
                                       MatchState *state, FILE *file );
    static int logTransaction( const Game *game, const State *state,
                               const Action *action,
                               const struct timeval *sendTime,
                               const struct timeval *recvTime,
                               FILE *file );
    static int checkVersion( const uint8_t seat, ReadBuf *readBuf );
    static int addToLogFile( const Game *game, const State *state,
			 const double value[ MAX_PLAYERS ],
			 const uint8_t player0Seat,
			 char *seatName[ MAX_PLAYERS ], FILE *logFile );
    static int printInitialMessage( const char *matchName, const char *gameName,
				const uint32_t numHands, const uint32_t seed,
				const ErrorInfo *info, FILE *logFile );
    static int printFinalMessage( const Game *game, char *seatName[ MAX_PLAYERS ],
			      const double totalValue[ MAX_PLAYERS ], FILE *logFile );
    static int gameLoop( const Game *game, char *seatName[ MAX_PLAYERS ],
		     const uint32_t numHands, const int quiet,
		     const int fixedSeats, rng_state_t *rng,
		     ErrorInfo *errorInfo, const int seatFD[ MAX_PLAYERS ],
		     ReadBuf *readBuf[ MAX_PLAYERS ],
		     FILE *logFile, FILE *transactionFile );

    int run( int argc, char **argv );

private:
    
    Game* m_pGame;
    MatchState m_MatchState;
    Action m_Action;
    char m_Line[ MAX_LINE_LEN ];
    int m_LineLen;
};

#endif /* DEALER_H */

