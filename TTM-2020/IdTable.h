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
			int idxFirstLE;
			std::string scope;
			std::string id;
			it::data_type dataType;
			it::id_type type;
			union
			{
				int intValue;
				struct
				{
					char length;
					char string[TI_STR_MAXSIZE - 1];
				} strValue;
			} value;
		};

	private:
		std::map<std::string, Entry> table;
		size_t size;
	};
}