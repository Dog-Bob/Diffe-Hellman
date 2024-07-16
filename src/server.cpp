#include"server.h"
#include"diff_hellman.h"
#include"exchange.h"
#include"func.h"
#include"AES.h"

#define PORT 8080
#define BACKLOG 10
#define BUFSIZE 256

int main()
{
	ServerRun();
	return 0;
}


void handle_client(int client_socket) {
	char buffer[BUFSIZE];
	int bytes_read;
	//交换秘钥
	Exchange* chga = new Exchange(0);
	chga->generate_private();//a
	chga->culculate_public();//A
	string pAg = p_A_g(chga->p, chga->A, chga->g);//pAg
	char pAgbuf[24];
	for (int i = 0; i < pAg.size(); i++)
		pAgbuf[i] = pAg[i];
	write(client_socket,pAgbuf, sizeof(pAgbuf));

	char B[10];
	if ((bytes_read = read(client_socket, B,sizeof(B))) < 0)
	{
		perror("read B");
		close(client_socket);
	}
	string sB = "";
	int i = 0;
	for (; B[i] != '\0'; i++)
		sB += B[i];
	chga->B = parseCH(sB);
	chga->culculate_shared_key();

	cout <<"shared_K:"<< chga->K << endl;

	AES* aes = new AES(chga->K);

	// 接收消息并回显
	while ((bytes_read = read(client_socket, buffer, BUFSIZE)) > 0) {
		//buffer[bytes_read] = '\0';
		//解密
		memcpy(aes->C, buffer, BUFSIZE);
		aes->decryption();
		printf("Received: %s\n", aes->De_P);
		write(client_socket, buffer, bytes_read);
	}
	
	close(client_socket);
	printf("Client disconnected\n");
}

void sigchld_handler(int s) {
	// 防止僵尸进程
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

int ServerRun() {
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_size;
	struct sigaction sa;
	
	// 创建套接字
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	
	// 设置地址和端口
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	memset(&(server_addr.sin_zero), '\0', 8);
	
	// 绑定套接字
	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		close(server_socket);
		exit(1);
	}
	
	// 监听端口
	if (listen(server_socket, BACKLOG) == -1) {
		perror("listen");
		close(server_socket);
		exit(1);
	}
	
	// 处理 SIGCHLD 信号以防止僵尸进程
	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	
	printf("Server is running on port %d\n", PORT);
	
	while (1) {
		sin_size = sizeof(struct sockaddr_in);
		if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
			perror("accept");
			continue;
		}
		
		printf("Server: got connection from %s\n", inet_ntoa(client_addr.sin_addr));
		
		if (!fork()) {  // 子进程
			close(server_socket);  // 子进程不需要监听套接字
			handle_client(client_socket);
			exit(0);
		}
		
		close(client_socket);  // 父进程不需要这个套接字
	}
	
	close(server_socket);
	return 0;
}


