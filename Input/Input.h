#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <limits>
#include <functional>
#include "ReadVectorMethod.h"
#include "ReadLimit.h"
#include "BaseSettings.h"
#include "VectorSettings.h"
#include "MatrixSettings.h"

class CInput
{
public:
	explicit CInput(std::istream & is)
			: m_is(is)
	{
		if (IsEndOfStream())
		{
			throw std::invalid_argument("Поток пуст");
		}
	}

	explicit CInput(const std::string & inputFileName)
	{
		m_inputFile.open(inputFileName);
		if (!m_inputFile.good())
		{
			throw std::invalid_argument("Файл \"" + inputFileName + "\" не существует");
		}
		if (IsEndOfStream())
		{
			throw std::invalid_argument("Файл \"" + inputFileName + "\" пуст");
		}
	}

	template<class T>
	bool SkipArgument()
	{
		T tempArgument;
		return ReadArguments(tempArgument);
	}

	template<class T>
	bool SkipArguments(size_t count)
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
		while (IsNotEndOfLine() && !IsEndOfStream())
		{
			m_is.get();
		}
		m_is.get();
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

	void SkipSymbols(const std::vector<char> & symbols)
	{
		bool symbolSkipped = true;
		while(symbolSkipped)
		{
			symbolSkipped = false;
			for (char symbol : symbols)
			{
				if (m_is.peek() == std::char_traits<char>::to_int_type(symbol))
				{
					m_is.get();
					symbolSkipped = true;
				}
			}
		}
	}

	template<typename... Targs>
	bool ReadArguments(Targs & ... args)
	{
		return GetArgumentsFromStream(args...);
	}

	template<typename T>
	bool ReadVector(std::vector<T> & vect, const VectorSettings<T> & settings = VectorSettings<T>())
	{
		return ReadVectorBase<T, T>(vect, settings);
	}

	template<class TReadElement, typename TVectorElement>
	bool ReadVector(std::vector<TVectorElement> & vect, const VectorSettings<TVectorElement> & settings = VectorSettings<TVectorElement>())
	{
		return ReadVectorBase<TReadElement>(vect, settings);
	}

	bool ReadVector(std::vector<bool> & vect, const VectorSettings<bool> & settings = VectorSettings<bool>())
	{
		return ReadVectorBase<char>(vect, settings);
	}

	template<typename T>
	bool ReadMatrix(std::vector<std::vector<T>> & matrix, const MatrixSettings & matrixSettings = MatrixSettings(), const VectorSettings<T> & vectorSettings = VectorSettings<T>())
	{
		bool enoughRows = true;
		for (size_t i = 0; i < matrixSettings.readLimit; ++i)
		{
			SkipSymbols(matrixSettings.skipSymbols);
			BasePush(matrix, std::vector<T>(), matrixSettings.readMethod);
			size_t activeContainerIndex = GetActiveContainerIndex(matrix, matrixSettings.readMethod);
			if (!ReadVector(matrix[activeContainerIndex], vectorSettings))
			{
				if (matrix[activeContainerIndex].empty())
				{
					VectorPop(matrix, matrixSettings.readMethod);
				}
				enoughRows = false;
				break;
			}
			SkipLine();
		}
		return !matrix.empty() && (enoughRows || matrixSettings.readLimit == ReadLimit::UNLIMITED);
	}

	bool Scan(std::string & scanned, std::string & delimiter, std::vector<std::string> delimiters)
	{
		std::string result;
		if (IsEndOfStream())
		{
			scanned = "";
			return false;
		}
		while (!IsEndOfStream())
		{
			if (FindDelimiter(delimiters, delimiter))
			{
				break;
			}
			else
			{
				char ch;
				if (ReadArguments(ch))
				{
					result += ch;
				}
				else
				{
					SkipLine();
				}
			}
		}
		scanned = result;
		return true;
	}

	bool IsEndOfStream()
	{
		return m_is.peek() == std::ifstream::traits_type::eof();
	}

private:
	template<class T>
	bool GetArgumentFromStream(T & arg)
	{
		return (IsNotEndOfLine() && !m_is.eof() && m_is >> arg);
	}

