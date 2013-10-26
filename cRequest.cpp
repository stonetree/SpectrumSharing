#include "StdAfx.h"
#include "cRequest.h"


cRequest::cRequest(void)
{
}

cRequest::cRequest(ID _id, bool _isService)
{
	id = _id;
	isService = _isService;
}

cRequest::cRequest(const cRequest& _request)
{
	operator=(_request);
}

cRequest::cRequest(ID _id, ID _srcID,ID _desID, int _resDemand,double _startingTime,double _durationTime,bool _isService)
{
	id = _id;
	sourceNodeID = _srcID;
	destNodeID = _desID;
	resourceDemand = _resDemand;
	startingTime = _startingTime;
	durationTime = _durationTime;
	isService = _isService;
}

cRequest& cRequest::operator=(const cRequest& _request)
{
	if (this!=&_request)
	{
		this->id = _request.id;

		this->sourceNodeID = _request.sourceNodeID;
		this->destNodeID = _request.destNodeID;

		this->resourceDemand = _request.resourceDemand;

		this->startingTime = _request.startingTime;
		this->durationTime = _request.durationTime;

		this->isService = _request.isService;

		this->workingPathSpectrumIndex = _request.workingPathSpectrumIndex;

		this->backupPathSpectrumIndex = _request.backupPathSpectrumIndex;

		this->workingPathNode_vec.clear();
		this->workingPathNode_vec.assign(_request.workingPathNode_vec.begin(),_request.workingPathNode_vec.end());

		this->workingPathLink_vec.clear();
		this->workingPathLink_vec.assign(_request.workingPathLink_vec.begin(),_request.workingPathLink_vec.end());

		this->backupPathNode_vec.clear();
		this->backupPathNode_vec.assign(_request.backupPathNode_vec.begin(),_request.backupPathNode_vec.end());
		
		this->backupPathLink_vec.clear();
		this->backupPathLink_vec.assign(_request.backupPathLink_vec.begin(),_request.backupPathLink_vec.end());

	}

	return *this;
}
cRequest::~cRequest(void)
{
}


int cRequest::havingCommonWorkingLink(cRequest* _request)
{
	//vector<ID>::iterator iter_find;
	//vector<ID>::iterator iter_link;

	//for (iter_link = workingPathLinkID_vec.begin();iter_link!=workingPathLinkID_vec.end();iter_link++)
	//{
	//	iter_find = find(_request->workingPathLinkID_vec.begin(),_request->workingPathLinkID_vec.end(),*iter_link);
	//	if (iter_find != _request->workingPathLinkID_vec.end())
	//	{
	//		//these two requests have common link(s) in their working paths
	//		return 1;
	//	}
	//}

	vector<pair<ID,cLink*>>::iterator iter_find;
	vector<pair<ID,cLink*>>::iterator iter_link;

	pair<ID,cLink*> tem_pair;

	for (iter_link = workingPathLink_vec.begin();iter_link!=workingPathLink_vec.end();iter_link++)
	{
		tem_pair = make_pair(iter_link->first,iter_link->second);
		iter_find = find(_request->workingPathLink_vec.begin(),_request->workingPathLink_vec.end(),tem_pair);
		if (iter_find != _request->workingPathLink_vec.end())
		{
			//these two requests have common link(s) in their working paths
			return 1;
		}
	}
	//there are no common links in these two requests' working path.
	return 0;
}