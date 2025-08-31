//
// Created by 정주신 on 24. 5. 7.
//

#include "Client.h"
char *serverIP = (char *)"127.0.0.1";
Client client = Client();

/**
 * 접속을 합니다.
 */
int Client::init() {
    int retval;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        err_display("socket()");
        return -1;
    };
    printf("socket is ready\n");

    sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, serverIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) {
        err_display("connect()");
        return -2;
    }
    printf("connected to server\n");
    pthread_create(&listen_tid, NULL, recvProccess, NULL);
}

/**
 *
 * @param arg
 * @return
 */
void *sendProccess(void *arg) {
    int retval;

    while (true) {

    }
    return nullptr;
}

/**
 * 
 * @param arg 
 * @return 
 */
void *recvProccess(void *arg) {
    int retval;
    int run = true;
    while (run) {
        MyMessage msg = {};
        retval = recv(client.sock, &msg, sizeof(msg), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        if (retval == 0) {
            printf("접속 끊김\n");
            break;
        }

        switch(msg.node) {
            case 'R': {
                ChessPiece chess_piece = client.chess_board.getChessPiece(msg.chess_piece_id);
                chess_piece.move(msg.x, msg.y);
                client.addMsg(msg);
                break;
            }

            case 'C': {
                std::string chat;
                std::string nickname(msg.nickname);
                std::string message(msg.msg);
                chat.append("[");
                chat.append(nickname);
                chat.append("]:");
                chat.append(message);
                client.addChat(chat);
                printf("%s\n", chat.c_str());
                break;
            }

            case 'w': {
                client.addWinCount();
                client.addMsg(msg);
                printf("승리했습니다.\n");
                break;
            }

            case 'l': {
                client.addLoseCount();
                client.addMsg(msg);
                printf("패배했습니다.\n");
                break;
            }

            case 'd': {
                client.addDrawCount();
                client.addMsg(msg);
                printf("비겼습니다.\n");
                break;
            }

            case 'p': {
                client.addMsg(msg);
                printf("체스말을 선택해야합니다.\n");
                break;
            }

            case 'P': {
                client.addMsg(msg);
                printf("위치를 선택해야합니다.\n");
                break;
            }

            case 'I': {
                client.addMsg(msg);
                printf("닉네임 중복 여부 판단\n");
                break;
            }

            case 'm': {
                client.addMsg(msg);
                if (msg.x==0) {
                    printf("체스 말 투표 결과 받았습니다.\n");
                }else {
                    printf("체스 위치 투표 결과 받았습니다.\n");
                }
                break;
            }
            case 'q': {
                printf("서버의 명령으로 종료합니다.\n");
                client.addMsg(msg);
                break;
            }
            case 'D': {
                client.addMsg(msg);
                break;
            }

            case 'b': {
                client.addMsg(msg);
                break;
            }
            case 'z': {
                client.addMsg(msg);
                break;
            }
            case 'x': {
                client.addMsg(msg);
                break;
            }
            case 't': {
                client.addMsg(msg);
                break;
            }

            default:
                printf("wrong response\n");
        }
    }
    if (retval == 0) {
        MyMessage message = {};
        message.node = 'q';
        client.addMsg(message);
    }
    printf("종료했습니다.\n");
    return nullptr;
}

/**
 * 
 * @param nickname 
 */
void Client::setNickName(char* nickname) {
    MyMessage msg = {};
    msg.node = 'n';
    memcpy(msg.nickname, nickname, strlen(nickname));
    int retval = send(sock, &msg, sizeof(msg), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return;
    }
}

/**
 * 
 * @param chess_piece_id 
 */
void Client::requestChessPiece(int chess_piece_id) {
    MyMessage msg = {};
    msg.node='r';
    msg.chess_piece_id = chess_piece_id;
    int retval = send(sock, &msg, sizeof(msg), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return;
    }
}

/**
 * 
 * @param str 
 */
void Client::addChat(std::string &str) {
    chats.push_back(str);
}

void Client::wait() {

}

void Client::sendSetting(Color color, char *nickname) {

}
void Client::sendServer(MyMessage& msg) {
    int retval;
    retval = send(sock, &msg, sizeof(msg), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
    }
}

void Client::setBlack() {
    color = Color::Black;
    MyMessage msg = {};
    msg.node = 'o';
    msg.color = color;
    sendServer(msg);
}

void Client::setWhite() {
    color = Color::White;
    MyMessage msg = {};
    msg.node = 'o';
    msg.color = color;
    sendServer(msg);
}

void Client::addDrawCount() {
    draw_count ++;
}

void Client::addLoseCount() {
    lose_count++;
}

void Client::addWinCount() {
    win_count++;
}

void Client::setDrawCount(int score) {
    draw_count= score;
}

void Client::setLoseCount(int score) {
    lose_count = score;
}

void Client::setWinCount(int score) {
    win_count = score;
}

MyMessage Client::readMsg() {
    if (!messages.empty()) {
        MyMessage m = messages[0];
        messages.erase(messages.begin());
        return m;
    }
}

char *Client::readChat() {
    char chat[BUFSIZE+NICKNAMESIZE+4];
    std::string chat_s = chats[0];
    chats.erase(chats.begin());
    strcpy(chat, chat_s.c_str());
    return chat;
}

void Client::addMsg(MyMessage &msg) {
    messages.push_back(msg);
}

void Client::sendChat(char *chat) {
    MyMessage msg = {};
    msg.node = 'c';
    strcpy(msg.msg, chat);
    sendServer(msg);
}

int Client::getChatCount() {
    return (int)chats.size();
}

int Client::getMsgCount() {
    return messages.size();
}

int init() {
    return client.init();
}

void setNickName(char *nickname) {
    client.setNickName(nickname);
}

void setWhite() {
    client.setWhite();
}

void setBlack() {
    client.setBlack();
}

void sendChat(char *chat) {
    client.sendChat(chat);
}

int getChatCount() {
    return client.getChatCount();
}

char *readChat() {
    return client.readChat();
}

int getMsgCount() {
    return client.getMsgCount();
}

MyMessage readMsg() {
    return client.readMsg();
}


MyMessage testStruct() {
    MyMessage msg = {};
    return msg;
}

void Client::votePiece(int index) {
    MyMessage msg = {};
    msg.node= 'v';
    msg.chess_piece_id = index;
    sendServer(msg);
}

void Client::voteLocation(int x, int y) {
    MyMessage msg = {};
    msg.node = 'V';
    msg.x = x;
    msg.y=y;
    sendServer(msg);
}

void votePiece(int index) {
    client.votePiece(index);
}

void voteLocation(int x, int y) {
    client.voteLocation(x,y);
}

void Client::setViewer() {
    MyMessage msg= {};
    msg.node = 'o';
    msg.color = Color::NonColor;
    sendServer(msg);
}

void setViewer() {
    client.setViewer();
}

bool Client::sendHasNickName(char *nickname) {
    MyMessage msg = {};
    msg.node = 'i';
    strcpy(msg.nickname, nickname);
    sendServer(msg);
}

void Client::closeSocket() {
    close(sock);
}

void closeSocket() {
    client.closeSocket();
}

void Client::sendLoseRequest() {
    MyMessage message = {};
    message.node = 'L';
    sendServer(message);
}

void Client::sendDrawRequest() {
    MyMessage message = {};
    message.node = 'W';
    sendServer(message);
}

void sendDrawRequest() {
    client.sendDrawRequest();
}

void sendLoseRequest() {
    client.sendLoseRequest();
}

void Client::votePromoteClass(int class_index) {
    MyMessage message = {};
    message.node = 'T';
    message.chess_piece_id = class_index;
    sendServer(message);
}

void votePromoteClass(int class_index) {
    client.votePromoteClass(class_index);
}



int main(int argc, char *argv[]) {
    client.init();
}

