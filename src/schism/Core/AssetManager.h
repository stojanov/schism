#pragma once

#include <entt/entt.hpp>

namespace Schism::Core
{
	template<typename ResourceType, typename ResourceLoader>
	class AssetManager
	{
	public:
		AssetManager()
		{
			m_Resource = {};
		}
		
		template<typename ...Args>
		entt::resource_handle<ResourceType> Load(const char* identifier, Args&& ...args)
		{
			return m_Resource.template load<ResourceLoader>(entt::hashed_string{ identifier }, std::forward<Args>(args)...);
		}

		entt::resource_handle<ResourceType> GetHandle(const char* identifier)
		{
			return m_Resource.handle(entt::hashed_string{ identifier });
		}
	private:
		entt::resource_cache<ResourceType> m_Resource;
	};
}