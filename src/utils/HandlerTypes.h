#ifndef HANDLER_TYPES_H
#define HANDLER_TYPES_H

#include <functional>
#include <memory>
#include <string>
#include "../server/session/ClientSession.h"

using HandlerFunc = std::function<void(const std::shared_ptr<ClientSession>&, const std::string&)>;

#endif
