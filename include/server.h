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



//�ͻ��˾��
void handle_client(int client_socket);

//��ֹ��ʬ����
void sigchld_handler(int s);

//������
int ServerRun();

