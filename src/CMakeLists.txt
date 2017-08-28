
include_directories("${PROJECT_SOURCE_DIR}/project_acpc_server")

add_library(ProbEvalLib Hand_Tables.cpp HandEval.cpp HandIter.cpp ProbEval.cpp)

# add the executable
add_executable(PokerBot Driver.cpp Hand_Tables.cpp HandEval.cpp HandIter.cpp ProbEval.cpp TestCases.cpp)
                #Dealer.cpp PlayerParent.cpp)

link_directories(${PokerBot_SOURCE_DIR}/project_acpc_server)

target_link_libraries(PokerBot AcpcLib)
