#include "StdAfx.h"
#include "cSpectrum.h"


cSpectrum::cSpectrum(void)
{
	isUsed = false;
	sharingAvailable = false;
	state = NONE;
}

cSpectrum::cSpectrum(const cSpectrum& _spretrum)
{
	operator=(_spretrum);
}

cSpectrum& cSpectrum::operator=(const cSpectrum& _spectrum)
{
	if (this!=&_spectrum)
	{
		this->id = _spectrum.id;
		this->isUsed = _spectrum.isUsed;
		this->sharingAvailable = _spectrum.sharingAvailable;
		this->state = _spectrum.state;

		this->request_map.erase(request_map.begin(),request_map.end());
		this->request_map.insert(_spectrum.request_map.begin(),_spectrum.request_map.end());
	}

	return *this;
}

cSpectrum::~cSpectrum(void)
{
}
