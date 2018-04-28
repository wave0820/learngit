#include "YZSocket.h"
#include "YZCommon/YZLog.h"
#include <errno.h>

namespace YZ {
    
#if defined(_WIN32) || defined(WIN32)
    bool	shasInit = false;
#endif
    
    const char *strError(int x)
    {
        static	char tmp[100];
        switch (x)
        {
            case 10004: return "Interrupted function call.";
            case 10013: return "Permission denied.";
            case 10014: return "Bad address.";
            case 10022: return "Invalid argument.";
            case 10024: return "Too many open files.";
            case 10035: return "Resource temporarily unavailable.";
            case 10036: return "Operation now in progress.";
            case 10037: return "Operation already in progress.";
            case 10038: return "Socket operation on nonsocket.";
            case 10039: return "Destination address required.";
            case 10040: return "Message too long.";
            case 10041: return "Protocol wrong type for socket.";
            case 10042: return "Bad protocol option.";
            case 10043: return "Protocol not supported.";
            case 10044: return "Socket type not supported.";
            case 10045: return "Operation not supported.";
            case 10046: return "Protocol family not supported.";
            case 10047: return "Address family not supported by protocol family.";
            case 10048: return "Address already in use.";
            case 10049: return "Cannot assign requested address.";
            case 10050: return "Network is down.";
            case 10051: return "Network is unreachable.";
            case 10052: return "Network dropped connection on reset.";
            case 10053: return "Software caused connection abort.";
            case 10054: return "Connection reset by peer.";
            case 10055: return "No buffer space available.";
            case 10056: return "Socket is already connected.";
            case 10057: return "Socket is not connected.";
            case 10058: return "Cannot send after socket shutdown.";
            case 10060: return "Connection timed out.";
            case 10061: return "Connection refused.";
            case 10064: return "Host is down.";
            case 10065: return "No route to host.";
            case 10067: return "Too many processes.";
            case 10091: return "Network subsystem is unavailable.";
            case 10092: return "Winsock.dll version out of range.";
            case 10093: return "Successful WSAStartup not yet performed.";
            case 10101: return "Graceful shutdown in progress.";
            case 10109: return "Class type not found.";
            case 11001: return "Host not found.";
            case 11002: return "Nonauthoritative host not found.";
            case 11003: return "This is a nonrecoverable error.";
            case 11004: return "Valid name, no data record of requested type.";
                
            default:
                break;
        }
        snprintf(tmp, sizeof(tmp), "Winsock error code: %d", x);
        return tmp;
    }
    
    static void initSocket()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        
        //if (!shasInit)
        {
            do
            {
                WORD wVersionRequested = MAKEWORD(1, 1);
                WSADATA wsaData = {0};
                // WinSock init
                int ret = WSAStartup(wVersionRequested, &wsaData);
                if (ret != 0)
                {
                    printf("WSAStartup() failed!\n");
                    break;
                }
                // Confirm the WinSock DLL version 2.2
                if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
                {
                    WSACleanup();
                    printf("Invalid WinSock version!\n");
                    break;
                }
                printf("WinSock init success!\n");
            } while (0);
            shasInit = true;
        }
        
#endif
    }
    
    static void uninstallSocket()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        WSACleanup();
