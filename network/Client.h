//
// Created by 정주신 on 24. 5. 7.
//

#ifndef MULTIPLECHESS_CLIENT_H
#define MULTIPLECHESS_CLIENT_H

#include "common.h"
#include "../chess/Chess.h"
#include <string>
#define SERVERPORT 9000
#define BUFSIZE 512



class Client {

private:
    Color color = Color::White;
    std::vector<std::string> chats;
    int chat_current_index= 0;
    int win_count = 0;
    int draw_count = 0;
    int lose_count = 0;
    std::vector<MyMessage> messages;
    pthread_t listen_tid;


public:
    SOCKET sock;
    ChessBoard chess_board;
    // MyMessage msg;
    int init();
    void setWhite();
    void setBlack();
    void setViewer();
    void sendServer(MyMessage &msg);
    void requestChessPiece(int chess_piece_id);
    void setNickName(char *nickname);
    bool sendHasNickName(char *nickname);
    void addChat(std::string &str);
    void wait();
    void sendSetting(Color color, char *nickname);
    void addWinCount();
    void addDrawCount();
    void addLoseCount();
    void setWinCount(int score);
    void setDrawCount(int score);
    void setLoseCount(int score);
    MyMessage readMsg();
    char* readChat();
    void addMsg(MyMessage &msg);
    void sendChat(char *chat);
    int getChatCount();
    int getMsgCount();
    void votePiece(int index);
    void voteLocation(int x, int y);
    void closeSocket();
    void sendLoseRequest();
    void sendDrawRequest();
    void votePromoteClass(int class_index);
};

void *sendProccess(void* arg);
void *recvProccess(void* arg);


extern "C"{
    int init();
    void setNickName(char *nickname);
    void setWhite();
    void setBlack();
    void setViewer();
    void sendChat(char* chat);
    int getChatCount();
    char* readChat();
    int getMsgCount();
    MyMessage readMsg();
    MyMessage testStruct();
    void votePiece(int index);
    void voteLocation(int x, int y);
    void closeSocket();
    void sendLoseRequest();
    void sendDrawRequest();
    void votePromoteClass(int class_index);
}

#endif //MULTIPLECHESS_CLIENT_H
