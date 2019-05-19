#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

template<int n, int k>
struct C
{
	enum 
	{
		result = C<n-1,k-1>::result + C<n-1,k>::result
	};
};

template<>
struct C<0,0>
{
static const int result = 1;
};

template<int n>
struct C<n,n>
{
static const int result = 1;
};

template<int n>
struct C<n,0>
{
	static const int result = 1;
};


int main()
{
	C<4, 3> result;

	std::cout << result.result;

	system("pause");
	return 0;
}