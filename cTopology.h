#pragma once

#include "stdafx.h"

#include "cNode.h"
#include "cLink.h"
#include "cVirtTopology.h"
#include "cVirtNode.h"
#include "cVirtLink.h"

class cTopology
{
private:
	//cNode* pnode;
	//cLink* plink;

public:
	vector<cNode> phyNode_vec;
	vector<cLink> phyLink_vec;

public:
	//int initial(char _topo[][totalNumNode+1]);
	int createTopology(cVirtTopology& _virtTopo);

public:
	cTopology(void);
	cTopology(const cTopology& _topo);
	cTopology& operator=(const cTopology& _topo);
	~cTopology(void);
};

