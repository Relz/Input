#ifndef PROJECT_VECTORSETTINGS_H
#define PROJECT_VECTORSETTINGS_H

#include <unordered_map>
#include "BaseSettings.h"
#include "Constant.h"

template <typename T>
class VectorSettings: public BaseSettings
{
public:
	VectorSettings()
			: m_trueChar(NOT_A_CHARACTER)
			, m_rules(std::unordered_map<char, T>())
	{}

	VectorSettings(
			const std::unordered_set<char> & skipCharacters,
			const std::unordered_set<char> & stopCharacters,
			ReadVectorMethod readVectorMethod,
			size_t readLimit,
			char trueChar,
			const std::unordered_map<char, T> & rules
	)
			: BaseSettings(skipCharacters, stopCharacters, readVectorMethod, readLimit)
			, m_trueChar(trueChar)
			, m_rules(rules)
	{}

	char GetTrueChar() const
	{
		return m_trueChar;
	}

	std::unordered_map<char, T> GetRules() const
	{
		return m_rules;
	}

private:
	char m_trueChar;
	std::unordered_map<char, T> m_rules;
};

#endif //PROJECT_VECTORSETTINGS_H
