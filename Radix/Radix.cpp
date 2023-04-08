#include <cassert>
#include <iostream>
#include <string>

constexpr int MIN_RADIX = 2;
constexpr int MAX_RADIX = 36;

enum Errcodes
{
	OK = 0,
	ERR_OVERFLOW = 1,
	ERR_INVALID_CHAR = 2,
	ERR_INVALID_RADIX = 3,
};

template <class Signed>
bool IsMultiplyOverflowSigned(Signed a, Signed b)
{
	Signed c = 9223372036854775807; // Decimal 64bit signed int representation
	if ((b != 0) && (a > (c / b)))
	{
		return true;
	}
	return false;
}

template <class Signed>
bool IsSumOverflowSigned(Signed a, Signed b)
{
	Signed c = 9223372036854775807;
	if (a > (c - b))
	{
		return true;
	}
	return false;
}

bool IsNegative(const std::string& str)
{
	if (str.front() == '-')
	{
		return true;
	}
	return false;
}

template <class Signed>
Signed GetDigitalRepr(char digit, bool& err)
{
	err = false;
	int alphaStartCode = 'A';
	int digitStartCode = '0';

	if (isalpha(digit))
	{
		return static_cast<Signed>(toupper(digit) - alphaStartCode + 10);
	}
	else if (isdigit(digit))
	{
		return static_cast<Signed>(digit - digitStartCode);
	}
	else
	{
		return -1;
		err = true;
	}
}

long long StringToLLInt(const std::string& str, int radix, int& errorCode)
{
	errorCode = Errcodes::OK;

	if ((radix < MIN_RADIX) || (radix > MAX_RADIX))
	{
		errorCode = Errcodes::ERR_INVALID_RADIX;
		return -1;
	}

	bool isNegative = IsNegative(str);

	int alphaStartCode = 'A';
	int digitStartCode = '0';
	long long wideRadix = radix;

	long long value = 0;
	long long base = 1;
	long long addPart;

	size_t negativeOffset = static_cast<size_t>(isNegative);
	std::string positiveString = str.substr(negativeOffset, str.size() - negativeOffset);

	long long currNum;
	bool isErrored;
	for (auto strIter = positiveString.rbegin(); strIter != positiveString.rend(); ++strIter)
	{
		isErrored = false;
		currNum = GetDigitalRepr<long long>(*strIter, isErrored);

		if (isErrored || (currNum >= radix))
		{
			errorCode = Errcodes::ERR_INVALID_CHAR;
			return -1;
		}

		if (IsMultiplyOverflowSigned(base, currNum))
		{
			errorCode = Errcodes::ERR_OVERFLOW;
			return -1;
		}
		addPart = base * currNum;

		if (IsSumOverflowSigned(value, addPart))
		{
			errorCode = Errcodes::ERR_OVERFLOW;
			return -1;
		}

		value += addPart;
		base *= wideRadix;
	}

	if (isNegative)
	{
		value *= -1;
	}

	return value;
}

std::string IntToString(long long n, int radix, int& errorCode)
{
	errorCode = Errcodes::OK;

	if ((radix < MIN_RADIX) || (radix > MAX_RADIX))
	{
		errorCode = Errcodes::ERR_INVALID_RADIX;
		return "error";
	}

	constexpr char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string result = "";

	if (n == 0)
	{
		return "0";
	}

	bool isNegativeSign = false;
	if (n < 0)
	{
		isNegativeSign = true;
	}

	while (n != 0)
	{
		size_t letterPos = static_cast<size_t>(abs(n % radix));
		n /= radix;
		result = alphabet[letterPos] + result;
	}

	if (isNegativeSign)
	{
		result = '-' + result;
	}

	return result;
}

long long ParseLL(const char* s)
{
	char* endp;
	long long result = _strtoi64(s, &endp, 10);
	return result;
}

struct InputParatemers
{
	int srcNotation;
	int dstNotatoon;
	std::string value;
};

InputParatemers ParseArguments(int argc, char** argv)
{
	constexpr int ARGS_COUNT = 4;
	if (argc != ARGS_COUNT)
	{
		throw std::exception("Invalid argumsnts");
	}

	return InputParatemers{
		static_cast<int>(ParseLL(argv[1])),
		static_cast<int>(ParseLL(argv[2])),
		argv[3]
	};
}

bool CatchError(long long errCode)
{
	if (errCode != Errcodes::OK)
	{
		switch (errCode)
		{
		case Errcodes::ERR_INVALID_CHAR:
			std::cout << "Invalid input value\n";
			break;
		case Errcodes::ERR_INVALID_RADIX:
			std::cout << "Invalid radix\n";
			break;
		case Errcodes::ERR_OVERFLOW:
			std::cout << "Overflow error\n";
			break;
		default:
			std::cout << "Undefined error\n";
		}
		return true;
	}
	return false;
}

int main(int argc, char** argv)
{
	InputParatemers arguments = ParseArguments(argc, argv);

	int errorCode;
	auto number = StringToLLInt(
		arguments.value,
		arguments.srcNotation,
		errorCode);

	if (CatchError(errorCode))
	{
		return -1;
	}

	auto outputString = IntToString(number, arguments.dstNotatoon, errorCode);
	if (CatchError(errorCode))
	{
		return -1;
	}

	std::cout << outputString << '\n';
	return 0;
}
