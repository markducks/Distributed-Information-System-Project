//
// Created by Lorenzo on 23/11/2022.
//

#include "MY_TCP.h"


/***
 * TODO DOCUMENT THAT CLASS!!!!!!
 */

class MYTCP {

private:
    int socket_server;

public:

    MYTCP(){}

    ~MYTCP(){}

    int InitializeSocket(int port, int ip, int num_client){
        socket_server = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

        if(socket_server<0){
            std::cerr << "Error while creating the server socket" << std::endl;
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
            std::cerr << "Error while binding the server socket" << std::endl;
            return -1;
        }

        int rListen = listen(socket_server, num_client);
        if(rListen<0){
            std::cerr << "Error while bringing the server in listening mode" << std::endl;
            return -1;
        }

        //Using a while(true) to continuosly accept connections
        while(true){
           std::cout << "Waiting for a connection (server) ..." <<std::endl;
            //Putting the server in a waiting mode
            //Empty object that will be filled with the address information of the client
            sockaddr_in clientAddr;
            socklen_t clientAddrSize = sizeof(clientAddr);

            int socketHandle = accept(socket_server, (sockaddr*) &clientAddr, &clientAddrSize);

            if(socketHandle<0){
               std::cerr << "Server error while accepting the connection with client" <<std::endl;
                return -1;
            }

            std::cout << "Connection estabilished!" <<std::endl;
            //Printing the information about the client (Task1.1b)
            char ipBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, ipBuffer, INET_ADDRSTRLEN);
            std::cout << "Client address: " << ipBuffer << " port: " << ntohs(clientAddr.sin_port) <<std::endl;

            while(true){
                //Receving the data from the socket after the connection with the client
               std::cout << "Reading client data..." <<std::endl;
                char msg[BUFFER_SIZE];
                int rRecv = recv(socketHandle, msg, BUFFER_SIZE,0);
                if(rRecv == -1){
                   std::cerr << "Error while receiving the data from the client" <<std::endl;
                    return -1;
                }else if(rRecv == 0){
                   std::cerr << "Partner socket closed" <<std::endl;
                    break;
                }

                //Printing the data received from the client
               std::cout << "Data received: " << msg <<std::endl;
                if(strcmp(msg,"drop") == 0){
                    memset(msg, 0, sizeof(msg));
                    int rClose = close(socketHandle);
                    if(rClose<0){
                       std::cerr << "Error while closing the communication with the client" <<std::endl;
                        return -1;
                    }
                    break;
                }else if(strcmp(msg,"shutdown") == 0){
                    memset(msg, 0, sizeof(msg));
                    int rClose = close(socketHandle);
                    if(rClose<0){
                       std::cerr << "Error while closing the communication with the client" <<std::endl;
                        return -1;
                    }else{
                        rClose = close(socket_server);
                        if(rClose<0){
                           std::cerr << "Error while closing the main server socket" <<std::endl;
                            return -1;
                        }
                        return 0;
                    }
                }

                memset(msg, 0, sizeof(msg));
                //Sending an "OK" message to the client
                char* message = (char*) "ECHO: some message from the client\0";
                int msgSize = strlen(message);
                int rSend = send(socketHandle, message,msgSize,0);
                if(rSend<0){
                   std::cerr << "Error while sending the data at the client" <<std::endl;
                    return -1;
                }
            }
        }

    }

};