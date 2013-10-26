#pragma once

class cNode;

class cVirtLink
{
private:
	ID id;
	ID endSrcNodeID;
	ID endDesNodeID;
	cNode* endSrcNode;
	cNode* endDesNode;
	//int totalSlot;

	int linkWeight;

public:
	void setId(ID _id){id = _id;}
	ID getId(void) const {return id;}

	void setEndSrcNodeID(ID _id) {endSrcNodeID = _id;}
	ID   getEndSrcNodeID(void) const {return endSrcNodeID;}

	void setEndDesNodeID(ID _id) {endDesNodeID = _id;}
	ID   getEndDesNodeID(void) const {return endDesNodeID;}

	void setEndSrcNode(cNode* _pnode){endSrcNode = _pnode;}
	cNode* getEndSrcNode(void) const {return endSrcNode;}

	void setEndDesNode(cNode* _pnode){endDesNode = _pnode;}
	cNode* getEndDesNode(void) const {return endDesNode;}

	//void setTotalSlot(int _slot){totalSlot = _slot;}
	//int  getTotalSlot(void) const {return totalSlot;}

	void setLinkWeight(int _weight){linkWeight = _weight;}
	int  getLinkWeight(void)const {return linkWeight;}

public:
	cVirtLink(void);
	~cVirtLink(void);

	cVirtLink(ID _id, int _linkWeight = 1);
	cVirtLink(ID _id, ID _endSrcNodeID, ID _endDesNodeID, int _linkWeight = 1);
	cVirtLink(const cVirtLink& _link);
	cVirtLink & operator=(const cVirtLink& _link);
};

