#include <vector>
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

class CInput
{
public:
	explicit CInput(std::istream & is)
		: m_is(is)
	{
		if (IsEndOfStream())
		{
			throw std::invalid_argument("Stream is empty");
		}
		m_lastPosition = CalculateStreamLastPosition();
	}

	explicit CInput(std::string inputFileName)
		: m_inputFileName(std::move(inputFileName))
	{
		m_inputFile.open(m_inputFileName);
		if (!m_inputFile.good())
		{
			throw std::invalid_argument("File \"" + inputFileName + "\" doesn't exists");
		}
		if (IsEndOfStream())
		{
			throw std::invalid_argument("File \"" + inputFileName + "\" is empty");
		}
		m_lastPosition = CalculateStreamLastPosition();
	}

	std::string const& GetInputFileName() const { return m_inputFileName; }

	template <class T> bool SkipArgument()
	{
		T tempArgument;
		return ReadArguments(tempArgument);
	}

	template <class T> bool SkipArguments(size_t count)
	{
		T tempArgument;
		while (count != 0)
		{
			if (!ReadArguments(tempArgument))
			{
				return false;
			}
			--count;
		}
		return true;
	}

	bool SkipLine()
	{
		while (GetChar()) {}
		if (IsEndOfLine())
		{
			m_is.get();
			m_position.IncreaseLine();
			m_position.ResetColumn();
		}
		return !IsEndOfStream();
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

	bool SkipSymbols(std::vector<char> const& symbols)
	{
		bool result = false;
		bool symbolSkipped = true;
		while (symbolSkipped)
		{
			symbolSkipped = false;
			for (char const symbol : symbols)
			{
				if (m_is.peek() == std::char_traits<char>::to_int_type(symbol))
				{
					if (!GetChar())
					{
						SkipLine();
					}
					symbolSkipped = result = true;
				}
			}
		}
		return result;
	}

	template <typename... Targs> bool ReadArguments(Targs &... args) { return GetArgumentsFromStream(args...); }

	template <typename T> bool ReadVector(
		std::vector<T> & vect,
		VectorSettings<T> const& settings = VectorSettings<T>()
	)
	{
		return ReadVectorBase<T, T>(vect, settings);
	}

	template <class TReadElement, typename TVectorElement>
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

	template <typename T>
	bool ReadMatrix(
		std::vector<std::vector<T>> & matrix,
		MatrixSettings const& matrixSettings = MatrixSettings(),
		VectorSettings<T> const& vectorSettings = VectorSettings<T>()
	)
	{
		bool enoughRows = true;
		for (size_t i = 0; i < matrixSettings.GetReadLimit(); ++i)
		{
			SkipSymbols(matrixSettings.GetSkipSymbols());
			BasePush(matrix, std::vector<T>(), matrixSettings.GetReadMethod());
			size_t activeContainerIndex = GetActiveContainerIndex(matrix, matrixSettings.GetReadMethod());
			if (!ReadVector(matrix[activeContainerIndex], vectorSettings))
			{
				if (matrix[activeContainerIndex].empty())
				{
					VectorPop(matrix, matrixSettings.GetReadMethod());
				}
				enoughRows = false;
				break;
			}
			SkipLine();
		}
		return !matrix.empty() && (enoughRows || matrixSettings.GetReadLimit() == ReadLimit::UNLIMITED);
	}

	bool Scan(
		std::vector<std::string> const& delimiters,
		std::string & scannedString,
		StreamPosition & scannedStringPosition,
		std::string & delimiter,
		StreamPosition & delimiterPosition
	)
	{
		scannedStringPosition = delimiterPosition = StreamPosition();
		std::string result;
		if (IsEndOfStream())
		{
			scannedString.clear();
			delimiter.clear();
			return false;
		}
		scannedStringPosition = m_position;
		bool delimiterPositionFound = false;
		while (!IsEndOfStream())
		{
			if (FindDelimiter(delimiters, delimiter))
			{
				delimiterPositionFound = true;
				delimiterPosition.SetLine(m_position.GetLine());
				delimiterPosition.SetColumn(scannedStringPosition.GetColumn());
				break;
			}
			else
			{
				char ch;
				if (ReadArguments(ch))
				{
					scannedStringPosition.IncreaseColumn();
					result += ch;
				}
				else
				{
					scannedStringPosition.IncreaseLine();
					SkipLine();
				}
			}
		}
		scannedString = result;
		scannedStringPosition.DecreaseColumn(scannedString.length());
		if (!delimiterPositionFound)
		{
			delimiterPosition = scannedStringPosition;
		}
		return true;
	}

	bool IsEndOfStream() const { return m_is.peek() == std::ifstream::traits_type::eof(); }

	bool SkipUntilCharacters(std::vector<char> const& characters, std::string & skippedString)
	{
		std::string possibleSkippedString;
		while (true)
		{
			if (FindDelimiter(characters))
			{
				skippedString = std::move(possibleSkippedString);
				return true;
			}
			else
			{
				if (IsEndOfStream())
				{
					skippedString = std::move(possibleSkippedString);
					return false;
				}
				char ch;
				if (GetChar(ch))
				{
					possibleSkippedString += ch;
				}
			}
		}
	}

	bool SkipUntilStrings(std::vector<std::string> const& strings, std::string & skippedString)
	{
		std::string possibleSkippedString;
		while (true)
		{
			std::string delimiter;
			StreamPosition savedPosition = m_position;
			if (FindDelimiter(strings, delimiter))
			{
				skippedString = std::move(possibleSkippedString);
				if (IsEndOfStream())
				{
					m_is.clear();
				}
				m_is.seekg(-delimiter.length(), m_is.cur);
				m_position = savedPosition;
				return true;
			}
			if (IsEndOfStream())
			{
				skippedString = std::move(possibleSkippedString);
				return false;
			}
			char ch;
			GetChar(ch, true);
			possibleSkippedString += ch;
		}
	}

	StreamPosition const& GetPosition() const { return m_position; }

private:
	template <class T> bool GetArgumentFromStream(T & arg)
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

	bool GetArgumentFromStream(char & arg) { return GetChar(arg); }

	bool GetArgumentsFromStream() { return true; }

	template <typename T, typename... Targs> bool GetArgumentsFromStream(T & arg, Targs &... args)
	{
		return GetArgumentFromStream(arg) && GetArgumentsFromStream(args...);
	}

	template <typename T> void BasePush(std::vector<T> & vect, T const& elem, ReadVectorMethod const readVectorMethod)
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

	template <typename T>
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

	template <typename TReadElement, typename TVectorElement>
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

	template <typename T> void VectorPop(std::vector<T> & vect, ReadVectorMethod const readVectorMethod)
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

	template <typename T> size_t GetActiveContainerIndex(
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

	template <class TReadElement, typename TVectorElement>
	bool ReadVectorBase(std::vector<TVectorElement> & vect, VectorSettings<TVectorElement> const& settings)
	{
		bool result = false;
		TReadElement elem;
		while (vect.size() != settings.GetReadLimit() && GetArgumentFromStream(elem))
		{
			if (!VectorPush(vect, elem, settings))
			{
				result = false;
				break;
			}
			SkipSymbols(settings.GetSkipSymbols());
			result = true;
		}
		return result && (vect.size() == settings.GetReadLimit() || settings.GetReadLimit() == ReadLimit::UNLIMITED);
	}

	static const int _ENDL_SYMBOL_CODE_LF = 10;
	static const int _ENDL_SYMBOL_CODE_CR = 13;

	bool IsEndOfLine()
	{
		if (m_is.peek() == _ENDL_SYMBOL_CODE_CR)
		{
			StreamPosition savedPosition = m_position;
			char nextSymbol;
			m_is.get(nextSymbol);
			if (m_is.peek() == _ENDL_SYMBOL_CODE_LF)
			{
				return true;
			}
			else
			{
				m_is.seekg(-1, std::ios::cur);
				m_position = savedPosition;
				return true;
			}
		}
		else if (m_is.peek() == _ENDL_SYMBOL_CODE_LF)
		{
			return true;
		}
		return false;
	}

	bool FindDelimiter(std::vector<char> const& delimiters)
	{
		char delimiter;
		return FindDelimiter(delimiters, delimiter);
	}

	bool FindDelimiter(std::vector<char> const& delimiters, char & result)
	{
		for (char const delimiter : delimiters)
		{
			if (m_is.peek() == std::char_traits<char>::to_int_type(delimiter))
			{
				result = delimiter;
				return true;
			}
		}
		return false;
	}

	bool FindDelimiter(std::vector<std::string> const & delimiters, std::string & result)
	{
		StreamPosition savedPosition = m_position;
		for (std::string const & delimiter : delimiters)
		{
			std::string possibleDelimiter;
			if (GetString(delimiter.length(), possibleDelimiter) && possibleDelimiter == delimiter)
			{
				result = std::move(possibleDelimiter);
				return true;
			}
			if (!possibleDelimiter.empty())
			{
				if (IsEndOfStream())
				{
					m_is.clear();
				}
				m_is.seekg(-possibleDelimiter.length(), m_is.cur);
				m_position = savedPosition;
			}
		}
		return false;
	}

	bool GetChar()
	{
		char ch;
		return GetChar(ch);
	}

	bool GetChar(char & ch, bool readEndOfLine = false)
	{
		bool isEndOfLine = IsEndOfLine();
		bool isEndOfStream = IsEndOfStream();
		if (isEndOfLine)
		{
			if (readEndOfLine)
			{
				m_is.get(ch);
				m_position.IncreaseLine();
				m_position.ResetColumn();
				return true;
			}
		}
		else if (!isEndOfStream && m_is.get(ch))
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

	bool GetString(size_t const length, std::string & result)
	{
		std::string possibleResult;
		char ch;
		while (possibleResult.length() < length && GetChar(ch, true))
		{
			possibleResult += ch;
		}
		if (possibleResult.length() == length)
		{
			result = std::move(possibleResult);
			return true;
		}
		m_is.clear();
		m_is.seekg(-possibleResult.length(), m_is.cur);
		return false;
	}

	std::string m_inputFileName;
	std::ifstream m_inputFile;
	std::istream & m_is = m_inputFile;

	long m_lastPosition;
	StreamPosition m_position;
};
