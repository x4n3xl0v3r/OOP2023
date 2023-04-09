#pragma once
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include "StringUtils.h"

using DictT = std::map<std::string, std::set<std::string>>;

class DictProvider
{
public:
	DictProvider(const std::string& translationsPath);
	
	void InitalizeTranslations(bool& wasError) noexcept;

	void SetTranslations(const std::string& word, const std::vector<std::string>& translations) noexcept;
	void SetFilePath(const std::string& newFilePath) noexcept;

	std::string GetFilePath() const noexcept;
	std::set<std::string> GetTranslations(const std::string& word) const noexcept;

	bool FlushTranslations(bool forcedFlush) const noexcept;
	bool GetIsTranslationsChanged() const noexcept;

private:
	static const std::string DELIMITER;

	mutable std::string m_translationsPath;
	mutable DictT m_translations;
	mutable std::set<std::string> m_reversedKeys;
	mutable std::map<std::string, std::string> m_normalizedKeys;

	bool m_isChanged;

	std::pair<std::string, std::string> GetNormalizedKey(const std::string& word) const noexcept;
	
	void AddTranslation(const std::string& word, const std::vector<std::string>& translations, bool reverse) noexcept;
	void SetTranslationsChanged() noexcept;
	void ReverseTranslation(const std::string& word, const std::vector<std::string>& translations) noexcept;
	void ReadDict(std::istream& inStream);
	inline void ParseDictFileLine(std::string& line, std::vector<std::string>& container) const noexcept;
};