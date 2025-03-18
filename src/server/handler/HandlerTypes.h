#ifndef HANDLER_TYPES_H
#define HANDLER_TYPES_H

#include "IMessage.h"
#include "ClientSession.h"
#include "ClientRequest.h"

#include <functional>
#include <memory>
#include <string>

using HandlerFunc = std::function<void(const ClientRequest&)>;

#endif
