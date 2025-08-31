//
// Created by 정주신 on 24. 5. 7.
//

#ifndef MULTIPLECHESS_COMMON_H
#define MULTIPLECHESS_COMMON_H

#include "../chess/Chess.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <poll.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <mutex>


typedef int SOCKET;
#define SOCKET_ERROR            -1
#define INVALID_SOCKET          -1
#define BUFSIZE                 512
#define NICKNAMESIZE            100
#define PORT                    9000
#define SelectChessPieceTime    60
#define SelectLocationTime      60



/**
 * 클라이언트 -> 서버
 *
 * 체스 말 위치 요청
 * node 'r'
 * chess_piece_id 숫자
 *
 * 닉네임 세팅
 * node 'n'
 * nickname 닉네임
 * client_sock 파이썬으로 정보를 보낼때 클라이언트의 소켓값을 가집니다.
 *
 * 체스 색깔 설정
 * node 'o'
 * color Color
 * client_sock 파이썬으로 정보를 보낼때 클라이언트의 소켓값을 가집니다.
 *
 * 체스 말 투표
 * node 'v'
 * chess_piece_id 숫자
 *
 * 체스 옮길 위치 투표
 * node 'V'
 * chess_piece_id 숫자
 * x 숫자
 * y 숫자
 *
 * 폰 승급 말 투표
 * node 'T'
 * chess_piece_id 체스 클래스 숫자로 전송
 *
 * 채팅
 * node 'c'
 * nickname 문자열
 * msg 문자열
 *
 * 무승부 신청
 * node 'W'
 * client_sock 파이썬으로 보낼때 사용함
 *
 * 패배 신청
 * node 'L'
 * client_sock 파이썬으로 보낼때 사용함
 *
 * 종료 (실제 클라이언트가 서버로 보내는 것은 아니고 파이썬을 종료 신호 보내기 위함)
 * node 'Q'
 * client_sock 클라이언트 소켓 값
 *
 * ---------------------
 *
 * 서버 -> 클라이언트
 *
 * 체스 말 선택 알림
 * node 'p'
 * x 타이머
 *
 * 체스 위치 선택 알림
 * node 'P'
 * chess_piece_id 선택된 체스 말
 * x 타이머
 *
 * 폰 승급 말 선택 알림
 * node 't'
 * chess_piece_id 승급할 체스 말
 * x 타이머
 *
 * 투표 종료 알림
 * node 'D'
 *
 * 채팅
 * node 'C'
 * nickname 문자열
 * msg 문자열
 *
 * 체스 말 위치 반환
 * node 'R'
 * chess_piece_id 숫자
 * x 숫자
 * y 숫자
 *
 * 체스 승리
 * node 'w'
 *
 * 체스 비김
 * node 'd'
 *
 * 체스 패배
 * node 'l'
 *
 * 닉네임 중복여부 반환
 * node 'I'
 * chess_piece_id 0~1 // 0이면 없음 1이면 있음 boolean이 없어서 이렇게함
 *
 * 투표 전송
 * node 'm'
 * chess_piece_id 체스말 숫자 (-1이면 무시)
 * x 좌표(0이면 무시)
 * y 좌표(0이면 무시)
 * client_sock 클라이언트 소켓값
 *
 * 죽이기
 * node 'b'
 * chess_piece_id 체스말 숫자
 *
 * 치트 활성화 여부
 * node 'z'
 * chess_piece_id 0~1 0이면 false 1이면 true
 *
 * 폰 승급 클래스
 * node 'x'
 * chess_piece_id 체스말 숫자
 * x 선택된 체스 클래스 숫자
 *
 * 종료
 * node 'q'
 *
 */
struct __attribute__((packed)) MyMessage{
    char node; // m: move, w: win, d: draw, l: lose,
    Color color;
    int chess_piece_id;
    int x;
    int y;
    char nickname[NICKNAMESIZE+1];
    char msg[BUFSIZE+1];
    int client_sock;
};

struct MoveData {
    int chess_piece_index;
    int to_x;
    int to_y;
};

struct Piece {
    int chess_piece_index;
    int color_num;
    int class_num;
    int x;
    int y;
};

struct Board {
    Piece pieces[32];
};

void message_init(MyMessage &msg) {
    msg.chess_piece_id = 0;
    msg.color = Color::White;
    msg.x = 0;
    msg.y = 0;
    msg.node = '?';
    memset(msg.msg, 0, BUFSIZE * sizeof(char));
    memset(msg.nickname, 0, NICKNAMESIZE * sizeof(char));
}

void err_quit(const char *msg){
    char *msgbuf = strerror(errno);
    printf("[%s] %s\n", msg, msgbuf);
    // exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char *msg)
{
    char *msgbuf = strerror(errno);
    printf("[%s] %s\n", msg, msgbuf);
}

// 소켓 함수 오류 출력
void err_display(int errcode)
{
    char *msgbuf = strerror(errcode);
    printf("[오류] %s\n", msgbuf);
}

#endif //MULTIPLECHESS_COMMON_H
