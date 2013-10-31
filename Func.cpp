#include "stdafx.h"
#include "common.h"
#include "cVirtNode.h"
#include "cVirtLink.h"
#include "cNode.h"
#include "cLink.h"
#include "cTopology.h"
#include "cVirtTopology.h"
#include "cRequest.h"

double lambda = 2;
double mu = 1;

int resourceDemandMax = 5;
int resourceDemandMin = 1;

extern void generateNodePair(vector<cRequest>& _request_vec,cTopology& _topo);

void initialRequests(vector<cRequest>& _request_vec, vector<double>& _arrivingTime, vector<double>& _durationTime,vector<int>& _resourceDemand,cTopology& _topo)
{
	int i;

	int srcNode;
	int desNode;


	int numNode = _topo.phyNode_vec.size();
	
	for (i = 0;i<totalRequest;i++)
	{
		cRequest request((ID)(i+1),0,0,_resourceDemand[i],_arrivingTime[i],_durationTime[i]);
		_request_vec.push_back(request);
	}

	generateNodePair(_request_vec,_topo);


	return;
}

void initialEventSet(vector<cRequest>& _request_vec,map<double,pair<event_class,cRequest*>>& _event_set)
{
	vector<cRequest>::iterator iter_request;
	
	pair<event_class,cRequest*> _pair;

	for (iter_request = _request_vec.begin();iter_request != _request_vec.end();iter_request++)
	{
		_event_set.insert(make_pair(iter_request->getStartingTime(),make_pair(ARRIVING,&(*iter_request))));
		_event_set.insert(make_pair((iter_request->getStartingTime() + iter_request->getDuringTime()),make_pair(LEAVING,&(*iter_request))));
	}

	return;
}


int initialInputfile(vector<pair<double,double>>& inputParameter)
{
	ifstream inputFile("input.txt");

	char _lambda[10];
	char _mu[10];


	char buf[30];

	if (!inputFile)
	{
		cout<<"Unable to open input file!"<<endl;
		return -2;
	}

	while(!inputFile.eof())
	{
		inputFile.getline(buf,19,'\n');
		sscanf(buf,"%s %s\n",_lambda, _mu);
		inputParameter.push_back(make_pair(atof(_lambda),atof(_mu)));
	}
	
	return 0;
}

int inputTopologyInfo(char _topo[][totalNumNode+1])
{
	char buf[totalNumNode+1];
	int  index = 0;

	ifstream topoFile("topology.txt");

	if (!topoFile)
	{
		cout<<"Unable to open topology file!"<<endl;
		return -2;
	}

	while(!topoFile.eof())
	{
		topoFile.getline(buf,totalNumNode+1,'\n');
		strcpy(_topo[index],buf);
		index++;
	}

	return 0;
}

int outputShortestPath(cVirtTopology& _topo,cVirtNode* _pnode,vector<ID>& _path)
{
	vector<ID> tem_path;
	vector<ID>::reverse_iterator riter_tem_revr_pah;
	
	tem_path.erase(tem_path.begin(),tem_path.end());

	while(_pnode->getPreNodeID() != 0) 
	{
		tem_path.push_back(_pnode->getId());
		_pnode = &(_topo.virtNode_vec[_pnode->getPreNodeID() - 1]);
	} 

	tem_path.push_back(_pnode->getId());

	for (riter_tem_revr_pah = tem_path.rbegin();riter_tem_revr_pah!=tem_path.rend();riter_tem_revr_pah++)
	{
		_path.push_back(*riter_tem_revr_pah);
	}

	return 0;
}

cVirtLink* existLink(cVirtTopology& _topo,cVirtNode* _pnode1,cVirtNode* _pnode2)
{
	list<cVirtLink>::iterator iter_link;

	ID node1 = _pnode1->getId();
	ID node2 = _pnode2->getId();
	ID tem_node;

	if (node2 < node1)
	{
		tem_node = node1;
		node1 = node2;
		node2 = tem_node;
	}

	//Since the links info may be updated in the virtual topology for calculating multiple shortest paths
	//between the specified two nodes,
	//we should use the updated topology info that some links have been deleted from it
	for (iter_link = _topo.virtLink_list.begin();iter_link!=_topo.virtLink_list.end();iter_link++)
	{
		if ((iter_link->getEndSrcNodeID() == node1) && (iter_link->getEndDesNodeID() == node2))
		{
			return &(*iter_link);
		}
	}
	
	return NULL;
}

