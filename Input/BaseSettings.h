#ifndef PROJECT_BASESETTINGS_H
#define PROJECT_BASESETTINGS_H

#include <vector>
#include "ReadVectorMethod.h"
#include "ReadLimit.h"

class BaseSettings
{
public:
	BaseSettings()
			: m_skipSymbols({})
			, m_readMethod(ReadVectorMethod::PUSH_BACK)
			, m_readLimit(ReadLimit::UNLIMITED)
	{}

	BaseSettings(
			const std::vector<char> & skipSymbols,
			ReadVectorMethod readMethod,
			size_t readLimit
	)
			: m_skipSymbols(skipSymbols)
			, m_readMethod(readMethod)
			, m_readLimit(readLimit)
	{}

	std::vector<char> GetSkipSymbols() const
	{
		return m_skipSymbols;
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
	std::vector<char> m_skipSymbols;
	ReadVectorMethod m_readMethod;
	size_t m_readLimit;
};

#endif //PROJECT_BASESETTINGS_H
