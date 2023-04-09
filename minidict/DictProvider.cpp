#include "DictProvider.h"

std::string ToLower(const std::string& s)
{
	std::string sCopy;
	std::transform(s.begin(), s.end(), std::back_inserter(sCopy), tolower);

	return sCopy;
}

const std::string DictProvider::DELIMITER = ";";

DictProvider::DictProvider(const std::string& translationsPath)
	: m_translationsPath(translationsPath)
	, m_isChanged(false)
{}

std::set<std::string> DictProvider::GetTranslations(const std::string& word) const noexcept
{
	auto [normalizedKey, originalCaseKey] = GetNormalizedKey(word);

	if (m_translations.find(originalCaseKey) != m_translations.end())
	{
		return m_translations.at(originalCaseKey);
	}
	return std::set<std::string>();
}

void DictProvider::SetTranslations(const std::string& word, const std::vector<std::string>& translations) noexcept
{
	AddTranslation(word, translations, true);
	SetTranslationsChanged();
}

std::string DictProvider::GetFilePath() const noexcept
{
	return m_translationsPath;
}

void DictProvider::SetFilePath(const std::string& newFilePath) noexcept
{
	m_translationsPath = newFilePath;
}

bool DictProvider::FlushTranslations(bool forcedFlush = false) const noexcept
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
			translationsFile << trKey;
			for (const auto& word : trVec)
			{
				translationsFile << DictProvider::DELIMITER << word;
			}
			translationsFile << '\n';
		}
	}
	return true;
}

bool DictProvider::GetIsTranslationsChanged() const noexcept
{
	return m_isChanged;
}

void DictProvider::InitalizeTranslations(bool& wasError) noexcept
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

void DictProvider::SetTranslationsChanged() noexcept
{
	m_isChanged = true;
}

void DictProvider::ReverseTranslation(const std::string& word, const std::vector<std::string>& translations) noexcept
{
	for (const auto& tran : translations)
	{
		if (m_translations.find(tran) == m_translations.end())
		{
			m_reversedKeys.insert(tran);
		}

		auto [normalizedKey, originalCaseKey] = GetNormalizedKey(tran);
		m_translations[originalCaseKey].insert(word);
	}
}

void DictProvider::AddTranslation(const std::string& word, const std::vector<std::string>& translations, bool reverse) noexcept
{
	auto [normalizedKey, originalCaseKey] = GetNormalizedKey(word);
	if (m_reversedKeys.contains(originalCaseKey))
		m_reversedKeys.erase(originalCaseKey);

	std::copy(
		translations.begin(),
		translations.end(),
		std::inserter(m_translations[originalCaseKey], m_translations[originalCaseKey].end()));

	if (reverse)
		ReverseTranslation(originalCaseKey, translations);
}

void DictProvider::ReadDict(std::istream& inStream)
{
	std::vector<std::string> container;

	std::string word;
	std::vector<std::string> translations;
	for (std::string line; std::getline(inStream, line);)
	{
		ParseDictFileLine(line, container);
		if (container.size() < 2)
		{
			throw std::runtime_error("Invalid resource file format");
		}

		word = container.front();
		translations.clear();
		std::copy(container.begin() + 1, container.end(), std::back_inserter(translations));

		AddTranslation(word, translations, false);
	}
}

inline void DictProvider::ParseDictFileLine(std::string& line, std::vector<std::string>& container) const noexcept
{
	strfn::SplitString(line, DictProvider::DELIMITER, container);
}

std::pair<std::string, std::string> DictProvider::GetNormalizedKey(const std::string& word) const noexcept
{
	std::pair<std::string, std::string> ret;  // normalizedKey, originalCaseKey
	ret.first = ToLower(word);

	if (m_normalizedKeys.find(ret.first) != m_normalizedKeys.end())
	{
		ret.second = m_normalizedKeys[ret.first];
	}
	else
	{
		ret.second = word;
		m_normalizedKeys[ret.first] = word;
	}
	return ret;
}