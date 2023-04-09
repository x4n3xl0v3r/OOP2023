#pragma once
#include <string>
#include <algorithm>
#include <locale>
#include <cctype>
#include <iterator>

namespace strfn
{

template<class T>
void SplitString(std::string& line, const std::string& delimiter, T& container)
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

		container.insert(container.end(), word);
		currPos += offset - currPos + delimiter.size();
	}
	word.clear();
	word.append(line.begin() + currPos, line.begin() + line.size());
	container.insert(container.end(), word);
}

static inline void ltrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

static inline void rtrim(std::string& s)
{
	s.erase(
		std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !std::isspace(ch);
		}).base(), 
		s.end()
	);
}

static inline void trim(std::string& s)
{
	rtrim(s);
	ltrim(s);
}
}