int markAdjacentNode(cVirtTopology& _topo, list<ID>& _searchSet,cVirtNode* _startPoint)
{
	list<ID>::iterator iter_node;
	//cVirtLink *plink;
	list<cVirtLink*>::iterator iter_link;

	ID startPointID = _startPoint->getId();
	ID endPointID;
	ID linkStartPointID, linkEndPointID;

	for (iter_link = _startPoint->adjacentLink_list.begin();iter_link != _startPoint->adjacentLink_list.end();iter_link++)
	{
		linkStartPointID = (*iter_link)->getEndSrcNodeID();
		linkEndPointID = (*iter_link)->getEndDesNodeID();

		if (startPointID == linkStartPointID)
		{
			endPointID = linkEndPointID;
		}
		else
		{
			endPointID = linkStartPointID;
		}
		
		if (_startPoint->getDistance() + (*iter_link)->getLinkWeight()<_topo.virtNode_vec[endPointID - 1].getDistance())
		{
			//find a node with shorter distance
			//update it's distance info
			_topo.virtNode_vec[endPointID - 1].setDistance(_startPoint->getDistance() + (*iter_link)->getLinkWeight());
			_topo.virtNode_vec[endPointID - 1].setPreNodeID(_startPoint->getId());
		}
	}
	
	//for (iter_node = _searchSet.begin();iter_node != _searchSet.end();iter_node++)
	//{
	//	plink = existLink(_topo,_startPoint,&(_topo.virtNode_vec[*iter_node - 1]));
	//	if(plink != NULL)
	//	{
	//		if (_startPoint->getDistance() + plink->getLinkWeight()<_topo.virtNode_vec[*iter_node - 1].getDistance())
	//		{
	//			//find a node with shorter distance
	//			//update it's distance info
	//			_topo.virtNode_vec[*iter_node - 1].setDistance(_startPoint->getDistance() + plink->getLinkWeight());
	//			_topo.virtNode_vec[*iter_node - 1].setPreNodeID(_startPoint->getId());
	//		}
	//	}
	//}

	return 0;
}

int shortestPath(cVirtTopology& _topo, ID _srcNode, ID _destNode,vector<ID>& _path)
{
	list<ID> beenFound;
	list<ID> notBeenFound;
	list<ID>::iterator iter_find;

	list<ID>::iterator iter_node_algo;
	
	cVirtNode* pvirtNode = NULL;

	vector<cVirtNode>::iterator iter_node;

	int maxDistAlgo;

	//clear all data already existing in every containers
	_path.erase(_path.begin(),_path.end());
	beenFound.erase(beenFound.begin(),beenFound.end());
	notBeenFound.erase(notBeenFound.begin(),notBeenFound.end());


	if (_topo.virtNode_vec[_srcNode-1].adjacentLink_list.empty() || _topo.virtNode_vec[_destNode-1].adjacentLink_list.empty())
	{
		//There is no path existing between the specified source node and destination node.
		//Since we will try to find up to K paths between these specified two nodes, therefore after updating the virtual topology,
		//it may have no adjacent link(s) with the source or destination node before K paths have been found.
		//In this case, the ShortestPath algorithm should be stopped.

		return -1;
	}

	
	for (iter_node = _topo.virtNode_vec.begin();iter_node != _topo.virtNode_vec.end();iter_node++)
	{
		//Put all nodes in the un-found set first
		notBeenFound.push_back(iter_node->getId());
		
		//Initial the distance and pre-node info for each node in the virtual topology
		if (iter_node->getId() == _srcNode)
		{
			iter_node->setDistance(0);
		}
		else
		{
			iter_node->setDistance(maxDistance);
		}
		iter_node->setPreNodeID(0);
	}

	//The main body of the Shortest Path algorithm
	while (!notBeenFound.empty())
	{
		
		maxDistAlgo = maxDistance;
		pvirtNode = NULL;

		//Find the node with minimum distance in un-found set
		for (iter_node_algo = notBeenFound.begin();iter_node_algo != notBeenFound.end();iter_node_algo++)
		{
			if (_topo.virtNode_vec[*iter_node_algo-1].getDistance() < maxDistAlgo)
			{
				pvirtNode = &(_topo.virtNode_vec[*iter_node_algo-1]);
				maxDistAlgo = _topo.virtNode_vec[*iter_node_algo-1].getDistance();
			}
		}

		
		if (pvirtNode == NULL)
		{
			//can't find a path from source node to destination node;
			return -1;
		}

		//determine if it has already arrived the destination node
		if (pvirtNode->getId() == _destNode)
		{
			//obtain the shortest path between the source node and destination node
			//The algorithm will stop when having arrived the destination node
			outputShortestPath(_topo,pvirtNode,_path);			
			break;
		}

		iter_find = find(notBeenFound.begin(),notBeenFound.end(),pvirtNode->getId());
		if (iter_find == notBeenFound.end())
		{
			//abnormal exit
			//did not find the node that should be exist
			return -2;

		}

		//delete the node that should be as a starting node from the non-found set
		//_path.push_back(pvirtNode->getId());
		notBeenFound.erase(iter_find);

		markAdjacentNode(_topo,notBeenFound,pvirtNode);
	}
	
	//succeed to find a shortest path between specified two nodes.
	return 0;
}

