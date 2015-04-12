/*
 * =====================================================================================
 *
 * Filename: xproxy_connection.cpp
 *
 * Description: 
 *
 * Version: 1.0
 * Created: 2009年11月26日 15时12分33秒
 * Revision: none
 * Compiler: gcc
 *
 * Author: David Fang (A free programmer), qi_fd@163.com
 * Company: nocompany
 *
 * =====================================================================================
 */

#include "xproxy_connection.hpp"
#include <vector>
#include <iostream>
#include <boost/bind.hpp>

xproxy_connection::xproxy_connection(boost::asio::io_service& io_service, 
    tcp::endpoint& ana_endpoint)
    :strand_(io_service),
    ana_endpoint_(ana_endpoint),
    login_clt_sock_(io_service),
    ana_srv_sock_(io_service)
{
    std::cout<<"new connection construct\n"; 
}

xproxy_connection::~xproxy_connection()
{
    std::cout<<"connection destruct\n";
}

tcp::socket& xproxy_connection::login_clt_sock()
{
    return login_clt_sock_;
}
        
void xproxy_connection::start()
{
    std::cout<<"connection start to connect to analysing server...\n";
    ana_srv_sock_.async_connect(ana_endpoint_,
        strand_.wrap(boost::bind(&xproxy_connection::handle_connect_to_ana_server, 
            shared_from_this(), boost::asio::placeholders::error)));
}

void xproxy_connection::handle_connect_to_ana_server(const boost::system::error_code& e)
{
    if(!e)
    {
        std::cout<<"connect to analysing server succeed,"
            <<"now start to receive data from both sides...\n";

        login_clt_sock_.async_read_some(boost::asio::buffer(clt_buffer_),
                strand_.wrap(
                    boost::bind(&xproxy_connection::handle_login_clt_sock_read,
                        shared_from_this(), boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)));

        ana_srv_sock_.async_read_some(boost::asio::buffer(srv_buffer_),
                strand_.wrap(
                    boost::bind(&xproxy_connection::handle_ana_srv_sock_read,
                        shared_from_this(), boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)));

    }
}

void xproxy_connection::handle_login_clt_sock_read
    (const boost::system::error_code& e, std::size_t bytes_transferred)
{
    if(!e)
    {
        std::cout<<"data read from login client:\n";
        std::cout.write(clt_buffer_.data(), bytes_transferred);
        std::cout<<"\nnow send it to analysing server...\n";
        boost::asio::async_write(ana_srv_sock_, 
                boost::asio::buffer(clt_buffer_.data(), bytes_transferred),
                strand_.wrap(boost::bind(
                        &xproxy_connection::handle_ana_srv_sock_write,
                        shared_from_this(), boost::asio::placeholders::error)));
    }
    else
    {
        std::cout<<"read data from login client error, "
            <<"now need to shutdown this connection\n";
        ana_srv_sock_.cancel();
    }
}

void xproxy_connection::handle_ana_srv_sock_write(const boost::system::error_code& e)
{
    if(!e)
    {
        std::cout<<"data send to analysing server complete, "
            <<"now start to receive data from login client again...\n";
        login_clt_sock_.async_read_some(boost::asio::buffer(clt_buffer_),
                strand_.wrap(
                    boost::bind(&xproxy_connection::handle_login_clt_sock_read,
                        shared_from_this(), boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)));
    }
}

void xproxy_connection::handle_ana_srv_sock_read(
        const boost::system::error_code& e,
        std::size_t bytes_transferred)
{
    if(!e)
    {
        std::cout<<"data read from analysing server:\n";
        std::cout.write(srv_buffer_.data(), bytes_transferred);
        std::cout<<"\nnow send it to login client...\n";
        boost::asio::async_write(login_clt_sock_,
                boost::asio::buffer(srv_buffer_.data(), bytes_transferred),
                strand_.wrap(
                    boost::bind(&xproxy_connection::handle_login_clt_sock_write,
                        shared_from_this(), boost::asio::placeholders::error)));
    }
    else
    {
        std::cout<<"read data from analysing server error, "
            <<"now need to shutdown this connection\n";
        login_clt_sock_.cancel();
    }
}

void xproxy_connection::handle_login_clt_sock_write(const boost::system::error_code& e)
{
    if(!e)
    {
        std::cout<<"data send to login client complete, "
            <<"now start to receive data from analysing server again...\n";
        ana_srv_sock_.async_read_some(boost::asio::buffer(srv_buffer_),
                strand_.wrap(
                    boost::bind(&xproxy_connection::handle_ana_srv_sock_read,
                        shared_from_this(), boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)));
    }
}

