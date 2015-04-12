// TestBoost.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


//int main(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

/*
 * =====================================================================================
 *
 * Filename: xproxy_main.cpp
 *
 * Description: 
 *
 * Version: 1.0
 * Created: 2009年11月26日 15时10分29秒
 * Revision: none
 * Compiler: gcc
 *
 * Author: David Fang (A free programmer), qi_fd@163.com
 * Company: nocompany
 *
 * =====================================================================================
 */

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "xproxy_server.hpp"

int main(int argc, char* argv[])
{
    try
    {
        if(5 != argc)
        {
            std::cerr<<"Usage: xproxy <local port> <server ip> <server_port> <[thread size>\n";
            std::cerr<<"local port: local port used to accept login client\n";
            std::cerr<<"server ip: analysing server address, ip string in decimal dot format\n";
            std::cerr<<"server port: analysing server port, an unsigned short value\n";
            std::cerr<<"thread size: number of threads to running xproxy server\n";
            return 1;
        }
        xproxy_server srv(atoi(argv[1]), argv[2], atoi(argv[3]), atoi(argv[4]));

        srv.run();
    }
    catch(std::exception& e)
    {
        std::cerr<<"exception: "<<e.what()<<"\n";
    }

    return 0;
}
