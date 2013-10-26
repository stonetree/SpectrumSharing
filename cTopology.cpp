#include "StdAfx.h"
#include "cTopology.h"

cTopology::cTopology(void)
{
}


cTopology::cTopology(const cTopology& _topo)
{
	operator=(_topo);
}

cTopology& cTopology::operator=(const cTopology& _topo)
{
	if (this!=&_topo)
	{
		//this->pnode = _topo.pnode;
		//this->plink = _topo.plink;
		
		this->phyNode_vec.erase(phyNode_vec.begin(),phyNode_vec.end());
		this->phyNode_vec.assign(_topo.phyNode_vec.begin(),_topo.phyNode_vec.end());

		this->phyLink_vec.erase(phyLink_vec.begin(),phyLink_vec.end());
		this->phyLink_vec.assign(_topo.phyLink_vec.begin(),_topo.phyLink_vec.end());
	}

	return *this;
}

cTopology::~cTopology(void)
{
}

//cLink* isDuplicate(vector<cLink>& _phyLink_vec,int _endSrcNodeID,int _endDesNodeID)
//{
//	vector<cLink>::iterator iter_phyLink_vec = _phyLink_vec.begin();
//
//	for (;iter_phyLink_vec!=_phyLink_vec.end();iter_phyLink_vec++)
//	{
//		if (iter_phyLink_vec->getEndSrcNodeID() == _endSrcNodeID && iter_phyLink_vec->getEndDesNodeID() == _endDesNodeID)
//			return &(*iter_phyLink_vec);
//	}
//	return NULL;
//}

//int cTopology::initial(char _topo[][totalNumNode+1])
//{
//	int i_index, j_index;
//	int link_index = 1;
//	cLink* tem_plink;
//	char   tem_slots[slotPerLink+1];
//	int i;
//	vector<cNode>::iterator iter_node;
//
//	for (i=0;i<slotPerLink;i++)
//	{
//		tem_slots[i] = 'E';
//	}
//	tem_slots[i] = '\0';
//	
//	//Initial cNode objectives//
//	for (i_index = 0;i_index<totalNumNode;i_index++)
//	{
//		cNode tem_node(i_index+1);
//		for (j_index = 0;j_index<totalNumNode;j_index++)
//		{
//			if ((int)(_topo[i_index][j_index]-48) == 1)
//			{
//				//If there is already a link connecting with current node, then just update the adjacent link info//
//				//Not need to create a new cLink objective//
//				if (j_index<i_index)
//				{
//					tem_plink = isDuplicate(phyLink_vec,j_index+1,i_index+1);
//					tem_node.adjacentLinkID_vec.push_back(tem_plink->getId());
//					
//					continue;
//				}
//
//				cLink tem_link(link_index,slotPerLink,(ID)(i_index+1),(ID)(j_index+1));
//				
//				//Initial spectrum objectives for each link//
//				for (i = 0;i<slotPerLink;i++)
//				{
//					cSpectrum _specSlot;
//					tem_link.spetrumAllocation_vec.push_back(_specSlot);
//				}
//
//				tem_link.spectrumIsAllocation.assign(tem_slots);
//				tem_node.adjacentLinkID_vec.push_back(link_index);
//				phyLink_vec.push_back(tem_link);
//				link_index++;
//			}
//		}
//
//		phyNode_vec.push_back(tem_node);
//	}
//
//	
//	for (iter_node = phyNode_vec.begin();iter_node!=phyNode_vec.end();iter_node++)
//	{
//		vector<ID>::iterator iter_id;
//		
//		for (iter_id = iter_node->adjacentLinkID_vec.begin();iter_id!=iter_node->adjacentLinkID_vec.end();iter_id++)
//		{
//			iter_node->adjacentLink_vec.push_back(&phyLink_vec[*iter_id - 1 ]);
//		}
//	}
//	
//	return 0;
//}

int cTopology::createTopology(cVirtTopology& _virtTopo)
{
	vector<cVirtNode>::iterator  iter_virNode_vec;
	vector<cVirtLink>::iterator  iter_virLink_vec;

	char   tem_slots[slotPerLink+1];
	int i;

	//Initial spectrum status//
	for (i=0;i<slotPerLink;i++)
	{
		tem_slots[i] = 'E';
	}
	tem_slots[i] = '\0';
	

	for (iter_virNode_vec = _virtTopo.virtNode_vec.begin();iter_virNode_vec!=_virtTopo.virtNode_vec.end();iter_virNode_vec++)
	{
		cNode tem_node(*iter_virNode_vec);
		phyNode_vec.push_back(tem_node);
	}

	for (iter_virLink_vec = _virtTopo.virtLink_vec.begin();iter_virLink_vec!=_virtTopo.virtLink_vec.end();iter_virLink_vec++)
	{
		cLink tem_link(*iter_virLink_vec);

		for (i = 0;i<slotPerLink;i++)
		{
			cSpectrum _specSlot;
			tem_link.spectrumAllocation_vec.push_back(_specSlot);
		}

		tem_link.spectrumAllocationWorkingPath.assign(tem_slots);
		tem_link.spectrumAllocationBackupPath.assign(tem_slots);

		tem_link.setTotalSlot(slotPerLink);

		tem_link.setFragmentationCount(1);

		phyLink_vec.push_back(tem_link);
	}

	return 0;
}