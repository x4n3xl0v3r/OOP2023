#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

class StreamReplacer
{
public:
	StreamReplacer() {}
	StreamReplacer(const std::string& pattern, const std::string& replacement)
		: m_pattern(pattern)
		, m_replacement(replacement)
	{
	}

	void ReplaceInFile(const std::string& fInPath, const std::string fOutPath)
	{
		std::ifstream input(fInPath, std::ios::in);
		std::ofstream output(fOutPath, std::ios::out);

		assert(input.is_open());
		assert(output.is_open());

		std::string readData;
		while (std::getline(input, readData))
		{
			output << Replace(readData) << std::endl;
		}

		input.close();
		output.close();
	}

private:
	std::string m_pattern;
	std::string m_replacement;

	std::string Replace(const std::string& line)
	{
		std::string result = "";

		size_t offset = 0;
		size_t currPos = 0;
		while (true)
		{
			offset = line.find(m_pattern, currPos);
			if (offset == std::string::npos)
				break;

			result += line.substr(currPos, offset - currPos);
			result += m_replacement;

			currPos += offset - currPos + m_pattern.size();
		}
		result += line.substr(currPos, line.size() - currPos);
		return result;
	}
};

int main(int argc, char** argv)
{
	assert(argc == 5);

	std::string inputFileName(argv[1]),
		outputFileName(argv[2]),
		searchString(argv[3]),
		replacementString(argv[4]);

	StreamReplacer replacer(searchString, replacementString);
	replacer.ReplaceInFile(inputFileName, outputFileName);
}
