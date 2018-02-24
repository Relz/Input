#ifndef PROJECT_MATRIXSETTINGS_H
#define PROJECT_MATRIXSETTINGS_H

#include "ReadVectorMethod.h"
#include "ReadLimit.h"
#include "BaseSettings.h"

class MatrixSettings: public BaseSettings
{
public:
	MatrixSettings() = default;

	MatrixSettings(
			const std::unordered_set<char> & skipCharacters,
			const std::unordered_set<char> & stopCharacters,
			ReadVectorMethod readVectorMethod,
			size_t readLimit
	)
			: BaseSettings(skipCharacters, stopCharacters, readVectorMethod, readLimit)
	{}
};

#endif //PROJECT_MATRIXSETTINGS_H