int obtainPathLinkID(cTopology& _topo, vector<ID>& _singlePath, vector<ID>& _linkID)
{
	vector<ID>::iterator iter_node_id;
	vector<cLink>::iterator iter_link_index;

	ID srcNode;
	ID desNode;
	ID tem_node;

	_linkID.clear();

	for (iter_node_id = _singlePath.begin();iter_node_id + 1 != _singlePath.end();iter_node_id++)
	{
		srcNode = *iter_node_id;
		desNode = *(iter_node_id+1);


		if (desNode < srcNode)
		{
			tem_node = desNode;
			desNode = srcNode;
			srcNode = tem_node;
		}

		for (iter_link_index = _topo.phyLink_vec.begin(); iter_link_index != _topo.phyLink_vec.end();iter_link_index++)
		{
			if ((iter_link_index->getEndSrcNodeID() == srcNode) && (iter_link_index->getEndDesNodeID() == desNode))
			{
				_linkID.push_back(iter_link_index->getId());
				break;
			}
		}
	}
	
	return 0;
}

int updateTopology(cVirtTopology& _topo,cTopology& _phyTopo,vector<ID>& _singlePath)
{
	vector<ID>::iterator iter_node_id;
	list<cVirtLink>::iterator iter_link_index;

	list<cVirtLink*>::iterator iter_adjacent_link;
	list<ID>::iterator iter_adjacent_link_id;

	vector<ID> deleteLinkID_vec;
	vector<ID>::iterator iter_delete_link_id;

	vector<cVirtNode>::iterator  iter_node;

	//ID srcNode;
	//ID desNode;
	//ID tem_node;
	//ID linkID;

	
	//obtain IDs of all links to be deleted
	obtainPathLinkID(_phyTopo,_singlePath,deleteLinkID_vec);

	//delete links from current topology
	for (iter_delete_link_id = deleteLinkID_vec.begin();iter_delete_link_id!=deleteLinkID_vec.end();iter_delete_link_id++)
	{
		for (iter_link_index = _topo.virtLink_list.begin();iter_link_index != _topo.virtLink_list.end();iter_link_index++)
		{
			if (iter_link_index->getId() == *iter_delete_link_id)
			{
				_topo.virtLink_list.erase(iter_link_index);
				break;
			}
		}
	}

	//delete adjacent link info stored in current iteration node
	for (iter_node_id = _singlePath.begin();iter_node_id!=_singlePath.end();iter_node_id++)
	{
		for (iter_delete_link_id = deleteLinkID_vec.begin();iter_delete_link_id!=deleteLinkID_vec.end();iter_delete_link_id++)
		{
			for (iter_adjacent_link = _topo.virtNode_vec[*iter_node_id - 1].adjacentLink_list.begin(); iter_adjacent_link != _topo.virtNode_vec[*iter_node_id - 1].adjacentLink_list.end();iter_adjacent_link++)
			{
				if ((*iter_adjacent_link)->getId() == *iter_delete_link_id)
				{
					_topo.virtNode_vec[*iter_node_id - 1].adjacentLink_list.erase(iter_adjacent_link);
					break;
				}
			}
		}
		
	}

	//update the distance and pre-node values for each node
	for (iter_node = _topo.virtNode_vec.begin();iter_node!=_topo.virtNode_vec.end();iter_node++)
	{
		iter_node->setDistance(maxDistance);
		iter_node->setPreNodeID(0);
	}

	//reset the info for source node
	_topo.virtNode_vec[*_singlePath.begin() - 1].setDistance(0);

	return 0;
}

