cmake_minimum_required(VERSION 3.28)
project(multipleChess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(chess chess/Chess.cpp
        chess/ChessBoard.cpp
        chess/ChessPiece.cpp
        chess/Location.cpp
        new_chess/chessPiece/Knight.cpp
        new_chess/chessPiece/Knight.h)

add_library(new_chess new_chess/ChessBoard.cpp new_chess/ChessBoard.h
        new_chess/ChessPiece.h
        new_chess/Location.cpp
        new_chess/Location.h)

add_library(client SHARED network/client.cpp
        network/common.h)
add_library(server SHARED network/server.cpp
        network/common.h)

add_library(
        network
        new_network/common.h
        new_network/bytes/ByteConverter.cpp
        new_network/bytes/ByteConverter.h
        new_network/bytes/ByteData.cpp
        new_network/bytes/ByteData.h
        new_network/bytes/ByteConverterExtend.h
        new_network/DataInterface.h
        new_network/message.h
        new_network/utils/UUID.cpp
        new_network/utils/UUID.h
)

add_library(new_client SHARED new_network/client/Client.cpp new_network/client/Client.h)
add_library(new_server SHARED new_network/server/Server.cpp new_network/server/Server.h
        new_network/server/ServerInterface.h
        new_network/server/KqueueServer.cpp
        new_network/server/KqueueServer.h
        new_network/server/ClientData.cpp
        new_network/server/ClientData.h)

#add_executable(multipleChess main.cpp)
#add_executable(clientRun network/client.cpp)
#add_executable(serverRun network/server.cpp)
# add_executable(testServer network/kqueue_test.cpp)


#target_link_libraries(clientRun chess)
#target_link_libraries(serverRun chess)
#target_link_libraries(multipleChess chess)
target_link_libraries(client chess)
target_link_libraries(server chess)
target_link_libraries(new_client network)
