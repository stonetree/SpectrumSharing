#include "StdAfx.h"
#include "cLink.h"



cLink::cLink(ID _id, int _slot, int _linkWeight)
{
	//id = _id;
	//totalSlot = _slot;

	//linkWeight = _linkWeight;
}

cLink::cLink(cVirtLink& _virLink):cVirtLink(_virLink)
{

}

//cLink::cLink(ID _id, int _slot,ID _endSrcNodeID, ID _endDesNodeID, int _linkWeight = 1)
//{
//	//id = _id;
//	//totalSlot = _slot;
//	//endSrcNodeID = _endSrcNodeID;
//	//endDesNodeID = _endDesNodeID;
//
//	//linkWeight = _linkWeight;
//}

//cLink(const cLink& _link)
//{
//	operator=(_link);
//}

cLink& cLink::operator=(const cLink& _link)
{
	if(this!=&_link)
	{
		//this->id = _link.id;

		//this->endSrcNodeID = _link.endSrcNodeID;
		//this->endDesNodeID = _link.endDesNodeID;

		//this->endSrcNode = _link.endSrcNode;
		//this->endDesNode = _link.endDesNode;

		this->totalSlot  = _link.totalSlot;

		this->fragmentationCount = _link.fragmentationCount;

		this->spectrumAllocation_vec.erase(spectrumAllocation_vec.begin(),spectrumAllocation_vec.end());
		this->spectrumAllocation_vec.assign(_link.spectrumAllocation_vec.begin(),_link.spectrumAllocation_vec.end());

		this->spectrumAllocationWorkingPath.erase(spectrumAllocationWorkingPath.begin(),spectrumAllocationWorkingPath.end());
		this->spectrumAllocationWorkingPath.assign(_link.spectrumAllocationWorkingPath.begin(),_link.spectrumAllocationWorkingPath.end());

		this->spectrumAllocationBackupPath.erase(spectrumAllocationBackupPath.begin(),spectrumAllocationBackupPath.end());
		this->spectrumAllocationBackupPath.assign(_link.spectrumAllocationBackupPath.begin(),_link.spectrumAllocationBackupPath.end());

		//this->linkWeight = _link.linkWeight;

	}

	return *this;
}

cLink::~cLink(void)
{
}


int  cLink::findSpectrumAvaiable(cRequest* _request, path_class _path_class,vector<int>& _slotAvailable)
{
	//find available spectrum 
	//output the starting indexes of all available spectrum
	//cout<<"FindSpectrunAvailable"<<endl;
	string::iterator iter_find;
	string::size_type find_index;
	char ch[slotPerLink +1];

	int i;

	string spectrumAvalableIndex;
	
	if (_path_class == WORKINGPATH)
	{
		spectrumAvalableIndex.assign(spectrumAllocationWorkingPath.begin(),spectrumAllocationWorkingPath.end());
	}
	else
	{
		spectrumAvalableIndex.assign(spectrumAllocationBackupPath.begin(),spectrumAllocationBackupPath.end());
	}


	_slotAvailable.erase(_slotAvailable.begin(),_slotAvailable.end());

	for (i = 0;i<_request->getResourceDemand();i++)
	{
		ch[i] = 'E';
	}

	ch[i] = '\0';

	int terminal_slot = 0;

	find_index = 0;
	while(1)
	{
		find_index = spectrumAvalableIndex.find(ch,find_index);
		if (find_index != string::npos)
		{
			if (_path_class == BACKUPPATH)
			{
				if (!determineShareable(_request,find_index, &terminal_slot))
				{
					find_index++;
					continue;
				}
			}
			_slotAvailable.push_back(find_index);
			find_index++;
		}
		else
		{
			break;
		}
	}

	return 0;
}

//int  cLink::locateSpectrum(cRequest* _request,const string& _spectrumAvalableIndex,vector<int>& _slotAvailable)
//{
//	
//	string::iterator iter_find;
//	string::size_type find_index;
//	char ch[slotPerLink +1];
//
//	int i;
//
//	_slotAvailable.erase(_slotAvailable.begin(),_slotAvailable.end());
//
//	for (i = 0;i<_request->getResourceDemand();i++)
//	{
//		ch[i] = 'E';
//	}
//
//	ch[i] = '\0';
//
//	find_index = 0;
//	while(1)
//	{
//		find_index = _spectrumAvalableIndex.find(ch,find_index);
//		if (find_index != string::npos)
//		{
//			_slotAvailable.push_back(find_index);
//			find_index++;
//		}
//		else
//		{
//			break;
//		}
//	}
//	
//	return 0;
//}


int cLink::allocateSpectrum(cRequest* _request, path_class _path_state, int _slotAllocated)
{
	//cout<<"allocateSpectrum"<<endl;
	int i;

	for (i = _slotAllocated;i<_slotAllocated + _request->getResourceDemand();i++)
	{
		
		spectrumAllocationWorkingPath[i] = 'U';
		spectrumAllocation_vec[i].setIsUsed(true);

		if (_path_state == WORKINGPATH)
		{
			spectrumAllocationBackupPath[i] = 'U';
			spectrumAllocation_vec[i].setSharingAvailable(false);
			spectrumAllocation_vec[i].setSpectrumState(WORKING);

		}
		else
		{
			//add the following sentence for dedicated protection strategy
			//in this case, all backup spectrum are seen to be unsharable.
			//therefore, we should set all the indexes of spectrum usage as being used for each backup path.
			spectrumAllocationBackupPath[i] = 'U';
			
			spectrumAllocation_vec[i].setSharingAvailable(true);
			spectrumAllocation_vec[i].setSpectrumState(BACKUP);
		}
		
		spectrumAllocation_vec[i].request_map.insert(make_pair(_request->getId(),_request));
	}
	
	
	return 0;
}

