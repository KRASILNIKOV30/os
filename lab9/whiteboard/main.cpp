#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <memory>
#include <wx/wx.h>
#include "WhiteboardServer.h"
#include "WhiteboardClient.h"

class WhiteboardApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        int argc = wxApp::argc;
        char **argv = wxApp::argv;

        if (argc < 2)
        {
            std::cerr << "Usage: whiteboard <mode> <address/port>" << std::endl;
            return false;
        }

        if (std::string(argv[1]) == "server")
        {
            if (argc < 3)
            {
                std::cerr << "Usage: whiteboard server <port>" << std::endl;
                return false;
            }

            uint16_t port = std::stoul(argv[2]);
            m_server = std::make_unique<WhiteboardServer>(port);
            m_server->Show();
            return true;
        }
        else if (std::string(argv[1]) == "client")
        {
            if (argc < 4)
            {
                std::cerr << "Usage: whiteboard client <address> <port>" << std::endl;
                return false;
            }

            std::string address = argv[2];
            uint16_t port = std::stoul(argv[3]);
            m_client = std::make_unique<WhiteboardClient>(L"Whiteboard Client", wxString(address), port);
            m_client->Show();
            return true;
        }
        else
        {
            std::cerr << "Invalid mode. Use 'server' or 'client'." << std::endl;
            return false;
        }
    }

private:
    std::unique_ptr<WhiteboardClient> m_client;
    std::unique_ptr<WhiteboardServer> m_server;
};

IMPLEMENT_APP(WhiteboardApp);
