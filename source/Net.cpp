#include "../header/Net.hpp"

bool Net::Init()
{
    if(SDLNet_Init() < 0)
    {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        return false;
    }
    else
        return true;
}

void Net::Quit()
{
    SDLNet_Quit();
}

NetMessage::NetMessage()
{
    this->reset();
}

void NetMessage::reset()
{
    for(int i = 0; i < 256; i++)
        this->buffer[i] = 0;

    this->state = EMPTY;
}

void NetMessage::finish()
{
    if (this->state == READING)
        this->state = FULL;
}

int NetMessage::NumToLoad()
{
    if (this->state == EMPTY)
        return 255;
    else
        return 0;
}

int NetMessage::NumToUnLoad()
{
    if (this->state == FULL)
        return strlen(this->buffer) + 1;
    else
        return 0;
}

void NetMessage::LoadBytes(charbuf& inputbuffer, int n)
{
    for(int i = 0; i < n; i++)
        this->buffer[i] = inputbuffer[i];

    this->state = READING;
}

void NetMessage::UnLoadBytes(charbuf& destbuffer)
{
    for(int i = 0; i < this->NumToUnLoad(); i++)
        destbuffer[i] = this->buffer[i];

    this->reset();
}

_IpAddress::_IpAddress(Uint16 port)
{
    if(SDLNet_ResolveHost(&this->m_Ip, NULL, port) < 0)
    {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        this->m_Ip.host = 0;
        this->m_Ip.port = 0;
    }
}

_IpAddress::_IpAddress(std::string host, Uint16 port)
{
    if(SDLNet_ResolveHost(&this->m_Ip, host.c_str(), port) < 0)
    {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        this->m_Ip.host = 0;
        this->m_Ip.port = 0;
    }
}

_IpAddress::_IpAddress()
{
    this->m_Ip.host = 0;
    this->m_Ip.port = 0;
}

bool _IpAddress::Ok() const
{
    return !(this->m_Ip.port == 0);
}

//sets a CTcpSocket object from a existing SDL socket
void _IpAddress::SetIp(IPaddress sdl_ip)
{
    this->m_Ip = sdl_ip;
}

IPaddress _IpAddress::GetIpAddress() const
{
    return this->m_Ip;
}

Uint32 _IpAddress::GetHost() const
{
    return this->m_Ip.host;
}

Uint16 _IpAddress::GetPort() const
{
    return this->m_Ip.port;
}

TcpSocket::TcpSocket()
{
    this->m_Socket = NULL;
    set = SDLNet_AllocSocketSet(1);
}

TcpSocket::~TcpSocket()
{
    if(!(this->m_Socket == NULL))
    {
        SDLNet_TCP_DelSocket(set, this->m_Socket);
        SDLNet_FreeSocketSet(set);
        SDLNet_TCP_Close(this->m_Socket);
    }
}

void TcpSocket::SetSocket(TCPsocket the_sdl_socket)
{
    if (!(m_Socket == NULL))
    {
        SDLNet_TCP_DelSocket(set, this->m_Socket);
        SDLNet_TCP_Close(this->m_Socket);
    }

    this->m_Socket = the_sdl_socket;

    if(SDLNet_TCP_AddSocket(set, this->m_Socket) == -1)
    {
        std::cerr << "SDLNet_TCP_AddSocket: " << SDLNet_GetError() << std::endl;
        this->m_Socket = NULL;
    }
}

bool TcpSocket::Ok() const
{
    return !(this->m_Socket == NULL);
}

bool TcpSocket::Ready() const
{
    bool rd = false;
    int numready = SDLNet_CheckSockets(set, 0);

    if(numready == -1)
        std::cerr << "SDLNet_CheckSockets: " << SDLNet_GetError() << std:: endl;
    else if (numready)
        rd = SDLNet_SocketReady(this->m_Socket);

    return rd;
}

void TcpSocket::OnReady()
{

}

