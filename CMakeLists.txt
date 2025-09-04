cmake_minimum_required(VERSION 3.28)
project(multipleChess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        argon2
        GIT_REPOSITORY  https://github.com/P-H-C/phc-winner-argon2.git
        GIT_TAG         20190702
)

FetchContent_MakeAvailable(argon2)


add_library(chess chess/Chess.cpp
        chess/ChessBoard.cpp
        chess/ChessPiece.cpp
        chess/Location.cpp)

add_library(new_chess
        new_chess/ChessBoard.cpp
        new_chess/ChessBoard.h
        new_chess/ChessPiece.h
        new_chess/Location.cpp
        new_chess/Location.h
        new_chess/chessPiece/Knight.cpp
        new_chess/chessPiece/Knight.h
        new_chess/chessPiece/Queen.cpp
        new_chess/chessPiece/Queen.h
        new_chess/chessPiece/Rook.cpp
        new_chess/chessPiece/Rook.h
        new_chess/chessPiece/Bishop.cpp
        new_chess/chessPiece/Bishop.h
        new_chess/chessPiece/Pawn.cpp
        new_chess/chessPiece/Pawn.h
)

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
        new_network/utils/PasswordHasher.cpp
        new_network/utils/PasswordHasher.h
)

add_library(new_client SHARED new_network/client/Client.cpp new_network/client/Client.h)
add_library(new_server SHARED new_network/server/Server.cpp new_network/server/Server.h
        new_network/server/ServerInterface.h
        new_network/server/KqueueServer.cpp
        new_network/server/KqueueServer.h
        new_network/server/ClientData.cpp
        new_network/server/ClientData.h
        new_network/server/Database.cpp
        new_network/server/Database.h
        new_network/server/RequestHandler.cpp
        new_network/server/RequestHandler.h)

#add_executable(multipleChess main.cpp)
#add_executable(clientRun network/client.cpp)
#add_executable(serverRun network/server.cpp)
# add_executable(testServer network/kqueue_test.cpp)

target_include_directories(network PUBLIC ${argon2_SOURCE_DIR}/include)
target_link_libraries(network PUBLIC argon2)
#target_link_libraries(clientRun chess)
#target_link_libraries(serverRun chess)
#target_link_libraries(multipleChess chess)
target_link_libraries(client chess)
target_link_libraries(server chess)
target_link_libraries(new_client network)
