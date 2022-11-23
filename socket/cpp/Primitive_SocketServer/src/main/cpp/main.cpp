#include <iostream> // cout, cin
#include <iostream> // cout, cin
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // IPPROTO_TCP, sockaddr_in,
                        // htons/ntohs, INADDR_ANY
#include <unistd.h> // close
#include <arpa/inet.h> // inet_ntop/inet_atop
#include <string.h> // strlen
#include <semaphore.h> // sem_init
#include <pthread.h>

#define BUFFER_SIZE 1024

using namespace std;

//TODO aggiungere comando per killare le porte, in build.gradle.kts
int main(int _argc, char** _argv) {

//Socket creation
	int socket_server = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(socket_server<0){
	    cerr << "Error while creating the server socket" << endl;;
	    return -1;
	}

//Socket binding
    //Creation of the struct for the address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    //OPEN THE PORT BEFORE!!!
    serverAddr.sin_port = htons(4001); //port number in Network-Byte-Order
    serverAddr.sin_addr.s_addr = INADDR_ANY; //takes the IP address in Network-Byte_order (sin_addr is a struct -> s_addr)
    memset(&(serverAddr.sin_zero),'\0',8);
    int rBind = bind(socket_server, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if(rBind<0){
        cerr << "Error while binding the server socket" << endl;
        return -1;
    }

//Entering in listening mode
    int rListen = listen(socket_server, 5);
    if(rListen<0){
        cerr << "Error while bringing the server in listening mode" << endl;
        return -1;
    }

    //Using a while(true) to continuosly accept connections
    while(true){
        cout << "Waiting for a connection (server) ..." << endl;
        //Putting the server in a waiting mode
        //Empty object that will be filled with the address information of the client
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);

        int socketHandle = accept(socket_server, (sockaddr*) &clientAddr, &clientAddrSize);

        if(socketHandle<0){
            cerr << "Server error while accepting the connection with client" << endl;
            return -1;
        }

        cout << "Connection estabilished!" << endl;
        //Printing the information about the client (Task1.1b)
        char ipBuffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, ipBuffer, INET_ADDRSTRLEN);
        cout << "Client address: " << ipBuffer << " port: " << ntohs(clientAddr.sin_port) << endl;

        while(true){
            //Receving the data from the socket after the connection with the client
            cout << "Reading client data..." << endl;
            char msg[BUFFER_SIZE];
            int rRecv = recv(socketHandle, msg, BUFFER_SIZE,0);
            if(rRecv == -1){
                cerr << "Error while receiving the data from the client" << endl;
                return -1;
            }else if(rRecv == 0){
                cerr << "Partner socket closed" << endl;
                break;
            }

            //Printing the data received from the client
            cout << "Data received: " << msg << endl;
            if(strcmp(msg,"drop") == 0){
                memset(msg, 0, sizeof(msg));
                int rClose = close(socketHandle);
                if(rClose<0){
                    cerr << "Error while closing the communication with the client" << endl;
                    return -1;
                }
                break;
            }else if(strcmp(msg,"shutdown") == 0){
                memset(msg, 0, sizeof(msg));
                int rClose = close(socketHandle);
                if(rClose<0){
                    cerr << "Error while closing the communication with the client" << endl;
                    return -1;
                }else{
                    rClose = close(socket_server);
                    if(rClose<0){
                        cerr << "Error while closing the main server socket" << endl;
                        return -1;
                    }
                    return 0;
                }
            }

            memset(msg, 0, sizeof(msg));
            //Sending an "OK" message to the client
            char* message = (char*) "OK\0";
            int msgSize = strlen(message); //TODO CHECK SIZE???
            int rSend = send(socketHandle, message,msgSize,0);
            if(rSend<0){
                cerr << "Error while sending the data at the client" << endl;
                return -1;
            }
        }
    }
}
