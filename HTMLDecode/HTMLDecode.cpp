#include "HtmlDecoder.h"
#include <iostream>

int main()
{
	htmlfn::HTMLStringDecoder dec;
	if (!dec.HtmlDecode(std::cin, std::cout))
	{
		std::cerr << "Invalid input\n";
		return -1;
	}

	return 0;
}
