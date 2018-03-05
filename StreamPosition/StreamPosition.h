class StreamPosition
{
public:
	explicit StreamPosition(long line = MIN_LINE, long column = MIN_COLUMN)
	{
		SafeSetLine(line);
		SafeSetColumn(column);
	}

	long GetLine() const
	{
		return m_line;
	}

	void SetLine(long value)
	{
		SafeSetLine(value);
	}

	long GetColumn() const
	{
		return m_column;
	}

	void SetColumn(long value)
	{
		SafeSetColumn(value);
	}

	void IncreaseLine(long value = 1)
	{
		SafeSetLine(m_line + value);
	}

	void IncreaseColumn(long value = 1)
	{
		SafeSetColumn(m_column + value);
	}

	void DecreaseLine(long value = 1)
	{
		SafeSetLine(m_line - value);
	}

	void DecreaseColumn(long value = 1)
	{
		SafeSetColumn(m_column - value);
	}

	void ResetColumn()
	{
		m_column = MIN_COLUMN;
	}

	bool operator==(const StreamPosition & other) const
	{
		return GetLine() == other.GetLine() && GetColumn() == other.GetColumn();
	}

	bool operator!=(const StreamPosition & other) const
	{
		return !(*this == other);
	}

private:
	static const long MIN_LINE = 1;
	static const long MIN_COLUMN = 1;

	long m_line;
	long m_column;

	void SafeSetLine(long value)
	{
		m_line = value >= MIN_LINE ? value : MIN_LINE;
	}

	void SafeSetColumn(long value)
	{
		m_column = value >= MIN_COLUMN ? value : MIN_COLUMN;
	}
};
