#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


int main()
{
	int N = 0;
	std::cin >> N;

	std::vector<std::string> Strings (N);

	std::getline(std::cin, Strings[0]);

	std::for_each(Strings.begin(), Strings.end(), [](std::string& x) 
	{
		std::getline  (std::cin, x);
		x.erase(std::remove(x.begin(), x.end(), ' '), x.end());
		std::transform(x.begin(), x.end(), x.begin(), tolower);
	});

	std::sort (Strings.begin(), Strings.end(), [&] (std::string a, std::string b) { return a > b; });

	std::vector<std::string>::iterator it;
	it = std::unique(Strings.begin(), Strings.end());
	Strings.resize(std::distance(Strings.begin(), it));

	std::cout << std::endl;
	std::for_each(Strings.begin(), Strings.end(), [](std::string x) { std::cout << x << std::endl; });

	system("pause");
	return 0;
}