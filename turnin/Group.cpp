#include "Group.h"
#include <vector>
#include <algorithm>
using namespace std;

Group::Group()
{
}


Group::~Group()
{

}

void Group::update()
{

}

void Group::addChild(Node * child)
{
	allNodes.push_back(child);
}

void Group::removeChild(Node * child)
{
	allNodes.erase(std::remove(allNodes.begin(), allNodes.end(), child), allNodes.end());
}