int findMultipleShortestPath(cVirtTopology& _topo, cTopology& _phyTopo, int _numPath, ID _srcNode, ID _desNode,vector<vector<ID>>& _multiPath)
{
	//clear data having been stored in the output data structure.
	_multiPath.erase(_multiPath.begin(),_multiPath.end());

	vector<ID> singlePath;
	vector<ID>::iterator iter_path_node;

	while (_numPath--)
	{
		singlePath.erase(singlePath.begin(),singlePath.end());

		if (shortestPath(_topo,_srcNode,_desNode, singlePath))
		{
			//can't find a shortest path between these two specified nodes
			return -1;
		}

		_multiPath.push_back(singlePath);

		//If a shortest path just have been found
		//and there are not enough number of shortest paths having been found
		//the topology should be updated according to the links info just founded
		if (_numPath>0)
		{
			updateTopology(_topo,_phyTopo,singlePath);
		}
	}

	return 0;
}


int obtainAvailblePathSlot(cTopology& _topo, cRequest* _request,vector<ID>& _path,path_class _path_class,vector<int>& _applicablePathSlot)
{
	//cout<<"obtainAvailblePathSlot"<<endl;
	vector<ID> linkID_vec;
	vector<ID>::iterator iter_link_id;

	//all available slots for each link
	vector<vector<int>>  availableLinkSlot_vec;
	vector<vector<int>>::iterator iter_availableLinkSlot;

	//all available slots for one link
	vector<int>  availableSlot;
	vector<int>::iterator iter_availableSlot;

	//output: available slots for the path
	//vector<int>  availablePathSlot;

	bool findFlag = true;

	obtainPathLinkID(_topo,_path,linkID_vec);

	
	//obtain all available slots for each link
	for (iter_link_id = linkID_vec.begin();iter_link_id != linkID_vec.end();iter_link_id++)
	{
		availableSlot.clear();
		_topo.phyLink_vec[*iter_link_id - 1].findSpectrumAvaiable(_request,_path_class,availableSlot);

		if (availableSlot.empty())
		{
			//if any one of the link along the path can not find available slot,
			//then return -1.
			_applicablePathSlot.clear();
			return -1;
		}

		availableLinkSlot_vec.push_back(availableSlot);
	}
	

	//find all available slots for the whole path
	for (iter_availableSlot = availableLinkSlot_vec.begin()->begin();iter_availableSlot != availableLinkSlot_vec.begin()->end();iter_availableSlot++)
	{
		for (iter_availableLinkSlot = availableLinkSlot_vec.begin() + 1;iter_availableLinkSlot != availableLinkSlot_vec.end();iter_availableLinkSlot++)
		{
			vector<int>::iterator find_iter;
			find_iter = find(iter_availableLinkSlot->begin(),iter_availableLinkSlot->end(),*iter_availableSlot);
			
			//if there is any one of link having no common available slot
			//then exit the loop for finding the next available available slot
			if (find_iter == iter_availableLinkSlot->end())
			{
				findFlag = false;
				break;
			}
		}

		if (findFlag == true)
		{
			_applicablePathSlot.push_back(*iter_availableSlot);
		}
		else
		{
			findFlag = true;
		}
	}

	//if the available slot is empty
	//then return can't find any applicable slot for this path
	if (_applicablePathSlot.empty())
	{
		return -1;
	}

	return 0;

}

int calculateMisAlignCount_Modified(cTopology& _topo,cRequest* _request,vector<ID>& _path,int _slot)
{
	vector<ID>::iterator iter_nodeID;

	vector<ID> linkID_vec;
	vector<ID>::iterator iter_linkID;
	vector<ID>::iterator find_link;

	list<ID>::iterator iter_adjacentLink;

	int index;

	ID srcNodeID;
	ID desNodeID;

	int misAligCoutn = 0;
	
	obtainPathLinkID(_topo,_path,linkID_vec);
	
	for (iter_nodeID = _path.begin();iter_nodeID != _path.end();iter_nodeID++)
	{
		//iterate all adjacent links of current node
		for (iter_adjacentLink = _topo.phyNode_vec[*iter_nodeID - 1].adjacentLinkID_list.begin();iter_adjacentLink != _topo.phyNode_vec[*iter_nodeID - 1].adjacentLinkID_list.end();iter_adjacentLink++)
		{
			find_link = find(linkID_vec.begin(),linkID_vec.end(),*iter_adjacentLink);

			//if return the end iterator that means current iterating link is not in the path link set
			//therefore we should calculate its mis-alignment count;
			if (find_link == linkID_vec.end())
			{
				for (index = _slot; index<_slot + _request->getResourceDemand();index++)
				{
					if(!_topo.phyLink_vec[*iter_adjacentLink - 1].spectrumAllocation_vec[index].getIsUsed())
					{
						misAligCoutn++;
					}
				}
			}
		}
	}
	
	return misAligCoutn;
}

