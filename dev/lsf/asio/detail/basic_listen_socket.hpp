// File:        basic_listen_socket.hpp
// Description: ---
// Notes:       ---
// Author:      leoxiang <leoxiang@tencent.com>
// Revision:    2012-06-08 by leoxiang

#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "lsf/basic/error.hpp"

namespace lsf {
namespace asio {
namespace detail {

template<typename Protocol>
class BasicListenSocket : public basic::Error
{
public:
    const static int DEF_LISTEN_QUEUE_SIZE = 128;

    typedef BasicSockAddr<Protocol>     sockaddr_type;
    typedef Protocol                    proto_type;

public:
    BasicListenSocket(proto_type proto = proto_type::V4()) {
        _sockfd = ErrWrap(::socket(proto.domain(), proto.type(), proto.protocol()));
    }

    BasicListenSocket(sockaddr_type const & local) {
        if (local.IsV4()) *this = BasicListenSocket(proto_type::V4());
        else              *this = BasicListenSocket(proto_type::V6());

        Bind(local);
    }

    BasicListenSocket(int sockfd) : _sockfd(sockfd) { }

    BasicListenSocket(BasicListenSocket const & rhs) : _sockfd(rhs._sockfd) { }

    // member funcs
    bool Bind(sockaddr_type const & local) {
        return ErrWrap(::bind(_sockfd, local.Data(), local.DataSize())) == 0;
    }

    bool Listen(int backlog = DEF_LISTEN_QUEUE_SIZE) {
        return ErrWrap(::listen(_sockfd, backlog)) == 0;
    }

    bool Accept(sockaddr_type & sockaddr) {
        int      sockfd;
        struct sockaddr addr;
        size_t   socklen;
        if ((sockfd = ErrWrap(::accept(_sockfd, NULL, NULL))) >= 0) {
            sockaddr.SetSockFd(sockfd);
            return true;
        }
        return false;
    }

    bool  Close() {
        return ErrWrap(::close(_sockfd)) == 0;
    }

    sockaddr_type LocalSockAddr() { 
        sockaddr addr;
        socklen_t   addrlen = sizeof(addr);
        ErrWrap(::getsockname(_sockfd, &addr, &addrlen));
        return sockaddr_type(&addr);
    }

    sockaddr_type RemoteSockAddr() {
        sockaddr addr;
        socklen_t   addrlen = sizeof(addr);
        if (ErrWrap(::getpeername(_sockfd, &addr, &addrlen)) == 0)
            return sockaddr_type(&addr);
        else if (IsV4())
            return sockaddr_type(proto_type::V4());
        else
            return sockaddr_type(proto_type::V6());
    }

    // SetSockOpt funcs
    bool SetNonBlock() {
        return ErrWrap(::fcntl(_sockfd, F_SETFL, ::fcntl(_sockfd, F_GETFL) | O_NONBLOCK)) == 0;
    }

    bool SetSockReuse() {
        size_t optval = 1;
        return ErrWrap(::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) == 0;
    }

    size_t GetRecvBufLen() const {
        size_t optval;
        socklen_t optlen;
        ::getsockopt(_sockfd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
        return optval;
    }

    size_t GetSendBufLen() const {
        size_t optval;
        socklen_t optlen;
        ::getsockopt(_sockfd, SOL_SOCKET, SO_SNDBUF, &optval, &optlen);
        return optval;
    }

    int  GetSockFd() const { return _sockfd; }

    // bool func

    bool IsV4() { return LocalSockAddr().IsV4(); }
    bool IsV6() { return LocalSockAddr().IsV6(); }

private:
    int             _sockfd;
};

} // end of namespace detail
} // end of namespace asio
} // end of namespace lsf

// vim:ts=4:sw=4:et:ft=cpp: