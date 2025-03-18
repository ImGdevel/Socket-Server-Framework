#include "MyEventHandler.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <rapidjson/document.h>
#include "IEventHandler.h"
#include "JsonMessage.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <iostream>
#include "IEventHandler.h"

using namespace rapidjson;
using namespace std;

void MyEventHandler::onLogin(const ClientRequest& request) const {
    const auto& session = request.getSession();
    const auto& message = request.getMessage();
    if (session) {
        string echoMessage = "MSG: " + message->serialize();
        Logger::info("send message : " + echoMessage);
        session->sendMessage(echoMessage);
    }
}

void MyEventHandler::onChat(const ClientRequest& request) const {
    const auto& session = request.getSession();
    const auto& message = request.getMessage();
    if (session) {
        auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
        if (jsonMessage) {
            string content = "Chat response!";
            string jsonResponse = createJsonResponse("MSG", content);
            Logger::info("send message : " + jsonResponse);
            session->sendMessage(jsonResponse);
        }
    }
}

void MyEventHandler::onEcho(const ClientRequest& request) const {
    const auto& session = request.getSession();
    const auto& message = request.getMessage();
    if (session) {
        auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
        if (jsonMessage) {
            string content = "Echo response!";
            string jsonResponse = createJsonResponse("MSG", content);
            Logger::info("send message : " + jsonResponse);
            session->sendMessage(jsonResponse);
        }
    }
}

void MyEventHandler::onDelay(const ClientRequest& request) const {
    const auto& session = request.getSession();
    const auto& message = request.getMessage();
    if (session) {
        auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
        if (jsonMessage) {
            string content = "Delay response!";
            string jsonResponse = createJsonResponse("MSG", content);
            Logger::info("send message : " + jsonResponse);
            session->sendMessage(jsonResponse);
        }
    }
}

void MyEventHandler::onTask(const ClientRequest& request) const {
    const auto& session = request.getSession();
    const auto& message = request.getMessage();
    if (session) {
        auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
        if (jsonMessage) {
            string content = "Task response!";
            string jsonResponse = createJsonResponse("MSG", content);
            Logger::info("send message : " + jsonResponse);
            session->sendMessage(jsonResponse);
        }
    }
}

string MyEventHandler::createJsonResponse(const string& type, const string& content) const {
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
