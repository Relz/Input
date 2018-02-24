#include "../StreamPosition/StreamPosition.h"
#include "BaseSettings.h"
#include "MatrixSettings.h"
#include "ReadLimit.h"
#include "ReadVectorMethod.h"
#include "VectorSettings.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>
#include <unordered_set>

class Input
{
public:
	explicit Input(std::istream & is)
		: m_is(is)
	{
		if (IsEndOfStream())
		{
			throw std::invalid_argument("Stream is empty");
		}
		m_lastPosition = CalculateStreamLastPosition();
	}

	explicit Input(std::string fileName)
		: m_fileName(std::move(fileName))
	{
		m_inputFile.open(m_fileName);
		if (!m_inputFile.good())
		{
			throw std::invalid_argument("File \"" + m_fileName + "\" doesn't exists");
		}
		if (IsEndOfStream())
		{
			throw std::invalid_argument("File \"" + m_fileName + "\" is empty");
		}
		m_lastPosition = CalculateStreamLastPosition();
	}

	std::string const& GetFileName() const { return m_fileName; }

	bool SkipLine()
	{
		while (ReadChar(false)) {}
		return ReadEndOfLine() && !IsEndOfStream();
	}

	bool SkipLines(size_t count)
	{
		while (count != 0)
		{
			if (!SkipLine())
			{
				return false;
			}
			--count;
		}
		return true;
	}

	bool SkipCharacters(std::unordered_set<char> const& characters)
	{
		bool result = false;
		while (FindDelimiter(characters))
		{
			ReadChar(true);
			result = true;
		}
		return result;
	}

	template<typename T> bool SkipArguments(size_t count)
	{
		while (count != 0)
		{
			if (!SkipArgument<T>())
			{
				return false;
			}
			--count;
		}
		return true;
	}

	template<typename T> bool SkipArgument()
	{
		T tempArgument;
		return ReadArguments(tempArgument);
	}

	template<typename... Targs> bool ReadArguments(Targs &... args) { return ReadArgumentsFromStream(args...); }

	template<typename T> bool ReadVector(
		std::vector<T> & vect,
		VectorSettings<T> const& settings = VectorSettings<T>()
	)
	{
		return ReadVectorBase<T, T>(vect, settings);
	}

	template<typename TReadElement, typename TVectorElement>
	bool ReadVector(
		std::vector<TVectorElement> & vect,
		VectorSettings<TVectorElement> const& settings = VectorSettings<TVectorElement>()
	)
	{
		return ReadVectorBase<TReadElement>(vect, settings);
	}

	bool ReadVector(
		std::vector<bool> & vect,
		VectorSettings<bool> const& settings = VectorSettings<bool>()
	)
	{
		return ReadVectorBase<char>(vect, settings);
	}

	template<typename T>
	bool ReadMatrix(
		std::vector<std::vector<T>> & matrix,
		MatrixSettings const& matrixSettings = MatrixSettings(),
		VectorSettings<T> const& vectorSettings = VectorSettings<T>()
	)
	{
		std::vector<std::vector<T>> possibleMatrix;
		bool enoughRows = true;
		for (size_t i = 0; i < matrixSettings.GetReadLimit(); ++i)
		{
			SkipCharacters(matrixSettings.GetSkipCharacters());
			BasePush(possibleMatrix, std::vector<T>(), matrixSettings.GetReadMethod());
			size_t activeContainerIndex = GetActiveContainerIndex(possibleMatrix, matrixSettings.GetReadMethod());
			if (!ReadVector(possibleMatrix[activeContainerIndex], vectorSettings))
			{
				if (possibleMatrix[activeContainerIndex].empty())
				{
					VectorPop(possibleMatrix, matrixSettings.GetReadMethod());
				}
				enoughRows = false;
				break;
			}
			SkipLine();
			while (ReadEndOfLine()) {}
		}
		bool isPossibleMatrixEmpty = possibleMatrix.empty();
		auto insertIterator = matrixSettings.GetReadMethod() == ReadVectorMethod::PUSH_BACK ? matrix.end() : matrix.begin();
		matrix.insert(insertIterator, std::make_move_iterator(possibleMatrix.begin()), std::make_move_iterator(possibleMatrix.end()));
		return !isPossibleMatrixEmpty && (enoughRows || matrixSettings.GetReadLimit() == ReadLimit::UNLIMITED);
	}

	bool Scan(
		std::vector<std::string> const& delimiters,
		std::string & scannedString,
		StreamPosition & scannedStringPosition,
		std::string & delimiter,
		StreamPosition & delimiterPosition
	)
	{
		std::string possibleScannedString;
		StreamPosition possibleScannedStringPosition = m_position;
		std::string possibleDelimiter;
		StreamPosition possibleDelimiterPosition;
		while (!IsEndOfStream())
		{
			if (FindDelimiter(delimiters, possibleDelimiter))
			{
				possibleDelimiterPosition = m_position;
				SkipArguments<char>(possibleDelimiter.length());
				break;
			}
			else
			{
				ReadCharToString(possibleScannedString, true);
			}
		}
		if (!possibleScannedString.empty() || !possibleDelimiter.empty())
		{
			scannedString = std::move(possibleScannedString);
			scannedStringPosition = possibleScannedStringPosition;
			delimiter = std::move(possibleDelimiter);
			delimiterPosition = possibleDelimiterPosition;
			return true;
		}
		return false;
	}

