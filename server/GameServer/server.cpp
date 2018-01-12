#include "server.h"
#include <iostream>
#include <QTimer>

/* Server initialization */
Server::Server(QObject *parent) {
    listener_status = listener.listen(PORT);
    // don't worry about listener status right now
}

/* Server object destructor */
Server::~Server() {
    listener.close(); // stop accepting connections
}

/* Get the pointer to the SQL interface class so this can use the database */
void Server::assignSQL(SQLInterface *sqlint) {
    sqlinterface = sqlint;
}

/* Wrapper to start the connection listener. This is nonblocking. */
int Server::startListening() {
    if(listener_status == sf::Socket::Status::Error) {
        std::cout << "Unexpected error setting up listener" << std::endl;
        return 1;
    }

    std::cout << "Listening..." << std::endl;
    QTimer::singleShot(100, this, SLOT(listenForConnections()));
    return 0;
}

/* Quick check to see if username is valid */
bool Server::isUsernameValid(std::string username) {
    for(unsigned int i=0; i<username.length(); i++) {
        //if(!std::isdigit(username[i])) {
            //std::cout << "invalid username " << username << std::endl;
            //return false;
        //}
    }
    return true;
}

/* Start listening for clients to connect. Once they've connected, fire off a
 * SLOT that reads data from that client (and start listening again). */
void Server::listenForConnections() {
    // create client socket
    sf::TcpSocket* client = new sf::TcpSocket();
    clientlist.push_back(client);

    // listen for connections
    listener.accept(*client);

    // a client connected - make a new thread with getData()
    std::cout << "New connection from " << client->getRemoteAddress() << ":" << client->getRemotePort() << std::endl;
    QTimer::singleShot(50, this, SLOT(getData()));

    // USING THE QT EVENT LOOP, restart the listener
    QTimer::singleShot(100, this, SLOT(listenForConnections()));
}

/* Read and process data from client */
void Server::getData() {
    // get current client list size - if another one gets added
    // while this is processing we won't delete another thread later
    int id = clientlist.size() - 1;

    // read the command packet (#0) from the client
    sf::TcpSocket *thisclient = clientlist.back();
    sf::Packet inpacket, outpacket;
    if(thisclient->receive(inpacket) == sf::Socket::Done) {
        // read the message from the packet
        std::string message;
        inpacket >> message;
        std::cout << "message/command from " << thisclient->getRemoteAddress() << ": " << message << std::endl;

        if(message == "login") { // login/new user request
            std::string status, username, password;
            inpacket >> status;    // existing or new student
            inpacket >> username;
            inpacket >> password;

            // check username to make sure it is an int, since that's how the DB stores it
            if(isUsernameValid(username)) {
                if(status == "existing") {      // existing user - check password
                    int result = sqlinterface->checkUser(username, password);
                    std::cout << "login for " << username << ": " << result << std::endl;
                    outpacket << (result > 0 ? "success" : "failure"); // login result: success or failure

                    outpacket << result;
                } else if(status == "new") {    // new user - create account
                    bool existinguser = sqlinterface->checkUsername(username);
                    if(existinguser) {
                        outpacket << "Error: user name already in use";
                    } else {
                        std::cout << "new user " << username << "...";
                        sqlinterface->newUser(username, password);
                        // check and see if user creation was successful. if so, respond with "success"
                        bool newuser = sqlinterface->checkUsername(username);
                        if(newuser) {
                            std::cout << " success!" << std::endl;
                            outpacket << "success";
                        } else {
                            std::cout << " failure" << std::endl;
                            outpacket << "Error: problem adding username to the database";
                        }
                    }
                } else {
                    outpacket << "Error: invalid user status type";
                }
            } else
                outpacket << "Error: user name not a numeric value";
        } else if(message == "levelinfo") { // level info
            std::string username;
            int level;
            inpacket >> username;
            inpacket >> level;

            if(isUsernameValid(username)) {
                std::cout << "get level " << level << " info for " << username << std::endl;
                std::vector<int> scores = sqlinterface->getScores(username, level);
                for(int i : scores)
                    outpacket << i;
            } else
                outpacket << "Error: user name not a numeric value";
        } else if(message == "allScores") { // all scores
            std::string username;
            inpacket >> username;

            if(isUsernameValid(username)) {
                std::cout << "get all scores for " << username << std::endl;
                std::vector<int> scores = sqlinterface->getAllScores(username);

                outpacket << "allscores";

                // return each score as an int
                for(int i=0; i<scores.size(); i++)
                    outpacket << scores[i];
            } else
                outpacket << "Error: user name not a numeric value";
        } else if(message == "level") { // level is complete
            std::string username;
            int level, round, score;
            inpacket >> username;
            inpacket >> level;
            inpacket >> round;
            inpacket >> score;

            if(isUsernameValid(username)) {
                std::cout << "update score for " << username << ", " << level << ", round " << round
                          << " (" << score << ")" << std::endl;
                sqlinterface->updateScore(username, level, round, score);
            } else
                outpacket << "Error: user name not a numeric value";
        } else if(message == "report") { // teacher report HTML data
            std::cout << "sending report data" << std::endl;
            std::vector<std::vector<int>> reportdata = sqlinterface->getReport();
            outpacket << "report";
            if(reportdata.size() == 0) {
                outpacket << "Error: no information available";
            }

            outpacket << (int) reportdata.size();

            for(int i=0; i<reportdata.size(); i++) {
                for(int j=0; j<reportdata[i].size(); j++) {
                    outpacket << reportdata[i][j];
                }
            }
        } else if(message == "remove") {
            std::string username;
            inpacket >> username;
            std::cout << "Removing user " << username << std::endl;

            outpacket << "remove";
            if(isUsernameValid(username)) {
                if(sqlinterface->removeUser(username))
                    outpacket << "OK";
                else
                    outpacket << "Error";
            } else
                outpacket << "Error: user name not a numeric value";
        } else {
            std::cout << "Error: unknown message" << std::endl;
            outpacket << "Error: unknown message";
        }
        thisclient->send(outpacket);
    } else {
        std::cout << "Received unknown packet type " << std::endl;
    }

    // clean up this client to prevent memory leaks
    std::cout << "Closing connection with " << thisclient->getRemoteAddress() << std::endl;
    thisclient->disconnect();
    delete thisclient;
    clientlist.erase(clientlist.begin() + id);
}
