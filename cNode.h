#pragma once
#include "cVirtNode.h"

class cLink;

class cNode:
	public cVirtNode
{
//private:
//	ID id;
//
//public:
//	vector<cLink*> adjacentLink_vec;
//	vector<ID> adjacentLinkID_vec;
//
//public:
//	void setId(ID _id) {id = _id;}
//	ID   getId(void) const {return id;}

public:
	cNode(void);
	cNode(cVirtNode& _virNode);
	cNode(ID _id):cVirtNode(_id){}
	cNode(const cNode& _node):cVirtNode(_node){}
	cNode& operator=(const cNode& _node);
	~cNode(void);
};

