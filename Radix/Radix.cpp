#include <cassert>
#include <iostream>
#include <string>

enum Errcodes
{
	ERR_OVERFLOW = 1,
	ERR_INVALID_CHAR = 2,
	ERR_INVALID_RADIX = 3
};

template <class Signed>
bool isMultiplyOverflowSigned(Signed a, Signed b)
{
	Signed c = 9223372036854775807;
	if ((b != 0) && (a > (c / b)))
		return true;
	return false;
}

template <class Signed>
bool isSumOverflowSigned(Signed a, Signed b)
{
	Signed c = 9223372036854775807;
	if (a > (c - b))
		return true;
	return false;
}

bool SkipMinus(const std::string& str)
{
	if (str.front() == '-')
	{
		return true;
	}
	return false;
}

long long StringToLLInt(const std::string& str, int radix, bool& wasError)
{
	if ((radix < 2) || (radix > 36))
	{
		wasError = true;
		return Errcodes::ERR_INVALID_RADIX;
	}

	bool isNegative = SkipMinus(str);

	int alphaStartCode = 'A';
	int digitStartCode = '0';
	long long wideRadix = radix;

	long long value = 0;
	long long base = 1;
	long long addPart;

	size_t negativeOffset = static_cast<size_t>(isNegative);
	std::string positiveString = str.substr(negativeOffset, str.size() - negativeOffset);

	long long currNum;
	for (auto strIter = positiveString.rbegin(); strIter != positiveString.rend(); ++strIter)
	{
		if (isalpha(*strIter))
		{
			currNum = static_cast<long long>(toupper(*strIter)) - alphaStartCode + 10;
		}
		else if (isdigit(*strIter))
		{
			currNum = static_cast<long long>(*strIter) - digitStartCode;
		}
		else
		{
			wasError = true;
			return Errcodes::ERR_INVALID_CHAR;
		}

		if (isMultiplyOverflowSigned(base, currNum))
		{
			wasError = true;
			return Errcodes::ERR_OVERFLOW;
		}
		addPart = base * currNum;

		if (isSumOverflowSigned(value, addPart))
		{
			wasError = true;
			return Errcodes::ERR_OVERFLOW;
		}

		value += addPart;
		base *= wideRadix;
	}
	return value;
}

std::string IntToString(long long n, int radix, bool& wasError)
{
	if ((radix < 2) || (radix > 36))
	{
		wasError = true;
		return "Invalid destination radix";
	}

	constexpr char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string result = "";
	while (n != 0)
	{
		size_t letterPos = abs(n % radix);
		n /= radix;
		result = alphabet[letterPos] + result;
	}
	return result;
}

long long ParseLL(const char* s)
{
	char* endp;
	long long result = _strtoi64(s, &endp, 10);
	return result;
}

int main(int argc, char** argv)
{
	assert(argc == 4);

	int srcNotation = static_cast<int>(ParseLL(argv[1]));
	int dstNotation = static_cast<int>(ParseLL(argv[2]));

	std::string value(argv[3]);

	bool wasError = false;
	auto decimalValue = StringToLLInt(value, srcNotation, wasError);

	if (wasError)
	{
		switch (decimalValue)
		{
		case Errcodes::ERR_INVALID_CHAR:
			std::cout << "Invalid input value\n";
			break;
		case Errcodes::ERR_INVALID_RADIX:
			std::cout << "Invalid source radix\n";
			break;
		case Errcodes::ERR_OVERFLOW:
			std::cout << "Overflow error\n";
			break;
		default:
			std::cout << "Undefined error\n";
		}
		return 0;
	}

	auto outputString = IntToString(decimalValue, dstNotation, wasError);
	std::cout << outputString << '\n';
}
