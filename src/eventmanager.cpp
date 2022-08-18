#include "eventmanager.h"

using namespace std;

EventManager::EventManager()
{

}

EventManager::~EventManager()
{
    for (const auto &[event, map] : _handlers) {
        delete map;
    }
}

void EventManager::dispatch(const char *event)
{
    _dispatch(event, nullptr);
}

void EventManager::dispatch(const char *event, void *args...)
{
    _dispatch(event, args);
}

unsigned int EventManager::subscribe(const char *event, event_handler_t *handler)
{
    return _subscribe(event, handler, nullptr);
}

unsigned int EventManager::subscribe(const char *event, event_handler_t *handler, void *args...)
{
    return _subscribe(event, handler, args);
}

void EventManager::unsubscribe(const char *event, unsigned int id)
{
    map<unsigned int, struct event_handler_s> *map = _map(event);

    if (map->count(id)) {
        map->erase(id);
    }
}

void EventManager::_dispatch(const char *event, void *args) {
    map<unsigned int, struct event_handler_s> *map = _map(event);

    for (const auto & [key, h] : *map) {
        if (h.args == nullptr && args == nullptr) {
            h.handler();
        } else if (h.args != nullptr && args == nullptr) {
            h.handler(h.args);
        } else if (h.args == nullptr && args != nullptr) {
            h.handler(args);
        } else {
            h.handler(h.args, args);
        }
    }
}

unsigned int EventManager::_subscribe(const char *event, event_handler_t *handler, void *args)
{
    map<unsigned int, struct event_handler_s> *map = _map(event);
    unsigned int i = map->empty() ? 1 : map->end()->first + 1;

    map->insert_or_assign(i, (struct event_handler_s) {.handler = handler, .args = args});

    return i;
}

map<unsigned int, struct EventManager::event_handler_s> *EventManager::_map(const char *event)
{
    if (_handlers.count(event) == 0) {
        _handlers[event] = new map<unsigned int, struct event_handler_s>;
    }

    return _handlers[event];
}
