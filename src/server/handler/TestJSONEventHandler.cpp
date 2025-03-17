#include "TestJSONEventHandler.h"
#include "Logger.h"
#include "../chat/ChatRoomManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <rapidjson/document.h>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <iostream>

using namespace rapidjson;

using namespace std;

unordered_map<string, HandlerFunc> TestJSONEventHandler::createHandlers() const {
    return {
        {"LOGIN", [this](const ClientRequest& ClientRequest) { this->onLogin(ClientRequest); }},
        {"ECHO", [this](const ClientRequest& ClientRequest) { this->onEcho(ClientRequest); }},
        {"DELAY", [this](const ClientRequest& ClientRequest) { this->onDelay(ClientRequest); }},
        {"CHAT", [this](const ClientRequest& ClientRequest) { this->onChat(ClientRequest); }},
        {"TASK", [this](const ClientRequest& ClientRequest) { this->onTask(ClientRequest); }},
    };
}

void TestJSONEventHandler::onLogin(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
    if (session) {
        string echoMessage = "MSG: " + message->serialize();
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void TestJSONEventHandler::onChat(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
    if (session) {
        auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
        if (jsonMessage) {
            string content = "Chat response!";
            string jsonResponse = createJsonResponse("MSG", content);
            string echoMessage = "MSG: " + jsonResponse;
            Logger::info("send message : " + echoMessage);
            session->sendMessage(jsonResponse);
        }
    }
}

void TestJSONEventHandler::onEcho(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
    if (session) {
        auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
        if (jsonMessage) {
            string content = "Echo response!";
            string jsonResponse = createJsonResponse("MSG", content);
            string echoMessage = "MSG: " + jsonResponse;
            Logger::info("send message : " + echoMessage);
            session->sendMessage(jsonResponse);
        }
    }
}

void TestJSONEventHandler::onDelay(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
    if (session) {
        auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
        if (jsonMessage) {
            string content = "Delay response!";
            string jsonResponse = createJsonResponse("MSG", content);
            string echoMessage = "MSG: " + jsonResponse;
            Logger::info("send message : " + echoMessage);
            session->sendMessage(jsonResponse);
        }
    }
}

void TestJSONEventHandler::onTask(const ClientRequest& ClientRequest) const {
    const auto& session = ClientRequest.getSession();
    const auto& message = ClientRequest.getMessage();
    if (session) {
        auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
        if (jsonMessage) {
            string content = "Task response!";
            string jsonResponse = createJsonResponse("MSG", content);
            string echoMessage = "MSG: " + jsonResponse;
            Logger::info("send message : " + echoMessage);
            session->sendMessage(jsonResponse);
        }
    }
}

string TestJSONEventHandler::createJsonResponse(const string& type, const string& content) const {
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    writer.StartObject();
    writer.Key("type");
    writer.String(type.c_str());
    writer.Key("content");
    writer.StartObject();
    writer.Key("message");
    writer.String(content.c_str());
    writer.EndObject();
    writer.EndObject();

    return buffer.GetString();
}
