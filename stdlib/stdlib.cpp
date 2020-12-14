#include <iostream>

extern "C"
{
	void __stdcall _echoInt(int num)
	{
		setlocale(LC_ALL, "rus");
		std::cout << num << '\n';
	}

	void __stdcall _echoStr(const char* str)
	{
		setlocale(LC_ALL, "rus");
		std::cout << str << '\n';
	}

	constexpr bool _isNumber(char c) { return c >= '0' && c <= '9'; }

	int __stdcall _charToInt(char c)
	{
		return static_cast<int>(c - '0');
	}

	int __stdcall _parseInt(const char* str)
	{
		int number = 0;
		for (int i = 0; str[i] && _isNumber(str[i]); ++i)
			number = (number * 10) + _charToInt(str[i]);

		return number;
	}

	const char* __stdcall _concat(const char* str1, const char* str2)
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