#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "io_functions.h"

void AddMinimals(std::vector<float>& result, size_t count = 3)
{
	count = std::clamp(count, static_cast<size_t>(0), result.size());

	std::vector<float> selectedItems(count);
	std::partial_sort_copy(result.begin(), result.end(), selectedItems.begin(), selectedItems.end());

	float value = std::accumulate(selectedItems.begin(), selectedItems.end(), 0.0f);

	std::for_each(result.begin(), result.end(), [value](float& x) {
		x += value;
	});
}

int main()
{
	std::vector<float> data;
	if (!iofn::ReadVector(std::cin, data, "!"))
	{
		std::cerr << "Invalid input\n";
		return -1;
	}

	AddMinimals(data);

	iofn::WriteVector(std::cout, data, " ", true);
}