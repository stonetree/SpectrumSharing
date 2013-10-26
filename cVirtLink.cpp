#include "StdAfx.h"
#include "cVirtLink.h"


cVirtLink::cVirtLink(void)
{
}


cVirtLink::~cVirtLink(void)
{
}

cVirtLink::cVirtLink(ID _id, int _linkWeight)
{
	id = _id;
	//totalSlot = _slot;
	linkWeight = _linkWeight;
}

cVirtLink::cVirtLink(ID _id,ID _endSrcNodeID, ID _endDesNodeID, int _linkWeight)
{
	id = _id;
	//totalSlot = _slot;
	endSrcNodeID = _endSrcNodeID;
	endDesNodeID = _endDesNodeID;

	linkWeight = _linkWeight;
}

cVirtLink::cVirtLink(const cVirtLink& _link)
{
	operator=(_link);
}

cVirtLink& cVirtLink::operator=(const cVirtLink& _link)
{
	if(this!=&_link)
	{
		this->id = _link.id;

		this->endSrcNodeID = _link.endSrcNodeID;
		this->endDesNodeID = _link.endDesNodeID;

		this->endSrcNode = _link.endSrcNode;
		this->endDesNode = _link.endDesNode;

		//this->totalSlot  = _link.totalSlot;

		this->linkWeight = _link.linkWeight;

	}

	return *this;
}

