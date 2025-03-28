/*************************************************************************
> FileName: for-each.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年03月27日 星期四 19时31分26秒
 ************************************************************************/
#include <list>
#include <string>
#include <iostream>
#include <algorithm>

int main (void)
{
	using namespace std;

	list<string> ls;

	ls.push_back("aaaa1");
	ls.push_back("aaaa2");
	ls.push_back("bbbb3");

	for_each(ls.begin(), ls.end(), [=] (string& s) {
		std::cout << "s: " << s << std::endl;
		string b = "s-" + s;
		s = b;
	});

	for (auto s : ls) {
		std::cout << s << std::endl;
	}

	return 0;
}
