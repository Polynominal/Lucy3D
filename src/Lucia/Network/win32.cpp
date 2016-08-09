#include "Lucia\Network\win32.h"
#include <Lucia/Utils/log.h>
// server win 32 implementation
#ifdef _WIN32
using namespace Lucia;
Sockets::Sockets(){};
Sockets::~Sockets(){};
bool Sockets::init()
{
    int result;
    WSADATA wsaData;
    result = WSAStartup(MAKEWORD(2,2),&wsaData);
    if (result != 0)
    {
        // failed to start win sock.
        return false;
    }
    return true;
}
Sockets::TCPServer::TCPServer()
{
    //ctor
}
bool Sockets::TCPServer::init()
{
    int iresult=0;
    ZeroMemory(&Hints,sizeof(Hints));
    Hints.ai_family      = AF_INET;
    Hints.ai_socktype    = SOCK_STREAM;
    Hints.ai_protocol    = IPPROTO_TCP;
    Hints.ai_flags       = AI_PASSIVE;
    iresult = getaddrinfo(NULL,std::to_string(port).c_str(),&Hints,&Info);
    if (iresult != 0)
    {
        // failed to get address info!
        WSACleanup();
        return false;
    }
    return true;
}
bool Sockets::TCPServer::buildListener()
{

    ListenSocket = socket(Info->ai_family,
                          Info->ai_socktype,
                          Info->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        // error creating socket at socket();
        LOG.setSeverity("Fatal");
        LOG << "Error creating socket!"<< WSAGetLastError() << std::endl;
        return false;
    };
    return true;
}
bool Sockets::TCPServer::bindListener()
{

    int result = bind(ListenSocket,
                      Info->ai_addr,
                      (int)Info->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        LOG.setSeverity("Fatal");
        LOG << "Listener Bind error: " << WSAGetLastError() << LOG.endl;
        return false;
    }
    freeaddrinfo(Info);
    return true;
}
bool Sockets::TCPServer::pool()
{
    if (listen(ListenSocket,SOMAXCONN) == SOCKET_ERROR)
    {
        LOG.setSeverity("Fatal");
        LOG << "Listen failed: " << WSAGetLastError() << LOG.endl;
        return false;
    }
    return true;
}
// NOTE: THIS ONLY ACCEPTS ONE CLIENT!
bool Sockets::TCPServer::newclient()
{
    user = INVALID_SOCKET;
    user = accept(ListenSocket,NULL,NULL);
    if (user == INVALID_SOCKET)
    {
        // tried to connect but failed!
        LOG << "Failed to accept client!!" << LOG.endl;
        return false;
    }
    // If all went well then we must handle the new client socket via threads etc.

    return true;
}
bool Sockets::TCPServer::receive(std::string &input,int lenght)
{
    char buffer[lenght];
    int result;
    do {
        result = recv(user,buffer,lenght,0);
        if(result > 0){
            input = std::string(buffer);
            return true;
        }else if (result == 0)
        {
            //connection is closing
            LOG << "Connection closing" << LOG.endl;
            return false;
        }
        else
        {
            LOG << "Receive failed!!!" << WSAGetLastError() << LOG.endl;
            return false;
        }
    }while(result < 0);
}
bool Sockets::TCPServer::dispatch(const char* buffer,int lenght)
{
    int result;
    do {
        result = send(user,buffer,lenght,0);
        if (result == SOCKET_ERROR)
        {
            // SEND FAILED!
            LOG << "Send failed!!!" << WSAGetLastError() << LOG.endl;
            return false;
        };
        return true;
    }while (result < 0);
}
bool Sockets::TCPServer::echo(char& msg,int lenght)
{
    int res,sendresult;
    do {
        res = recv(user,&msg,lenght,0);
        if (res > 0)
        {
            // Bytes incoming!
            sendresult = send(user,&msg,res,0);
            if (sendresult == SOCKET_ERROR)
            {
                // SEND FAILED!
                LOG << "Send failed!!!" << WSAGetLastError() << LOG.endl;
                return false;
            };

        }else if(res == 0)
        {
            // Connection is closing
        }else
        {
            // FAILED!
            LOG << "Recv failed!!!" << WSAGetLastError() << LOG.endl;
            return false;
        }
    }while (res > 0);
    return false;
}
bool Sockets::TCPServer::terminate()
{
    int result = shutdown(user,SD_SEND);
    if (result == SOCKET_ERROR)
    {
        // shut down failed use destructor
        LOG << "Error closing socket: " << WSAGetLastError() << LOG.endl;
        return false;
    };
}
Sockets::TCPServer::~TCPServer()
{
    closesocket(ListenSocket);
    closesocket(user);
    WSACleanup();
    //dtor
}

