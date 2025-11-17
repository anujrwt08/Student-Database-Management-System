// backend.cpp
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio> // Include for std::to_string

#pragma comment(lib, "ws2_32.lib")

// URL-decode helper
std::string urldecode(const std::string &src) {
    std::string ret;
    char ch;
    int i, ii;
    for (i=0; i<src.length(); i++) {
        if (src[i] == '%') {
            sscanf(src.substr(i+1,2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i+2;
        } else if (src[i] == '+') {
            ret += ' ';
        } else {
            ret += src[i];
        }
    }
    return ret;
}

// Join entries into JSON array string
std::string makeJsonArray(const std::vector<std::string>& entries) {
    std::string s = "[";
    for (size_t i = 0; i < entries.size(); ++i) {
        s += entries[i];
        if (i + 1 < entries.size()) s += ",";
    }
    s += "]";
    return s;
}

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
        std::cerr << "socket() failed\n";
        closesocket(server);
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "bind() failed\n";
        closesocket(server);
        WSACleanup();
        return 1;
    }

    listen(server, 5);
    std::cout << "Backend running at http://localhost:8080\n";

    // initial sample data (JSON object strings)
    std::vector<std::string> students;
    students.push_back("{\"name\":\"Anuj\",\"roll\":1,\"age\":20,\"course\":\"B.Tech Cyber Security\"}");
    students.push_back("{\"name\":\"Riya\",\"roll\":2,\"age\":19,\"course\":\"BCA\"}");

    while (true) {
        SOCKET client = accept(server, NULL, NULL);
        if (client == INVALID_SOCKET) continue;

        char buffer[8192];
        int bytesReceived = recv(client, buffer, sizeof(buffer)-1, 0);
        if (bytesReceived <= 0) {
            closesocket(client);
            continue;
        }
        buffer[bytesReceived] = '\0';
        std::string request(buffer, bytesReceived);

        // Parse request line
        std::string firstLine;
        {
            size_t pos = request.find("\r\n");
            if (pos != std::string::npos) firstLine = request.substr(0, pos);
            else firstLine = request;
        }
        // e.g. "GET /students HTTP/1.1"
        std::string method, path;
        {
            size_t sp1 = firstLine.find(' ');
            if (sp1 != std::string::npos) {
                method = firstLine.substr(0, sp1);
                size_t sp2 = firstLine.find(' ', sp1 + 1);
                if (sp2 != std::string::npos)
                    path = firstLine.substr(sp1 + 1, sp2 - sp1 - 1);
            }
        }

        // Routing:
        if (method == "GET" && path.rfind("/students", 0) == 0) {
            std::string body = makeJsonArray(students);
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Length: " + std::to_string(body.size()) + "\r\n"
                "\r\n" +
                body;
            send(client, response.c_str(), (int)response.size(), 0);

        } else if (method == "GET" && path.rfind("/add?student=", 0) == 0) {
            // extract urlencoded json after /add?student=
            std::string enc = path.substr(strlen("/add?student="));
            std::string decoded = urldecode(enc);

            // decoded should be a JSON object string like {"id":"1","name":"X",...}
            // Basic validation: ensure it starts with { and ends with }
            if (!decoded.empty() && decoded.front() == '{' && decoded.back() == '}') {
                // push into students vector
                students.push_back(decoded);
            }

            // Return updated array
            std::string body = makeJsonArray(students);
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Length: " + std::to_string(body.size()) + "\r\n"
                "\r\n" +
                body;
            send(client, response.c_str(), (int)response.size(), 0);

        } else if (method == "GET" && path.rfind("/delete?roll=", 0) == 0) {
            // extract roll number after /delete?roll=
            std::string rollStr = path.substr(strlen("/delete?roll="));
            int rollToDelete = 0;
            try {
                rollToDelete = std::stoi(rollStr);
            } catch (...) {
                // Ignore invalid roll number
            }

            if (rollToDelete > 0) {
                // Find and remove the student with matching roll
                students.erase(
                    std::remove_if(students.begin(), students.end(),
                        [&rollToDelete](const std::string& studentJson) {
                            // FIX: Search for the roll number followed by a comma or closing brace to ensure exact match.
                            std::string search_comma = "\"roll\":" + std::to_string(rollToDelete) + ",";
                            std::string search_end = "\"roll\":" + std::to_string(rollToDelete) + "}";

                            return (studentJson.find(search_comma) != std::string::npos || studentJson.find(search_end) != std::string::npos);
                        }),
                    students.end());
            }

            // Return updated array
            std::string body = makeJsonArray(students);
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Length: " + std::to_string(body.size()) + "\r\n"
                "\r\n" +
                body;
            send(client, response.c_str(), (int)response.size(), 0);

        } else if (method == "OPTIONS") {
            // for preflight (if browser sends)
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
                "Access-Control-Allow-Headers: Content-Type\r\n"
                "Content-Length: 0\r\n"
                "\r\n";
            send(client, response.c_str(), (int)response.size(), 0);
        } else {
            std::string html = "<h1>SDMS Backend Active</h1><p>Use /students or /add?student=...</p>";
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Length: " + std::to_string(html.size()) + "\r\n"
                "\r\n" +
                html;
            send(client, response.c_str(), (int)response.size(), 0);
        }

        closesocket(client);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}