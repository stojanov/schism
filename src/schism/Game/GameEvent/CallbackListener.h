#pragma once

#include <phmap.h>
#include <type_traits>
#include <typeindex>
#include <any>
#include "schism/System/SyncQueue.h"
#include "schism/System/Log.h"

namespace Schism::GameEvent
{
    class CallbackListener
    {
        template<typename T>
        using EventCallback = std::function<void(T&&)>;

        using Queue = System::SyncQueue<std::any>;
    public:
        template<typename T>
        void RegisterGameEvent()
        {
            m_QueueMap[typeid(T)];
            m_CallbackMap[typeid(T)];
        }

        template<typename T>
        [[nodiscard]] bool GameEventRegistered()
        {
            return m_QueueMap.find(typeid(T)) != m_QueueMap.end();
        }

        template<typename T>
        void ListenGameEvent(EventCallback<T>&& eventFunction)
        {
            m_CallbackMap[typeid(T)] = std::move(eventFunction);
        }

        template<typename T>
        void Process()
        {
            if (auto queueIt = m_QueueMap.find(typeid(T)); queueIt != m_QueueMap.end())
            {
                if (auto callbackIt = m_CallbackMap.find(typeid(T)); callbackIt != m_CallbackMap.end())
                {
                    auto callback = std::any_cast<EventCallback<T>>(callbackIt->second);

                    queueIt->second.Drain(5, [&callback](std::any&& e)
                    {
                        auto event = std::any_cast<T>(e);
                        callback(event);
                    });
                }
                else
                {
                    SC_CORE_WARN("(CallbackListener::Process) EventCallback not registered");
                }
            }
            else
            {
                SC_CORE_WARN("(CallbackListener::Process) Event not registered");
            }
        }
    private:
        template<typename T>
        void ProduceGameEvent(T&& o)
        {
            if (auto i = m_QueueMap.find(typeid(T)); i != m_QueueMap.end())
            {
                i->second.Push(std::forward<T>(o));
                return;
            }
            SC_CORE_WARN("(CallbackListener) ProduceGameEvent: type not found");
        }

        template<typename T>
        void ProduceGameEvent(const T& o)
        {
            if (auto i = m_QueueMap.find(typeid(T)); i != m_QueueMap.end())
            {
                i->second.Push(o);
                return;
            }
            SC_CORE_WARN("(CallbackListener) ProduceGameEvent: type not found");
        }

        phmap::parallel_flat_hash_map<std::type_index, Queue> m_QueueMap;
        phmap::parallel_flat_hash_map<std::type_index, std::any> m_CallbackMap;
    };
}