int calculateMisAlignCount(cTopology& _topo,cRequest* _request,vector<ID>& _path,int _slot)
{
	vector<ID>::iterator iter_nodeID;

	vector<ID> linkID_vec;
	vector<ID>::iterator iter_linkID;
	vector<ID>::iterator find_link;

	list<ID>::iterator iter_adjacentLink;
	
	int index;

	ID srcNodeID;
	ID desNodeID;

	int misAligCoutn = 0;

	obtainPathLinkID(_topo,_path,linkID_vec);

	//iterate each link along the path to check it's adjacent link whether each slot that
	//will be allocated has already been used in the adjacent links
	for(iter_linkID = linkID_vec.begin();iter_linkID != linkID_vec.end();iter_linkID++)
	{
		srcNodeID = _topo.phyLink_vec[*iter_linkID - 1].getEndSrcNodeID();
		desNodeID = _topo.phyLink_vec[*iter_linkID - 1].getEndDesNodeID();

		//for the src node of the link
		for (iter_adjacentLink = _topo.phyNode_vec[srcNodeID - 1].adjacentLinkID_list.begin();iter_adjacentLink != _topo.phyNode_vec[srcNodeID - 1].adjacentLinkID_list.end();iter_adjacentLink++)
		{
			find_link = find(linkID_vec.begin(),linkID_vec.end(),*iter_adjacentLink);

			//if return the end iterator that means current iterating link is not in the path link set
			//therefore we should calculate its mis-alignment count;
			if (find_link == linkID_vec.end())
			{
				for (index = _slot; index<_slot + _request->getResourceDemand();index++)
				{
					if(!_topo.phyLink_vec[*iter_adjacentLink - 1].spectrumAllocation_vec[index].getIsUsed())
					{
						misAligCoutn++;
					}
				}
			}
		}

		//for the des node of the link
		for (iter_adjacentLink = _topo.phyNode_vec[desNodeID - 1].adjacentLinkID_list.begin();iter_adjacentLink != _topo.phyNode_vec[desNodeID - 1].adjacentLinkID_list.end();iter_adjacentLink++)
		{
			find_link = find(linkID_vec.begin(),linkID_vec.end(),*iter_adjacentLink);

			//if return the end iterator that means current iterating link is not in the path link set
			//therefore we should calculate its mis-alignment count;
			if (find_link == linkID_vec.end())
			{
				for (index = _slot; index<_slot + _request->getResourceDemand();index++)
				{
					if(!_topo.phyLink_vec[*iter_adjacentLink - 1].spectrumAllocation_vec[index].getIsUsed())
					{
						misAligCoutn++;
					}
				}
			}
		}
		

	}
	
	
	return misAligCoutn;
}


double calculateObjective(int _fragmentationCount, int _misAlignmentCount,int _spectrumUsed,int _unUsedSlot,int _numLinks)
{
	return ((weight_spectrum_used * _spectrumUsed + weight_fragmentation * _fragmentationCount + weight_alignment * _misAlignmentCount));
}


