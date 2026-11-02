// HTTP entrypoint for hosted deployment (Render, etc.).
// Listens on $PORT and answers POST /api/calculate by calling the same
// processRequest() the CLI uses — no subprocess, no stdin.

#include "httplib.h"

#include <cstdlib>
#include <iostream>
#include <string>

namespace numericore {
    std::string processRequest(const std::string& jsonInput);
}

static void addCors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

int main() {
    httplib::Server server;

    // CORS preflight for the browser frontend.
    server.Options("/api/calculate", [](const httplib::Request&, httplib::Response& res) {
        addCors(res);
        res.status = 204;
    });

    // Main endpoint: request body -> processRequest -> JSON response.
    server.Post("/api/calculate", [](const httplib::Request& req, httplib::Response& res) {
        addCors(res);
        res.set_content(numericore::processRequest(req.body), "application/json");
    });

    // Health check (Render pings this).
    server.Get("/", [](const httplib::Request&, httplib::Response& res) {
        addCors(res);
        res.set_content("NumeriCore backend OK\n", "text/plain");
    });

    const char* portEnv = std::getenv("PORT");
    const int port = portEnv ? std::atoi(portEnv) : 8080;

    std::cerr << "NumeriCore server listening on 0.0.0.0:" << port << std::endl;

    if (!server.listen("0.0.0.0", port)) {
        std::cerr << "Failed to bind port " << port << std::endl;
        return 1;
    }

    return 0;
}
