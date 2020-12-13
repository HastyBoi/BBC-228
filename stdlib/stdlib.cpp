#include <iostream>

extern "C"
{
	void __stdcall echoInt(int num)
	{
		setlocale(LC_ALL, "rus");
		std::cout << num << '\n';
	}

	void __stdcall echoStr(const char* str)
	{
		setlocale(LC_ALL, "rus");
		std::cout << str << '\n';
	}

	constexpr bool isNumber(char c) { return c >= '0' && c <= '9'; }

	int __stdcall charToInt(char c)
	{
		return static_cast<int>(c - '0');
	}

	int __stdcall parseInt(const char* str)
	{
		int number = 0;
		for (int i = 0; str[i] && isNumber(str[i]); ++i)
			number = (number * 10) + charToInt(str[i]);

		return number;
	}

	const char* __stdcall concat(const char* str1, const char* str2)
	{
		char* buffer = reinterpret_cast<char*>(calloc(strlen(str1) + strlen(str2) + 1, sizeof(char)));

		if (buffer == nullptr)
			return "error: no enough memory";

		for (int i = 0; str1[i]; ++i)
			buffer[i] = str1[i];

		for (int i = 0; str2[i]; ++i)
			buffer[i + strlen(str1)] = str2[i];

		return buffer;
	}
}