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
		Validate();
	}

	void ReplaceInFile(const std::string& fInPath, const std::string fOutPath)
	{
		OpenStreams(fInPath, fOutPath);

		std::string readData;
		while (std::getline(m_input, readData))
		{
			m_output << Replace(readData) << std::endl;
		}

		CloseStreams();
	}

private:
	std::string m_pattern;
	std::string m_replacement;
	std::ifstream m_input;
	std::ofstream m_output;

	
	void Validate() const
	{
		if (m_pattern.empty())
		{
			throw std::invalid_argument("Search pattern is empty");
		}
	}

	std::string Replace(const std::string& line) const noexcept
	{
		std::string result;

		size_t offset = 0;
		size_t currPos = 0;
		while (true)
		{
			offset = line.find(m_pattern, currPos);
			
			if (offset == std::string::npos)
				break;

			result.append(line.begin() + currPos, line.begin() + offset);
			result.append(m_replacement);

			currPos += offset - currPos + m_pattern.size();
		}
		result.append(line.begin() + currPos, line.begin() + (line.size() - currPos));

		return result;
	}

	void OpenStreams(const std::string& fInPath, const std::string fOutPath)
	{
		m_input.open(fInPath, std::ios::in);
		m_output.open(fOutPath, std::ios::out);

		if (!(m_input.is_open() && m_output.is_open()))
		{
			throw std::runtime_error("I/O error");
		}
	}

	void CloseStreams()
	{
		m_input.close();
		m_output.close();
	}
};

struct InputData
{
	std::string fInPath;
	std::string fOutPath;
	std::string searchString;
	std::string replacementString;
};

InputData ParseInputData(int argc, char** argv)
{
	if (argc != 5)
	{
		throw std::invalid_argument("Invalid arguments");
	}

	return InputData{
		argv[1],
		argv[2],
		argv[3],
		argv[4]
	};
}

int main(int argc, char** argv)
{
	InputData inputData;
	try
	{
		inputData = ParseInputData(argc, argv);
	}
	catch (const std::invalid_argument& e)
	{
		std::cerr << e.what();
		std::cout << "Invalid arguments passed\n";
		return -1;
	}

	StreamReplacer replacer(inputData.searchString, inputData.replacementString);
	try
	{
		replacer.ReplaceInFile(inputData.fInPath, inputData.fOutPath);
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << e.what();
		std::cout << "I/O error\n";
		return -1;
	}
	catch(const std::invalid_argument& e)
	{
		std::cerr << e.what();
		std::cout << "Invalid replacement\n";
		return -2;
	}
}
