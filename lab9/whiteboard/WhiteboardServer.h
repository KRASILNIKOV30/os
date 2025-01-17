#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <wx/wx.h>

// хранить как один путь
struct Line
{
    int x1, y1, x2, y2;
};

class WhiteboardServer : public wxFrame
{
public:
    WhiteboardServer(uint16_t port)
        : wxFrame(NULL, wxID_ANY, "Whiteboard Server")
        , m_acceptor(m_ioContext)
        , m_port(port)
    {
        StartServer();
    }

private:
    wxDECLARE_EVENT_TABLE();

    void StartServer()
    {
        m_acceptor.open(boost::asio::ip::tcp::v4());
        m_acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_port));
        m_acceptor.listen();

        std::jthread([this]() {
            while (true)
            {
                boost::asio::ip::tcp::socket socket(m_ioContext);
                m_acceptor.accept(socket);
                m_clients.push_back(std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket)));
                std::jthread(&WhiteboardServer::HandleClient, this, m_clients.back()).detach();
            }
        }).detach();

        m_ioContext.run();
    }

    void OnPaint(wxPaintEvent& event)
    {
        wxPaintDC dc(this);
        dc.SetPen(*wxBLACK_PEN);
        for (const auto& line : m_lines)
        {
            dc.DrawLine(line.x1, line.y1, line.x2, line.y2);
        }
    }

    void OnMouseDown(wxMouseEvent& event)
    {
        m_drawing = true;
        m_lastX = event.GetX();
        m_lastY = event.GetY();
    }

    void OnMouseUp(wxMouseEvent& event)
    {
        if (m_drawing)
        {
            m_lines.push_back({ m_lastX, m_lastY, event.GetX(), event.GetY() });
            BroadcastLine(m_lines.back());
            Refresh();
            m_drawing = false;
        }
    }

    void OnMouseMove(wxMouseEvent& event)
    {
        if (m_drawing)
        {
            m_lines.push_back({ m_lastX, m_lastY, event.GetX(), event.GetY() });
            BroadcastLine(m_lines.back());
            m_lastX = event.GetX();
            m_lastY = event.GetY();
            Refresh();
        }
    }

    void BroadcastLine(const Line& line)
    {
        for (const auto& client : m_clients)
        {
            try
            {
                boost::asio::write(*client, boost::asio::buffer(&line, sizeof(line)));
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error broadcasting line: " << e.what() << std::endl;
            }
        }
    }

    void HandleClient(std::shared_ptr<boost::asio::ip::tcp::socket> client)
    {
        try
        {
            while (true)
            {
                Line line;
                boost::asio::read(*client, boost::asio::buffer(&line, sizeof(line)));
                m_lines.push_back(line);
                CallAfter([this]() { Refresh(); });
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Client disconnected: " << e.what() << std::endl;
        }
    }

private:
    boost::asio::io_context m_ioContext;
    boost::asio::ip::tcp::acceptor m_acceptor;
    uint16_t m_port;
    std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>> m_clients;
    std::vector<Line> m_lines;
    bool m_drawing = false;
    int m_lastX;
    int m_lastY;
};

wxBEGIN_EVENT_TABLE(WhiteboardServer, wxFrame)
    EVT_PAINT(WhiteboardServer::OnPaint)
    EVT_LEFT_DOWN(WhiteboardServer::OnMouseDown)
    EVT_LEFT_UP(WhiteboardServer::OnMouseUp)
    EVT_MOTION(WhiteboardServer::OnMouseMove)
wxEND_EVENT_TABLE()
