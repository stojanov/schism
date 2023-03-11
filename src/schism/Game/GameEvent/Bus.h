#pragma once

#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <typeindex>
#include <any>
#include <type_traits>
#include <memory>
#include "Listener.h"

namespace Schism::GameEvent
{
	class Bus
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
			static_assert(std::is_base_of_v<Listener, T>, "(Bus, AttachListener) T is not of base Listener type");

			m_Listeners.emplace_back(std::dynamic_pointer_cast<Listener>(listener));
		}
	private:
		std::vector<std::shared_ptr<Listener>> m_Listeners;
	};
}