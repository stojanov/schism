#pragma once

#include <type_traits>
#include <condition_variable>
#include <mutex>
#include <typeindex>
#include <unordered_map>
#include <any>
#include "schism/System/SyncQueue.h"

namespace Schism::GameEvent
{
	class Bus;
	class Listener
	{
		using Queue = System::SyncQueue<std::any>;
	public:
		Listener() = default;
		virtual ~Listener() = default;

		template<typename T>
		void RegisterGameEvent()
		{
			m_QueueMap[typeid(T)];
		}

		template<typename T>
		bool GameEventRegistered()
		{
			return m_QueueMap.find(typeid(T)) != m_QueueMap.end();
		}

		template<typename T>
		T ListenGameEvent()
		{
			return std::any_cast<T>(m_QueueMap[typeid(T)].PeekPop());
		}
		template<typename... ArgsT>
		void Select()
		{
			
		}

		template<typename T>
		std::optional<T> ListenGameEventFor(std::chrono::milliseconds timeout)
		{
			Queue* queue = nullptr;
			{
				std::scoped_lock lck(m_Mutex);
				queue = &m_QueueMap[typeid(T)];
			}

			if (!queue)
			{
				return std::nullopt; // Log out error
			}
			const auto possibleEvent = queue->WaitForPeekPop(timeout);

			if (!possibleEvent)
			{
				return std::nullopt;
			}

			if ((*possibleEvent).type() != typeid(T))
			{
				return std::nullopt; // Log out error
			}

			return std::any_cast<T>(*possibleEvent);
		}

	private:
		template<typename T>
		void ProduceGameEvent(T&& o)
		{
			Queue* queue = nullptr;
			{
				std::scoped_lock lck(m_Mutex);
				queue = &m_QueueMap[typeid(T)];
			}

			if (!queue)
			{
				return; // Log out error
			}

			queue->Push(std::forward<T>(o));
		}

		template<typename T>
		void ProduceGameEvent(const T& e)
		{
			Queue* queue = nullptr;
			{
				std::scoped_lock lck(m_Mutex);
				queue = &m_QueueMap[typeid(T)];
			}

			if (!queue)
			{
				return; // Log out error
			}

			queue->Push(e);
		}

		std::unordered_map<std::type_index, Queue> m_QueueMap;
		std::mutex m_Mutex;

		friend Bus;
	};
}