#endif
    }
    
    std::vector<std::string> YZSocket::getIpAddress(const char *host)
    {
        initSocket();
        unsigned long lAddr = inet_addr(host);
        std::vector<std::string> ips;
        if (lAddr == INADDR_NONE)
        {
            hostent *pHE = gethostbyname(host);
            
            
            
            if (nullptr == pHE) return std::vector<std::string>();
            
            for (int iHost = 0; pHE->h_addr_list[iHost]; iHost++)
            {
                std::string ip("");
                for (int i = 0; i < 4; i++)
                {
                    pHE->h_addr_list[iHost][i] & 0x00ff;
                    if (i > 0)
                    {
                        ip += ".";
                    }
                    char buff[8];
                    sprintf(buff, "%d", pHE->h_addr_list[iHost][i] & 0x00ff);
                    ip += buff;
                }
                ips.push_back(ip);
            }
            lAddr = *((unsigned long*) pHE->h_addr_list[0]);
        }
        uninstallSocket();
        return ips;
    }
    
    std::vector<std::string> YZSocket::getIpV6Address(const char *host)
    {
        initSocket();
        unsigned long lAddr = inet_addr(host);
        std::vector<std::string> ips;
        if (lAddr == INADDR_NONE)
        {
            hostent *pHE = gethostbyaddr(host,NULL,AF_INET);
            
            if (nullptr == pHE) return std::vector<std::string>();
            
            for (int iHost = 0; pHE->h_addr_list[iHost]; iHost++)
            {
                std::string ip("");
                for (int i = 0; i < 4; i++)
                {
                    pHE->h_addr_list[iHost][i] & 0x00ff;
                    if (i > 0)
                    {
                        ip += ".";
                    }
                    char buff[8];
                    sprintf(buff, "%d", pHE->h_addr_list[iHost][i] & 0x00ff);
                    ip += buff;
                }
                ips.push_back(ip);
            }
            lAddr = *((unsigned long*) pHE->h_addr_list[0]);
        }
        uninstallSocket();
        return ips;
    }
    
    
    YZSocket::YZSocket(void) : _socket(INVALID_SOCKET), _port(-1)
    {
        initSocket();
    }
    
    YZSocket::~YZSocket(void)
    {
        close();
        uninstallSocket();
    }
    
    bool YZSocket::create()
    {
        _socket = INVALID_SOCKET;
        bool ret = false;
        do
        {
            _socket = socket(AF_INET, SOCK_STREAM, 0);
            if (INVALID_SOCKET == _socket)
            {
                YZLOG_ERROR ("socket create: %s", strerror(errno));
                close();
                break;
            }
            ret = true;
        } while (0);
        
        return ret;
    }
    
    INT YZSocket::connect()
    {
        return connect(_ip.c_str(), _port);
    }
    
    INT YZSocket::connectWithHost(const CHAR* host, INT port)
    {
        INT ret = -1;
        do
        {
            _ips = getIpAddress(host);
            
            CCLOG("_ips size : %lu" , _ips.size());
            
            for (auto iter = _ips.begin(); iter != _ips.end(); ++iter)
            {
                ret = connect(iter->c_str(), port);
                if (0 == ret)
                {
                    break;
                }
            }
        } while (0);
        return ret;
    }
    
    
    INT YZSocket::connect(const CHAR* ip, INT port)
    {
        INT nready = -1;
        
        char domain[100];
        sprintf(domain,"%s",ip);
        
        char strPort[100];
        sprintf(strPort,"%d",port);
        
        
        setNonblocking(true);
        
        struct addrinfo *result;
        struct addrinfo *res;
        
        struct addrinfo addrCriteria;
        memset(&addrCriteria,0,sizeof(addrCriteria));
        addrCriteria.ai_family=AF_UNSPEC;
        addrCriteria.ai_socktype=SOCK_STREAM;
        addrCriteria.ai_protocol=IPPROTO_TCP;
        
        nready = getaddrinfo(domain, strPort, &addrCriteria, &result);
        CCLOG("getaddrinfo : %d" , nready);
        
        if (nready == 0)
        {
            struct sockaddr_in *sa;
            for (res = result; res != NULL; res = res->ai_next)
            {
                if (AF_INET6 == res->ai_addr->sa_family)
                {
                    char buf[128] = {};
                    sa = (struct sockaddr_in*)res->ai_addr;
                    inet_ntop(AF_INET6, &((reinterpret_cast<struct sockaddr_in6*>(sa))->sin6_addr), buf, 128);
                    
                    _socket = socket(res->ai_family, res->ai_socktype, 0);
                    if (_socket == -1) {
                        log("error socket create");
                        return -1;
                    }
                    
                    struct sockaddr_in6 svraddr;
                    memset(&svraddr, 0, sizeof(svraddr)); //注意初始化
                    svraddr.sin6_family = AF_INET6;
                    svraddr.sin6_port = htons(port);
                    if (inet_pton(AF_INET6,buf,&svraddr.sin6_addr) < 0)
                    {
                        log("error addr");
                    }
                    nready = ::connect(_socket, (struct sockaddr*) &svraddr, sizeof(svraddr));
                    break;
                }
                else if (AF_INET == res->ai_addr->sa_family)
                {
                    char buf[32] = {};
                    sa = (struct sockaddr_in*)res->ai_addr;
                    inet_ntop(AF_INET, &((reinterpret_cast<struct sockaddr_in*>(sa))->sin_addr), buf, 32);
                    _socket = socket(res->ai_family, res->ai_socktype, 0);
                    if (_socket == -1) {
                        log("error socket create");
                        return - 1;
                    }
                    
                    struct sockaddr_in svraddr;
                    svraddr.sin_family = AF_INET;
                    svraddr.sin_addr.s_addr = inet_addr(buf);
                    svraddr.sin_port = htons(port);
                    nready = ::connect(_socket, (struct sockaddr*) &svraddr, sizeof(svraddr));
                }
            }
        }else{
            nready = TCP_CONNECT_ADDRERROR;
        }
        
        setNonblocking(false);
        
        if (TCP_CONNECT_OUTTIME == nready)
        {
            YZLOG_ERROR ("connect network outtime: %s", strerror(errno));
            close();
        }
        
        if (TCP_CONNECT_ERROR == nready)
        {
            YZLOG_ERROR ("connect network error: %s", strerror(errno));
            close();
        }
        
        if (TCP_CONNECT_ADDRERROR == nready)
        {
            YZLOG_ERROR ("getaddrinfo error: %s", strerror(errno));
            close();
        }
        
        YZLOG_INFO ("connect network nready = %d", nready);
        return nready;
        
    }
    
    bool YZSocket::isIpv6(const char * domain)
    {
#ifdef _WIN32
        return false;
#else
        
        bool isIPV6Net = false;
        
        struct addrinfo *result = nullptr,*curr;
        
        struct sockaddr_in6 dest;
        bzero(&dest, sizeof(dest));
        
        dest.sin6_family  = AF_INET6;
        
        int ret = getaddrinfo(domain,nullptr,nullptr,&result);
        if (ret == 0)
        {
            for (curr = result; curr != nullptr; curr = curr->ai_next)
            {
                switch (curr->ai_family)
                {
                    case AF_INET6:
                    {
                        isIPV6Net = true;
                        break;
                    }
                    case AF_INET:
                        
                        break;
                        
                    default:
                        break;
                }
            }
        }
        
        freeaddrinfo(result);
        
        return isIPV6Net;
#endif
        
    }
    
    std::string  YZSocket::domainToIp(const char* pDomain)
    {
#ifdef _WIN32
        return "";
#else
        if (m_isIpv6)
        {
            struct addrinfo hint;
            memset(&hint, 0x0, sizeof(hint));
            hint.ai_family = AF_INET6;
            hint.ai_flags = AI_V4MAPPED;
            
            addrinfo* answer = nullptr;
            getaddrinfo(pDomain, nullptr, &hint, &answer);
            
            if (answer != nullptr)
            {
                char hostname[1025] = "";
                
                getnameinfo(answer->ai_addr,answer->ai_addrlen,hostname,1025,nullptr,0,0);
                
                char ipv6[128] = "";
                memcpy(ipv6,hostname,128);
                
                CCLOG("domainToIP addrStr:%s", ipv6);
                
                return ipv6;
            }
            
            freeaddrinfo(answer);
        }
#endif
        
    }
    
    
    INT YZSocket::send(const CHAR* data, INT dataSize)
    {
        int n = ::send(_socket, data, dataSize, 0);
        if (n == -1)
        {
#ifdef _WIN32
            if (Errno != WSAEWOULDBLOCK)
#else
                if (Errno != EWOULDBLOCK)
#endif
                {
                    YZLOG_ERROR ("network send >> %d >> %s", Errno, strError(Errno));
                    close();
                }
            return 0;
        }
        return n;
        //INT pos = 0;
        //int sendLen = dataSize;
        //do
        //{
        //	int count = ::send(_socket, data + pos, sendLen, 0);
        //	pos += count;
        //	sendLen -= count;
        //} while (sendLen > 0);
        //return pos;
    }
    
    INT YZSocket::recv(CHAR* buffer, INT size)
    {
        return ::recv(_socket, buffer, size, 0);
    }
    
    INT YZSocket::select(timeval* timeOut)
    {
        fd_set readfd;
        FD_ZERO(&readfd);
        FD_SET(_socket, &readfd);
        /*
         EBADF  An invalid file descriptor was given in one of the sets.
         EINTR  A non blocked signal was caught.
         EINVAL n is negative. Or struct timeval contains bad time values (<0).
         ENOMEM select was unable to allocate memory for internal tables.
         */
        int nready = ::select(_socket + 1, &readfd, nullptr, nullptr, timeOut);
        
        if (nready < 0)
        {
            int nready = Errno;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            switch (nready)
            {
                case WSAENOTSOCK:
                    break;
                case WSAEINTR:
                case WSAEINPROGRESS:
                    break;
                case WSAEINVAL:
                    YZLOG_ERROR ("select : %d >> %s", nready, strError(nready));
                    break;
                case WSAEFAULT:
                    YZLOG_ERROR ("select : %d >> %s", nready, strError(nready));
                    break;
                case WSANOTINITIALISED:
                    YZLOG_ERROR ("WSAStartup not successfully called");
                    break;
                case WSAENETDOWN:
                    YZLOG_ERROR ("Network subsystem failure");
                    break;
            }
#else
            switch (nready)
            {
                case EBADF:
                    break;
                case EINTR:
                    break;
                case EINVAL:
                    YZLOG_ERROR ("select : %d >> %s", nready, strError(nready));
                    break;
                case ENOMEM:
                    YZLOG_ERROR ("select : %d >> %s", nready, strError(nready));
                    break;
            }
#endif
        }
        else if (0 == nready)
        {
            // outtime
        }
        else
        {
        }
        return nready;
    }
    
    bool YZSocket::close()
    {
        int ret = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        ret = ::closesocket(_socket);
#else
        ret = ::close(_socket);
#endif
        return ret >= 0;
    }
    
    bool YZSocket::shutdown()
    {
        int ret = 0;
        ret = ::shutdown(_socket, SHUT_RDWR);
        return ret >= 0;
    }
    
    int YZSocket::isConnected()
    {
        fd_set	fd;
        struct timeval tv;
        
        FD_ZERO(&fd);
        FD_SET(_socket, &fd);
        
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        if (::select(_socket + 1, nullptr, &fd, nullptr, &tv) > 0)
        {
            if( FD_ISSET(_socket, &fd) )
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                return 1;		// �ɹ�
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                int nError;
                socklen_t len = sizeof(nError);
                if (getsockopt(_socket, SOL_SOCKET, SO_ERROR, &nError, &len) < 0 )
                {
                    return 4;	// ʧ��
                }
                if( nError == ECONNREFUSED || nError == ETIMEDOUT )
                {
                    return 4;	// ʧ��
                }
                return 1;	// �ɹ�
#endif
            }
        }
        return 2;	// ������
    }
    
    
    bool YZSocket::setNonblocking(bool bNb)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        unsigned long l = bNb ? 1 : 0;
        int n = ioctlsocket(_socket, FIONBIO, &l);
        if (n != 0)
        {
            YZLOG_ERROR ("ioctlsocket(FIONBIO) >> %d", Errno);
            return false;
        }
        return true;
