#include "stdafx.h"
#include "common.h"
#include "cRequest.h"
#include "cTopology.h"

#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"


int generateArrivingTime(vector<double>& _randomNum,double _mu)
{
  const gsl_rng_type * T;
  gsl_rng * r;

  int i;

  vector<double> intervalTime;
  vector<double>::iterator iter_itervalTime;
  
  _mu = 1/_mu;

  /* create a generator chosen by the 
     environment variable GSL_RNG_TYPE */

  _randomNum.clear();

  gsl_rng_env_setup();

  T = gsl_rng_default;
  r = gsl_rng_alloc (T);

  /* print n random variates chosen from 
     the exponential distribution with mean 
     parameter mu */

  for (i = 0; i < totalRequest - 1; i++) 
    {
      double k = gsl_ran_exponential(r, _mu);
	  intervalTime.push_back(k);
    }

  double nextStartingTime;
  _randomNum.push_back(0);
  nextStartingTime = 0;

  for (iter_itervalTime = intervalTime.begin();iter_itervalTime != intervalTime.end();iter_itervalTime++)
  {
	  nextStartingTime += *iter_itervalTime;
	  _randomNum.push_back(nextStartingTime);
  }

  gsl_rng_free (r);
  return 0;
}

int generateDuringTime(vector<double>& _randomNum,double _mu)
{
  const gsl_rng_type * T;
  gsl_rng * r;

  int i;

  vector<double> intervalTime;
  vector<double>::iterator iter_itervalTime;
  
  _mu = 1/_mu;

  /* create a generator chosen by the 
     environment variable GSL_RNG_TYPE */

  _randomNum.clear();

  gsl_rng_env_setup();

  T = gsl_rng_default;
  r = gsl_rng_alloc (T);

  /* print n random variates chosen from 
     the exponential distribution with mean 
     parameter mu */

  for (i = 0; i < totalRequest; i++) 
    {
      double k = gsl_ran_exponential(r, _mu);
	  _randomNum.push_back(k);
    }

  gsl_rng_free (r);
  return 0;
}

int uniformGenerator(vector<int>& _demand, int _max,int _min)
{
	const gsl_rng_type * T;
	gsl_rng * r;

	int i;

	gsl_rng_env_setup();

	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

	int window = _max - _min;

	for (i = 0; i < totalRequest; i++) 
	{
		int u = gsl_rng_uniform_int(r,window + 1);
		_demand.push_back(u + _min);
	}

	gsl_rng_free (r);

	return 0;
}

void generateNodePair(vector<cRequest>& _request_vec,cTopology& _topo)
{

	vector<cRequest>::iterator iter_request;
	
	const gsl_rng_type * T;
	gsl_rng * r;

	int  src,des;

	int numNode = _topo.phyNode_vec.size();

	gsl_rng_env_setup();

	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

	for (iter_request = _request_vec.begin();iter_request != _request_vec.end();iter_request++)
	{
		src = gsl_rng_uniform_int(r, numNode);
		des = gsl_rng_uniform_int(r, numNode);

		while(src == des)
		{
			des = gsl_rng_uniform_int(r, numNode);
		}

		iter_request->setSourceNodeID((ID)(src + 1));
		iter_request->setDestNodeID((ID)(des + 1));

	}
	


	gsl_rng_free (r);

	return;
	
	
	return ;
}