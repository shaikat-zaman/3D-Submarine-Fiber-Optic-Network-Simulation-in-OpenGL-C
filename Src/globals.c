#include "common.h"

// Window configuration parameters
int   winW=1440, winH=820;

// Environmental physics and animation timers
float gTime=0.0f, windAngle=0.0f, waveOff=0.0f;

// Global simulation toggles (Day/Night cycle, Cable integrity)
int   isDay=1, cableOK=1;

// Packet transfer speed multiplier
float dataspd=0.55f;

// Network statistics counters
int   pktCnt=0, totalSent=0;

// Mouse input tracking state variables
int   mDown=0, mLX=0, mLY=0;

// Camera view matrix parameters (Yaw, Pitch, Distance)
float camYaw=0.0f, camPitch=15.0f, camDist=33.2f;

// Active communication packet array
Packet pkts[MAX_PKT];

// Aquatic ambient life arrays
Fish   fish[MAX_FISH];
Bubble bbl[MAX_BUBBLE];

// Traffic vehicle simulation arrays for both regions
Car    bdCar[MAX_CARS], sgCar[MAX_CARS];
char skyColor[4][3] = {{.7f,.85f,1.f},{.05f,.05f,.2f}}; // Store sky color for fog blending

// Predator entity array
Shark  sharks[MAX_SHARK];

// Ocean surface wave heightmap array
float  wH[WAVE_RES+1][WAVE_RES+1];

// NEW: Initialize Admin & UI State Variables
int simState = 0; // Starts at BSCCL Admin Panel
char bscclGbps[20] = "";
int menuSelectedHouse = 0;
int menuEditMode = 0;


// Initializing 8 houses to offline state with default packet types (0 to 4)
HouseInfo housesData[8] = {
    {0, "", "", 0,0,0, 0}, {0, "", "", 0,0,0, 1}, {0, "", "", 0,0,0, 2}, {0, "", "", 0,0,0, 3},
    {0, "", "", 0,0,0, 4}, {0, "", "", 0,0,0, 0}, {0, "", "", 0,0,0, 1}, {0, "", "", 0,0,0, 2}
};
