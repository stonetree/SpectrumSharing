#pragma once

#include "cSpectrum.h"
#include "cVirtLink.h"
#include "cRequest.h"
#include "cNode.h"

class cLink:
	public cVirtLink
{
private:
	//ID id;
	//ID endSrcNodeID;
	//ID endDesNodeID;
	//cNode* endSrcNode;
	//cNode* endDesNode;
	int totalSlot;
	int fragmentationCount;

	//int linkWeight;

public:
	vector<cSpectrum> spectrumAllocation_vec;
	
	//string for conveniently finding available spectrum for working path
	string  spectrumAllocationWorkingPath;

	string  spectrumAllocationBackupPath;

public:
	//void setId(ID _id){id = _id;}
	//ID getId(void) const {return id;}

	//void setEndSrcNodeID(ID _id) {endSrcNodeID = _id;}
	//ID   getEndSrcNodeID(void) const {return endSrcNodeID;}

	//void setEndDesNodeID(ID _id) {endDesNodeID = _id;}
	//ID   getEndDesNodeID(void) const {return endDesNodeID;}

	//void setEndSrcNode(cNode* _pnode){endSrcNode = _pnode;}
	//cNode* getEndSrcNode(void) const {return endSrcNode;}

	//void setEndDesNode(cNode* _pnode){endDesNode = _pnode;}
	//cNode* getEndDesNode(void) const {return endDesNode;}

	void setTotalSlot(int _slot){totalSlot = _slot;}
	int  getTotalSlot(void) const {return totalSlot;}

	void setFragmentationCount(int _count){fragmentationCount = _count;}
	int  getFragmentatinoCount(void) const {return fragmentationCount;}

	int  findSpectrumAvaiable(cRequest* _request, path_class _path_class,vector<int>& _slotAvailable);
	//int  locateSpectrum(cRequest* _request,const string& _spectrumAvalableIndex,vector<int>& _slotAvailable);

	int  allocateSpectrum(cRequest* _request, path_class _path_state, int _slotAllocatedIndex);
	int  releaseSpectrum(cRequest* _request, path_class _path_state);

	//return the difference value between the value of fragmentation if current requests is inserted and the value of current link fragmentation count
	pair<int,int>  calculateFragmentation(cRequest* _request, int _slot);
	void updateFragmentation(void);
	//int  locateFragmentation(cRequest* _request,int _slot,string& _spectrumAllocation);

	int  determineShareable(cRequest* _request, int _slot, int* _terminal_slot);

	int  calculateSepctrumUsed(cRequest* _request,path_class _path_state, int _slot);

public:
	cLink(void):cVirtLink(){}
	cLink(cVirtLink& _virLink);
	cLink(ID _id, int _slot, int _linkWeight = 1);
	cLink(ID _id, int _slot,ID _endSrcNodeID, ID _endDesNodeID, int _linkWeight = 1):cVirtLink(_id, _endSrcNodeID, _endDesNodeID, _linkWeight = 1){totalSlot = _slot;}
	cLink(const cLink& _link):cVirtLink(_link){operator=(_link);}
	cLink & operator=(const cLink& _link);
	~cLink(void);
};