#else
        if (bNb)
        {
            if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1)
            {
                YZLOG_ERROR ("fcntl(F_SETFL, O_NONBLOCK) %d >>> %s", Errno, strError(Errno));
                return false;
            }
        }
        else
        {
            if (fcntl(_socket, F_SETFL, 0) == -1)
            {
                YZLOG_ERROR ("fcntl(F_SETFL, 0) %d >>> %s", Errno, strError(Errno));
                return false;
            }
        }
        return true;
#endif
    }
    
    bool YZSocket::setSoRcvtimeo(struct timeval& tv)
    {
#ifdef SO_RCVTIMEO
        if (setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) == -1)
        {
            YZLOG_ERROR ("setsockopt(SOL_SOCKET, SO_RCVTIMEO) >> %d >> %s", Errno, strError(Errno));
            return false;
        }
        return true;
#else
        YZLOG_ERROR ("socket option not available >> %d >> %s", 0, "SO_RCVTIMEO");
        return false;
#endif
    }
    
    bool YZSocket::setSoSndtimeo(struct timeval& tv)
    {
#ifdef SO_SNDTIMEO
        if (setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)) == -1)
        {
            YZLOG_ERROR ("setsockopt(SOL_SOCKET, SO_SNDTIMEO) >> %d >> %s", Errno, strError(Errno));
            return false;
        }
        return true;
