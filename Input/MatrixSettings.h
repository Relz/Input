#ifndef PROJECT_MATRIXSETTINGS_H
#define PROJECT_MATRIXSETTINGS_H

#include "BaseSettings.h"
#include "ReadVectorMethod.h"

class MatrixSettings
{
	friend class MatrixSettingsBuilder;

public:
	MatrixSettings() = default;

	BaseSettings const & GetBaseSettings() const
	{
		return m_baseSettings;
	}

private:
	BaseSettings m_baseSettings;
};

class MatrixSettingsBuilder
{
public:
	MatrixSettingsBuilder() = default;

	MatrixSettings const & Build(BaseSettings const & baseSettings = BaseSettings())
	{
		m_matrixSettings.m_baseSettings = baseSettings;
		return m_matrixSettings;
	}

private:
	MatrixSettings m_matrixSettings;
};

#endif //PROJECT_MATRIXSETTINGS_H
