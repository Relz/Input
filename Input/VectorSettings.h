#ifndef PROJECT_VECTORSETTINGS_H
#define PROJECT_VECTORSETTINGS_H

#include <unordered_map>
#include "BaseSettings.h"
#include "Constant.h"

template <typename T>
class VectorSettings: public BaseSettings
{
public:
	char trueChar;
	std::unordered_map<char, T> rules;

	VectorSettings()
			: trueChar(NOT_A_CHARACTER)
			, rules(std::unordered_map<char, T>())
	{}

	VectorSettings(
			const std::vector<char> & skipSymbols,
			ReadVectorMethod readVectorMethod,
			size_t readLimit,
			char trueChar,
			const std::unordered_map<char, T> & rules
	)
			: BaseSettings(skipSymbols, readVectorMethod, readLimit)
			, trueChar(trueChar)
			, rules(rules)
	{}
};

#endif //PROJECT_VECTORSETTINGS_H