// client win 32 implementation

Sockets::TCPClient::TCPClient()
{
    //ctor
}
bool Sockets::TCPClient::init(const char* address,unsigned short sport)
{
    ZeroMemory(&Hints,sizeof(Hints));
    Hints.ai_family = AF_UNSPEC;
    Hints.ai_socktype = SOCK_STREAM;
    Hints.ai_protocol = IPPROTO_TCP;

    int res = 0;
    getaddrinfo(address,std::to_string(sport).c_str(),&Hints,&Info);
    if (res != 0)
    {
        LOG << "Failed to getaddrinfo!" << LOG.endl;
        WSACleanup();
        return false;
    }
    auto ptr = Info;
    Connect = socket(ptr->ai_family,
                     ptr->ai_socktype,
                     ptr->ai_protocol);
    if (Connect == INVALID_SOCKET)
    {
        LOG << "Failed to create socket:"<< WSAGetLastError() << LOG.endl;
        freeaddrinfo(Info);
        WSACleanup();
        return false;
    }
    // process connection.
    res = 0;
    res = connect(Connect,
                  ptr->ai_addr,
                  (int)ptr->ai_addrlen
                  );
    if (res == SOCKET_ERROR){
        closesocket(Connect);
        Connect = INVALID_SOCKET;
    };
    freeaddrinfo(Info);
    if (Connect == INVALID_SOCKET)
    {
        LOG << "Unable to connect to server!" << LOG.endl;
        WSACleanup();
        return false;
    }
    return true;
}
bool Sockets::TCPClient::dispatch(const char* msg,int length)
{
    int res =0;
    res = send(Connect,msg,length,0);
    if (res == SOCKET_ERROR)
    {
        LOG << "Send failed: " << WSAGetLastError();
        return false;
    };
    return true;
}
bool Sockets::TCPClient::receive(std::string &input,int lenght)
{
    char buffer[lenght];
    int res = 0;
    do {
        res = recv(Connect,buffer,lenght,0);
        if (res > 0)
        {
            LOG << "Received:" << buffer << LOG.endl;
            input = std::string(buffer);
            return true;

        }else if(res == 0)
        {
            LOG << "Connection closed" << LOG.endl;
            return false;
        }else
        {
            LOG << "Receive failed: " << WSAGetLastError() << LOG.endl;
            return false;
        };
    }while(res > 0);
}
bool Sockets::TCPClient::disconnect()
{
    int result;
    result = shutdown(Connect,SD_SEND);
    if (result == SOCKET_ERROR)
    {
        LOG << "Disconnect failed" << WSAGetLastError() << LOG.endl;
        closesocket(Connect);
        WSACleanup();
        return false;
    }
    return true;
}
Sockets::TCPClient::~TCPClient()
{
    closesocket(Connect);
    WSACleanup();
    //dtor
}
// UDP Sockets
Sockets::UDPServer::UDPServer(){}
bool Sockets::UDPServer::init(std::string ipaddress,unsigned short port)
{
    Core = socket(
        AF_INET,
        SOCK_DGRAM,
        IPPROTO_UDP
    );
    if (Core == INVALID_SOCKET)
    {
        LOG.setSeverity("Fatal");
        LOG << "Error creating UDP Server: " << WSAGetLastError() << LOG.endl;
        return false;
    };
    memset(&address,0,sizeof(SOCKADDR_IN));
    //sets all of the memory at &address as 0 effectively clearing it
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if (ipaddress == "")
    {
        address.sin_addr.s_addr = htonl(INADDR_ANY);
    }else
    {
        address.sin_addr.s_addr = inet_addr(ipaddress.c_str());
    };
}
bool Sockets::UDPServer::bindListener()
{
    // bind the core socket to the address and if it returns an error print it.
    if(bind(Core,(SOCKADDR*)&address,sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
    {
        LOG << "Failed to bind socket: " << WSAGetLastError() << LOG.endl;
        return false;
    };
    return true;
};
void Sockets::UDPServer::flush()
{
    // clear the client info since they have disconnected.
}
bool Sockets::UDPServer::broadcast(std::string msg,int lenght)
{
    for (auto const &var : Clients)
    {
        SOCKADDR_IN clientInfo = var.second.address;
        int bytes_out = sendto(Core,msg.c_str(),lenght,
        0,(SOCKADDR*)&clientInfo,sizeof(SOCKADDR_IN));

        if (bytes_out == SOCKET_ERROR)
        {
            LOG << "Send failed: " << WSAGetLastError() << LOG.endl;
            return false;
        };

    }
    return true;
}
bool Sockets::UDPServer::receive(int &user_id,std::string &out,int lenght,bool reliable)
{
    SOCKADDR_IN clientInfo;

    char buffer[lenght];
    int in_bytes;
    int in_addrSize;
    do {
        in_bytes = recvfrom(Core,buffer,lenght,0,(SOCKADDR*)&clientInfo,&in_addrSize);
        if (in_bytes > 0)
        {
            bool found;
            for(auto const &v : Clients)
            {
                LOG << "Found Client!" << clientInfo.sin_port << LOG.endl;
                Client client = v.second;
                // compare two ip addresses if they are the same, s_addr is ulong so its fine
                if (client.address.sin_addr.s_addr == clientInfo.sin_addr.s_addr &&
                    client.address.sin_port == clientInfo.sin_port)
                {
                    user_id = client.id;
                    found = true;
                }
            }
            if (!found)
            {
                int id = Clients.size()+1;
                LOG << "The new ID is" << id << LOG.endl;
                user_id = id;
                Clients.insert({id,Client(id,clientInfo)});
            };
            LOG << "Received:" << buffer << LOG.endl;
            if (reliable && lenght != in_bytes)
            {
                return false;
            }
            out = std::string(buffer);
            return true;

        }else if(in_bytes == 0)
        {
            LOG << "Connection closed" << LOG.endl;
            return false;
        }else
        {

            return false;
        };
    }while(in_bytes > 0);
}
bool Sockets::UDPServer::dispatch(int user_id,std::string msg,int lenght)
{
    //[!] MUST BE SENT ONLY WHEN A CLIENT HAS CONNECTED [!]
    auto iter = Clients.find(user_id);
    if (iter != Clients.end())
    {
        SOCKADDR_IN clientInfo = iter->second.address;

        int bytes_out = sendto(Core,msg.c_str(),lenght,
        0,(SOCKADDR*)&clientInfo,sizeof(SOCKADDR_IN));

        if (bytes_out == SOCKET_ERROR)
        {
            LOG << "Send failed: " << WSAGetLastError() << LOG.endl;
            return false;
        };

        return true;

    }else{

        LOG << "Client not found: " << user_id << LOG.endl;
        return false;
    }
}
const char* Sockets::UDPServer::getIP()
{
    // MUST BE CONNECTED FOR THIS TO WORK
    int addrlen=0;
    return std::to_string(getsockname(Core,(SOCKADDR*)&address,&addrlen)).c_str();
}
Sockets::UDPServer::~UDPServer()
{
    closesocket(Core);
    WSACleanup();
}

Sockets::UDPClient::UDPClient(){};
Sockets::UDPClient::~UDPClient(){
    closesocket(Core);
};
bool Sockets::UDPClient::init(std::string address,unsigned short port)
{
    Core = socket(AF_INET, SOCK_DGRAM,IPPROTO_UDP);
    if (Core == INVALID_SOCKET)
    {
        LOG << "Failed to create Client Socket: " << WSAGetLastError() << LOG.endl;
        return false;
    };
    memset(&Target,0,sizeof(SOCKADDR_IN));
    Target.sin_family = AF_INET;
    Target.sin_port = htons(port);
    Target.sin_addr.s_addr = inet_addr(address.c_str());
}
bool Sockets::UDPClient::dispatch(std::string data,int lenght)
{
    int bytes_out = sendto(Core,data.c_str(),lenght,
    0,(SOCKADDR*)&Target,sizeof(SOCKADDR_IN));

    if (bytes_out == SOCKET_ERROR)
    {
        LOG << "Send failed: " << WSAGetLastError() << LOG.endl;
        return false;
    };
    return true;
};
bool Sockets::UDPClient::receive(std::string &data,int lenght,bool reliable)
{
    char buffer[lenght];
    int in_bytes;
    int in_addrSize;
    do {
        in_bytes = recvfrom(Core,buffer,lenght,0,(SOCKADDR*)&Target,&in_addrSize);
        if (in_bytes > 0)
        {
            LOG << "Received:" << buffer << LOG.endl;
            if (reliable && in_bytes != lenght)
            {
                return false;
            }
            data = std::string(buffer);
            return true;

        }else if(in_bytes == 0)
        {
            LOG << "Connection closed" << LOG.endl;
            return false;
        }else
        {

            return false;
        };
    }while(in_bytes > 0);
    return false;
}
#endif
