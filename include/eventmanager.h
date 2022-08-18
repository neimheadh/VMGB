#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <map>
#include <string>

typedef void (event_handler_t)(...);

class EventManager
{
public:
    EventManager();
    ~EventManager();


    /**
     * @brief Dispatch an event.
     * @param event The event.
     */
    void dispatch(const char *event);
    /**
     * @brief Dispatch an event.
     * @param event The event.
     * @param args  Event arguments.
     */
    void dispatch(const char *event, void *args...);


    /**
     * @brief Subscribe an event handler.
     * @param event   The event.
     * @param handler The handler.
     * @return The subscription id.
     */
    unsigned int subscribe(const char *event, event_handler_t *handler);
    /**
     * @brief Subscribe an event handler.
     * @param event   The event.
     * @param handler The handler.
     * @param args    Extended arguments.
     * @return The subscription id.
     */
    unsigned int subscribe(const char *event, event_handler_t *handler, void *args...);
    /**
     * @brief Unsubscribe an event handler.
     * @param event The event.
     * @param id    The subscription id.
     */
    void unsubscribe(const char *event, unsigned int id);

private:
    struct event_handler_s {
        event_handler_t *handler;
        void *args;
    };
    std::map<std::string, std::map<unsigned int, struct event_handler_s> *> _handlers;

    void _dispatch(const char *event, void *args);
    unsigned int _subscribe(const char *event, event_handler_t *handler, void *args);

    std::map<unsigned int, struct event_handler_s> *_map(const char *event);
};

#endif // EVENTDISPATCHER_H