int cLink::releaseSpectrum(cRequest* _request, path_class _path_state)
{
	int i;

	map<ID,cRequest*>::iterator iter_request;

	
	if (_path_state == WORKINGPATH)
	{
		for(i = _request->getWorkingPathSpecIndex();i<_request->getWorkingPathSpecIndex() + _request->getResourceDemand();i++)
		{
			iter_request = spectrumAllocation_vec[i].request_map.find(_request->getId());

			if (iter_request!=spectrumAllocation_vec[i].request_map.end())
			{
				spectrumAllocation_vec[i].request_map.erase(iter_request);
			}
			
			spectrumAllocationWorkingPath[i] = 'E';
			spectrumAllocationBackupPath[i] = 'E';
			spectrumAllocation_vec[i].setIsUsed(false);
			spectrumAllocation_vec[i].setSpectrumState(NONE);
		}
	}
	else
	{
		for(i = _request->getBackupPathSpecIndex();i<_request->getBackupPathSpecIndex() + _request->getResourceDemand();i++)
		{
			iter_request = spectrumAllocation_vec[i].request_map.find(_request->getId());

			if (iter_request!=spectrumAllocation_vec[i].request_map.end())
			{
				spectrumAllocation_vec[i].request_map.erase(iter_request);
			}
			
			if (spectrumAllocation_vec[i].request_map.empty())
			{
				spectrumAllocationWorkingPath[i] = 'E';
				spectrumAllocationBackupPath[i] = 'E';
				spectrumAllocation_vec[i].setIsUsed(false);
				spectrumAllocation_vec[i].setSharingAvailable(false);
				spectrumAllocation_vec[i].setSpectrumState(NONE);
			}			
		}
	}
	
	return 0;
}


//int cLink::locateFragmentation(cRequest* _request,int _slot,string& _spectrumAllocation)
//{
//	//If the slot before the slots that will be allocated is empty and the slot after them is also empty
//	//then a fragmentation will be produced.
//	if (_spectrumAllocation[_slot - 1] == 'E' && _spectrumAllocation[_slot + _request->getResourceDemand()] == 'E')
//	{
//		return 1;
//	}
//	
//	return 0;
//}

void cLink::updateFragmentation(void)
{
	//cout<<"updateFragmentation"<<endl;
	int i;

	fragmentationCount = 1;

	for (i = 0;i<slotPerLink;)
	{
		if (spectrumAllocationWorkingPath[i] == 'U')
		{
			i++;
			continue;
		}

		while (i<slotPerLink)
		{
			if (spectrumAllocationWorkingPath[i] == 'U')
			{
				fragmentationCount++;
				break;
			}
			else

			i++;
		}

	}
	
	return;
}

pair<int,int>  cLink::calculateFragmentation(cRequest* _request, int _slot)
{
	//cout<<"calculateFragmentation"<<endl;
	int i;
	int tem_fragmentationCount = 1;

	int unUsedSlotCount = 0;

	pair<int,int> output_result;
	
	//we obtain the fragmentation count by counting the # of consecutive empty slots in each link
	string spectrumIndex(spectrumAllocationWorkingPath);

	for (i = _slot;i<_slot + _request->getResourceDemand();i++)
	{
		spectrumIndex[i] = 'U';
	}

	for (i = 0;i<spectrumIndex.size();)
	{
		if (spectrumIndex[i] == 'U')
		{
			i++;
			continue;
		}

		while (i<spectrumIndex.size())
		{
			if (spectrumIndex[i] == 'U')
			{
				tem_fragmentationCount++;
				break;
			}
			else
			{
				unUsedSlotCount++;
			}

			i++;
		}

	}
	
	output_result.first = tem_fragmentationCount - fragmentationCount;
	output_result.second = unUsedSlotCount;


	return output_result;
	
}

int  cLink::calculateSepctrumUsed(cRequest* _request,path_class _path_state, int _slot)
{
	//cout<<"calculateSepctrumUsed"<<endl;
	int i;

	int totalSpectrum = 0;

	if (_path_state == WORKINGPATH)
	{
		return _request->getResourceDemand();
	}
	
	for (i = _slot;i<_slot + _request->getResourceDemand();i++)
	{
		if (!spectrumAllocation_vec[i].getIsUsed())
		{
			totalSpectrum++;
		}
	}
	return totalSpectrum;
}

int cLink::determineShareable(cRequest* _request, int _slot, int* _terminal_slot)
{
	//cout<<"DetermineSharable"<<endl;
	int i;
	map<ID,cRequest*>::iterator iter_request;

	for (i = _slot;i<_slot + _request->getResourceDemand();i++)
	{
		if(spectrumAllocation_vec[i].getIsUsed())
		{
			//should determine if the request need to be allocated is shareable with those requests\
			//that have been already allocated in current slot

			//those requests that have different working paths can share same link and same spectrum
			for(iter_request = spectrumAllocation_vec[i].request_map.begin();iter_request!=spectrumAllocation_vec[i].request_map.end();iter_request++)
			{
				if ((iter_request->second)->havingCommonWorkingLink(_request))
				{
					//the request to be allocated has common link(s) with the requests
					//the slot is not shareable with the just arriving request
					*_terminal_slot = i;
					return 0;
				}
			}
		}
	}
	
	//is shareable
	return 1;
}