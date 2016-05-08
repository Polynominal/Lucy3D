// specificicaly for windows systems, use alongside the network library.
// DO NOT include this file when you are using linux or other OS other than windows.
// Minimum requirements are windows xp!

// [!] USE -lws_32 WHEN LINKING [!]


#ifndef WIN32_HS
#define WIN32_HS

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <string>
#include <map>

class Sockets
{
    public:
        Sockets();
        bool init();
        virtual ~Sockets();
        unsigned short port = 29229;
        class TCPServer
        {
            public:
                unsigned short port = 29229;
                TCPServer();

                bool init();
                bool buildListener();
                bool bindListener();
                bool pool();
                bool newclient();
                bool receive(std::string &str,int lenght=512);
                bool dispatch(const char* buffer,int lenght=512);
                bool echo(char& msg,int lenght=512);
                bool terminate();

                virtual ~TCPServer();
            private:
                int buffer_size = 512;
                SOCKET ListenSocket = INVALID_SOCKET;
                SOCKET user = INVALID_SOCKET;
                struct addrinfo *Info = NULL,*ptr = NULL, Hints;
        };
        class TCPClient
        {
            public:
                unsigned short port = 29229;
                TCPClient();
                bool init(const char* address,unsigned short sport= 29229);
                bool dispatch(const char* msg,int length=512);
                bool receive(std::string &input,int lenght);
                bool disconnect();

                virtual ~TCPClient();
            protected:
            private:
                 int buffer_size = 512;
                 struct addrinfo *Info = NULL,*ptr = NULL, Hints;
                 SOCKET Connect=INVALID_SOCKET;
        };
        class UDPServer
        {
            private:
                struct Client{
                    SOCKADDR_IN address;
                    void* userdata;
                    int id=-1;
                    Client(int tid,SOCKADDR_IN sock):address(sock),id(tid){};
                };
                SOCKET Core=INVALID_SOCKET;
                SOCKADDR_IN address;
                std::map<int,Client> Clients;
            public:
                unsigned short port = 29229;
                UDPServer();

                bool init(std::string address = "127.0.0.1",unsigned short port = 29229);
                bool bindListener();
                void flush();
                bool receive(int &user_id,std::string &out,int lenght,bool reliable=false); // if reliable is true then it will return false if the data size is not the same as expected data size
                bool dispatch(int user_id,std::string msg,int lenght);
                bool broadcast(std::string msg,int lenght);
                const char* getIP();

                virtual ~UDPServer();
        };
        class UDPClient
        {
            private:
                SOCKET Core=INVALID_SOCKET;
                SOCKADDR_IN Target;
            public:
                unsigned short port = 29229;
                UDPClient();
                bool init(std::string address,unsigned short port = 29229);
                bool dispatch(std::string data,int lenght);
                bool receive(std::string &data,int lenght,bool reliable=false);
                virtual ~UDPClient();
        };
        class Multicast
        {
            private:
                Multicast();
                ~Multicast();
            public:

        };
        class Broadcast
        {
        private:
        public:
            Broadcast();
            ~Broadcast();
        };
};
#endif // WIN32_HS
