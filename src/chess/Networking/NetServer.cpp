#include "NetServer.h"


namespace Chess
{
    NetServer::NetServer(short port) :
        m_Acceptor(m_Context,asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
        m_IsStarted(false),
        soc(m_Context)
    {
        Accept();
    }

    NetServer::~NetServer()
    {
        Stop();
    }

    void NetServer::Accept()
    {
        m_Acceptor.async_accept(soc, [&](std::error_code ec)
        {
            if (!ec)
            {
                m_Clients.push_back(std::make_shared<NetClient>(std::move(soc)));
                Accept();
            }
        });
    }

    void NetServer::Stop()
    {
        if (!m_Context.stopped())
        {
            m_Context.stop();
        }
        m_IsStarted = false;
    }

    void NetServer::Start()
    {
        m_Context.run();
    }
}