HostSocket::HostSocket(_IpAddress& the_ip_address)
{
    //TcpSocket::TcpSocket();

    IPaddress iph = the_ip_address.GetIpAddress();

    if(!(this->m_Socket = SDLNet_TCP_Open(&iph)))
    {
        SDLNet_FreeSocketSet(set);
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
    }
}

HostSocket::HostSocket(Uint16 port)
{
    _IpAddress iplistener(port);

    if (!iplistener.Ok())
    {
        this->m_Socket = NULL;
    }
    else
    {
        //TcpSocket::TcpSocket();

        IPaddress iph = iplistener.GetIpAddress();

        if(!(this->m_Socket = SDLNet_TCP_Open(&iph)))
        {
            SDLNet_FreeSocketSet(this->set);
            std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        }
    }
}

bool HostSocket::Accept (ClientSocket& the_client_socket)
{
    TCPsocket cs;

    if((cs = SDLNet_TCP_Accept(this->m_Socket)))
    {
        the_client_socket.SetSocket(cs);
        return true;
    }
    else
        return false;
}

void HostSocket::OnReady()
{

}

ClientSocket::ClientSocket()
{
    //TcpSocket::TcpSocket();
}

ClientSocket::ClientSocket(std::string host, Uint16 port)
{
    _IpAddress remoteip (host.c_str(), port);

    if (!remoteip.Ok())
    {
        this->m_Socket = NULL;
    }
    else
    {
        //TcpSocket::TcpSocket();
        Connect(remoteip);
    }
}

bool ClientSocket::Connect(HostSocket& the_listener_socket)
{
    return false;
}

bool ClientSocket::Connect(_IpAddress& remoteip)
{
    TCPsocket cs;
    IPaddress ip = remoteip.GetIpAddress();

    if((cs = SDLNet_TCP_Open(&ip)))
    {
        SetSocket(cs);
        return true;
    }
    else {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        return false;
    }
}

void ClientSocket::SetSocket(TCPsocket the_sdl_socket)
{
    TcpSocket::SetSocket(the_sdl_socket);

    IPaddress* ips;

    if((ips = SDLNet_TCP_GetPeerAddress(this->m_Socket)))
    {
        /* Print the address, converting it onto the host format */
        this->m_RemoteIp.SetIp(*ips);
        Uint32 hbo = this->m_RemoteIp.GetHost();
        Uint16 pbo = this->m_RemoteIp.GetPort();
        std::cout << "Client connected: " << SDLNet_Read32(&hbo) << ' '
            << SDLNet_Read16(&pbo) << std::endl;
    }
    else
        std::cerr << "SDLNet_TCP_GetPeerAddress: " << SDLNet_GetError() << std::endl;
}

_IpAddress ClientSocket::getIpAddress() const
{
    return this->m_RemoteIp;
}

bool ClientSocket::Receive(NetMessage& rData)
{
    // Firstly, check if there is a socket
    if (this->m_Socket == NULL)
        return false;

    charbuf buf;

    // Check if the instance can receive bytes, if it can, load the
    // number of bytes specified by NumToLoad() virtual function
    while (rData.NumToLoad() > 0)
    {
        if(SDLNet_TCP_Recv(m_Socket, buf, rData.NumToLoad()) > 0)
        {
            rData.LoadBytes (buf, rData.NumToLoad());
        }
        else
        {
            return false;
        }
    }

    rData.finish();

    return true;
}

bool ClientSocket::Send(NetMessage& sData)
{
    // check if there is a socket
    if(this->m_Socket == NULL)
        return false;

    charbuf buf;
    int len;

    //Check if the instance can send bytes, if it can, unload the number of
    // bytes specified by NumToLoad() virtual function
    while((len = sData.NumToUnLoad()) > 0)
    {
        sData.UnLoadBytes(buf);
        if (SDLNet_TCP_Send(this->m_Socket, (void *)buf, len) < len) {
            std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
            return false;
        }
    }

    return true;
}

void ClientSocket::OnReady()
{

}
