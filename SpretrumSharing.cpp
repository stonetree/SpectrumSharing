// SpretrumSharing.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "common.h"
#include "cTopology.h"
#include "cVirtTopology.h"
#include "cRequest.h"

extern int inputTopologyInfo(char _topo[][totalNumNode+1]);
extern int initialInputfile(vector<pair<double,double>>& inputParameter);
extern int shortestPath(cVirtTopology& _topo, ID _srcNode, ID _destNode,vector<ID>& _path);
extern int findMultipleShortestPath(cVirtTopology& _topo, int _numPath, ID _srcNode, ID _desNode,vector<vector<ID>>& _multiPath);
extern int requestArriving(cVirtTopology& _virTopo, cTopology& _topo, cRequest* _request);
extern int requestLeaving(cTopology& _topo, cRequest* _request);

extern int generateArrivingTime(vector<double>& _randomNum,double _mu);
extern int generateDuringTime(vector<double>& _randomNum,double _mu);
extern void initialRequests(vector<cRequest>& _request_vec, vector<double>& _arrivingTime, vector<double>& _durationTime,vector<int>& _resourceDemand,cTopology& _topo);
extern void initialEventSet(vector<cRequest>& _request_vec,map<double,pair<event_class,cRequest*>>& _event_set);
extern int uniformGenerator(vector<int>& _demand, int _max,int _min);


int _tmain(int argc, _TCHAR* argv[])
{
	
	/////Variable definition////
	char topoInfo[totalNumNode][totalNumNode+1];
	map<double,pair<event_class,cRequest*>>::iterator iter_event;

	vector<pair<double,double>> inputParameter;

	initialInputfile(inputParameter);

	//test request set
	vector<cRequest> request_vec;

	//event set containing all events of requests arriving and leaving
	map<double,pair<event_class,cRequest*>> event_set;

		
	cout<<"Initial virtual topology...."<<endl;
	//Read topology file//
	//If loading topology file fails, exit directly.
	if(inputTopologyInfo(topoInfo) != 0)
	{
		cout<<"Fail to load topology file!!!"<<endl;
		return -2;
	}

	ofstream outputFile;
	vector<pair<double,double>>::iterator iter_inputParameter;

	for (iter_inputParameter = inputParameter.begin();iter_inputParameter != inputParameter.end();iter_inputParameter++)
	//for (resourceDemandMax = 20;resourceDemandMax<=40;resourceDemandMax+=10)
	{
		
		//initial input parameter erlang
		lambda = iter_inputParameter->first;
		mu = iter_inputParameter->second;
		//lambda = 50;
		//mu = 1;

		//Initial virtual topology, which includes the nodes and links info as well as the connections info between links//
		//The virtual topology will be used to find the shortest path for each request.//
		cVirtTopology virtTopology;
		virtTopology.createVirtTopology(topoInfo);

		//Initial physical topology, which is created from the virtual topology objective//
		//Besides the info that are already included in the virtual topology, //
		//it also initial the spectrum objectives for each link(physical link)//
		cout<<"Initial physical topology...."<<endl;
		cTopology topology;
		topology.createTopology(virtTopology);

		
		//generate arriving time sequence
		vector<double> arrivingTime;
		generateArrivingTime(arrivingTime,lambda);

		//generate duration time sequence
		vector<double> durationTime;
		generateDuringTime(durationTime,mu);

		//generate resource demand sequence
		vector<int> resourceDemand;
		vector<int>::iterator iter_resourceDemand;
		uniformGenerator(resourceDemand,resourceDemandMax,resourceDemandMin);

		cout<<"Initial requests...."<<endl;
		//initial requests
		initialRequests(request_vec,arrivingTime,durationTime,resourceDemand,topology);

		//initial event set
		initialEventSet(request_vec,event_set);

		
		iter_resourceDemand = resourceDemand.begin();
		for (iter_event = event_set.begin();iter_event!=event_set.end();iter_event++)
		{
			if (iter_event->second.first == ARRIVING)
			{
				iter_event->second.second->setResourceDemand(*iter_resourceDemand);
				iter_resourceDemand++;
			}
		}

		//store paths obtained
		vector<vector<ID>> path;

		cout<<"Start Processing....."<<endl;

		int i = 1;
		for (iter_event = event_set.begin();iter_event!=event_set.end();iter_event++)
		{
			cout<<"Processing the "<<i++<<"th even"<<endl;
			if (iter_event->second.first == ARRIVING)
			{
				requestArriving(virtTopology, topology, iter_event->second.second);
			}
			else
			{
				if (iter_event->second.second->getIsService())
				{
					requestLeaving(topology,iter_event->second.second);
				}
			}

		}

		//collect results
		vector<cRequest>::iterator iter_request;
		int numBlockedReq = 0;
		int numTotalReq = 0;
		int blockedBandwidth = 0;
		int totalBandwidthRequied = 0;
		for (iter_request = request_vec.begin();iter_request != request_vec.end();iter_request++)
		{
			totalBandwidthRequied += iter_request->getResourceDemand();
			numTotalReq++;
			if (iter_request->getIsService() == false)
			{
				blockedBandwidth += iter_request->getResourceDemand();
				numBlockedReq++;
			}
		}

		//output results
		outputFile.open("output.txt",ios::app);

		if (!outputFile)
		{
			cout<<"Find open output file!"<<endl;
			return 0;
		}
		outputFile<<lambda<<" "<<mu<<" "<<(double)(totalBandwidthRequied - blockedBandwidth)/totalBandwidthRequied<<" "<<(double)numBlockedReq/numTotalReq<<endl;

		outputFile.close();


		//clear or data structures
		arrivingTime.clear();
		durationTime.clear();
		resourceDemand.clear();
		request_vec.clear();
		event_set.clear();
		path.clear();
		
	}


	return 0;
}

