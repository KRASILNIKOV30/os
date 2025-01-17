#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <wx/wx.h>
#include "WhiteboardServer.h"

class WhiteboardClient : public wxFrame
{
public:
    WhiteboardClient(const wxString& title, const wxString& address, uint16_t port)
        : wxFrame(NULL, wxID_ANY, title)
        , m_socket(m_io_context)
    {
        // обработать случай, когда соединение развалится (переподключение)
        boost::asio::ip::tcp::resolver resolver(m_io_context);
        auto endpoints = resolver.resolve(address.ToStdString(), std::to_string(port));
        boost::asio::connect(m_socket, endpoints);

        std::jthread([this] { ReceiveData(); }).detach();
    }

private:
    void OnPaint(wxPaintEvent& event)
    {
        wxPaintDC dc(this);
        dc.SetPen(*wxBLACK_PEN);
        for (const auto& line : m_lines)
        {
            // можно передавать битмап
            dc.DrawLine(line.x1, line.y1, line.x2, line.y2);
        }
    }

    void ReceiveData()
    {
        std::vector<char> buffer(sizeof(Line));
        while (true)
        {
            boost::system::error_code ec;
            size_t bytesRead = m_socket.read_some(boost::asio::buffer(buffer), ec);
            if (ec == boost::asio::error::eof)
            {
                break;
            }
            if (ec)
            {
                std::cerr << "Error receiving data: " << ec.message() << std::endl;
                break;
            }

            Line* line = reinterpret_cast<Line*>(buffer.data());
            m_lines.push_back(*line);
            CallAfter([this]() { Refresh(); });
        }
    }

private:
    boost::asio::io_context m_io_context;
    boost::asio::ip::tcp::socket m_socket;
    // мьютекс
    std::vector<Line> m_lines;

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(WhiteboardClient, wxFrame)
    EVT_PAINT(WhiteboardClient::OnPaint)
wxEND_EVENT_TABLE()