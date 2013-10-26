#pragma once
#include "cVirtLink.h"
#include "cVirtNode.h"


class cVirtTopology
{

public:
	vector<cVirtNode> virtNode_vec;
	vector<cVirtLink> virtLink_vec;

public:
	//storage updated link info in the updated virtual topology
	//some links may be deleted from it due to updating the topology
	//for finding multiple shortest paths between two specified nodes.
	list<cVirtLink> virtLink_list;

public:
	int createVirtTopology(char _topo[][totalNumNode+1]);

public:
	cVirtTopology(void);
	cVirtTopology(const cVirtTopology& _topo);
	cVirtTopology& operator=(const cVirtTopology& _topo);
	~cVirtTopology(void);
};

