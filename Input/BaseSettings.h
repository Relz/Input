#ifndef PROJECT_BASESETTINGS_H
#define PROJECT_BASESETTINGS_H

#include <utility>
#include <unordered_set>
#include "ReadVectorMethod.h"
#include "ReadLimit.h"

class BaseSettings
{
public:
	BaseSettings()
			: m_skipCharacters({})
			, m_stopCharacters({})
			, m_readMethod(ReadVectorMethod::PUSH_BACK)
			, m_readLimit(ReadLimit::UNLIMITED)
	{}

	BaseSettings(
			std::unordered_set<char> skipCharacters,
			std::unordered_set<char> stopCharacters,
			ReadVectorMethod readMethod,
			size_t readLimit
	)
			: m_skipCharacters(std::move(skipCharacters))
			, m_stopCharacters(std::move(stopCharacters))
			, m_readMethod(readMethod)
			, m_readLimit(readLimit)
	{}

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
	ReadVectorMethod m_readMethod;
	size_t m_readLimit;
};

#endif //PROJECT_BASESETTINGS_H