	bool GetArgumentFromStream(char & arg)
	{
		return (IsNotEndOfLine() && !m_is.eof() && m_is.get(arg));
	}

	bool GetArgumentsFromStream() { return true; }

	template<typename T, typename... Targs>
	bool GetArgumentsFromStream(T & arg, Targs & ... args)
	{
		return GetArgumentFromStream(arg) && GetArgumentsFromStream(args...);
	}

	template<typename T>
	void BasePush(std::vector<T> & vect, const T & elem, const ReadVectorMethod readVectorMethod)
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

	template<typename T>
	bool VectorPush(std::vector<T> & vect, const T & elem, const VectorSettings<T> & settings = VectorSettings<T>())
	{
		T elemToPush = elem;
		if (!settings.rules.empty())
		{
			if (settings.rules.find(elem) == settings.rules.end())
			{
				return false;
			}
			elemToPush = settings.rules.at(elem);
		}
		BasePush(vect, elemToPush, settings.readMethod);
		return true;
	}

	template<typename TReadElement, typename TVectorElement>
	bool VectorPush(std::vector<TVectorElement> & vect, const TReadElement & elem, const VectorSettings<TVectorElement> & settings = VectorSettings<TVectorElement>())
	{
		if (!settings.rules.empty())
		{
			if (settings.rules.find(elem) == settings.rules.end())
			{
				return false;
			}
			TVectorElement elemToPush = settings.rules.at(elem);
			BasePush(vect, elemToPush, settings.readMethod);
			return true;
		}
		return false;
	}

	bool VectorPush(std::vector<bool> & vect, const char & elem, const VectorSettings<bool> & settings = VectorSettings<bool>())
	{
		if (settings.trueChar == NOT_A_CHARACTER && settings.rules.empty())
		{
			throw(std::invalid_argument("True char and rules are not specified"));
		}
		bool elemToPush;
		if (settings.trueChar != NOT_A_CHARACTER)
		{
			elemToPush = elem == settings.trueChar;
		}
		else
		{
			if (settings.rules.find(elem) == settings.rules.end())
			{
				return false;
			}
			elemToPush = settings.rules.at(elem);
		}
		BasePush(vect, elemToPush, settings.readMethod);
		return true;
	}

	template<typename T>
	void VectorPop(std::vector<T> & vect, ReadVectorMethod readVectorMethod)
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

	template <typename T>
	size_t GetActiveContainerIndex(std::vector<T> & vect, ReadVectorMethod readVectorMethod)
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

	template<class TReadElement, typename TVectorElement>
	bool ReadVectorBase(std::vector<TVectorElement> & vect, const VectorSettings<TVectorElement> & settings)
	{
		bool result = false;
		TReadElement elem;
		while (vect.size() != settings.readLimit && GetArgumentFromStream(elem))
		{
			if (!VectorPush(vect, elem, settings))
			{
				result = false;
				break;
			}
			SkipSymbols(settings.skipSymbols);
			result = true;
		}
		return result && (vect.size() == settings.readLimit || settings.readLimit == ReadLimit::UNLIMITED);
	}

	static const int ENDL_SYMBOL_CODE_LF = 10;
	static const int ENDL_SYMBOL_CODE_CR = 13;

	bool IsNotEndOfLine()
	{
		if (m_is.peek() == ENDL_SYMBOL_CODE_CR)
		{
			char nextSymbol;
			m_is.get(nextSymbol);
			if (m_is.peek() == ENDL_SYMBOL_CODE_LF)
			{
				return false;
			}
			else
			{
				m_is.seekg(-1, std::ios::cur);
				return false;
			}
		}
		else if (m_is.peek() == ENDL_SYMBOL_CODE_LF)
		{
			return false;
		}
		return true;
	}

	bool FindDelimiter(const std::vector<std::string> & delimiters, std::string & result)
	{
		for (const std::string & delimiter : delimiters)
		{
			std::string possibleDelimiter;
			bool found = true;
			char ch;
			while (possibleDelimiter.length() < delimiter.length() && !m_is.eof() && m_is.get(ch))
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
			m_is.seekg(-possibleDelimiter.length(), m_is.cur);
		}
		result = "";

		return false;
	}

	std::ifstream m_inputFile;
	std::istream & m_is = m_inputFile;
};
