/*
 * =====================================================================================
 *
 * Filename: xproxy_server.hpp
 *
 * Description: 
 *
 * Version: 1.0
 * Created: 2009年11月26日 15时12分01秒
 * Revision: none
 * Compiler: gcc
 *
 * Author: David Fang (A free programmer), qi_fd@163.com
 * Company: nocompany
 *
 * =====================================================================================
 */
#ifndef XPROXY_SERVER_HPP
#define XPROXY_SERVER_HPP

#include <string>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "xproxy_connection.hpp"

using boost::asio::ip::tcp;

class xproxy_server:private boost::noncopyable
{
public:

    //construction of xproxy_server, which takes the destination machine(analysing server)'s 

    //address and port(ipv4) as arguments

    explicit xproxy_server(unsigned short local_port, const std::string& ana_address, unsigned short ana_port,
            std::size_t thread_pool_size = 1);
    
    //Run the server's io_service loop

    void run();

    //Stop the server

    void stop();
private:
    //Handle the completion of an asynchronous accept from the login client

    void handle_accept(const boost::system::error_code& e);

    //The number of threads that will call io_service::run()

    std::size_t thread_pool_size_;

    //The io_service used to perform asynchronous operations.

    boost::asio::io_service io_service_;

    //Acceptor used to listen for incoming proxy connectins

    boost::asio::ip::tcp::acceptor acceptor_;

    //Local endpoint corresponding to the login client

    tcp::endpoint local_endpoint_;

    //The endpoint to analysing server 

    tcp::endpoint analysing_server_endpoint_;

    //The next connectin to be accepted.

    xproxy_connection_ptr new_connection_;
};

#endif
