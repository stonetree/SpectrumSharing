#pragma once

#include "stdafx.h"

class cRequest;

class cSpectrum
{
private:
	ID   id;
	bool isUsed;
	bool sharingAvailable; 
	spectrum_state state;


public:
	//list<cRequest*> request_list;
	map<ID,cRequest*> request_map;

public:
	void setId(ID _id){id = _id;}
	ID   getId(void)const {return id;}

	void setIsUsed(bool _isUsed){isUsed = _isUsed;}
	bool getIsUsed(void)const {return isUsed;}

	void setSharingAvailable(bool _sharing){sharingAvailable = _sharing;}
	bool getSharingAvailable(void) const {return sharingAvailable;}

	void setSpectrumState(spectrum_state _state) {state = _state;}
	spectrum_state getSpectrumState(void) const {return state;} 

public:
	cSpectrum(void);
	cSpectrum(const cSpectrum& _spretrum);
	cSpectrum& operator=(const cSpectrum& _spretrum);
	~cSpectrum(void);
};

