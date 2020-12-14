#pragma once

namespace TTM
{
	namespace in
	{
		enum { endl = '\n', comment = '/', delimiter = '|' };

		// T - ���������� ������, F - ������������, I - ������������, O - ��������, Q - ', S - ���������
		enum { T = 1024, F = 2048, I = 4096, O = 8192, Q = 16384, S = 32768 };

#define IN_CODE_TABLE \
			in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::delimiter,in::delimiter, in::F, in::F, in::I, in::F, in::F, \
			in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, \
			in::delimiter, in::F, in::F, in::F, in::F, in::O, in::F, in::Q, in::S, in::S, in::O, in::O, in::S, in::O, in::F, in::O, \
			in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::F, in::S, in::F, in::O, in::F, in::F, \
			in::F, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, \
			in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::F, in::F, in::F, in::F, in::F, \
			in::F, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, \
			in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::T, in::S, in::F, in::S, in::F, in::F, \
																															\
			in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, \
			in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, \
			in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, \
			in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, \
			in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, \
			in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, \
			in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, \
			in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F, in::F
	}

	class InputFileReader
	{
	public:
		size_t fileSize() const { return m_fileSize; }
		int linesCount() const { return m_linesCount; }
		int ignoredCharsCount() const { return m_ignoredCharsCount; }

		const std::string& fileText() const { return m_fileText; }

		int m_codeTable[256];

		InputFileReader();
		void read(const char* inFilePath);

		static std::vector<std::pair<std::string, int>> splitStringByDelimiter(std::string s, char delimiter);
	private:
		size_t m_fileSize;
		int m_linesCount;
		int m_ignoredCharsCount;
		std::string m_fileText;

		void writeDelimiter();
	};
}