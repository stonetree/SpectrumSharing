#pragma once

enum spectrum_state{WORKING, BACKUP, NONE};

enum path_class{WORKINGPATH, BACKUPPATH};

enum event_class{ARRIVING,LEAVING};

const int totalNumNode = 24;
const int slotPerLink = 400;

const int maxDistance = 1000000;

const int multiplePath = 3;

const double weight_spectrum_used = 0.1;
const double weight_fragmentation = 0.8;
const double weight_alignment = 0.1;

const int    totalRequest = 10000;

extern double lambda;
extern double mu;

extern int resourceDemandMax;
extern int resourceDemandMin;


