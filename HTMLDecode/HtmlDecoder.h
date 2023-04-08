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
	HTMLStringDecoder() 
	{
		replacements = {
			{ "&quot;", '"' },
			{ "&apos;", '\'' },
			{ "&lt;", '<' },
			{ "&gt;", '>' },
			{ "&amp;", '&' }
		};

		illegalSymbols = { '"', '\'', '<', '>', '&' } ;

		maxHtmlEntityLenght = 0;
		for (const auto& I: replacements)
		{
			maxHtmlEntityLenght = std::max(maxHtmlEntityLenght, I.first.size());
		}
	}

	bool HtmlDecode(std::istream& inStream, std::ostream& outStream) const
	{
		std::string decodedLine = "";
		for (std::string line; std::getline(inStream, line);)
		{
			if (!HtmlReplace(line, decodedLine))
			{
				return false;
			}
			outStream << decodedLine;
		}

		return true;
	}

private:
	static const char HTML_ENTITY_START = '&';
	static const char HTML_ENTITY_END = ';';

	std::unordered_map<std::string, char> replacements;
	std::set<char> illegalSymbols;
	size_t maxHtmlEntityLenght;

	bool HtmlReplace(const std::string& line, std::string& decodedLine) const noexcept
	{
		decodedLine.clear();

		std::string htmlEntity = "";
		char symbol;
		for (size_t currPos = 0; currPos < line.size(); )
		{
			symbol = line[currPos];

			if (illegalSymbols.find(symbol) != illegalSymbols.end())
			{
				if (symbol != HTMLStringDecoder::HTML_ENTITY_START)
				{
					return false;
				}

				htmlEntity = FetchHtmlEntity(line, currPos);
				if (htmlEntity.empty())
				{
					return false;
				}

				decodedLine += replacements.at(htmlEntity);
				currPos += htmlEntity.size();
			}
			else
			{
				decodedLine += symbol;
				++currPos;
			}
		}

		return true;
	}

	std::string FetchHtmlEntity(const std::string& line, size_t pos) const noexcept
	{
		std::string htmlEntity;
		htmlEntity.reserve(maxHtmlEntityLenght + 1);

		size_t gain = std::min(line.size(), pos + maxHtmlEntityLenght);
		for (size_t i = pos; i < gain; ++i)
		{
			htmlEntity.push_back(line[i]);
			if (replacements.find(htmlEntity) != replacements.end())
			{
				return htmlEntity;
			}
		}

		return "";
	}
};
}