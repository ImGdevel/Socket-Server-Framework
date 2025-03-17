#ifndef EXAMPLE_EVENT_HANDLER_H
#define EXAMPLE_EVENT_HANDLER_H

#include "EventMacros.h"
#include "ClientRequest.h"
#include "Logger.h"

#include "IEventHandler.h"
#include "JsonMessage.h"
#include "ClientRequest.h"
#include <unordered_map>

#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <rapidjson/document.h>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <iostream>

#include <string>

using namespace std;
using namespace rapidjson;


class ExampleEventHandler {
public:

    // 또는 일반 멤버 함수로 정의
    static void handleChat(const ClientRequest& request) {
        Logger::info("Handling chat event");
        const auto& session = request.getSession();
        const auto& message = request.getMessage();
        if (session) {
            auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
            if (jsonMessage) {
                string content = "chat response!";
                string jsonResponse = createJsonResponse("MSG", content);
                Logger::info("send message : " + jsonResponse);
                session->sendMessage(jsonResponse);
            }
        }
    }

    static void handleLogin(const ClientRequest& request) {
        Logger::info("Handling login event");
        const auto& session = request.getSession();
        const auto& message = request.getMessage();
        if (session) {
            auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
            if (jsonMessage) {
                string content = "login response!";
                string jsonResponse = createJsonResponse("MSG", content);
                Logger::info("send message : " + jsonResponse);
                session->sendMessage(jsonResponse);
            }
        }
    }

    static void handleDelay(const ClientRequest& request) {
        Logger::info("Handling delay event");
        const auto& session = request.getSession();
        const auto& message = request.getMessage();
        if (session) {
            auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
            if (jsonMessage) {
                string content = "delay response!";
                string jsonResponse = createJsonResponse("MSG", content);
                Logger::info("send message : " + jsonResponse);
                session->sendMessage(jsonResponse);
            }
        }
    }

    static void handleTask(const ClientRequest& request) {
        Logger::info("Handling task event");
        const auto& session = request.getSession();
        const auto& message = request.getMessage();
        if (session) {
            auto jsonMessage = dynamic_cast<JSONMessageRapid*>(message.get());
            if (jsonMessage) {
                string content = "task response!";
                string jsonResponse = createJsonResponse("MSG", content);
                Logger::info("send message : " + jsonResponse);
                session->sendMessage(jsonResponse);
            }
        }
    }

    static void handleEcho(const ClientRequest& request) {
        Logger::info("Handling echo event");
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

    ExampleEventHandler() {
        // Fluent API 방식으로 등록
        EventRegistry::registerHandlers()
            .on("CHAT", handleChat)
            .on("LOGIN", handleLogin)
            .on("DELAY", handleDelay)
            .on("TASK", handleTask)
            .on("ECHO", handleEcho);
    }

    static string createJsonResponse(const string& type, const string& content) {
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

};

#endif 