int obtainMinPathObjective(cTopology& _topo,cRequest* _request,vector<ID>& _path,path_class _path_class,vector<int>& _applicableSlot,map<double,pair<int,vector<ID>>>& _applicablePath)
{
	//cout<<"obtainMinPathObjective"<<endl;
	vector<int>::iterator iter_slot;

	vector<ID> linkID_vec;
	vector<ID>::iterator iter_linkID;

	int fragmentationCount = 0;
	int misAlignmentCount = 0;
	int spectrumUsed = 0;
	int unUsedSlot = 0;

	pair<int,int> fragmen_unUsedSlot_count;

	//store objective value for each slot
	int slot;
	double objective;

	double minimunObjective = 1000000;

	obtainPathLinkID(_topo,_path,linkID_vec);

	for (iter_slot = _applicableSlot.begin();iter_slot != _applicableSlot.end();iter_slot++)
	{

		fragmentationCount = 0;
		unUsedSlot = 0;
		spectrumUsed = 0;
		
		//Given a specified spectrum slot, calculate the fragmentation count, the number of spectrum used 
		//and mis-alignment count for each link along the path.
		for (iter_linkID = linkID_vec.begin();iter_linkID != linkID_vec.end();iter_linkID++)
		{
			fragmen_unUsedSlot_count = _topo.phyLink_vec[*iter_linkID - 1].calculateFragmentation(_request,*iter_slot); 
			fragmentationCount += fragmen_unUsedSlot_count.first;
			unUsedSlot += fragmen_unUsedSlot_count.second;
			spectrumUsed += _topo.phyLink_vec[*iter_linkID - 1].calculateSepctrumUsed(_request,_path_class,*iter_slot);
		}	

		misAlignmentCount = calculateMisAlignCount(_topo,_request,_path,*iter_slot);

		
		objective = calculateObjective(fragmentationCount,misAlignmentCount,spectrumUsed,unUsedSlot,linkID_vec.size());
		if(objective < minimunObjective)
		{
			minimunObjective = objective;
			slot = *iter_slot;
		}
	}

	_applicablePath.insert(make_pair(minimunObjective,make_pair(slot,_path)));
	return 0;
}


int processingPath(cVirtTopology& _virTopo,cTopology& _topo,cRequest* _request,path_class _path_class,vector<vector<ID>>& _path,vector<int>& _slot)
{
	//cout<<"processingPath"<<endl;

	vector<vector<ID>> multiPath;
	vector<vector<ID>>::iterator iter_multiPath;

	vector<ID> linkID_vec;
	vector<ID>::iterator iter_linkID;

	map<double,pair<int,vector<ID>>> applicablePath;

	vector<int> applicableSpectrumSlot;

	int index = 0;


	findMultipleShortestPath(_virTopo,_topo,multiplePath,_request->getSourceNodeID(),_request->getDestNodeID(),multiPath);

	if (multiPath.empty())
	{
		//can't establish working path for current arriving request
		return -1;
	}

	for (iter_multiPath = multiPath.begin(),index = 0;iter_multiPath != multiPath.end();iter_multiPath++,index++)
	{

		applicableSpectrumSlot.clear();

		obtainAvailblePathSlot(_topo,_request,*iter_multiPath,_path_class,applicableSpectrumSlot);
		if(!applicableSpectrumSlot.empty())
		{
			//calculate the minimum objective for all available slot in each path.
			obtainMinPathObjective(_topo,_request,*iter_multiPath,_path_class,applicableSpectrumSlot,applicablePath);
		}
	}


	if (!applicablePath.empty())
	{
		_path.push_back(applicablePath.begin()->second.second);
		_slot.push_back(applicablePath.begin()->second.first);

		//one or more path(s) that has available spectrum has been found
		//select the one with minimum objective as the working path for current request

		//update request info to add working path info to it
		//_request->setIsService(true);

		//vector<ID>::iterator tem_iter_nodeID;
		////_path.clear();
		//for (tem_iter_nodeID = applicablePath.begin()->second.second.begin();tem_iter_nodeID != applicablePath.begin()->second.second.end();tem_iter_nodeID++)
		//{
		//	if (_path_class == WORKINGPATH)
		//	{
		//		_request->workingPathNode_vec.push_back(make_pair(*tem_iter_nodeID,&(_topo.phyNode_vec[*tem_iter_nodeID - 1])));
		//	}
		//	else
		//	{
		//		_request->backupPathNode_vec.push_back(make_pair(*tem_iter_nodeID,&(_topo.phyNode_vec[*tem_iter_nodeID - 1])));
		//	}
		//	_path.push_back(*tem_iter_nodeID);
		//}

		//vector<ID> tem_linkID_vec;
		//vector<ID>::iterator tem_iter_linkID;
		//obtainPathLinkID(_topo,applicablePath.begin()->second.second,tem_linkID_vec);

		//for (tem_iter_linkID = tem_linkID_vec.begin();tem_iter_linkID != tem_linkID_vec.end();tem_iter_linkID++)
		//{
		//	cLink* plink = &(_topo.phyLink_vec[*tem_iter_linkID - 1]);

		//	if (_path_class == WORKINGPATH)
		//	{
		//		_request->workingPathLink_vec.push_back(make_pair(*tem_iter_linkID,plink));
		//	}
		//	else
		//	{
		//		_request->backupPathLink_vec.push_back(make_pair(*tem_iter_linkID,plink));
		//	}


		//	//spectrum allocation and request info updated
		//	plink->allocateSpectrum(_request,_path_class,applicablePath.begin()->second.first);	
		//	plink->updateFragmentation();
		//}
	}

	return 0;
}
int updateRequestPathInfo(cTopology& _topo,cRequest* _request, vector<ID>& _path,path_class _path_class,int _slot)
{
	//cout<<"updateRequestPathInfo"<<endl;
	//one or more path(s) that has available spectrum has been found
	//select the one with minimum objective as the working path for current request

	//update request info to add working path info to it

	if (_path_class == WORKINGPATH)
	{
		_request->setWorkingPathSpecIndex(_slot);
	}
	else
	{
		_request->setBackupPathSpecIndex(_slot);
	}

	vector<ID>::iterator tem_iter_nodeID;
	for (tem_iter_nodeID = _path.begin();tem_iter_nodeID != _path.end();tem_iter_nodeID++)
	{
		if (_path_class == WORKINGPATH)
		{
			_request->workingPathNode_vec.push_back(make_pair(*tem_iter_nodeID,&(_topo.phyNode_vec[*tem_iter_nodeID - 1])));
		}
		else
		{
			_request->backupPathNode_vec.push_back(make_pair(*tem_iter_nodeID,&(_topo.phyNode_vec[*tem_iter_nodeID - 1])));
		}
	}

	vector<ID> tem_linkID_vec;
	vector<ID>::iterator tem_iter_linkID;
	obtainPathLinkID(_topo,_path,tem_linkID_vec);

	for (tem_iter_linkID = tem_linkID_vec.begin();tem_iter_linkID != tem_linkID_vec.end();tem_iter_linkID++)
	{
		cLink* plink = &(_topo.phyLink_vec[*tem_iter_linkID - 1]);

		if (_path_class == WORKINGPATH)
		{
			_request->workingPathLink_vec.push_back(make_pair(*tem_iter_linkID,plink));
		}
		else
		{
			_request->backupPathLink_vec.push_back(make_pair(*tem_iter_linkID,plink));
		}

	}
	return 0;
}

