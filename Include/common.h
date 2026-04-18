#ifndef COMMON_H
#define COMMON_H

// Standard headers and graphics libraries
#ifdef _WIN32
  #include <windows.h>
#endif
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants: Mathematical parameters, simulation array limits, and mesh resolutions
#define PI          3.14159265358979f
#define TAU         (2.0f*PI)
#define D2R         (PI/180.0f)
#define MAX_PKT     40
#define TRAIL_LEN   14
#define MAX_FISH    20
#define MAX_BUBBLE  80
#define MAX_CARS    6
#define WAVE_RES    50
#define CABLE_SEG   100
#define MAX_SHARK   4

// Packet: Data payload structure tracking trajectory and trail history
typedef struct { float t; int active, dir; float r,g,b; float tx[TRAIL_LEN], ty[TRAIL_LEN], tz[TRAIL_LEN]; int tlen; } Packet;
// Fish: Aquatic entity with spatial, movement, and color properties
typedef struct { float x,y,z, spd, r,g,b, phase; int dir; } Fish;
// Bubble: Particle entity with lifespan and buoyancy characteristics
typedef struct { float x,y,z, spd, sz, life; } Bubble;
// Car: Vehicle entity for lane-based traffic simulation
typedef struct { float x,z, spd, r,g,b; int dir, state; float phase, angle; } Car;
// Shark: Predator entity with designated patrol states
typedef struct { float x,y,z, spd, phase; int dir; } Shark;

// Extern Globals: Shared simulation states, user input tracking, and camera matrices
extern int   winW, winH;
extern float gTime, windAngle, waveOff;
extern int   isDay, cableOK;
extern float dataspd;
extern int   pktCnt, totalSent;
extern int   mDown, mLX, mLY;
extern float camYaw, camPitch, camDist;

extern Packet pkts[MAX_PKT];
extern Fish   fish[MAX_FISH];
extern Bubble bbl[MAX_BUBBLE];
extern Car    bdCar[MAX_CARS], sgCar[MAX_CARS];
extern Shark  sharks[MAX_SHARK];
extern float  wH[WAVE_RES+1][WAVE_RES+1];

// cablePt: Computes 3D coordinates along the parametric cable curve
void cablePt(float t, float*ox, float*oy, float*oz);
// spawnPkt: Initializes and injects a new data packet into the simulation
void spawnPkt(int dir,float r,float g,float b);

// Material Utilities: Sets OpenGL lighting material properties (diffuse, specular, emission)
void mat(float r,float g,float b,float sp,float sh);
void matE(float r,float g,float b,float er,float eg,float eb);
void noE(void);

// 3D Primitives: Renders basic geometric meshes
void box(float cx,float cy,float cz,float w,float h,float d);
void sph(float x,float y,float z,float r,int sl);
void cyl(float x1,float y1,float z1,float x2,float y2,float z2,float rad,int sl);
void disk3(float x,float y,float z,float r,int sl);

// drawStr3: Renders bitmap 3D text in spatial coordinates
void drawStr3(float x,float y,float z,const char*s,void*f);

// drawLineDDA: Draws 3D line segments utilizing the DDA (Digital Differential Analyzer) algorithm
void drawLineDDA(float x1, float y1, float x2, float y2, float z);
// drawCircleMidpoint: Draws filled circular shapes utilizing the Midpoint Circle algorithm
void drawCircleMidpoint(float cx, float cy, float cz, float r);
// drawCircleOutline: Draws circular boundaries utilizing the Midpoint Circle algorithm
void drawCircleOutline(float cx, float cy, float cz, float r);

// drawBeach: Generates procedural terrain mesh for coastal landmasses
void drawBeach(float xC,float zC,float w,float d,int isBD);
// drawRealTree & drawPalm: Renders procedural foliage and tree models
void drawRealTree(float x, float z, float scale);
void drawPalm(float x,float z);
// drawWindmill: Renders rotating energy turbine structure
void drawWindmill(float wx,float wz);
// drawSatellite: Renders orbiting communication satellite
void drawSatellite(void);

// Environment: Renders atmospheric elements, billboard clouds, and celestial bodies
void draw2DCloud(float x, float y, float scale, float alpha);
void drawSky(void);
void drawSunMoon(void);

// drawHUD: Renders 2D orthographic UI overlay
void drawHUD(void);
// setupLighting: Configures global Gouraud shading illumination
void setupLighting(void);

// Regional Renders: Assembles the respective environments for Bangladesh and Singapore
void drawBangladesh(void);
void drawSingapore(void);

// NEW: House Configuration Structure
typedef struct {
    int active;
    char ip[20];
    char bw[10];
    float currR, currG, currB; // For monitor table
    int pktType;               // NEW: 0=HTTP, 1=Data, 2=Video, 3=VoIP, 4=Encrypted
} HouseInfo;

// NEW: State and UI Management Globals
extern int simState; // 0 = BSCCL Admin, 1 = ISP Admin, 2 = Simulation
extern char bscclGbps[20];
extern HouseInfo housesData[8];
extern int menuSelectedHouse;
extern int menuEditMode; // 0=View, 1=Edit IP, 2=Edit BW, 3=Edit Gbps, 4=Edit Type

#endif
