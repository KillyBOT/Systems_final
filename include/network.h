#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define TEST_IP "127.0.0.1"
#define PORT "8080"

int server_setup();
int server_connect(int sDesc);
int client_setup(char* server);