void allocateResource(cTopology& _topo,cRequest* _request,path_class _path_class,vector<ID>& _path,int _slot)
{
	
	//one or more path(s) that has available spectrum has been found
	//select the one with minimum objective as the working path for current request

	//update request info to add working path info to it

	//updateRequestPathInfo(_topo,_request,_path,_path_class,_slot);

	//vector<ID>::iterator tem_iter_nodeID;
	//for (tem_iter_nodeID = _path.begin();tem_iter_nodeID != _path.end();tem_iter_nodeID++)
	//{
	//	if (_path_class == WORKINGPATH)
	//	{
	//		_request->workingPathNode_vec.push_back(make_pair(*tem_iter_nodeID,&(_topo.phyNode_vec[*tem_iter_nodeID - 1])));
	//	}
	//	else
	//	{
	//		_request->backupPathNode_vec.push_back(make_pair(*tem_iter_nodeID,&(_topo.phyNode_vec[*tem_iter_nodeID - 1])));
	//	}
	//}

	vector<ID> tem_linkID_vec;
	vector<ID>::iterator tem_iter_linkID;
	obtainPathLinkID(_topo,_path,tem_linkID_vec);

	for (tem_iter_linkID = tem_linkID_vec.begin();tem_iter_linkID != tem_linkID_vec.end();tem_iter_linkID++)
	{
		cLink* plink = &(_topo.phyLink_vec[*tem_iter_linkID - 1]);

		//if (_path_class == WORKINGPATH)
		//{
		//	_request->workingPathLink_vec.push_back(make_pair(*tem_iter_linkID,plink));
		//}
		//else
		//{
		//	_request->backupPathLink_vec.push_back(make_pair(*tem_iter_linkID,plink));
		//}


		//spectrum allocation and request info updated
		plink->allocateSpectrum(_request,_path_class,_slot);	
		plink->updateFragmentation();
	}
	
	return;
}


