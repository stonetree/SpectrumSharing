#include "StdAfx.h"
#include "cNode.h"

#include "cVirtNode.h"


cNode::cNode(void)
{
}

cNode::cNode(cVirtNode& _virNode):cVirtNode(_virNode)
{

}

//cNode::cNode(ID _id)
//{
//	id = _id;
//}

//cNode::cNode(const cNode& _node)
//{
//	operator=(_node);
//}

cNode& cNode::operator=(const cNode& _node)
{
	if (this!= &_node)
	{
		//this->id = _node.id;

		//this->adjacentLinkID_vec.erase(adjacentLinkID_vec.begin(),adjacentLinkID_vec.end());
		//this->adjacentLinkID_vec.assign(_node.adjacentLinkID_vec.begin(),_node.adjacentLinkID_vec.end());

		//this->adjacentLink_vec.erase(adjacentLink_vec.begin(),adjacentLink_vec.end());
		//this->adjacentLink_vec.assign(_node.adjacentLink_vec.begin(),_node.adjacentLink_vec.end());
	}

	return *this;
}

cNode::~cNode(void)
{
}