	bool IsEndOfStream() const { return m_is.peek() == std::ifstream::traits_type::eof(); }

	bool SkipUntilCharacters(std::unordered_set<char> const& characters, std::string & skippedString)
	{
		std::string possibleSkippedString;
		while (!FindDelimiter(characters) && !IsEndOfStream() && ReadCharToString(possibleSkippedString)) {}
		skippedString = std::move(possibleSkippedString);
		return !IsEndOfStream();
	}

	bool SkipUntilStrings(std::vector<std::string> const& strings, std::string & skippedString)
	{
		std::string delimiter;
		std::string possibleSkippedString;
		while (!FindDelimiter(strings, delimiter) && !IsEndOfStream() && ReadCharToString(possibleSkippedString, true)) {}
		skippedString = std::move(possibleSkippedString);
		return !delimiter.empty();
	}

	StreamPosition const& GetPosition() const { return m_position; }

private:
	template<typename T> bool ReadArgumentFromStream(T & arg)
	{
		if (!IsEndOfLine() && !IsEndOfStream() && m_is >> arg)
		{
			if (m_is.tellg() != -1)
			{
				m_position.SetColumn(m_is.tellg() + 1);
			}
			else
			{
				m_position.SetColumn(m_lastPosition);
			}
			return true;
		}
		return false;
	}

	bool ReadArgumentFromStream(char & arg) { return ReadChar(arg, true); }

	bool ReadArgumentsFromStream() { return true; }

	template<typename T, typename... Targs> bool ReadArgumentsFromStream(T & arg, Targs &... args)
	{
		return ReadArgumentFromStream(arg) && ReadArgumentsFromStream(args...);
	}

	template<typename T> void BasePush(std::vector<T> & vect, T const& elem, ReadVectorMethod const readVectorMethod)
	{
		if (readVectorMethod == PUSH_BACK)
		{
			vect.emplace_back(elem);
		}
		else // if (readVectorMethod == PUSH_FRONT)
		{
			vect.insert(vect.begin(), elem);
		}
	}

	template<typename T>
	bool VectorPush(std::vector<T> & vect, T const& elem, VectorSettings<T> const& settings = VectorSettings<T>())
	{
		T elemToPush = elem;
		if (!settings.GetRules().empty())
		{
			if (settings.GetRules().find(elem) == settings.GetRules().end())
			{
				return false;
			}
			elemToPush = settings.GetRules().at(elem);
		}
		BasePush(vect, elemToPush, settings.GetReadMethod());
		return true;
	}

	template<typename TReadElement, typename TVectorElement>
	bool VectorPush(
		std::vector<TVectorElement> & vect,
		TReadElement const& elem,
		VectorSettings<TVectorElement> const& settings = VectorSettings<TVectorElement>()
	)
	{
		if (!settings.GetRules().empty())
		{
			if (settings.GetRules().find(elem) == settings.GetRules().end())
			{
				return false;
			}
			TVectorElement elemToPush = settings.GetRules().at(elem);
			BasePush(vect, elemToPush, settings.GetReadMethod());
			return true;
		}
		return false;
	}

	bool VectorPush(
		std::vector<bool> & vect, char const elem, VectorSettings<bool> const& settings = VectorSettings<bool>())
	{
		if (settings.GetTrueChar() == NOT_A_CHARACTER && settings.GetRules().empty())
		{
			throw(std::invalid_argument("True char and rules are not specified"));
		}
		bool elemToPush;
		if (settings.GetTrueChar() != NOT_A_CHARACTER)
		{
			elemToPush = elem == settings.GetTrueChar();
		}
		else
		{
			if (settings.GetRules().find(elem) == settings.GetRules().end())
			{
				return false;
			}
			elemToPush = settings.GetRules().at(elem);
		}
		BasePush(vect, elemToPush, settings.GetReadMethod());
		return true;
	}

	template<typename T> void VectorPop(std::vector<T> & vect, ReadVectorMethod const readVectorMethod)
	{
		if (readVectorMethod == PUSH_BACK)
		{
			vect.pop_back();
		}
		else // if (readVectorMethod == PUSH_FRONT)
		{
			vect.erase(vect.begin());
		}
	}

	template<typename T> size_t GetActiveContainerIndex(
		std::vector<T> & vect, ReadVectorMethod const readVectorMethod)
	{
		if (readVectorMethod == PUSH_BACK)
		{
			return vect.size() - 1;
		}
		else // if (readVectorMethod == PUSH_FRONT)
		{
			return 0;
		}
	}

