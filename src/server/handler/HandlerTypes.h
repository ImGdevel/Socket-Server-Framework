#ifndef HANDLER_TYPES_H
#define HANDLER_TYPES_H

#include <functional>
#include <memory>
#include <string>
#include "../session/ClientSession.h"
#include "../messages/IMessage.h"
#include "ClientRequest.h"

using MessagePtr = std::unique_ptr<IMessage>;
using HandlerFunc = std::function<void(const ClientRequest&)>;

#endif
