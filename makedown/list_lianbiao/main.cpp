#include"List.h"

using namespace std;

int main()
{
	vector<int> vec = {1,2,3,4,5};
	List list(vec);
	list.printList();
	Node* a=list.getTail();
	Node* b=list.getHead();
	std::cout<<a->val;
	std::cout<<b->val;
	return 0;
}
