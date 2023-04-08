#include <iostream>
#include <fstream>
#include <cassert>
#include <bitset>
#include <array>

class Crypter
{
public:
    static const bool ENCRYPT = true;
    static const bool DECYPT = false;

    Crypter() 
    : m_key(55)
		, m_offsets({2, 3, 4, 6, 7, 0, 1, 5})
    {
    }
    Crypter(const char key, const std::array<char, 8> &&offsetMask)
        : m_key(key)
        , m_offsets(offsetMask)
    {
    }

    void Crypt(bool mode, const std::string &fInPath, const std::string &fOutPath)
    {
        OpenStreams(fInPath, fOutPath);

        char c;
        if (mode)
        {
            while (m_input >> c)
            {
				m_output << unsigned char(MixBitsCrypt(c) ^ m_key);
            }
        }
        else
        {
            while (m_input >> c)
            {
				m_output << unsigned char(MixBitsDecrypt(c ^ m_key));
            }
        }
    }

// private:
    char m_key;
    std::ifstream m_input;
    std::ofstream m_output;
    std::array<char, 8> m_offsets;

    inline char MixBitsCrypt(char c)
    {
		char result = 0;
		char mask = 1;

		char normalizer = 0;
		for (auto offset : m_offsets)
		{
			result += ((mask & c) >> (normalizer++)) << offset;
			mask <<= 1;
		}
		return result;
    }

    inline char MixBitsDecrypt(char c)
    {
		char result = 0;

		char ctr = 0;
		for (auto index : m_offsets)
		{
			result += (((1 << index) & c) >> index) << (ctr++);
		}
		return result;
    }

    void OpenStreams(const std::string& fInPath, const std::string fOutPath)
    {
        m_input.open(fInPath, std::ios::binary);
        m_output.open(fOutPath, std::ios::binary);

        assert(m_input.is_open());
        assert(m_output.is_open());
    }
};

unsigned char mix_c(unsigned char e, const std::array<unsigned char, 8> &offsets)
{
	unsigned char result = 0;
	unsigned char mask = 1;

	unsigned char normalizer = 0;
	for (auto offset : offsets)
	{
		result += ((mask & e) >> (normalizer++)) << offset;
		mask <<= 1;
	}
    return result;
}

unsigned char mix_d(unsigned char e, const std::array<unsigned char, 8> &offsets)
{
    unsigned char result = 0;
    
    unsigned char ctr = 0;
    for (auto index : offsets)
	{
		result += (((1 << index) & e) >> index) << (ctr++);
    }
    return result;
}

int main()
{
	Crypter c;
	/* std::string crypted = "";
    for (auto e : s)
	{
		crypted += c.MixBitsCrypt(e);
    }

    std::string decr = "";
    for (auto e : crypted)
	{
		decr += c.MixBitsDecrypt(e);
    }

    std::cout << decr << '\n';*/

    // c.Crypt(Crypter::ENCRYPT, "C:\\Users\\Roman\\algos_1_tsk.txt", "C:\\Users\\Roman\\algos_1_tsk_crypted.txt");
	// c.Crypt(Crypter::DECYPT, "C:\\Users\\Roman\\algos_1_tsk_crypted.txt", "C:\\Users\\Roman\\algos_1_tsk_decrypted.txt");
}