#else
        YZLOG_ERROR ("socket option not available >> %d >> %s", 0, "SO_SNDTIMEO");
        return false;
#endif
    }
    
    bool YZSocket::setSoRcvbuf(int x)
    {
#ifdef SO_RCVBUF
        if (setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (char *)&x, sizeof(x)) == -1)
        {
            YZLOG_ERROR ("setsockopt(SOL_SOCKET, SO_RCVBUF) >> %d >> %s", Errno, strError(Errno));
            return false;
        }
        return true;
#else
        YZLOG_ERROR ("socket option not available >> %d >> %s", 0, "SO_RCVBUF");
        return false;
#endif
    }
    
    bool YZSocket::soRcvbuf()
    {
        int value = 0;
#ifdef SO_RCVBUF
        socklen_t len = sizeof(value);
        if (setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (char *)&value, sizeof(len)) == -1)
        {
            YZLOG_ERROR ("setsockopt(SOL_SOCKET, SO_RCVBUF) >> %d >> %s", Errno, strError(Errno));
            return false;
        }
        return true;
#else
        YZLOG_ERROR ("socket option not available >> %d >> %s", 0, "SO_RCVBUF");
        return false;
#endif
    }
    
    
    bool YZSocket::setSoSendbuf(int x)
    {
#ifdef SO_SNDBUF
        if (setsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (char *)&x, sizeof(x)) == -1)
        {
            YZLOG_ERROR ("setsockopt(SOL_SOCKET, SO_SNDBUF) >> %d >> %s", Errno, strError(Errno));
            return false;
        }
        return true;
#else
        YZLOG_ERROR ("socket option not available >> %d >> %s", 0, "SO_SNDBUF");
        return false;
#endif
    }
    
    bool YZSocket::soSendbuf()
    {
        int value = 0;
#ifdef SO_SNDBUF
        socklen_t len = sizeof(value);
        if (setsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (char *)&value, sizeof(len)) == -1)
        {
            YZLOG_ERROR ("setsockopt(SOL_SOCKET, SO_SNDBUF) >> %d >> %s", Errno, strError(Errno));
            return false;
        }
        return true;
#else
        YZLOG_ERROR ("socket option not available >> %d >> %s", 0, "SO_SNDBUF");
        return false;
#endif
    }
    
    unsigned short YZSocket::getSockPort()
    {
        struct sockaddr_in sa;
        socklen_t sockaddr_length = sizeof(struct sockaddr_in);
        if (getsockname(_socket, (struct sockaddr *)&sa, (socklen_t*)&sockaddr_length) == -1)
        {
            memset(&sa, 0, sizeof(sa));
        }
        return ntohs(sa.sin_port);
    }
    
}
