/*
 * deploy_manager.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: walnut
 */

#include <cstddef>
#include <iostream>
#include <string>
using namespace std;

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
namespace po = boost::program_options;
po::options_description desc("Allowed options");
po::variables_map vm;

#include "jscm.hpp"
#include "jsocket.hpp"

unsigned short SERVER_PORT;

//commands
//deploy, undeploy, redeploy(undeploy and deploy), start, stop, restart(stop and start)

//example
//deploy_manager -c -host xxx.xxx.xxx.xxx –p port_number -command "deploy:pkg_name_with_full_path:optional_configuration_file_with_full_path "
//deploy_manager -c -host xxx.xxx.xxx.xxx –p port_number -command "start:optional_hpcc_component_name"

// [TODO] parsing cli commands using dautils

void usage() {
    //printf("(server mode)\n");
    //printf("deploy_manager -s -p portnumber (default 6000)\n");
    //printf("\n(client mode)\n");
    //printf("deploy_manager -c\n");
    //printf("-host xxx.xxx.xxx.xxx -p port_number (default 6000)\n");
    //printf("-command [deploy, undeploy, redeploy]\n");
    //printf("-pkg pkg_name_with_full_path -conf optional_configuration_file_with_full_path");
    //printf("-command [start, stop, restart]\n");
    //printf("-component optional_hpcc_component_name\n");
    cout << desc << "\n";
}

int command = 0;
enum Commands 
{
    deploy = 0,
    undeploy,
    redeploy,
    start,
    stop,
    restart
};

static const char * CommandsStrings[] = {"deploy", "undeploy", "redeploy", "start", "stop", "restart"};

int serverFunc(int command){


}

int server() {
    cout<<"running as server... listening port "<<SERVER_PORT<<endl;
    ISocket *server;
    server = ISocket::udp_create(SERVER_PORT);
    // listen for request from client
    unsigned TIMEOUT = 100;
    int size = 0;
    while (true) {
        size = server->wait_read(TIMEOUT);
        // command arrives
        unsigned short command = 0;
        if (size > 0) {
            server->read(&command, server->avail_read());
            cout<<"Receive command: "<<CommandsStrings[command]<<endl;
            serverFunc(command);
        }
    }
    // parse request, and [TODO] check
    // process request, collect feedback
    // send response to client
    return 0;
}

int client() {
    cout<<"running as client"<<endl;
    unsigned short command = 0;
    // [TODO] check input
    if (!vm.count("host")) {
        cout<<"ERROR: please specify server host\n";
        return 1;
    }
    if (!vm.count("command")) {
        cout<<"ERROR: please specify command\n";
        return 1;
    } else {
       if (vm["command"].as<string>() == "deploy") command = deploy;
       if (vm["command"].as<string>() == "undeploy") command = undeploy;
       if (vm["command"].as<string>() == "redeploy") command = redeploy;
       if (vm["command"].as<string>() == "start") command = start;
       if (vm["command"].as<string>() == "stop") command = stop;
       if (vm["command"].as<string>() == "restart") command = restart;
    }
    // send request to server
    ISocket *client;
    client = ISocket::udp_connect(SERVER_PORT, vm["host"].as<string>().c_str());
    cout<<vm["host"].as<string>().c_str()<<vm["command"].as<string>()<<command<<endl;
    client->write((const void *) &command, sizeof(command));
    // get response from server
    return 0;
}

int main(int argc, char **argv) {
    // Declare the supported options.
    desc.add_options()
        ("help", "produce help message")
        ("mode", po::value<bool>(), "0 for client mode (c), 1 for server mode (s)")
        ("port", po::value<unsigned short>(&SERVER_PORT)->default_value(6000), "(c/s) server host port")
        ("host", po::value<string>(), "(c) server host ip")
        ("command", po::value<string>(), "(c) command [deploy, undeploy, redeploy, start, stop, restart]")
        ("component", po::value<string>(), "(c) optional_hpcc_component_name")
        ("pkg", po::value<string>(), "(c) pkg_name_with_full_path")
        ("conf", po::value<string>(), "(c) optional_configuration_file_with_full_path")
        ;

    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    if (vm.count("mode")) {
        if (vm["mode"].as<bool>()){
            server();
        } else {
            client();
        }
    } else {
        usage();
    }
    return 0;
}



