#pragma once
//#include "cLink.h"
//#include "cNode.h"

class cLink;
class cNode;


class cRequest
{
private:
	ID	id;
	ID  sourceNodeID;
	ID  destNodeID;
	int resourceDemand;
	double startingTime;
	double durationTime;
	bool isService;

public:
	void setId(ID _id){id = _id;}
	ID   getId(void) const {return id;}

	void setSourceNodeID(ID _id){sourceNodeID = _id;}
	ID   getSourceNodeID(void) const {return sourceNodeID;}

	void setDestNodeID(ID _id){destNodeID = _id;}
	ID   getDestNodeID(void) const {return destNodeID;}

	void setResourceDemand(int _demand){resourceDemand = _demand;}
	int  getResourceDemand(void) const {return resourceDemand;}

	void setStartingTime(double _time){startingTime = _time;}
	double getStartingTime(void) const {return startingTime;}

	void setDuringTime(double _time){durationTime = _time;}
	double getDuringTime(void) const {return durationTime;}

	void setIsService(bool _isService) {isService = _isService;}
	bool getIsService(void)const {return isService;}

public:
	//store the nodes info along the working path
	vector<pair<ID,cNode*>> workingPathNode_vec;


	//store the links info along the working path
	vector<pair<ID,cLink*>> workingPathLink_vec;


	//store the nodes info along the backup path
	vector<pair<ID,cNode*>> backupPathNode_vec;

	
	//store the links info along the backup path
	vector<pair<ID,cLink*>> backupPathLink_vec;
	
	//store the spectrum index allocated to working path 
	int workingPathSpectrumIndex;

	//store the spectrum index allocated to backup path
	int backupPathSpectrumIndex;

public:
	void setWorkingPathSpecIndex(int _index){workingPathSpectrumIndex = _index;}
	int  getWorkingPathSpecIndex(void) const{return workingPathSpectrumIndex;}

	void setBackupPathSpecIndex(int _index){backupPathSpectrumIndex = _index;}
	int  getBackupPathSpecIndex(void) const {return backupPathSpectrumIndex;}

public:
	int havingCommonWorkingLink(cRequest* _request);
public:
	cRequest(void);
	cRequest(ID _id, bool _isService);
	cRequest(ID _id, ID _srcID,ID _desID, int _resDemand,double _startingTime,double _durationTime,bool _isService = false);
	cRequest(const cRequest& _request);
	cRequest& operator=(const cRequest& _request);
	~cRequest(void);
};



