#pragma once

#include <memory>
#include <vector>
#include "CallbackListener.h"

namespace Schism::GameEvent
{
    class CallbackBus
    {
    public:
        template<typename T>
        void PostEvent(T&& e)
        {
            for (auto& listener : m_Listeners)
            {
                if (!listener->GameEventRegistered<T>())
                {
                    continue;
                }

                listener->ProduceGameEvent(std::forward<T>(e));
            }
        }

        template<typename T>
        void PostEvent(const T& e)
        {
            for (auto& listener : m_Listeners)
            {
                if (!listener->GameEventRegistered<T>())
                {
                    continue;
                }

                listener->ProduceGameEvent(e);
            }
        }

        template<typename T>
        void AttachListener(const std::shared_ptr<T>& listener)
        {
            static_assert(std::is_base_of_v<CallbackListener, T>, "(SyncBus, AttachListener) T is not of base SyncListener type");

            m_Listeners.emplace_back(std::dynamic_pointer_cast<CallbackListener>(listener));
        }

        void AttachListener(const std::shared_ptr<CallbackListener>& listener)
        {
            m_Listeners.emplace_back(listener);
        }
    private:
        std::vector<std::shared_ptr<CallbackListener>> m_Listeners;
    };
}