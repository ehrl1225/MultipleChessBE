//
// Created by 정주신 on 24. 5. 7.
//
#include "Server.h"
#include <iostream>

Server server = Server();

void Server::init(){
    int retval;
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (retval == INVALID_SOCKET) err_quit("socket()");
    int optval = 1;

    retval = setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR,&optval, sizeof(optval));
    if (retval == SOCKET_ERROR) err_quit("setsockopt()");
    printf("socket is ready\n");
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);
}

void Server::start() {
    int retval;
    // std::cout << "server is ready" << std::endl;
    retval = bind(listen_sock, (sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");
    printf("bind done\n");
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");
    printf("listening now\n");
    pthread_create(&listen_tid, NULL, listenClient, NULL);
}

void Server::stop() {
    closeAllSock();
}


void Server::vote(SOCKET client_sock, MoveData &mv) {
    int vote_index = getVoteIndex(client_sock);
    if (vote_index==-1) {
        Vote vote = {};
        vote.client_sock = client_sock;
        vote.chess_piece_index = mv.chess_piece_index;
        vote.x = mv.to_x;
        vote.y = mv.to_y;
        votes.push_back(vote);
    }else {
        Vote& vote = votes[vote_index];
        vote.chess_piece_index = mv.chess_piece_index;
        vote.x = mv.to_x;
        vote.y = mv.to_y;

    }
}

MoveData Server::mostVoted() {
    std::vector<MoveData> move_datas;
    std::vector<int> voteCount;
    if (votes.empty()) {
        MoveData move_data = {};
        move_data.chess_piece_index = -1;
        move_data.to_x = 0;
        move_data.to_y = 0;
        return move_data;
    }
    if (votes.size()==1) {
        Vote vote = votes[0];
        MoveData move_data = {};
        move_data.chess_piece_index =vote.chess_piece_index;
        move_data.to_x = vote.x;
        move_data.to_y = vote.y;
        return move_data;
    }

    if (votes.size() == 2) {
        Vote vote = votes[0];
        MoveData move_data = {};
        move_data.chess_piece_index =vote.chess_piece_index;
        move_data.to_x = vote.x;
        move_data.to_y = vote.y;
        return move_data;
    }

    for (Vote vote : votes) {
        bool found = false;
        for (int i = 0; i<move_datas.size();i++) {
            MoveData move_data = move_datas[i];
            if (move_data.chess_piece_index != vote.chess_piece_index) {
                continue;
            }
            if (move_data.to_x != vote.x) {
                continue;
            }
            if (move_data.to_y == vote.y) {
                found = true;
                voteCount[i]++;
                break;
            }
        }
        if (!found) {
            MoveData move_data = {};
            move_data.chess_piece_index = vote.chess_piece_index;
            move_data.to_x = vote.x;
            move_data.to_y = vote.y;
            move_datas.push_back(move_data);
            voteCount.push_back(1);
        }
    }

    int max_index = 0;
    for (int i = 1;i<voteCount.size();i++) {
        if (voteCount[max_index]<voteCount[i]) {
            max_index = i;
        }
    }
    return move_datas[max_index];
}


void Server::sendMessageColor(Color color,MyMessage& msg) {
    int retval;
    if (color == Color::White) {
        send_mutex.lock();
        for (int client:white_team) {
            send(client, &msg, sizeof(msg), 0);
        }
        send_mutex.unlock();
        return;
    }
    if (color == Color::Black) {
        send_mutex.lock();
        for (int client:black_team) {
            send(client, &msg, sizeof(msg), 0);
        }
        send_mutex.unlock();
        return;
    }
}

void Server::sendMessageViwers(MyMessage &msg) {
    int retval;
    send_mutex.lock();
    for (SOCKET client:viewers) {
        send(client, &msg, sizeof(msg), 0);
    }
    send_mutex.unlock();
    return;
}

void Server::sendMessageAll(MyMessage &msg) {
    int retval;
    sendMessageColor(Color::White, msg);
    sendMessageColor(Color::Black, msg);
    sendMessageViwers(msg);
}


void Server::addClient(SOCKET sock) {
    clients.push_back(sock);
}

int Server::get_client_index(SOCKET sock) {
    int index = 0;
    for (SOCKET client : clients) {
        if (client == sock) {
            return index;
        }
        index++;
    }
    return -1;
}

Chess Server::getChess() {
    return chess_;
}

void *listenClient(void *arg) {

    SOCKET client_sock;
    sockaddr_in clientaddr;
    socklen_t addrlen;


    while (server.run_server) {
        pthread_t recv_tid;
        addrlen = sizeof(client_sock);
        client_sock = accept(server.listen_sock, (sockaddr *)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }
        printf("%d client connected\n",server.getClientCount());
        server.addClient(client_sock);
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        pthread_create(&recv_tid, NULL, receiveProccess, (void *) (long long)client_sock);
    }
    server.closeAllSock();
    return nullptr;
}

void *receiveProccess(void* arg) {
    int retval;
    SOCKET recv_sock = (SOCKET)(long long)arg;
    sockaddr_in client_addr;
    char addr[INET_ADDRSTRLEN];
    socklen_t addrlen;
    MyMessage msg;
    addrlen = sizeof(client_addr);
    getpeername(recv_sock, (sockaddr *)&client_addr, &addrlen);
    inet_ntop(AF_INET, &client_addr.sin_addr, addr, sizeof(addr));

    while (server.run_server) {
        retval = recv(recv_sock, &msg, sizeof(msg), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        if (retval == 0) {
            printf("%d 클라이언트 접속 끊김\n", recv_sock);
            break;
        }
        Chess chess = server.getChess();
        switch (msg.node) {
            case 'r': {
                MyMessage response{};
                ChessPiece chp = chess.getChessBoard().getChessPiece(msg.chess_piece_id);
                message_init(response);
                response.node = 'R';
                response.chess_piece_id = msg.chess_piece_id;
                response.x = chp.getLocation().get_x();
                response.y = chp.getLocation().get_y();
                server.send_mutex.lock();
                retval = send(recv_sock, &response, sizeof(response), 0);
                if (retval == SOCKET_ERROR) {
                    err_display("send()");
                }
                server.send_mutex.unlock();
                server.addMessage(msg);
                break;

            }
            case 'n': {
                MyMessage response = {};
                response.node = 'I';
                bool hasNickname = server.hasNickname(msg.nickname);
                if (hasNickname==true) {
                    response.chess_piece_id=1;
                    printf("같은 닉네임이 존재합니다.\n");
                }else {
                    response.chess_piece_id=0;
                    printf("같은 닉네임이 존재하지 않습니다.\n");
                    char nickname[BUFSIZE+1];
                    memcpy(nickname, msg.nickname, BUFSIZE+1);
                    server.setNickname(recv_sock ,nickname);
                    int index = server.get_client_index(recv_sock);
                    printf("%d 클라이언트의 닉네임을 %s로 설정했습니다.\n", index, nickname);
                    msg.client_sock = recv_sock;
                    server.addMessage(msg);
                }
                server.send_mutex.lock();
                retval = send(recv_sock, &response, sizeof(response), 0);
                if (retval == SOCKET_ERROR) {
                    err_display("send()");
                }
                server.send_mutex.unlock();
                break;
            }
            case 'v': {
                MoveData mv = {msg.chess_piece_id, 0,0};
                server.vote(recv_sock, mv);
                char *nickname = server.getClientNickName(recv_sock);
                printf("%s가 %d번 체스 말에 투표했습니다.\n", nickname, msg.chess_piece_id);
                msg.client_sock = recv_sock;
                server.addMessage(msg);
                Color color = getClientColor(recv_sock);
                MyMessage response = {};
                response.node = 'm';
                response.chess_piece_id = msg.chess_piece_id;
                response.x = 0;
                response.y = 0;
                response.client_sock = recv_sock;
                server.sendMessageColor(color ,response);
                break;
            }
            case 'V': {
                MoveData mv = {msg.chess_piece_id, msg.x, msg.y};
                server.vote(recv_sock, mv);
                char*nickname = server.getClientNickName(recv_sock);
                printf("%s가 x=%d y=%d에 투표했습니다.,\n",nickname, msg.x, msg.y);
                msg.client_sock = recv_sock;
                Color color = server.getClientColor(recv_sock);
                MyMessage response = {};
                response.node = 'm';
                response.chess_piece_id = -1;
                response.x = msg.x;
                response.y = msg.y;
                response.client_sock = recv_sock;
                server.addMessage(msg);
                server.sendMessageColor(color,response);
                break;
            }
            case 'c': {
                msg.node = 'C';
                char * nickname = server.getClientNickName(recv_sock);
                strcpy(msg.nickname, nickname);
                server.sendMessageAll(msg);
                printf("[%s]:%s\n",nickname,msg.msg);
                server.addChat(nickname, msg.msg);
                break;
            }
            case 'o' :{
                if (msg.color == Color::White) {
                    server.white_team.push_back(recv_sock);
                }else if (msg.color == Color::Black) {
                    server.black_team.push_back(recv_sock);
                }else if (msg.color == Color::NonColor) {
                    server.viewers.push_back(recv_sock);
                }
                msg.client_sock = recv_sock;
                server.addMessage(msg);
                server.initClient(recv_sock);
                break;
            }

            case 'W': {
                server.addDrawVote(recv_sock);
                server.addMessage(msg);
                printf("%d가 무승부에 투표했습니다.\n",recv_sock);
                break;
            }
            case 'L': {
                server.addLoseVote(recv_sock);
                server.addMessage(msg);
                Color color = getClientColor(recv_sock);
                if (color == Color::White) {
                    printf("%d가 백팀 패배에 투표했습니다.\n", recv_sock);
                }else {
                    printf("%d가 흑팀 패배에 투표했습니다.\n", recv_sock);
                }
                break;
            }

            case 'T': {
                msg.client_sock = recv_sock;
                server.addMessage(msg);
            }
            default:
                printf("%c request\n", msg.node);
                printf("%s msg\n", msg.msg);
                printf("wrong request\n");
        }
    }
    {
        MyMessage message = {};
        message.node = 'Q';
        message.client_sock = recv_sock;
        server.addMessage(message);
    }
    printf("%d client closed\n", server.get_client_index(recv_sock));
    server.close_client_socket(recv_sock);
    return nullptr;
}

void Server::setNickname(SOCKET client_sock, char *name) {
    int index = get_client_index(client_sock);
    nicknames.resize(clients.size());
    nicknames[index] = name;
}



void *sendProccess(void* arg) {
    while (server.run_server) {
        if (server.white_team.empty()&&server.black_team.empty()) {
            continue;
        }

        MyMessage msg = {};
        msg.node = 'p';
        msg.color = Color::White;
        server.getChess().setTurn(Color::White);
        int mostVoteIndex = 0;
        Location mostVoteLocation = Location();

        server.sendMessageColor(Color::White, msg);
        for (int i = 0 ; i<SelectChessPieceTime; i++) {
            if (!server.allVoted(Color::White)) {
                sleep(1);
            }
        }
        if (!server.isVoteEmpty()) {
            mostVoteIndex = server.mostVotedChessIndex();

            msg.node = 'P';
            msg.color = Color::White;
            server.sendMessageColor(Color::White, msg);
            server.resetVote();
            for (int i = 0 ; i<SelectLocationTime; i++) {
                if (!server.allVoted(Color::White)) {
                    sleep(1);
                }
            }
            if (!server.isVoteEmpty()) {

                mostVoteLocation = server.mostVotedLocation();

                msg.node = 'R';
                msg.chess_piece_id = mostVoteIndex;
                msg.x = mostVoteLocation.get_x();
                msg.y = mostVoteLocation.get_y();
                server.sendMessageColor(Color::White, msg);
            }
        }


        msg.node = 'p';
        msg.color = Color::Black;
        server.sendMessageColor(Color::Black, msg);
        server.resetVote();
        server.getChess().setTurn(Color::Black);
        for (int i = 0 ; i<SelectChessPieceTime; i++) {
            if (!server.allVoted(Color::Black)) {
                sleep(1);
            }
        }
        if (!server.isVoteEmpty()) {

            mostVoteIndex = server.mostVotedChessIndex();

            msg.node = 'P';
            msg.color = Color::Black;
            server.sendMessageColor(Color::Black, msg);
            server.resetVote();
            for (int i = 0 ; i<SelectLocationTime; i++) {
                if (!server.allVoted(Color::Black)) {
                    sleep(1);
                }
            }
            if (!server.isVoteEmpty()) {
                mostVoteLocation = server.mostVotedLocation();
                msg.node = 'R';
                msg.chess_piece_id = mostVoteIndex;
                msg.x = mostVoteLocation.get_x();
                msg.y = mostVoteLocation.get_y();
                server.sendMessageColor(Color::Black, msg);
            }

        }

    }
    return nullptr;
}

void Server::resetVote() {
    votes.clear();
}

int Server::mostVotedChessIndex() {
    return mostVoted().chess_piece_index;
}

Location &Server::mostVotedLocation() {
    MoveData md = mostVoted();
    Location loc = {md.to_x, md.to_y};
    return loc;
}

bool Server::isInTeam(Color color, int client_index) {
    if (color == Color::White) {
        for (int team_index:white_team) {
            if (team_index == client_index) {
                return true;
            }
        }
        return false;
    }
    if (color == Color::Black) {
        for (int team_index:black_team) {
            if (team_index == client_index) {
                return true;
            }
        }
        return false;
    }
}

bool Server::allVoted(Color color) {
    int vote_count = 0;
    // for (int client : voted) {
    //     if (isInTeam(color, client)) {
    //         vote_count++;
    //     }
    // }
    if (color == Color::White) {
        if (white_team.size() == vote_count) {
            return true;
        }else {
            return false;
        }
    }
    if (color == Color::Black) {
        if (black_team.size() == vote_count) {
            return true;
        }else {
            return false;
        }
    }
    return false;
}

void Server::applyVote(MyMessage &msg) {
    ChessPiece chp = chess_.getChessBoard().getChessPiece(msg.chess_piece_id);
    chp.move(msg.x, msg.y);
}

void Server::wait() {
    pthread_join(server.listen_tid, nullptr);
}

void Server::closeAllSock() {
    int index = 0;
    for (SOCKET sock:clients) {
        sendCloseClient(sock);
        close(sock);
        printf("%d client closed\n", index);
        index++;
    }
    shutdown(listen_sock, SHUT_RDWR);
    close(listen_sock);
    printf("listen socket closed\n");
}

void Server::addChat(char *nickname, char *msg) {
    std::string chat;
    chat.append("["),
    chat.append(nickname);
    chat.append("]:");
    chat.append(msg);

    chats.push_back(chat);
}

void Server::sendChat(char *msg) {
    MyMessage m = {};
    char serverName[NICKNAMESIZE+1] = "Server";
    m.node = 'C';
    strcpy(m.nickname, serverName);
    strcpy(m.msg, msg);
    addChat(serverName, m.msg);
    sendMessageAll(m);
}

void Server::sendChatColor(Color color, char *chat) {
    MyMessage msg = {};
    char serverName[NICKNAMESIZE+1] = "Server";
    msg.node = 'C';
    strcpy(msg.nickname, serverName);
    strcpy(msg.msg, chat);
    sendMessageColor(color, msg);
}

int Server::getClientCount() {
    return (int)clients.size();
}

char *Server::getClientNickName(SOCKET client_sock) {
    int index = get_client_index(client_sock);
    return nicknames[index];
}

void Server::close_client_socket(SOCKET client_sock) {
    int index = get_client_index(client_sock);
    for (int i=0; i<white_team.size() ;i++) {
        if (white_team[i]==client_sock) {
            white_team.erase(white_team.begin()+i);
            break;
        }
    }
    for (int i=0;i<black_team.size(); i++) {
        if (black_team[i]==client_sock) {
            black_team.erase(black_team.begin()+i);
            break;
        }
    }
    for (int i =0; i<viewers.size();i++) {
        if (viewers[i] == client_sock) {
            viewers.erase(viewers.begin() + i);
            break;
        }
    }
    nicknames.erase(nicknames.begin()+index);
    clients.erase(clients.begin() + index);
}

SOCKET Server::getClient(int client_index) {
    return clients[client_index];
}

char *Server::readChat() {
    std::string chat = chats[0];
    chats.erase(chats.begin());
    return (char *)chat.c_str();
}

int Server::getChatCount() {
    return (int)chats.size();
}

std::vector<SOCKET> Server::getClients() {
    return clients;
}

void Server::sendPieceSelect(Color color, int timer) {
    MyMessage msg= {};
    msg.node = 'p';
    msg.x = timer;
    sendMessageColor(color, msg);
    if (color == Color::White) {
        printf("백팀 체스말 선택\n");
        voteIndex = 0;
    }else if (color ==Color::Black) {
        printf("흑팀 체스말 선택\n");
        voteIndex = 3;
    }
    sendChatColor(color, (char *)"체스말 선택하세요");

}

void Server::sendLocationSelect(Color color, int timer) {
    printf("%d로 보냄\n",mostVoteChessPieceIndex);
    MyMessage msg = {};
    msg.node = 'P';
    msg.chess_piece_id = mostVoteChessPieceIndex;
    msg.x = timer;
    sendMessageColor(color, msg);
    if (color == Color::White) {
        printf("백팀 위치 선택\n");
        voteIndex = 1;
    }else if (color == Color::Black) {
        printf("흑팀 위치 선택\n");
        voteIndex = 4;
    }
    sendChatColor(color, (char *)"체스 위치 선택하세요");
}

void Server::sendDoneVote(Color color) {
    MyMessage msg = {};
    msg.node = 'D';
    msg.color = color;
    sendMessageColor(color, msg);
    if (color == Color::White) {
        printf("백팀 선택 완료\n");
        voteIndex = 2;
    }else if (color == Color::Black) {
        printf("흑팀 선택 완료\n");
        voteIndex = 5;
    }
    sendChatColor(color, (char *)"투표 종료");
}


int init() {
    server.init();
}

void start() {
    server.start();
}

void stop() {
    server.stop();
}

int getClientCount() {
    return server.getClientCount();
}

char *getClientNickname(int client_sock) {
    return server.getClientNickName(client_sock);
}

void sendWhitePieceSelect(int timer) {
    return server.sendPieceSelect(Color::White, timer);
}

void sendWhiteLocationSelect(int timer) {
    return server.sendLocationSelect(Color::White, timer);
}

void sendBlackPieceSelect(int timer) {
    return server.sendPieceSelect(Color::Black, timer);
}

void sendBlackLocationSelect(int timer) {
    return server.sendLocationSelect(Color::Black, timer);
}

void sendWhiteDoneVote() {
    server.clearVote();
    return server.sendDoneVote(Color::White);
}

void sendBlackDoneVote() {
    server.clearVote();
    return server.sendDoneVote(Color::Black);
}

int *getClients() {
    std::vector<SOCKET> arr = server.getClients();
    return arr.data();
}

std::vector<char *> Server::getClientNicknames() {
    return nicknames;
}

char **getClientNicknames() {
    return server.getClientNicknames().data();
}


void resetVote() {
    server.resetVote();
}


int getVoteCount() {
    return server.getVoteCount();
}

void closeClient(SOCKET sock) {
    server.sendCloseClient(sock);
}

Color Server::getClientColor(SOCKET sock) {
    Color color = Color::NonColor;
    for (SOCKET white_sock : white_team) {
        if (sock == white_sock) {
            color = Color::White;
            break;
        }
    }

    for (SOCKET black_sock :black_team) {
        if (sock == black_sock) {
            color = Color::Black;
            break;
        }
    }
    return color;
}

bool Server::hasNickname(char *nickname) {
    bool found = false;
    for (char* nick: nicknames) {
        if (strcmp(nickname, nick)==0) {
            found = true;
        }
    }
    return found;
}

Color Server::getColor(SOCKET sock) {
    for (SOCKET s:white_team) {
        if (s == sock) {
            return Color::White;
        }
    }
    for (SOCKET s:black_team) {
        if (s==sock) {
            return Color::Black;
        }
    }
    return Color::NonColor;
}



void Server::sendMoveData(MoveData move_data) {
    MyMessage msg = {};
    msg.node = 'R';
    msg.chess_piece_id = move_data.chess_piece_index;
    msg.x = move_data.to_x;
    msg.y = move_data.to_y;
    sendMessageAll(msg);
}

void sendMoveData(MoveData move_data) {
    server.sendMoveData(move_data);
}

void Server::addMessage(MyMessage msg) {
    message_mutex.lock();
    messages.push_back(msg);
    message_mutex.unlock();
}

Board Server::getBoard() {
    Board board = {};
    ChessBoard chess_board = getChess().getChessBoard();
    for (int i =0 ; i<32; i++) {
        ChessPiece chess_piece = chess_board.getChessPiece(i);
        Location location = chess_piece.getLocation();
        board.pieces[i].chess_piece_index = i;
        board.pieces[i].color_num = chess_piece.getColor();
        board.pieces[i].class_num = chess_piece.getChessClass();
        board.pieces[i].x = location.get_x();
        board.pieces[i].y= location.get_y();
    }
    return board;
}

Board getBoard() {
    return server.getBoard();
}

int Server::getMessageCount() {
    return (int) messages.size();
}

int getMessageCount() {
    return server.getMessageCount();
}

int getChatCount() {
    return server.getChatCount();
}

MyMessage Server::readMessage() {
    message_mutex.lock();
    MyMessage message = messages[0];
    messages.erase(messages.begin());
    message_mutex.unlock();
    return message;
}

MyMessage readMessage() {
    return server.readMessage();
}

void Server::sendVoteResult(Color color) {

}

bool Server::hasVote(SOCKET sock) {
    for (Vote vote:votes) {
        if (vote.client_sock == sock) {
            return true;
        }
    }
    return false;
}

int Server::getVoteIndex(SOCKET sock) {
    for (int i =0;i<votes.size();i++) {
        if (votes[i].client_sock==sock) {
            return i;
        }
    }
    return -1;
}



bool Server::isVoteEmpty() {
    return votes.empty();
}

void Server::sendVoteResult() {
    MoveData move_data = mostVoted();
    MyMessage message = {};
    message.node = 'R';
    message.chess_piece_id = move_data.chess_piece_index;
    message.x = move_data.to_x;
    message.y = move_data.to_y;
    sendMessageAll(message);
}

int Server::getVoteCount() {
    return (int)votes.size();
}

void Server::setMostVotedPieceIndex() {
    MoveData most_vote = mostVoted();
    if (most_vote.chess_piece_index == -1) {
        printf("-1번이 선택되었습니다. 에러입니다.\n");
        return;
    }
    printf("%d번 째 체스말이 선택되었습니다.\n", most_vote.chess_piece_index);
    mostVoteChessPieceIndex = most_vote.chess_piece_index;
}

int Server::getMostVotedPieceIndex() {
    return mostVoteChessPieceIndex;
}

MoveData Server::getMostVote() {
    MoveData move_data = mostVoted();
    MoveData result = {};
    if (move_data.to_x == 0) {
        return move_data;
    }
    result.chess_piece_index = mostVoteChessPieceIndex;
    result.to_x = move_data.to_x;
    result.to_y = move_data.to_y;
    return result;
}

MoveData getMostVote() {
    return server.getMostVote();
}

void setMostVotedPieceIndex() {
    server.setMostVotedPieceIndex();
}

void sendVoteResult() {
    server.sendVoteResult();
}

bool Server::isAllVoted() {
    if (voteIndex == 0 || voteIndex == 1) {
        if (white_team.size() == votes.size()) {
            return true;
        }
        return false;
    }
    if (voteIndex == 2||voteIndex == 3) {
        if (black_team.size() == votes.size()) {
            return true;
        }
        return false;
    }
}

bool isAllVoted() {
    return server.isAllVoted();
}

void Server::setVoteIndex(int index) {
    voteIndex = index;
}

void Server::clearVote() {
    votes.clear();
}

void setVoteIndex(int index) {
    server.setVoteIndex(index);
}

std::vector<SOCKET> Server::getWhiteTeam() {
    return white_team;
}

std::vector<SOCKET> Server::getBlackTeam() {
    return black_team;
}

int *getWhiteTeam() {
    return server.getWhiteTeam().data();
}

int *getBlackTeam() {
    return server.getBlackTeam().data();
}

int Server::getWhiteTeamCount() {
    return white_team.size();
}

int Server::getBlackTeamCount() {
    return black_team.size();
}

int getWhiteTeamCount() {
    return server.getWhiteTeamCount();
}
int getBlackTeamCount() {
    return server.getBlackTeamCount();
}

Color getClientColor(int client_sock) {
    return server.getClientColor(client_sock);
}

void Server::sendCloseClient(SOCKET client_sock) {
    MyMessage message = {};
    message.node = 'q';
    send(client_sock, &message, sizeof(message), 0);
}

void Server::sendWin(Color color) {
    MyMessage message = {};
    message.node = 'w';
    sendMessageColor(color, message);
    clearHistory();
    clearLDVote();
}

void Server::sendLose(Color color) {
    MyMessage message = {};
    message.node = 'l';
    sendMessageColor(color, message);
    clearHistory();
    clearLDVote();
}

void Server::sendDraw() {
    MyMessage message = {};
    message.node = 'd';
    sendMessageColor(Color::White, message);
    sendMessageColor(Color::Black, message);
    clearHistory();
    clearLDVote();
}

void sendWhiteWin() {
    server.sendWin(Color::White);
}

void sendWhiteLose() {
    server.sendLose(Color::White);
}

void sendBlackWin() {
    server.sendLose(Color::Black);
}

void sendBlackLose() {
    server.sendLose(Color::Black);
}

void sendDraw() {
    server.sendDraw();
}

void sendChat(char *msg) {
    server.sendChat(msg);
}

char *readChat() {
    return server.readChat();
}

void Server::sendMostVote() {
    MoveData vote = getMostVote();
    if (vote.chess_piece_index != -1&&vote.to_x != 0) {
        sendMoveData(vote);
        mostVoteChessPieceIndex = -1;
        history.push_back(vote);
    }
}

void sendMostVote() {
    server.sendMostVote();
}

void Server::addLoseVote(SOCKET client_sock) {
    bool found = false;
    for (LDVote vote : ldVotes) {
        if (vote.client_sock == client_sock) {
            found = true;
            vote.draw = false;
            vote.lose = true;
            break;
        }
    }
    if (!found) {
        LDVote vote = {};
        vote.client_sock = client_sock;
        vote.draw = false;
        vote.lose = true;
        ldVotes.push_back(vote);
    }
}

void Server::addDrawVote(SOCKET client_sock) {
    bool found = false;
    for (LDVote vote : ldVotes) {
        if (vote.client_sock == client_sock) {
            found = true;
            vote.draw = true;
            vote.lose = false;
            break;
        }
    }
    if (!found) {
        LDVote vote = {};
        vote.client_sock = client_sock;
        vote.draw = true;
        vote.lose = false;
        ldVotes.push_back(vote);
    }
}

bool Server::allLDVoted() {
    int count = 0;
    count +=(int) white_team.size();
    count += (int) black_team.size();
    if (count == (int)ldVotes.size()) {
        return true;
    }
    int whiteLoseCount = 0;
    int blackLoseCount = 0;
    for (LDVote vote : ldVotes) {
        Color color = getColor(vote.client_sock);
        if (vote.lose) {
            if (color == Color::White) {
                whiteLoseCount+=1;
            }else if (color == Color::Black) {
                blackLoseCount += 1;
            }
        }
    }
    if (whiteLoseCount == (int)white_team.size()) {
        return true;
    }
    if (blackLoseCount == (int)black_team.size()) {
        return true;
    }
    return false;
}

int Server::getWhiteLoseCount() {
    int count = 0;
    for (LDVote vote : ldVotes) {
        Color color = getColor(vote.client_sock);
        if (vote.lose) {
            if (color == Color::White) {
                count+=1;
            }
        }
    }
    return count;
}

int Server::getBlackLoseCount() {
    int count = 0;
    for (LDVote vote : ldVotes) {
        Color color = getColor(vote.client_sock);
        if (vote.lose) {
            if (color == Color::Black) {
                count+=1;
            }
        }
    }
    return count;
}

void Server::ldvoteWork() {
    int count = 0;
    count +=(int) white_team.size();
    count += (int) black_team.size();
    if (count == (int)ldVotes.size()) {
        sendDraw();
    }
    int whiteLoseCount = 0;
    int blackLoseCount = 0;
    for (LDVote vote : ldVotes) {
        Color color = getColor(vote.client_sock);
        if (vote.lose) {
            if (color == Color::White) {
                whiteLoseCount+=1;
            }else if (color == Color::Black) {
                blackLoseCount += 1;
            }
        }
    }
    if (whiteLoseCount == (int)white_team.size()) {
        sendWhiteLose();
        sendBlackWin();
    }
    if (blackLoseCount == (int)black_team.size()) {
        sendBlackWin();

    }
}

int Server::getDrawCount() {
    int count = 0;
    for (LDVote vote:ldVotes) {
        if (vote.draw) {
            count++;
        }
    }
    return count;
}

void Server::addHistory(MoveData move_data) {
    history.push_back(move_data);
}

void Server::initClient(SOCKET client_socket) {
    Color color = getClientColor(client_socket);
    MyMessage message = {};
    switch (voteIndex) {
        case 0: {
            if (color == Color::White) {
                message.node = 'p';
                send(client_socket, &message, sizeof(message), 0);
            }
            break;
        }
        case 1: {
            if (color == Color::White) {
                message.node = 'P';
                message.chess_piece_id = mostVoteChessPieceIndex;
                send(client_socket, &message, sizeof(message), 0);
            }
            break;
        }
        case 2: {
            if (color == Color::White) {
                message.node = 'D';
                send(client_socket, &message, sizeof(message), 0);
            }
            break;
        }
        case 3: {
            if (color == Color::Black) {
                message.node = 'p';
                send(client_socket, &message, sizeof(message), 0);
            }
            break;
        }
        case 4: {
            if (color == Color::Black) {
                message.node = 'P';
                message.chess_piece_id = mostVoteChessPieceIndex;
                send(client_socket, &message, sizeof(message), 0);
            }
            break;
        }
        case 5: {
            if (color == Color::Black) {
                message.node = 'D';
                send(client_socket, &message, sizeof(message), 0);
            }
            break;
        }
    }
    sendHistory(client_socket);
}

void Server::sendHistory(SOCKET client_socket) {
    for (MoveData move_data : history) {
        MyMessage message = {};
        message.node = 'R';
        message.chess_piece_id = move_data.chess_piece_index;
        message.x = move_data.to_x;
        message.y = move_data.to_y;
        send(client_socket, &message, sizeof(message), 0);
    }
}

void Server::clearHistory() {
    history.clear();
}

bool allLDVoted() {
    return server.allLDVoted();
}

int getWhiteLoseCount() {
    return server.getWhiteLoseCount();
}
int getBlackLoseCount() {
    return server.getBlackLoseCount();
}

int getDrawCount() {
    return server.getDrawCount();
}

void Server::clearLDVote() {
    ldVotes.clear();
}

void clearLDVote() {
    server.clearLDVote();
}

void Server::killAllBlack() {
    for (int i = 16; i<32;i++) {
        if (i!= 30) {
            MyMessage message = {};
            message.node = 'b';
            message.chess_piece_id = i;
            sendMessageAll(message);
        }
    }
}


void killAllBlack() {
    server.killAllBlack();
}

void Server::sendPromote(int index, int class_index) {
    MyMessage message = {};
    message.node = 'x';
    message.chess_piece_id = index;
    message.x = class_index;
    sendMessageAll(message);
}

void Server::sendPromoteSelect(Color color, int index, int timer) {
    MyMessage message = {};
    message.node = 't';
    message.chess_piece_id = index;
    message.x = timer;
    sendMessageColor(color, message);

}

void Server::enableCheat(bool state) {
    MyMessage message = {};
    message.node = 'z';
    if (state == false) {
        message.chess_piece_id = 0;
    }else {
        message.chess_piece_id = 1;
    }
    sendMessageAll(message);
}

void enableCheat(bool state) {
    server.enableCheat(state);
}


void sendWhitePromoteSelect(int index, int timer) {
    server.sendPromoteSelect(Color::White, index, timer);
}

void sendBlackPromoteSelect(int index, int timer) {
    server.sendPromoteSelect(Color::Black, index, timer);
}

void sendPromote(int index, int chess_class_index) {
    server.sendPromote(index,chess_class_index);
}




int main(int argc, char *argv[]) {
    server.init();
    server.start();
    server.wait();
}
