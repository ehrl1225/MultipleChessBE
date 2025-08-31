//
// Created by 정주신 on 24. 5. 7.
//

#ifndef MULTIPLECHESS_SERVER_H
#define MULTIPLECHESS_SERVER_H



#include "common.h"
#include <vector>



struct Vote {
    SOCKET client_sock;
    int chess_piece_index;
    int x;
    int y;
};

struct LDVote{
    SOCKET client_sock;
    bool lose;
    bool draw;
};

class Server {

private:
    sockaddr_in serveraddr = {};
    std::vector<SOCKET> clients;
    std::vector<char*> nicknames;
    std::vector<Vote> votes;
    Chess chess_;
    char buf[BUFSIZE+1];
    pthread_t listen_tid;
    std::vector<std::string> chats;
    std::vector<MyMessage> messages;
    int mostVoteChessPieceIndex = -1;
    std::vector<LDVote> ldVotes;
    std::vector<MoveData> history;

public:
    // variables
    std::mutex send_mutex;
    std::mutex message_mutex;
    SOCKET listen_sock;
    std::vector<SOCKET> white_team;
    std::vector<SOCKET> black_team;
    std::vector<SOCKET> viewers;
    bool run_server = true;
    int voteIndex = -1;

    // server socket
    void init();
    void start();
    void stop();
    void wait();
    void close_client_socket(SOCKET client_sock);
    void closeAllSock();

    // vote
    void resetVote();
    void vote(SOCKET client_sock, MoveData &mv);
    int mostVotedChessIndex();
    Location &mostVotedLocation();
    MoveData mostVoted();
    bool hasVote(SOCKET sock);
    bool allVoted(Color color);
    void applyVote(MyMessage &msg);
    bool isVoteEmpty();
    void clearVote();
    bool isAllVoted();

    // ldvote
    bool allLDVoted();
    void ldvoteWork();
    int getWhiteLoseCount();
    int getBlackLoseCount();
    int getDrawCount();
    void clearLDVote();

    // add
    void addClient(SOCKET sock);
    void addChat(char *nickname, char* msg);
    void addLoseVote(SOCKET client_sock);
    void addDrawVote(SOCKET client_sock);
    void addHistory(MoveData move_data);

    // get
    int get_client_index(SOCKET sock);
    Chess getChess();
    int getClientCount();
    char *getClientNickName(SOCKET client_sock);
    Color getColor(SOCKET sock);
    int getVoteIndex(SOCKET sock);
    int getVoteCount();
    SOCKET getClient(int client_index);
    int getChatCount();
    std::vector<SOCKET> getClients();
    std::vector<char*> getClientNicknames();
    Color getClientColor(SOCKET sock);
    Board getBoard();
    int getMessageCount();
    int getMostVotedPieceIndex();
    MoveData getMostVote();
    std::vector<SOCKET> getWhiteTeam();
    std::vector<SOCKET> getBlackTeam();
    int getWhiteTeamCount();
    int getBlackTeamCount();

    // set
    void setNickname(SOCKET client_index, char* name);
    void setMostVotedPieceIndex();
    void setVoteIndex(int index);

    // send
    void sendMessageColor(Color color, MyMessage& msg);
    void sendMessageViwers(MyMessage& msg);
    void sendMessageAll(MyMessage &msg);
    void sendChat(char* msg);
    void sendChatColor(Color color, char* msg);
    void sendPieceSelect(Color color, int timer);
    void sendLocationSelect(Color color, int timer);
    void sendDoneVote(Color color);
    void sendMoveData(MoveData move_data);
    void sendVoteResult();
    void sendVoteResult(Color color);
    void sendCloseClient(SOCKET client_sock);
    void sendWin(Color color);
    void sendLose(Color color);
    void sendDraw();
    void sendMostVote();
    void sendHistory(SOCKET client_socket);
    void sendPromote(int index, int class_index);
    void sendPromoteSelect(Color color, int index, int timer);

    // read
    char *readChat();
    MyMessage readMessage();

    // etc
    bool isInTeam(Color color, int client_index);
    bool hasNickname(char * nickname);
    void addMessage(MyMessage msg);
    void initClient(SOCKET client_socket);
    void clearHistory();
    void killAllBlack();
    void enableCheat(bool state);

};

int main(int argc, char *argv[]);
void* listenClient(void* arg);
void* receiveProccess(void*arg);
void* sendProccess(void*arg);

extern "C"{
    int init();
    void start();
    void stop();
    void sendChat(char *msg);
    int getClientCount();
    char *getClientNickname(int client_sock);
    void sendWhitePieceSelect(int timer);
    void sendWhiteLocationSelect(int timer);
    void sendBlackPieceSelect(int timer);
    void sendBlackLocationSelect(int timer);
    void sendWhiteDoneVote();
    void sendBlackDoneVote();
    int* getClients();
    char** getClientNicknames();
    void resetVote();
    MoveData * getVoteList();
    int getVoteCount();
    void closeClient(SOCKET sock);
    void sendMoveData(MoveData move_data);
    Board getBoard();
    int getMessageCount();
    int getChatCount();
    MyMessage readMessage();
    MoveData getMostVote();
    void setMostVotedPieceIndex();
    void sendVoteResult();
    bool isAllVoted();
    void setVoteIndex(int index);
    int* getWhiteTeam();
    int* getBlackTeam();
    int getWhiteTeamCount();
    int getBlackTeamCount();
    Color getClientColor(int client_sock);
    void sendWhiteWin();
    void sendWhiteLose();
    void sendBlackWin();
    void sendBlackLose();
    void sendDraw();
    char* readChat();
    void sendMostVote();
    bool allLDVoted();
    int getWhiteLoseCount();
    int getBlackLoseCount();
    int getDrawCount();
    void clearLDVote();
    void killAllBlack();
    void enableCheat(bool state);
    void sendPromote(int index, int chess_class_index);
    void sendWhitePromoteSelect(int index, int timer);
    void sendBlackPromoteSelect(int index, int timer);
}

#endif //MULTIPLECHESS_SERVER_H
