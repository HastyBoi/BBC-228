#pragma once
#define ID_MAXSIZE		5
#define SCOPE_MAXSIZE	10
#define TI_MAXSIZE		4096
#define TI_INT_DEFAULT	0x00000000
#define TI_STR_DEFAULT	0x00
#define TI_NULLIDX		((int)0xffffffff)
#define TI_STR_MAXSIZE	255

namespace TTM
{
	namespace it
	{
		enum class data_type { i32, str };
		enum class id_type { variable, function, parameter, literal };
	}

	class IdTable
	{
	public:
		struct Entry
		{
			std::string name;
			std::string scope;
			int lexTableIndex;
			it::data_type dataType;
			it::id_type idType;
			union
			{
				int intValue;
				struct
				{
					char length;
					char string[TI_STR_MAXSIZE - 1];
				} strValue;
			} value;

			void setValue(int new_value);
			void setValue(const char* new_value);

			Entry() = default;
			Entry(std::string name, std::string scope, int lexTableIndex, it::id_type idType, int value);
			Entry(std::string name, std::string scope, int lexTableIndex, it::id_type idType, const char* value);
			Entry(std::string name, std::string scope, int lexTableIndex, it::data_type dataType, it::id_type idType, const char* value);
		};

		int getIdIndexByName(std::string scope, std::string name, it::id_type idType);
		int getLiteralIndexByValue(int value);
		int getLiteralIndexByValue(const char* value);

		int addEntry(const Entry& entry);

		int size() const { return m_table.size(); }

		const std::string dumpTable(size_t startIndex = 0, size_t endIndex = 0) const;

		const Entry& operator[](size_t index) const
		{
			return m_table[index];
		}

	private:
		std::vector<Entry> m_table;
	};
}