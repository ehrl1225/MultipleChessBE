cmake_minimum_required(VERSION 3.28)
project(multipleChess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(chess chess/Chess.cpp
        chess/ChessBoard.cpp
        chess/ChessPiece.cpp
        chess/Location.cpp)

add_library(client SHARED network/client.cpp
        network/common.h)
add_library(server SHARED network/server.cpp
        network/common.h)

#add_executable(multipleChess main.cpp)
#add_executable(clientRun network/client.cpp)
#add_executable(serverRun network/server.cpp)
# add_executable(testServer network/kqueue_test.cpp)


#target_link_libraries(clientRun chess)
#target_link_libraries(serverRun chess)
#target_link_libraries(multipleChess chess)
target_link_libraries(client chess)
target_link_libraries(server chess)
