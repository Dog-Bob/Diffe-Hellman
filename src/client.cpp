#include"client.h"
#include"exchange.h"
#include"func.h"

#define SERVER_IP "127.0.0.1"  // 服务器 IP 地址
#define SERVER_PORT 8081       // 服务器端口
#define BUFSIZE 256

int main()
{
	ClientRun();
	return 0;
}

int ClientRun() {
	int client_socket;
	struct sockaddr_in server_addr;
	char buffer[BUFSIZE];
	int bytes_read;
	
	// 创建套接字
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	
	// 设置服务器地址
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	memset(&(server_addr.sin_zero), '\0', 8);
	
	// 连接服务器
	if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
		perror("connect");
		close(client_socket);
		exit(1);
	}
	
	printf("Connected to server %s on port %d\n", SERVER_IP, SERVER_PORT);
	
	//exchange
	Exchange* echg = new Exchange(1);//p,g

	char pAg[24];
	int br;
	if ((br = recv(client_socket, pAg,sizeof(pAg), 0)) == -1)
	{
		perror("exchange pAg");
		close(client_socket);
		exit(1);
	}
	string sp = "", sA = "", sg = "";
	int i = 0;
	for (; pAg[i] != '\0'; i++)
		sp += pAg[i];
	i++;
	for (; pAg[i] != '\0'; i++)
		sA += pAg[i];
	i++;
	for (; pAg[i] != '\0'; i++)
		sg += pAg[i];
	cout << "p " << sp << endl;
	cout << "A " << sA << endl;
	cout << "g " << sg << endl;

	echg->p = parseCH(sp);
	echg->A = parseCH(sA);
	echg->g = parseCH(sg);
	echg->generate_private();//b
	echg->culculate_public();//B
	echg->culculate_shared_key();//K
	
	cout << "K " << echg->K << endl;

	//发送秘钥B '\0'结尾
	string B=itoc(echg->B);
	char Bbuf[10];
	for (int i = 0; i < B.size(); i++)
		Bbuf[i] = B[i];
	if (send(client_socket, Bbuf,sizeof(Bbuf), 0) == -1) {
		perror("send B");
		close(client_socket);
		exit(1);
	}
	
	//AES加密明文
	AES* aes = new AES(echg->K);
	cout << "aes key: ";
	for (int i = 0; i < 16; i++)
		cout << hex << aes->key[i] << " ";

	// 发送和接收数据
	while (1) {
		printf("Enter message: ");
		fgets(buffer, BUFSIZE, stdin);
		//加密明文
		memcpy(aes->P, buffer, BUFSIZE);
		aes->encryption();
		memcpy(buffer, aes->C, BUFSIZE);

		// 发送消息到服务器
		if (send(client_socket,buffer,BUFSIZE, 0) == -1) {
			perror("send");
			close(client_socket);
			exit(1);
		}
		
		// 接收服务器的回显消息
		if ((bytes_read = recv(client_socket, buffer, BUFSIZE, 0)) == -1) {
			perror("recv");
			close(client_socket);
			exit(1);
		}
		
		printf("Server reply(decrypted): %s\n", buffer);
	}
	
	close(client_socket);
	return 0;
}