int requestArriving(cVirtTopology& _originalVirTopo, cTopology& _topo, cRequest* _request)
{
	//vector<vector<ID>> multiPath;
	//vector<vector<ID>>::iterator iter_multiPath;

	//vector<ID> linkID_vec;
	//vector<ID>::iterator iter_linkID;

	//copy the original virtual topology for obtaining multiple paths 
	cVirtTopology virTopoWorking(_originalVirTopo);
	vector<vector<ID>> path;
	vector<int> slot;

	path.clear();
	slot.clear();
	processingPath(virTopoWorking,_topo,_request,WORKINGPATH,path,slot);

	//find at lease one path for current processing request
	//try to find at least one path as the backup path
	if (!path.empty())
	{
		cVirtTopology virTopoBackup(_originalVirTopo);
		updateTopology(virTopoBackup,_topo,*(path.begin()));	

		updateRequestPathInfo(_topo,_request,*(path.begin()),WORKINGPATH,*(slot.begin()));

		processingPath(virTopoBackup,_topo,_request,BACKUPPATH,path,slot);
	}

	if (path.size() == 2)
	{
		updateRequestPathInfo(_topo,_request,*(path.begin() + 1),BACKUPPATH,*(slot.begin() + 1));

		_request->setIsService(true);
		
		allocateResource(_topo,_request,WORKINGPATH,*(path.begin()),*(slot.begin()));
		allocateResource(_topo,_request,BACKUPPATH,*(path.begin() + 1),*(slot.begin() + 1));
	}
	

	//map<double,pair<int,vector<ID>>> applicablePath;

	//vector<int> applicableSpectrumSlot;

	//int index = 0;

	//Find multiple working path candidates 



	//findMultipleShortestPath(virTopo,_topo,multiplePath,_request->getSourceNodeID(),_request->getDestNodeID(),multiPath);

	//if (multiPath.empty())
	//{
	//	//can't establish working path for current arriving request
	//	return -1;
	//}
	//
	//for (iter_multiPath = multiPath.begin(),index = 0;iter_multiPath != multiPath.end();iter_multiPath++,index++)
	//{
	//	
	//	if(!obtainAvailblePathSlot(_topo,_request,*iter_multiPath,WORKINGPATH,applicableSpectrumSlot))
	//	{
	//		//calculate the minimum objective for 
	//		obtainMinPathObjective(_topo,_request,*iter_multiPath,WORKINGPATH,applicableSpectrumSlot,applicablePath);
	//	}
	//}

	//
	//if (!applicablePath.empty())
	//{
	//	//one or more path(s) that has available spectrum has been found
	//	//select the one with minimum objective as the working path for current request
	//	
	//	//update request info to add working path info to it
	//	_request->setIsService(true);

	//	vector<ID>::iterator tem_iter_nodeID;
	//	for (tem_iter_nodeID = applicablePath.begin()->second.second.begin();tem_iter_nodeID != applicablePath.begin()->second.second.end();tem_iter_nodeID++)
	//	{
	//		_request->workingPathNode_vec.push_back(make_pair(*tem_iter_nodeID,&(_topo.phyNode_vec[*tem_iter_nodeID - 1])));
	//	}

	//	vector<ID> tem_linkID_vec;
	//	vector<ID>::iterator tem_iter_linkID;
	//	obtainPathLinkID(_topo,applicablePath.begin()->second.second,tem_linkID_vec);

	//	for (tem_iter_linkID = tem_linkID_vec.begin();tem_iter_linkID != tem_linkID_vec.end();tem_iter_linkID++)
	//	{
	//		cLink* plink = &(_topo.phyLink_vec[*tem_iter_linkID - 1]);
	//		_request->workingPathLink_vec.push_back(make_pair(*tem_iter_linkID,plink));

	//		//spectrum allocation and request info updated
	//		plink->allocateSpectrum(_request,WORKINGPATH,applicablePath.begin()->second.first);	
	//		plink->updateFragmentation();
	//	}

	//}

	return 0;
}

int requestLeaving(cTopology& _topo, cRequest* _request)
{
	vector<pair<ID,cLink*>>::iterator iter_link;
	
	for (iter_link = _request->workingPathLink_vec.begin();iter_link != _request->workingPathLink_vec.end();iter_link++)
	{
		iter_link->second->releaseSpectrum(_request,WORKINGPATH);
		iter_link->second->updateFragmentation();
	}

	for (iter_link = _request->backupPathLink_vec.begin();iter_link != _request->backupPathLink_vec.end();iter_link++)
	{
		iter_link->second->releaseSpectrum(_request,BACKUPPATH);
		iter_link->second->updateFragmentation();
	}

	return 0;
}