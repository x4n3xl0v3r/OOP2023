#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <vector>

namespace iofn
{
template <class T>
bool StringParse(const std::string& str, T& value)
{
	std::stringstream stream;
	stream << str;
	stream >> value;

	if (stream.fail())
	{
		return false;
	}
	return true;
}

template <class T>
bool ReadVector(std::istream& inputStream, std::vector<T>& result, std::string terminator)
{
	if (terminator.empty())
	{
		throw std::exception("ReadVector: empty stream terminator");
	}

	result.clear();
	T value{};

	std::istream_iterator<std::string> iter(inputStream), end;
	for (; iter != end; ++iter)
	{
		if (*iter == terminator)
		{
			return true;
		}

		if (!StringParse<T>(*iter, value))
		{
			result.clear();
			return false;
		}
		result.push_back(value);
	}

	return true;
}

template <class T, class Traits>
void WriteVector(std::ostream& stream, const std::vector<T>& data, const Traits& delimiter, bool sorted = false)
{
	if (!sorted)
	{
		std::copy(data.begin(), data.end(), std::ostream_iterator<T>(stream, delimiter));
		return;
	}

	std::vector<T> auxContainer(data.size());
	std::copy(data.begin(), data.end(), auxContainer.begin());
	std::stable_sort(auxContainer.begin(), auxContainer.end());

	std::copy(auxContainer.begin(), auxContainer.end(), std::ostream_iterator<T>(stream, delimiter));
}
} // namespace iofn