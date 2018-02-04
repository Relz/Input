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
	explicit CInput(std::istream& is)
		: m_is(is)
	{
		if (IsEndOfStream())
		{
			throw std::invalid_argument("Stream is empty");
		}

		m_lastPosition = GetStreamLastPosition();
	}

	explicit CInput(std::string inputFileName)
		: m_inputFileName(std::move(inputFileName))
	{
		m_inputFile.open(inputFileName);
		if (!m_inputFile.good())
		{
			throw std::invalid_argument("File \"" + inputFileName + "\" doesn't exists");
		}
		if (IsEndOfStream())
		{
			throw std::invalid_argument("File \"" + inputFileName + "\" is empty");
		}

		m_lastPosition = GetStreamLastPosition();
	}

	std::string GetInputFileName() const { return m_inputFileName; }

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
		while (GetChar())
		{
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

	bool SkipSymbols(const std::vector<char>& symbols)
	{
		bool result = false;
		bool symbolSkipped = true;
		while (symbolSkipped)
		{
			symbolSkipped = false;
			for (char symbol : symbols)
			{
				if (m_is.peek() == std::char_traits<char>::to_int_type(symbol))
				{
					GetChar();
					symbolSkipped = true;
					result = true;
				}
			}
		}
		return result;
	}

	template <typename... Targs> bool ReadArguments(Targs&... args) { return GetArgumentsFromStream(args...); }

	template <typename T> bool ReadVector(std::vector<T>& vect, const VectorSettings<T>& settings = VectorSettings<T>())
	{
		return ReadVectorBase<T, T>(vect, settings);
	}

	template <class TReadElement, typename TVectorElement>
	bool ReadVector(std::vector<TVectorElement>& vect,
		const VectorSettings<TVectorElement>& settings = VectorSettings<TVectorElement>())
	{
		return ReadVectorBase<TReadElement>(vect, settings);
	}

	bool ReadVector(std::vector<bool>& vect, const VectorSettings<bool>& settings = VectorSettings<bool>())
	{
		return ReadVectorBase<char>(vect, settings);
	}

	template <typename T>
	bool ReadMatrix(std::vector<std::vector<T>>& matrix, const MatrixSettings& matrixSettings = MatrixSettings(),
		const VectorSettings<T>& vectorSettings = VectorSettings<T>())
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

	bool Scan(std::vector<std::string> delimiters, std::string& scannedString, StreamPosition& scannedStringPosition,
		std::string& delimiter, StreamPosition& delimiterPosition)
	{
		scannedStringPosition = StreamPosition();
		delimiterPosition = StreamPosition();
		std::string result;
		if (IsEndOfStream())
		{
			scannedString.clear();
			delimiter.clear();
			return false;
		}
		scannedStringPosition.SetLine(m_position.GetLine());
		scannedStringPosition.SetColumn(m_position.GetColumn());
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

	bool SkipUntilSymbols(const std::vector<char>& symbols, std::string& skippedString)
	{
		std::string possibleSkippedString;
		bool symbolReached = false;
		while (!symbolReached)
		{
			for (char symbol : symbols)
			{
				if (m_is.peek() == std::char_traits<char>::to_int_type(symbol))
				{
					skippedString = std::move(possibleSkippedString);
					symbolReached = true;
					break;
				}
			}
			if (!symbolReached)
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
		return symbolReached;
	}

	bool SkipUntilStrings(const std::vector<std::string>& strings, std::string& skippedString)
	{
		std::string possibleSkippedString;
		while (true)
		{
			std::string delimiter;
			long savedLine = m_position.GetLine();
			long savedColumn = m_position.GetColumn();
			if (FindDelimiter(strings, delimiter))
			{
				skippedString = std::move(possibleSkippedString);
				m_is.seekg(-delimiter.length(), m_is.cur);
				m_position.SetLine(savedLine);
				m_position.SetColumn(savedColumn);
				return true;
			}
			if (IsEndOfStream())
			{
				skippedString = std::move(possibleSkippedString);
				return false;
			}
			char ch;
			GetChar(ch);
			possibleSkippedString += ch;
		}
	}

	const StreamPosition& GetPosition() const { return m_position; }

private:
	template <class T> bool GetArgumentFromStream(T& arg)
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

	bool GetArgumentFromStream(char& arg) { return (!IsEndOfLine() && !IsEndOfStream() && GetChar(arg)); }

	bool GetArgumentsFromStream() { return true; }

	template <typename T, typename... Targs> bool GetArgumentsFromStream(T& arg, Targs&... args)
	{
		return GetArgumentFromStream(arg) && GetArgumentsFromStream(args...);
	}

	template <typename T> void BasePush(std::vector<T>& vect, const T& elem, const ReadVectorMethod readVectorMethod)
	{
		if (readVectorMethod == PUSH_BACK)
		{
			vect.emplace_back(elem);
		}
		else if (readVectorMethod == PUSH_FRONT)
		{
			vect.insert(vect.begin(), elem);
		}
	}

	template <typename T>
	bool VectorPush(std::vector<T>& vect, const T& elem, const VectorSettings<T>& settings = VectorSettings<T>())
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
	bool VectorPush(std::vector<TVectorElement>& vect, const TReadElement& elem,
		const VectorSettings<TVectorElement>& settings = VectorSettings<TVectorElement>())
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
		std::vector<bool>& vect, const char& elem, const VectorSettings<bool>& settings = VectorSettings<bool>())
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

	template <typename T> void VectorPop(std::vector<T>& vect, ReadVectorMethod readVectorMethod)
	{
		if (readVectorMethod == PUSH_BACK)
		{
			vect.pop_back();
		}
		else if (readVectorMethod == PUSH_FRONT)
		{
			vect.erase(vect.begin());
		}
	}

	template <typename T> size_t GetActiveContainerIndex(std::vector<T>& vect, ReadVectorMethod readVectorMethod)
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
	bool ReadVectorBase(std::vector<TVectorElement>& vect, const VectorSettings<TVectorElement>& settings)
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
			long savedLine = m_position.GetLine();
			long savedColumn = m_position.GetColumn();
			char nextSymbol;
			m_is.get(nextSymbol);
			if (m_is.peek() == _ENDL_SYMBOL_CODE_LF)
			{
				return true;
			}
			else
			{
				m_is.seekg(-1, std::ios::cur);
				m_position.SetLine(savedLine);
				m_position.SetColumn(savedColumn);
				return true;
			}
		}
		else if (m_is.peek() == _ENDL_SYMBOL_CODE_LF)
		{
			return true;
		}
		return false;
	}

	bool FindDelimiter(const std::vector<std::string>& delimiters, std::string& result)
	{
		long savedLine = m_position.GetLine();
		long savedColumn = m_position.GetColumn();
		for (const std::string& delimiter : delimiters)
		{
			std::string possibleDelimiter;
			bool found = true;
			char ch;
			while (possibleDelimiter.length() < delimiter.length() && !IsEndOfStream() && !IsEndOfLine() && GetChar(ch))
			{
				possibleDelimiter += ch;
				size_t index = possibleDelimiter.length() - 1;
				if (possibleDelimiter.at(index) != delimiter.at(index))
				{
					found = false;
					break;
				}
			}
			if (possibleDelimiter.length() != delimiter.length())
			{
				found = false;
			}
			if (found)
			{
				result = possibleDelimiter;
				return true;
			}
			if (!possibleDelimiter.empty())
			{
				m_is.seekg(-possibleDelimiter.length(), m_is.cur);
				m_position.SetLine(savedLine);
				m_position.SetColumn(savedColumn);
			}
		}
		result.clear();

		return false;
	}

	bool GetChar()
	{
		char ch;
		return GetChar(ch);
	}

	bool GetChar(char& ch)
	{
		bool isEndOfLine = IsEndOfLine();
		bool isEndOfStream = IsEndOfStream();
		bool result = !isEndOfLine && !isEndOfStream;
		if (isEndOfLine && !isEndOfStream)
		{
			m_position.IncreaseLine();
			m_position.ResetColumn();
		}
		if (m_is.get(ch) && !isEndOfLine)
		{
			m_position.IncreaseColumn();
		}
		return result;
	}

	long GetStreamLastPosition()
	{
		m_is.seekg(0, std::ios::end);
		long result = m_is.tellg();
		++result;
		m_is.seekg(0, std::ios::beg);
		return result;
	}

	std::string m_inputFileName;
	std::ifstream m_inputFile;
	std::istream& m_is = m_inputFile;

	long m_lastPosition;
	StreamPosition m_position;
};
