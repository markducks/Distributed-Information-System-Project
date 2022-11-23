//
// Created by Lorenzo on 17/11/2022.
//

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

/***
 *
 * CLose the current socket. Prints an error message if the closing action fails
 *
 * @param _socket
 *
 */

void mCloseSocket(int _socket){
    int rVal = close(_socket);

    if (rVal < 0) {
        std::cerr << "Error occured during closing the socket. Status: " << rVal << std::endl;
    }
}

/***
 *
 * Sends a message to the server. Prints an error message if the sending action fails
 *
 * @param _socket
 * @param msg
 *
 */
void mSendMSG(int _socket, std::string msg){
    int rVal = send(_socket, msg.c_str(), msg.length(), 0);

    if (rVal < 0){
        std::cerr << "Error during sending the message to the server. Status: " << rVal << std::endl;
    }
}

int main(int _argc, char **_argv) {
    std::cout << "Create socket" << std::endl;

    int commSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (commSocket < 0) {
        std::cerr << "Error occured during creation of the socket. Status: " << commSocket << std::endl;
        return -1;
    }

    //std::cout << "Port: " << _argv[1] << " Server IP " << _argv[2] << std::endl;

    sockaddr_in serverAddr {};
    serverAddr.sin_family = AF_INET;

    serverAddr.sin_port = htons(4001);
    serverAddr.sin_addr.s_addr = inet_addr("193.170.132.152");


    memset(&(serverAddr.sin_zero), '\0', 8);

    int rVal = connect(commSocket,
                       (sockaddr *) &serverAddr,
                       sizeof(sockaddr_in));

    if (rVal < 0) {
        std::cerr << "Error occured during connection. Status: " << rVal << std::endl;
        return -1;
    }

    while (true)
    {

        std::string msg = "";
        std::cout << "inserisci val " << std::endl;
        std::cin >> msg;

        if (msg == "quit")
        {
            mCloseSocket(commSocket);
            return 0;
        }
        else
        {
            mSendMSG(commSocket, msg);

            if (msg == "drop" || msg == "shutdown")
                break;
        }

        //wait for ACK from the server
        char ACK[BUFFER_SIZE];
        int bytes = recv(commSocket, ACK, BUFFER_SIZE, 0);

        if (bytes < 0){
            std::cerr << "Error during the recieve" << std::endl;
        }else if (bytes == 0){
            std::clog << "Server side socket closed" << std::endl;
            break;
        }else if (memcmp(ACK, "OK", BUFFER_SIZE) == 0){
            std::clog << "Invalid ACK from the server." << std::endl;
        }
    }

    mCloseSocket(commSocket);
    return 0;
}