	template<typename TReadElement, typename TVectorElement>
	bool ReadVectorBase(std::vector<TVectorElement> & vect, VectorSettings<TVectorElement> const& settings)
	{
		std::vector<TVectorElement> possibleVect;
		bool result = false;
		std::unordered_set<char> const& stopCharacters = settings.GetStopCharacters();
		TReadElement elem;
		SkipCharacters(settings.GetSkipCharacters());
		while (possibleVect.size() != settings.GetReadLimit() && ReadArgumentFromStream(elem) && stopCharacters.find(elem) == stopCharacters.end())
		{
			if (!VectorPush(possibleVect, elem, settings))
			{
				result = false;
				break;
			}
			SkipCharacters(settings.GetSkipCharacters());
			result = true;
		}
		size_t possibleVectSize = possibleVect.size();
		auto insertIterator = settings.GetReadMethod() == ReadVectorMethod::PUSH_BACK ? vect.end() : vect.begin();
		vect.insert(insertIterator, std::make_move_iterator(possibleVect.begin()), std::make_move_iterator(possibleVect.end()));
		return result && (possibleVectSize == settings.GetReadLimit() || settings.GetReadLimit() == ReadLimit::UNLIMITED);
	}

	static const int ENDL_SYMBOL_CODE_LF = 10;
	static const int ENDL_SYMBOL_CODE_CR = 13;

	bool IsEndOfLine()
	{
		return m_is.peek() == ENDL_SYMBOL_CODE_CR || m_is.peek() == ENDL_SYMBOL_CODE_LF;
	}

	bool FindDelimiter(std::unordered_set<char> const& delimiters)
	{
		char delimiter;
		return FindDelimiter(delimiters, delimiter);
	}

	bool FindDelimiter(std::unordered_set<char> const& delimiters, char & result)
	{
		char nextCharacter = std::char_traits<char>::to_char_type(m_is.peek());
		if (delimiters.find(nextCharacter) == delimiters.end())
		{
			return false;
		}
		else
		{
			result = nextCharacter;
			return true;
		}
	}

	bool FindDelimiter(std::vector<std::string> const& delimiters, std::string & resultDelimiter)
	{
		bool result = false;
		StreamPosition savedPosition = m_position;
		for (std::string const& delimiter : delimiters)
		{
			std::string possibleDelimiter;
			if (ReadString(delimiter.length(), possibleDelimiter) && possibleDelimiter == delimiter)
			{
				if (IsEndOfStream())
				{
					m_is.clear();
				}
				m_is.seekg(-possibleDelimiter.length(), m_is.cur);
				resultDelimiter = std::move(possibleDelimiter);
				result = true;
				break;
			}
			if (!possibleDelimiter.empty())
			{
				if (IsEndOfStream())
				{
					m_is.clear();
				}
				m_is.seekg(-possibleDelimiter.length(), m_is.cur);
			}
		}
		m_position = savedPosition;
		return result;
	}

	bool ReadCharToString(std::string & str, bool readEndOfLine = true)
	{
		char ch;
		if (ReadChar(ch, readEndOfLine))
		{
			str.push_back(ch);
			return true;
		}
		return false;
	}

	bool ReadChar(bool readEndOfLine = true)
	{
		char ch;
		return ReadChar(ch, readEndOfLine);
	}

	bool ReadChar(char & ch, bool readEndOfLine = true)
	{
		if (readEndOfLine && ReadEndOfLine(ch))
		{
			return true;
		}
		else if (!IsEndOfLine() && !IsEndOfStream() && m_is.get(ch))
		{
			m_position.IncreaseColumn();
			return true;
		}
		return false;
	}

	long CalculateStreamLastPosition()
	{
		m_is.seekg(0, std::ios::end);
		long result = m_is.tellg();
		++result;
		m_is.seekg(0, std::ios::beg);
		return result;
	}

	bool ReadString(size_t const length, std::string & result)
	{
		std::string possibleResult;
		while (possibleResult.length() < length && ReadCharToString(possibleResult, true))
		{
		}
		if (possibleResult.length() == length)
		{
			result = std::move(possibleResult);
			return true;
		}
		if (IsEndOfStream())
		{
			m_is.clear();
		}
		m_is.seekg(-possibleResult.length(), m_is.cur);
		return false;
	}

	bool ReadEndOfLine()
	{
		char ch;
		return ReadEndOfLine(ch);
	}

	bool ReadEndOfLine(char & endOfLine)
	{
		if (m_is.peek() == ENDL_SYMBOL_CODE_CR || m_is.peek() == ENDL_SYMBOL_CODE_LF)
		{
			m_is.get(endOfLine);
			m_position.IncreaseLine();
			m_position.ResetColumn();
			return true;
		}
		return false;
	}

	std::string m_fileName;
	std::ifstream m_inputFile;
	std::istream & m_is = m_inputFile;

	long m_lastPosition;
	StreamPosition m_position;
};
