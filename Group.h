#pragma once
#include "Node.h"

class Group :
	public Node
{
private:
	
public:
	vector<Node *> allNodes;
	Group();
	~Group();
	virtual void draw(glm::mat4 trans, GLint shader)=0;
	void update();
	void addChild(Node * child);
	void removeChild(Node * child);
};

