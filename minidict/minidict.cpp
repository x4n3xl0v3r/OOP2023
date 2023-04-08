#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <windows.h>


using DictT = std::map<std::string, std::vector<std::string>>;

static const std::string STD_TRANSLATIONS_SEPARATOR = ", ";
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

void SplitString(std::string& line, const std::string& delimiter, std::vector<std::string>& container)
{
	container.clear();

	size_t currPos = 0;
	size_t offset = 0;

	std::string word;
	while (true)
	{
		word.clear();

		offset = line.find(delimiter, currPos);
		if (offset == std::wstring::npos)
			break;

		word.append(line.begin() + currPos, line.begin() + offset);

		container.push_back(word);
		currPos += offset - currPos + delimiter.size();
	}
	word.clear();
	word.append(line.begin() + currPos, line.begin() + line.size());
	container.push_back(word);
}

class DictProvider
{
public:
	DictProvider()
		: m_translationsPath(STD_TRANSLATIONS_PATH)
		, m_isChanged(false)
	{
		DELIMITER = ";";
	}

	DictProvider(const std::string& translationsPath)
		: m_translationsPath(translationsPath)
		, m_isChanged(false)
	{
		DELIMITER = ";";
	}

	std::vector<std::string>&& GetTranslations(const std::string& word) const noexcept
	{
		if (m_translations.find(word) != m_translations.end())
		{
			return std::move(m_translations.at(word));
		}
		return std::vector<std::string>();
	}

	void SetTranslations(const std::string& word, const std::vector<std::string>& translations) noexcept
	{
		if (m_translations.find(word) == m_translations.end())
		{
			m_translations[word].reserve(translations.size());
		}

		SetTranslationsChanged();

		std::copy(
			translations.begin(), 
			translations.end(),
			std::back_inserter(m_translations.at(word))
		);

		for (const auto& t: translations)
		{
			if (m_translations.find(t) == m_translations.end())
			{
				m_reversedKeys.insert(t);
			}
			m_translations[t].push_back(word);
		}
		
		for (auto I : m_reversedKeys)
		{
			std::cout << ",* " << I;
		}

		for (const auto& [trKey, trVec] : m_translations)
		{
			/*if (m_reversedKeys.contains(trKey))
			{
				continue;
			}*/

			std::cout << "<--> " << trKey;
			for (const auto& word : trVec)
			{
				std::cout << DictProvider::DELIMITER << word;
			}
			std::cout << '\n';
		}
	}
	
	std::string GetFilePath() const noexcept
	{
		return m_translationsPath;
	}

	void SetFilePath(const std::string &newFilePath) noexcept
	{
		m_translationsPath = newFilePath;
	}

	bool FlushTranslations(bool forcedFlush=false)
	{
		if (forcedFlush || m_isChanged)
		{
			std::ofstream translationsFile(m_translationsPath, std::ios::out);
			if (!translationsFile.is_open())
			{
				return false;
			}

			for (const auto& [trKey, trVec] : m_translations)
			{
				if (m_reversedKeys.contains(trKey))
				{
					continue;
				}

				translationsFile << trKey;
				for (const auto& word: trVec)
				{
					translationsFile << DictProvider::DELIMITER << word;
				}
				translationsFile << '\n';
			}
		}
	}

	bool GetIsTranslationsChanged() const noexcept
	{
		return m_isChanged;
	}

	void InitalizeTranslations(bool& wasError) noexcept
	{
		wasError = false;
		std::ifstream translationsFile(m_translationsPath, std::ios::in);

		if (!translationsFile.is_open())
		{
			wasError = true;
			std::cerr << "Cannot open resource file\n";
			return;
		}

		try
		{
			ReadDict(translationsFile);
		}
		catch (const std::runtime_error)
		{
			wasError = true;
			std::cerr << "Resource file parsing error\n";
		}
	}

private:
	mutable std::string m_translationsPath;
	mutable DictT m_translations;
	mutable std::set<std::string> m_reversedKeys;
	
	bool m_isChanged;
	std::string DELIMITER;

	void SetTranslationsChanged() noexcept
	{
		m_isChanged = true;
	}

	void ReadDict(std::istream& inStream)
	{
		std::vector<std::string> container;
		for (std::string line; std::getline(inStream, line);)
		{
			ParseDictFileLine(line, container);
			if (container.size() < 2)
			{
				throw std::runtime_error("Invalid translation dict format");
			}

			m_translations[container.front()].reserve(container.size() - 1);

			std::copy(
				container.begin() + 1,
				container.end(),
				std::back_inserter(m_translations[container.front()]));

			for (auto it = container.begin() + 1; it != container.end(); ++it)
			{
				if (m_translations.find(*it) == m_translations.end())
				{
					m_reversedKeys.insert(*it);
				}


				m_translations[*it].push_back(container.front());
			}
		}
	}

	inline void ParseDictFileLine(std::string& line, std::vector<std::string>& container) const noexcept
	{
		SplitString(line, DictProvider::DELIMITER, container);
	}
};

static inline void ltrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

static inline void rtrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(),
		s.end());
}

static inline void trim(std::string& s)
{
	rtrim(s);
	ltrim(s);
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
	trim(line);

	if (line.empty())
	{
		outStream << "Word ``" << translationKey << "`` ignored.\n";
		return;
	}

	std::vector<std::string> translations;
	SplitString(line, STD_TRANSLATIONS_SEPARATOR, translations);

	provider.SetTranslations(translationKey, translations);
	
	outStream << "Word ``" << translationKey << "`` saved in dictionary with translations: ";
	PrintElements(translations, STD_TRANSLATIONS_SEPARATOR, outStream);
	outStream << '\n';
}

void HandleRequest(std::istream& inStream, std::ostream& outStream, DictProvider& provider)
{
	std::string line;
	while (true)
	{
		std::getline(inStream, line);
		trim(line);

		if (line == SHUTDOWN_KEYWORD)
		{
			if (provider.GetIsTranslationsChanged())
			{
				char code;
				outStream << "Dictionary has been changed. Press `Y`/`y` to save changes.\n";
			
				inStream >> code;
				if (tolower(code) == 'y')
				{
					provider.FlushTranslations(true);
					outStream << "Changes saved. ";
				}
				else
				{
					outStream << "Changes discarded. ";
				}
			}
			outStream << "Bye.\n";
			return;
		}
		
		std::vector<std::string> translations = provider.GetTranslations(line);
		if (!translations.empty())
		{
			PrintElements(translations, ", ", outStream);
			outStream << '\n';
			continue;
		}

		HandleAddTranslation(inStream, outStream, provider, line);
	}
}

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	DictProvider dictProvider;

	bool wasError = false;
	dictProvider.InitalizeTranslations(wasError);

	if (wasError)
	{
		std::cerr << "Cannot initalize from resource file\n";
		return -1;
	}

	HandleRequest(std::cin, std::cout, dictProvider);
}
