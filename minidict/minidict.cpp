#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <windows.h>
#include "DictProvider.h"

static const std::string STD_INPUT_TRANSLATIONS_SEPARATOR = ";";
static const std::string STD_OUTPUT_TRANSLATIONS_SEPARATOR = ", ";
static const std::string SHUTDOWN_KEYWORD = "...";
static const std::string STD_TRANSLATIONS_PATH = "dict.txt";

template <class T>
void PrintElements(const T& container, const std::string& separator, std::ostream& outStream)
{
	size_t ctr = 0;
	for (const auto& iter : container)
	{
		if (ctr != 0)
		{
			outStream << separator;
		}
		outStream << iter;
		++ctr;
	}
}

void HandleAddTranslation(
	std::istream& inStream, 
	std::ostream& outStream, 
	DictProvider& provider, 
	const std::string& translationKey
)
{
	outStream << "Unknown word ``" << translationKey << "``. Enter translation or empty string [deny translation]\n";

	std::string line;
	std::getline(inStream, line);
	strfn::trim(line);

	if (line.empty())
	{
		outStream << "Word ``" << translationKey << "`` ignored.\n";
		return;
	}

	std::vector<std::string> translations;
	strfn::SplitString(line, STD_INPUT_TRANSLATIONS_SEPARATOR, translations);

	provider.SetTranslations(translationKey, translations);
	
	outStream << "Word ``" << translationKey << "`` saved in dictionary with translations: ";
	PrintElements(translations, STD_OUTPUT_TRANSLATIONS_SEPARATOR, outStream);
	outStream << '\n';
}

bool HandleEndSession(std::istream& inStream, std::ostream& outStream, DictProvider& provider)
{
	if (provider.GetIsTranslationsChanged())
	{
		outStream << "Dictionary has been changed. Press `Y`/`y` to save changes.\n";
		
		char code;
		inStream >> code;

		if (tolower(code) == 'y')
		{
			if (provider.FlushTranslations(true))
			{
				outStream << "Changes saved. ";
			}
			else
			{
				std::cerr << "I/O failure: cannot write to resource file\n";
				return false;
			}
		}
		else
		{
			outStream << "Changes discarded. ";
		}
	}
	outStream << "Bye.\n";
	return true;
}

bool HandleRequest(std::istream& inStream, std::ostream& outStream, DictProvider& provider)
{
	std::string line;
	while (true)
	{
		std::getline(inStream, line);
		strfn::trim(line);
		
		if (line == SHUTDOWN_KEYWORD)
			return HandleEndSession(inStream, outStream, provider);
		
		auto translations = provider.GetTranslations(line);
		if (!translations.empty())
		{
			PrintElements(translations, STD_OUTPUT_TRANSLATIONS_SEPARATOR, outStream);
			outStream << '\n';
			continue;
		}

		HandleAddTranslation(inStream, outStream, provider, line);
	}
	return true;
}

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	setlocale(LC_ALL, ".1251");

	DictProvider dictProvider(STD_TRANSLATIONS_PATH);

	bool wasError = false;
	dictProvider.InitalizeTranslations(wasError);

	if (wasError)
		return -1;

	if (!HandleRequest(std::cin, std::cout, dictProvider))
		return -2;

	return 0;
}
