/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <thread>
#include <netdb.h>
#include <cstring>
#include "commands.h"
#include "CLI.h"

#include<signal.h>

using namespace std;

// edit your ClientHandler interface here:
//abstract class of client handler
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file
//This class represents the socket Input/Output communication
class SocketIO:public DefaultIO {
    int aClient;
public:
    SocketIO(int aClient);

    virtual string read();

    virtual void write(string text);

    virtual void write(float f);

    virtual void read(float *f);

    virtual void closeSocketIO();

    virtual ~SocketIO();
};

// edit your AnomalyDetectionHandler class here
//This class handles the client by the anomaly detector CLI
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        SocketIO sockDio(clientID);
        CLI cli(&sockDio);
        cli.start();
        sockDio.closeSocketIO();
    }
};


// implement on Server.cpp
//This class represents the server which works with threads
class Server {
	thread* t; // the thread to run the start() method in
	// you may add data members
    int fileDescriptor;
    sockaddr_in server;
    sockaddr_in client;
    bool isStop = false;
public:
    Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch) throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
