#pragma once

#include <fstream>
#include <string>
#include <set>
#include <unordered_map>
#include <iostream>

namespace htmlfn
{

class HTMLStringDecoder
{
public:
	HTMLStringDecoder();
	bool HtmlDecode(std::istream& inStream, std::ostream& outStream) const;

private:
	static char const HTML_ENTITY_START;
	static char const HTML_ENTITY_END;

	std::unordered_map<std::string, char> replacements;
	std::set<char> illegalSymbols;
	size_t maxHtmlEntityLenght;

	bool HtmlReplace(const std::string& line, std::string& decodedLine) const noexcept;
	std::string FetchHtmlEntity(const std::string& line, size_t pos) const noexcept;
};
}