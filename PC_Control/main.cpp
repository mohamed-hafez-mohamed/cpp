#include <cstdint>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "Server.hpp"
#include "PCControl.hpp"

using namespace App;
constexpr int PORT{8080};                          ///< Port number for the server
const std::string SERVER_IP{"192.168.1.11"};    ///< IP address for the server

void runServer(Server& server, uint32_t interval_ms)
{
    while (true)
    {
        try
        {
            auto start = std::chrono::steady_clock::now();
            server.saveClientRequests();
            // Sleep until next cycle
            std::this_thread::sleep_until(start + std::chrono::milliseconds(interval_ms));
        } catch (const std::exception& e) {
            std::cerr << "Server thread error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void runApp(Server& server, PCControl& pcControl, uint32_t interval_ms)
{
    while (true)
    {
        try
        {
            auto start = std::chrono::steady_clock::now();
            std::cout << "Checking for new requests..." << std::endl;
            // Get the next request if available and process it
            std::string request = server.getNextRequest();
            if(!request.empty())
            {
                std::cout << "Processing request: " << request << std::endl;
                pcControl.handleRequest(request);
            }
            else
            {
                std::cout << "No requests to process." << std::endl;
            }
            // Sleep until next cycle
            std::this_thread::sleep_until(start + std::chrono::milliseconds(interval_ms));
        } catch (const std::exception& e) {
            std::cerr << "App thread error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

int main()
{
    try {
        Server server(PORT);
        PCControl pcControl;

        std::cout << "Waiting for client connection..." << std::endl;
        server.acceptClientConnection();
        std::cout << "Client connected. Starting threads..." << std::endl;

        // Create the server thread
        auto serverThread = std::thread(runServer, std::ref(server), 500);
        // Wait a bit to ensure server thread is running
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // Create the app thread
        auto appThread = std::thread(runApp, std::ref(server), std::ref(pcControl), 1000);

        // Wait for threads to finish (they won't in this case unless there's an error)
        serverThread.join();
        appThread.join();
    } catch (const std::exception& e) {
        std::cerr << "Main thread error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
