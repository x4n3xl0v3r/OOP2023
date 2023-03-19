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

    void Validate()
    {
        assert(!m_pattern.empty());
    }

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

    void OpenStreams(const std::string& fInPath, const std::string fOutPath)
    {
        m_input.open(fInPath, std::ios::in);
        m_output.open(fOutPath, std::ios::out);

        assert(m_input.is_open());
        assert(m_output.is_open());
    }

    void CloseStreams()
    {
        m_input.close();
        m_output.close();
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
