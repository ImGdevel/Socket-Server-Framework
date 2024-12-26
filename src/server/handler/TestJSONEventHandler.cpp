
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
        {"LOGIN", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onLogin(session, message); }},
        {"ECHO", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onEcho(session, message); }},
        {"DELAY", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onDelay(session, message); }},
        {"CHAT", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onChat(session, message); }},
        {"TASK", [this](const shared_ptr<ClientSession>& session, const MessagePtr& message) { this->onTask(session, message); }},
    };
}

void TestJSONEventHandler::onLogin(const shared_ptr<ClientSession>& session, const MessagePtr& message) const {
    if (session) {
        auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
        if (jsonMessage) {
            string content = "Login response!";
            string jsonResponse = createJsonResponse("MSG", content);
            string echoMessage = "MSG: " + jsonResponse;
            Logger::info("send message : " + echoMessage);
            session->sendMessage(jsonResponse);
        }
    }
    
}

void TestJSONEventHandler::onChat(const shared_ptr<ClientSession>& session, const MessagePtr& message) const {
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

void TestJSONEventHandler::onEcho(const shared_ptr<ClientSession>& session, const MessagePtr& message) const {
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

void TestJSONEventHandler::onDelay(const shared_ptr<ClientSession>& session, const MessagePtr& message) const {
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

void TestJSONEventHandler::onTask(const shared_ptr<ClientSession>& session, const MessagePtr& message) const {
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
