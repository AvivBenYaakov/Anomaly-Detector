/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <thread>
#include <pthread.h>
#include <iostream>
#include "CLI.h"

#define PORT 8080

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler {
public:
    virtual void handle(int clientID) = 0;
};


class SocketIO : public DefaultIO {
    int clientID;

public:
    SocketIO(int clientID) {
        this->clientID = clientID;
    }

    virtual string read() {
        string serverInput = "";
        char c = 0;
        recv(clientID, &c, sizeof(char), 0);
        while (c != '\n') {
            serverInput += c;
            recv(clientID, &c, sizeof(char),0);
        }
        return serverInput;
    }

    virtual void write(string str) {
        send(clientID, str.c_str(), str.length(), 0);
    }

    virtual void write(float f)
    {
        stringstream stream;
        stream << f;
        string str = stream.str();
        send(clientID, str.data(), str.size(), 0);

    }


    virtual void read(float *f)
    {
        recv(clientID, f, sizeof(float), 0);
    }

};


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler : public ClientHandler {


public:

    void handle(int clientID) override;
};


// implement on Server.cpp
class Server {
    thread *t; // the thread to run the start() method in
    int fd; // file descriptor
    sockaddr_in server;
    sockaddr_in client;
    bool running;


public:
    Server(int port) throw(const char *);

    virtual ~Server();

    void start(ClientHandler &ch) throw(const char *);

    void stop();
};

#endif /* SERVER_H_ */
