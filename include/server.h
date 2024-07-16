#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include<sys/wait.h>



//客户端句柄
void handle_client(int client_socket);

//防止僵尸进程
void sigchld_handler(int s);

//主函数
int ServerRun();

