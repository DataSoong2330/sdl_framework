#ifndef __NET__
#define __NET__

#include <cstring>
#include <iostream>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

typedef char charbuf [256];

class Net
{
    public:
        // initialize SDL_net
        static bool Init();
        // quit SDL_net
        static void Quit();
};

class NetMessage
{
    public:
        // constuctor
        NetMessage();
        // virtual member function that indicates how many bytes
        // may have to be loaded into the instance. By default,
        // it works with strings that have a maximum of 255 characters
        virtual int NumToLoad();

        // virtual member function that indicates how many bytes
        // may have to be downloaded from the instance
        virtual int NumToUnLoad();

        // Load a char set into the message buffer
        void LoadBytes(charbuf& inputbuffer, int n);
        // Unload a char set from the buffer
        void UnLoadBytes(charbuf& destbuffer);
        // set the state object to full. No more data to be loaded
        void finish();

    protected:
        // message stored in buffer, limited to 256 bytes
        charbuf buffer;
        // message states
        enum bufstates { EMPTY, READING, WRITING, FULL };
        // the message state
        bufstates state;
        // reset message: fulfill it with zeroes and change its state to EMPTY
        void reset();

    private:
};

class _IpAddress
{
    public:
        // constructor
        _IpAddress();
        // create and associate a port to the instance
        _IpAddress(Uint16 port);
        // create and associate a port and a host to the instance
        _IpAddress(std::string host, Uint16 port);
        // set a _IpAddress object from an existing SDL IPaddress
        void SetIp (IPaddress sdl_ip);
        // True if the object have a port and a host associated to it
        bool Ok() const;
        // return a SDL_net IPaddress structure
        IPaddress GetIpAddress() const;
        //return the host
        Uint32 GetHost() const;
        //return the port
        Uint16 GetPort() const;

    protected:
    private:
        // the IPaddress structure
        IPaddress m_Ip;
};

class TcpSocket
{
    public:
        // ctor and virtual dtor
        TcpSocket();
        virtual ~TcpSocket();

        //set a TcpSocket object from a existing SDL socket
        virtual void SetSocket(TCPsocket the_sdl_socket);
        //indicate if theres is a TCPsocket associated to the instance
        bool Ok() const;
        //True if there are bytes ready to be read
        bool Ready() const;
        //pure virtual
        virtual void OnReady();

    protected:
        //the TCP socket structure
        TCPsocket m_Socket;
        //a set of sockets. Used here only to check existing packets
        SDLNet_SocketSet set;
};

class ClientSocket;

class HostSocket : public TcpSocket
{
    public:
        //create and open a new socket, with an existing _IpAddress object
        HostSocket(_IpAddress& the_ip_address);
        //create and open a new socket with the desired port
        HostSocket(Uint16 port);
        //set a client CTcpSocket object after listening to the port
        bool Accept(ClientSocket&);
        //pure virtual
        virtual void OnReady();
};

class ClientSocket : public TcpSocket
{
    public:
        //constructor
        ClientSocket();
        //Create the object and connect to a host, in a given port
        ClientSocket(std::string host, Uint16 port);
        //make a connection to communicate with a remote host
        bool Connect(_IpAddress& remoteip);
        //make a connection to communicate with a client
        bool Connect(HostSocket& the_listener_socket);
        //set a CTcpSocket object from an existing SDL_net socket
        void SetSocket(TCPsocket  the_sdl_socket);
        //return a _IpAddress object associated to the remote host
        _IpAddress getIpAddress() const;
        //pure virtual
        virtual void OnReady();
        //receive data and load it into a NetMessage object
        bool Receive(NetMessage& rData);
        //send data from a NetMessage object
        bool Send (NetMessage& sData);

    private:
        //the _IpAddress object corresponding to the remote host
        _IpAddress m_RemoteIp;
};

#endif // __NET__
