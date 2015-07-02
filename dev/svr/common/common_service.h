// File:        common_service.h
// Description: ---
// Notes:       ---
// Author:      leoxiang <leoxiang727@qq.com>
// Revision:    2015-06-17 by leoxiang

#include "lsf/basic/singleton.hpp"
#include "svr/common/basic_service.h"
#include "svr/common/common_header.h"
#include "svr/common/common_proto.h"

////////////////////////////////////////////////////////////
// ConnectConfigService
class ConnectConfigService : public BasicConnectService, public lsf::basic::Singleton<ConnectConfigService> {
public:
    ConnectConfigService() : BasicConnectService(conf::SERVICE_TYPE_CONFIG_CENTER) {}

    void SetServiceConfig(std::string const& address);
    bool GetServerConfig(conf::Server & config);
    bool GetAllConfig(google::protobuf::RepeatedPtrField<conf::Server> & all_config);

    // do not init config from server config
    // because server config is not init yet
    virtual bool OnInitConfig() { return true; }
};

////////////////////////////////////////////////////////////
// ConnectClientMsgTransferService
class ConnectClientMsgTransferService : public BasicConnectService, public lsf::basic::Singleton<ConnectClientMsgTransferService> {
public:
    ConnectClientMsgTransferService() : BasicConnectService(conf::SERVICE_TYPE_CLIENT_MSG_TRANSFER) {}

protected:
    virtual bool OnConnectionMessage(lsf::asio::Socket socket, std::string& message);
};

////////////////////////////////////////////////////////////
// ConnectServerMsgTransferService
class ConnectServerMsgTransferService : public BasicConnectService, public lsf::basic::Singleton<ConnectServerMsgTransferService> {
public:
    ConnectServerMsgTransferService() : BasicConnectService(conf::SERVICE_TYPE_SERVER_MSG_TRANSFER) {}

protected:
    // virtual bool OnConnectionMessage(lsf::asio::Socket socket, std::string& message);

};

// vim:ts=4:sw=4:et:ft=cpp: