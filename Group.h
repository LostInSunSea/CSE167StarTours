#pragma once
#include "Node.h"

class Group :
	public Node
{
private:
	vector<Node *> allNodes;
public:
	Group();
	~Group();
	virtual void draw(glm::mat4 trans, GLint shader)=0;
	void update();
	void addChild();
	void removeChild();
};

