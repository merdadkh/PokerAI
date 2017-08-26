
# add the executable

add_executable(all_in_expectation all_in_expectation.c game.c net.c rng.c net.c)

add_executable(bm_server bm_server.c game.c rng.c net.c bm_server.c game.c rng.c)

add_executable(bm_widget bm_widget.c net.c)

add_executable(bm_run_matches bm_run_matches.c net.c)

add_executable(dealer game.c game.h evalHandTables rng.c rng.h dealer.c net.c)


add_executable(example_player game.c evalHandTables rng.c example_player.cpp net.c ParentPlayer.cpp)

add_executable(player1 game.c evalHandTables rng.c player1.cpp net.c ParentPlayer.cpp CPlayer.cpp)