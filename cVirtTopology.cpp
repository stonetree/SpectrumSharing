#include "StdAfx.h"
#include "cVirtTopology.h"


cVirtTopology::cVirtTopology(void)
{
}


cVirtTopology::cVirtTopology(const cVirtTopology& _topo)
{
	operator=(_topo);
}

cVirtTopology& cVirtTopology::operator=(const cVirtTopology& _topo)
{
	if (this!=&_topo)
	{
		//this->pnode = _topo.pnode;
		//this->plink = _topo.plink;

		this->virtNode_vec.erase(virtNode_vec.begin(),virtNode_vec.end());
		this->virtNode_vec.assign(_topo.virtNode_vec.begin(),_topo.virtNode_vec.end());

		this->virtLink_vec.erase(virtLink_vec.begin(),virtLink_vec.end());
		this->virtLink_vec.assign(_topo.virtLink_vec.begin(),_topo.virtLink_vec.end());

		this->virtLink_list.erase(virtLink_list.begin(),virtLink_list.end());
		this->virtLink_list.assign(_topo.virtLink_list.begin(),_topo.virtLink_list.end());
	}

	return *this;
}

cVirtTopology::~cVirtTopology(void)
{
}

//int cVirtTopology::createVirtTopology(cTopology& _topo)
//{
//	vector<cNode>::iterator iter_node_vec;
//	vector<cLink>::iterator iter_link_vec;
//
//	virtNode_vec.erase(virtNode_vec.begin(),virtNode_vec.end());
//	virtLink_vec.erase(virtLink_vec.begin(),virtLink_vec.end());
//	
//	for (iter_node_vec = _topo.phyNode_vec.begin();iter_node_vec!=_topo.phyNode_vec.end();iter_node_vec++)
//	{
//		cVirtNode tem_virNode(iter_node_vec->getId(),iter_node_vec->getDistance(),iter_node_vec->getPreNodeID());
//
//		tem_virNode.adjacentLinkID_vec.assign(iter_node_vec->adjacentLinkID_vec.begin(),iter_node_vec->adjacentLinkID_vec.end());
//		tem_virNode.adjacentLink_vec.assign(iter_node_vec->adjacentLink_vec.begin(),iter_node_vec->adjacentLink_vec.end());
//
//		virtNode_vec.push_back(tem_virNode);
//	}
//}

cVirtLink* isDuplicate(vector<cVirtLink>& _virtLink_vec,int _endSrcNodeID,int _endDesNodeID)
{
	vector<cVirtLink>::iterator iter_virtLink_vec = _virtLink_vec.begin();

	for (;iter_virtLink_vec!=_virtLink_vec.end();iter_virtLink_vec++)
	{
		if (iter_virtLink_vec->getEndSrcNodeID() == _endSrcNodeID && iter_virtLink_vec->getEndDesNodeID() == _endDesNodeID)
			return &(*iter_virtLink_vec);
	}
	return NULL;
}

int cVirtTopology::createVirtTopology(char _topo[][totalNumNode+1])
{
	int i_index, j_index;
	int link_index = 1;
	cVirtLink* tem_plink;
	//char   tem_slots[slotPerLink+1];
	//int i;
	vector<cVirtNode>::iterator iter_node;

	//for (i=0;i<slotPerLink;i++)
	//{
	//	tem_slots[i] = 'E';
	//}
	//tem_slots[i] = '\0';

	//Initial cNode objectives//
	for (i_index = 0;i_index<totalNumNode;i_index++)
	{
		cVirtNode tem_node(i_index+1);
		for (j_index = 0;j_index<totalNumNode;j_index++)
		{
			if ((int)(_topo[i_index][j_index]-48) == 1)
			{
				//If there is already a link connecting with current node, then just update the adjacent link info//
				//Not need to create a new cLink objective//
				if (j_index<i_index)
				{
					tem_plink = isDuplicate(virtLink_vec,j_index+1,i_index+1);
					tem_node.adjacentLinkID_list.push_back(tem_plink->getId());

					continue;
				}

				cVirtLink tem_link(link_index,(ID)(i_index+1),(ID)(j_index+1));

				//Initial spectrum objectives for each link//
				//for (i = 0;i<slotPerLink;i++)
				//{
				//	cSpectrum _specSlot;
				//	tem_link.spetrumAllocation_vec.push_back(_specSlot);
				//}

				//tem_link.spectrumIsAllocation.assign(tem_slots);
				tem_node.adjacentLinkID_list.push_back(link_index);
				virtLink_vec.push_back(tem_link);
				virtLink_list.push_back(tem_link);
				link_index++;
			}
		}

		virtNode_vec.push_back(tem_node);
	}


	
	//create the point list of adjacent links for each node
	for (iter_node = virtNode_vec.begin();iter_node!=virtNode_vec.end();iter_node++)
	{
		list<ID>::iterator iter_id;

		for (iter_id = iter_node->adjacentLinkID_list.begin();iter_id!=iter_node->adjacentLinkID_list.end();iter_id++)
		{
			iter_node->adjacentLink_list.push_back(&virtLink_vec[*iter_id - 1]);
		}
	}

	return 0;
}