#ifndef PROJECT_BASESETTINGS_H
#define PROJECT_BASESETTINGS_H

#include <utility>
#include <unordered_set>
#include "ReadVectorMethod.h"
#include "ReadLimit.h"

class BaseSettings
{
	friend class BaseSettingsBuilder;
public:
	BaseSettings() = default;

	BaseSettings & operator=(BaseSettings const& right) noexcept
	{
		if (this == &right)
		{
			return *this;
		}
		m_skipCharacters = right.m_skipCharacters;
		m_stopCharacters = right.m_stopCharacters;
		m_readMethod = right.m_readMethod;
		m_readLimit = right.m_readLimit;
		return *this;
	}

	BaseSettings & operator=(BaseSettings && right) noexcept
	{
		if (this == &right)
		{
			return *this;
		}
		m_skipCharacters = std::move(right.m_skipCharacters);
		m_stopCharacters = std::move(right.m_stopCharacters);
		m_readMethod = right.m_readMethod;
		m_readLimit = right.m_readLimit;
		return *this;
	}

	std::unordered_set<char> const& GetSkipCharacters() const
	{
		return m_skipCharacters;
	}

	std::unordered_set<char> const& GetStopCharacters() const
	{
		return m_stopCharacters;
	}

	ReadVectorMethod GetReadMethod() const
	{
		return m_readMethod;
	}

	size_t GetReadLimit() const
	{
		return m_readLimit;
	}

private:
	std::unordered_set<char> m_skipCharacters;
	std::unordered_set<char> m_stopCharacters;
	ReadVectorMethod m_readMethod = ReadVectorMethod::PUSH_BACK;
	size_t m_readLimit = ReadLimit::UNLIMITED;
};

class BaseSettingsBuilder
{
public:
	BaseSettingsBuilder() = default;

	BaseSettingsBuilder & SetSkipCharacters(std::unordered_set<char> skipCharacters)
	{
		m_baseSettings.m_skipCharacters = std::move(skipCharacters);
		return *this;
	}

	BaseSettingsBuilder & SetStopCharacters(std::unordered_set<char> stopCharacters)
	{
		m_baseSettings.m_stopCharacters = std::move(stopCharacters);
		return *this;
	}

	BaseSettingsBuilder & SetReadMethod(ReadVectorMethod readMethod)
	{
		m_baseSettings.m_readMethod = readMethod;
		return *this;
	}

	BaseSettingsBuilder & SetReadLimit(size_t readLimit)
	{
		m_baseSettings.m_readLimit = readLimit;
		return *this;
	}

	BaseSettings const& Build() const
	{
		return m_baseSettings;
	}

private:
	BaseSettings m_baseSettings;
};

#endif //PROJECT_BASESETTINGS_H
