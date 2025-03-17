#ifndef EVENT_MACROS_H
#define EVENT_MACROS_H

#include "EventRegistry.h"

#ifndef REGISTER_EVENT
#define REGISTER_EVENT(eventType, handler) \
    inline static bool registered_##eventType = EventRegistry::registerHandler(#eventType, handler)
#endif

#endif