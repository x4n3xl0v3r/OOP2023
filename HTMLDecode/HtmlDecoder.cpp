#include "HtmlDecoder.h"

namespace htmlfn
{
char const HTMLStringDecoder::HTML_ENTITY_START = '&';
char const HTMLStringDecoder::HTML_ENTITY_END = ';';


HTMLStringDecoder::HTMLStringDecoder()
{
	replacements = {
		{ "&quot;", '"' },
		{ "&apos;", '\'' },
		{ "&lt;", '<' },
		{ "&gt;", '>' },
		{ "&amp;", '&' }
	};

	illegalSymbols = { '"', '\'', '<', '>', '&' };

	maxHtmlEntityLenght = 0;
	for (const auto& I : replacements)
	{
		maxHtmlEntityLenght = std::max(maxHtmlEntityLenght, I.first.size());
	}
}

bool HTMLStringDecoder::HtmlDecode(std::istream& inStream, std::ostream& outStream) const
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

bool HTMLStringDecoder::HtmlReplace(const std::string& line, std::string& decodedLine) const noexcept
{
	decodedLine.clear();

	std::string htmlEntity = "";
	char symbol;
	for (size_t currPos = 0; currPos < line.size();)
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

std::string HTMLStringDecoder::FetchHtmlEntity(const std::string& line, size_t pos) const noexcept
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
}