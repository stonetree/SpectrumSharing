#pragma once

#include "cVirtLink.h"
#include "cVirtNode.h"

class cVirtNode
{
private:
	ID id;

private:
	int distance;
	ID  preNodeID;

public:
	list<cVirtLink*> adjacentLink_list;
	list<ID> adjacentLinkID_list;

public:
	void setId(ID _id) {id = _id;}
	ID   getId(void) const {return id;}

	void setDistance(int _distance){distance = _distance;}
	int  getDistance(void)const {return distance;}

	void setPreNodeID(ID _preid) {preNodeID = _preid;}
	ID   getPreNodeID(void) const {return preNodeID;}

public:
	cVirtNode(void);
	cVirtNode(ID _id,int _distance = maxDistance, ID _preNodeID = 0);
	cVirtNode(const cVirtNode& _node);
	cVirtNode& operator=(const cVirtNode& _node);
	~cVirtNode(void);
};

