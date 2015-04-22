 /* libnltrack
    Copyright (C) 2008 Ercan Akyürek <geforcefan@googlemail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more dedas warn spaß, ich würde dich nicht mal auf unser geländer lassentails.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef _LIBNLTRACK_H_
#define _LIBNLTRACK_H_

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Vertex type */
typedef struct 
{
	float x, y, z;
} Vertex;

/* Color type */
typedef struct 
{
    signed int a,r,g,b;
} Color;

/* Train type */
typedef struct 
{
	int useIndividualColor;
	char *textureFilename;
	Color seatColor, restraintColor, trainColor, gearColor;
} Train;

/* Bezier type */
typedef struct
{
	float posX, posY, posZ;
	float cp1X, cp1Y, cp1Z;
	float cp2X, cp2Y, cp2Z;
	float roll;
	int continuesRoll, equalDistanceCP, relativeRoll;
} Bezier;

typedef struct
{
	int type;
	float pos;		
} SegmentNode;

typedef struct
{
	int totalRASCs;
	SegmentNode *data;
} SegmentNodes;


/* Fundament Node type */
typedef struct
{
	int useSizeValue;
	float size;
	int useRotationValue;
	float rotation;
	char fundType;
    int flags;
    float posX, posY, posZ;
    Color color;
} FundNode;

/* Free Nodes type */
typedef struct 
{
	float posX, posY, posZ;
	int flags;
} FreeNode;

/* Tube type */
typedef struct 
{
	int n1type, n1segIndex, n1index;
	int n2type, n2segIndex, n2index;
	int tubeType, flags;
	float rotation;
    Color color;
} Tube;

/* Object type */
typedef struct
{
	char *dummyString, *objectFilename;
	float
		_11, _12, _13, _14,
		_21, _22, _23, _24,
		_31, _32, _33, _34,
		_41, _42, _43, _44
	;
} SCOBObject;

/* Tree object type*/
typedef struct
{
	float posX, posY, posZ;
	int treeType;
	float rotation;
} Tree;

typedef struct 
{
	char reserved;
} SkipByte;

typedef struct Catwalk
{
	int fn1index, fn2index, fn3index, fn4index;
	int flags, catwalkType, skipByteCount;
	SkipByte *skipBytes;
} Catwalk;

/* Segment Station Type */
typedef struct 
{
	float waitAvg, waitMin, waitMax, waitDev;
	char entranceSide, stationType;
	int sync;
	char stationHidden;
	float mainBrakeDecel;
	char numPasses, reverseCableLift;
	int hideBreaks;
	int usePreBrake;
	float preBrakeSpeedLimit, preBrakeDecel, preBrakeHyster;
	int useTransportation;
	float transportSpeed, transportAccel, transportDecel;
	char transportType;
	float launchAccel;
	int useLaunch;
} NLSegmentStation;

/* Segment Lift Type */
typedef struct 
{
	char liftStyle;
	float speed, accel, decel;
	int railingLeft, railingRight;
	int flags;
} SegmentLift;

/* Segment Transport Type */
typedef struct 
{
	float speed, accel, decel;
	char transportStyle;
	int railingLeft, railingRight;
	char speedUpPasses, speedDown;
	float speedDownSpeedLimit;
} SegmentTransport;

/* Segment Break Type */
typedef struct 
{
	float speedLimit, decel, hyster;
	float triggerPos;
	int blockSegment, useTransport;
	float transportSpeed, transportAccel, transportDecel;
	int railingLeft, railingRight, completeStop;
	char stopWaitTime;
	int transportType;
	int launch;
	float launchAccel;
	int newTriggerBehaviour;
} SegmentBreak;

/* Segment */
typedef struct
{
    int size;
	int type;
	int individualTrackColor;
    Color trackSpineColor, trackRailColor, trackCrosstieColor;
	int useTunnel;
	int flags;
    NLSegmentStation station;
	SegmentLift lift;
	SegmentTransport transport;
	SegmentBreak breaks;
} SEGMSegment;

/* NLTRACK type */
typedef struct 
{
	/* NLIM */
	int nlimsize;
	char major;
	char minor;
	char revision;
	char markup;
 	char reserved;
	int use16friction;		

	/* TRCK */
  int trcksize;
  int style;
  int numTrains;
  int numCars;		

	Train *train;
	Color trackSpineColor, trackRailColor, trackCrosstieColor;
	Color supportsColor;
	Color trainSeatColor;
	Color trainRestraintColor;
	Color trainColor;
	Color trainGearColor;
	char reserved0, reserved1, reserved2, reserved3;
	int trackMode;
	int useTunnelColor;
	Color tunnelColor;

	/* INFO */
	int infosize;
	char *authorComments;
	
	/* BEZR */
	int bezrsize;
	int numBeziers;
	Bezier *beziers;

	/* SEGM */
	int segmsize;
    int totalSegments;
	int trackClosedCircuit;
	SEGMSegment *segs;

	/* SUPP */
	int suppsize;
	

	/* FUND */
  int fundsize;
  int totalFundNodes;
  FundNode *fundNodes;

	/* RASC */
  int rascsize;
  int totalSegment;
  SegmentNodes *seg;	
	
	/* FREN */
  int frensize;
  int totalFreeNodes;
  FreeNode *freeNodes;

	/* TUBE */
	int tubesize;
	int totalTubes;
	Tube *tubes;

	/* CATW */
  int catwsize;
  int totalCatwalks;
  Catwalk *catwalks;

	/* SCEN */
	int scensize;
	int chunkscen;
	int totalTrees;
	Tree *trees;

	/* SCOB */
	int scobsize;
	int totalObjects;
	SCOBObject *objects;

	/* ENVC */
	int envcsize;
	char *envFilename, *reservedString;

	/* TERA */
	int terasize;
	int sizeX, sizeZ;
	float scaleX, scaleZ;
	char *groundTextureFilename;
	float numGroundTextureRepeats;
	char *detailTextureFilename;
	float numDetailTextureRepeats;
	int enableWater;
	float seaLevel;
	char *waterTextureFilename;
	float numWaterTextureRepeats;
	char waterReflectiveness;
	int useWaves;
	float waveScale;
    float terrainHeight[2048][2048];
} NlTrack;

/* Little or Big Endian */
#define LITTLE_ENDIAN_NL 0
#define BIG_ENDIAN_NL 1

/* NoLimits Track Chunks */
#define NLIM 1
#define TRCK 2 
#define INFO 3
#define BEZR 4 
#define SEGM 5
#define SUPP 6
#define FUND 7
#define RASC 8 
#define FREN 9
#define TUBE 10
#define CATW 11
#define SCEN 12
#define SCOB 13
#define TERA 14
#define ENVC 15

/* NoLimits Coaster Types */
#define CLASSIC_STEEL_LOOPING 0
#define CORKSCREW_COASTER 1
#define INVERTED_COASTER_2 2
#define TWISTED_STEEL_COASTER 3
#define INVERTED_COASTER_4 4
#define HYPER_COASTER 5
#define FLOORLESS_TWISTED_STEEL_COASTER 6
#define STANDUP_TWISTED_STEEL_COASTER 7
#define HYPER_COASTER_4 8
#define WOODEN_COASTER_TRAILERED_2 9
#define WOODEN_COASTER_CLASSIC_4 10
#define WOODEN_COASTER_CLASSIC_6 11
#define WOODEN_COASTER_TRAILERED_4 12
#define LIM_LAUNCHED_COASTER 13
#define INVERTED_FACE_TO_FACE_COASTER 14
#define INVERTED_IMPULSE_COASTER 15
#define VEKOMA_FLYING_DUTCHMAN 18
#define MAURER_SOEHNE_SPINNING_COASTER 20
#define GERSTLAUER_EURO_FIGHTER 36
#define VEKOMA_MOTORBIKE_COASTER 38
#define GERSTLAUER_BOBSLED_COASTER 39
#define GERSTLAUER_SPINNING_COASTER 41
#define GERSTLAUER_EURO_FIGHTER_2 47
#define STEEL_LOOPING_COASTER 49
#define VERKOMA_MINE_TRAIN_COASTER 34
#define VERKOMA_MINE_TRAIN_COASTER_LOC 35
#define MAURER_SOEHNE_X_CAR_COASTER 50

/* NoLimits Segment Types */
#define SEGMENT_NORMAL 0
#define SEGMENT_STATION 1 
#define SEGMENT_LIFT 2
#define SEGMENT_TRANSPORT 3
#define SEGMENT_BREAK 4
#define SEGMENT_UNKNOWN 5

/* NoLimits Node Types */
#define NODE_FUND 1
#define NODE_FREE 2
#define NODE_SEGMENT 3

/* NoLimits RASC Types */
#define RASC_TRACK_NODE 0
#define RASC_NONE 1
#define RASC_SINGLE 2
#define RASC_COMPLEX 3 
#define RASC_INVERTED 4
#define RASC_90_BANK 5
#define RASC_SMALL_INVERTED 6
#define RASC_SHAPE 7
#define RASC_LOOPING_BOX 8

/* NoLimits Tube Types */
#define TUBE_MEDIUM 0
#define TUBE_LARGE 1
#define TUBE_SMALL 2
#define TUBE_L_BEAM 3
#define TUBE_H_BEAM 4
#define TUBE_X_LARGE 5
#define TUBE_XX_LARGE 6
#define TUBE_4_4_WOOD_BEAM 7
#define TUBE_1_4_WOOD_BEAM 8
#define TUBE_2_2_WOOD_BEAM 9

/* NoLimits Operating Mode */
#define MODE_NORMAL 0
#define MODE_SHUTTLE 1

#define toRad(value) value / 57.29577951

/* 
 * Swap bits 
 */
void swap( char* v1, char* v2 );

/* 
 * Swap Endian from float type
 */
void SwapEndianFloat( float* val );

/* 
 * Swap Endian from int type
 */
void SwapEndianInt( int* val );

/* 
 * Byte to Int
 */
int byteInt ( char data[3], int endian ) ;

/* 
 * Byte from FILE to Float
 */
float byteFloat ( FILE *file, int endian ) ;

/* 
 * File Length
 */
long filelength(int f);

/* 
 * Read Bytes
 */
char* ReadBytes( FILE *file, size_t length );

/* 
 * Read Byte
 */
char ReadByte( FILE *file );

/* 
 * Read Bool
 */
int ReadBool( FILE *file );

/* 
 * Read Key
 */
int ReadKey( FILE *file );

/* 
 * Read Int
 */
int ReadInt( FILE *file );

/* 
 * Read Float
 */
float ReadFloat( FILE *file  );

/* 
 * Read Null
 */
void ReadNull( FILE *file , size_t length );

/* 
 * Read String
 */
char *ReadString( FILE *file, size_t length );

/* 
 * Read Color
 */
Color ReadColor( FILE *file );

/*
 * Open NLTRACK
 */
int NlOpen( char *filename, NlTrack *Track);
int NlSave( char *filename, NlTrack *Track);

/*
 * Get the string of a coaster type
 */
char *NlGetCoasterType(int type);


int 	addBezier (float posX, float posY, float posZ, float cp1X, float cp1Y, float cp1Z, float cp2X, float cp2Y, float cp2Z, float roll, int continuesRoll, int equalDistanceCP, int relativeRoll, NlTrack *Track);
void 	modifyBezier (int bezierindex, float posX, float posY, float posZ, float cp1X, float cp1Y, float cp1Z, float cp2X, float cp2Y, float cp2Z, float roll, int continuesRoll, int equalDistanceCP, int relativeRoll, NlTrack *Track);
void 	deleteBezier (int bezierindex, NlTrack *Track);
Bezier 	getBezier (int bezierindex, NlTrack *Track);
int 	getNumBeziers (NlTrack *Track);


int 	setEnvc (char *envFilename, char *reservedString, NlTrack *Track);
char* 	getEnvFilename (NlTrack *Track);
char* 	getReservedString (NlTrack *Track);

int 	addFreeNode (float posX, float posY, float posZ, NlTrack *Track);
void 	modifyFreeNode (int nodeindex, float posX, float posY, float posZ, NlTrack *Track);
void 	deleteFreeNode (int nodeindex, NlTrack *Track);
FreeNode 	getFreeNode (int nodeindex, NlTrack *Track);
int 	getTotalFreeNodes (NlTrack *Track);

int 	addFundNode (int useSizeValue, float size, int useRotationValue, float rotation, char fundType, int flags, float posX, float posY, float posZ, NlTrack *Track);
FundNode getFundNode(int fundindex, NlTrack *Track);
int 	modifyFundNode (int fundindex, int useSizeValue, float size, int useRotationValue, float rotation, char fundType, int flags, float posX, float posY, float posZ, NlTrack *Track);
void 	deleteFundNode (int fundindex, NlTrack *Track);
int 	getTotalFundNodes (NlTrack *Track);
int     getFundamentSize(int fundindex, NlTrack *Track);

void 	setAuthorComments (char *authorComments, NlTrack *Track);
char * 	getAuthorComments (NlTrack *Track);

int 	initNewNlTrack (NlTrack *Track);
void 	setCoasterStyle (int style, NlTrack *Track);
int 	getCoasterStyle (NlTrack *Track);
void 	setMajor (int major, NlTrack *Track);
int 	getMajor (NlTrack *Track);
void 	setMinor (int minor, NlTrack *Track);
int 	getMinor (NlTrack *Track);
void 	setRevision (int revision, NlTrack *Track);
int 	getRevision (NlTrack *Track);
void 	setMarkup (int markup, NlTrack *Track);
int 	getMarkup (NlTrack *Track);
void 	setUse16friction (int use16friction, NlTrack *Track);
int 	getUse16friction (NlTrack *Track);

int 	addSegment (int segmenttype, int bezierindex, int individualTrackColor, Color trackSpineColor, Color trackRailColor, Color trackCrosstieColor, int useTunnel, int flags, NlTrack *Track);
int addSegmentFromStruct(SEGMSegment Segment, int bezierindex, NlTrack *Track);

void 	modifySegment (int segmenttype, int bezierindex, int individualTrackColor, Color trackSpineColor, Color trackRailColor, Color trackCrosstieColor, int useTunnel, int flags, NlTrack *Track);
SEGMSegment 	getSegment (int bezierindex, NlTrack *Track);
void 	deleteSegment (int segmenttype, NlTrack *Track);
int 	getTotalSegments (NlTrack *Track);
void 	setSegmentStation (int bezierindex, float waitAvg, float waitMin, float waitMax, float waitDev, char entranceSide, char stationType, int sync, char stationHidden, float mainBrakeDecel, char numPasses, char reverseCableLift, int hideBreaks, int usePreBrake, float preBrakeSpeedLimit, float preBrakeDecel, float preBrakeHyster, int useTransportation, float transportSpeed, float transportAccel, float transportDecel, char transportType, float launchAccel, int useLaunch, NlTrack *Track);
NLSegmentStation 	getSegmentStation (int bezierindex, NlTrack *Track);
void 	setSegmentLift (int bezierindex, char liftStyle, float speed, float accel, float decel, int railingLeft, int railingRight, int flags, NlTrack *Track);
SegmentLift 	getSegmentLift (int bezierindex, NlTrack *Track);
void 	setSegmentTransport (int bezierindex, float speed, float accel, float decel, char transportStyle, int railingLeft, int railingRight, char speedUpPasses, char speedDown, float speedDownSpeedLimit, NlTrack *Track);
SegmentTransport 	getSegmentTransport (int bezierindex, NlTrack *Track);
void 	setSegmentBreak (int bezierindex, float speedLimit, float decel, float hyster, float triggerPos, int blockSegment, int useTransport, float transportSpeed, float transportAccel, float transportDecel, int railingLeft, int railingRight, int completeStop, char stopWaitTime, int transportType, int launch, float launchAccel, int newTriggerBehaviour, NlTrack *Track);
SegmentBreak 	getSegmentBreak (int bezierindex, NlTrack *Track);
Vertex 	getPosSegmentNode (int index, int segindex, NlTrack *Track);

Color 	createColorARGB (int a, int r, int g, int b);
float 	toG (float index);
float 	toNewton (float index);
float 	toMs (float index);
float 	toKmh (float index);

void 	setNumTrains (int numTrains, NlTrack *Track);
int 	getNumTrains (NlTrack *Track);
void 	setNumCars (int numCars, NlTrack *Track);
int 	getNumCars (NlTrack *Track);
int 	setTrainProperty (int trainindex, char *textureFilename, int useIndividualColor, Color seatColor, Color restraintColor, Color trainColor, Color gearColor, NlTrack *Track);
Train 	getTrainProperty (int trainindex, NlTrack *Track);
Color 	getTrackSpineColor (NlTrack *Track);
Color 	getTrackRailColor (NlTrack *Track);
Color 	getTrackCrosstieColor (NlTrack *Track);
Color 	getTrackSupportsColor (NlTrack *Track);
Color 	getTrackTrainSeatColor (NlTrack *Track);
Color 	getTrackTrainRestraintColor (NlTrack *Track);
Color 	getTrackTrainColor (NlTrack *Track);
Color 	getTrackTrainGearColor (NlTrack *Track);
Color 	getTrackTunnelColor (NlTrack *Track);
int 	getTrackUseTunnelColor (NlTrack *Track);
void 	setTrackSpineColor (NlTrack *Track, Color trackSpineColor);
void 	setTrackRailColor (NlTrack *Track, Color trackRailColor);
void 	setTrackCrosstieColor (NlTrack *Track, Color trackCrosstieColor);
void 	setTrackSupportsColor (NlTrack *Track, Color supportsColor);
void 	setTrackTrainSeatColor (NlTrack *Track, Color trainSeatColor);
void 	setTrackTrainRestraintColor (NlTrack *Track, Color trainRestraintColor);
void 	setTrackTrainColor (NlTrack *Track, Color trainColor);
void 	setTrackTrainGearColor (NlTrack *Track, Color trainGearColor);
void 	setTrackTunnelColor (NlTrack *Track, Color tunnelColor);
void 	setTrackUseTunnelColor (NlTrack *Track, int useTunnelColor);

int 	addSupport (int n1type, int n1index, int n1segIndex, int n2type, int n2index, int n2segIndex, int tubeType, int flags, int rotation, NlTrack *Track);
void 	modifySupport (int suppindex, int tubeType, int flags, int rotation, NlTrack *Track);
void 	deleteSupport (int suppindex, NlTrack *Track);
Tube 	getSupport (int suppindex, NlTrack *Track);
int 	getTotalSupport (NlTrack *Track);

int 	addTrackNode(int segindex, int type, float pos, NlTrack *Track);
void	modifyTrackNode(int segindex, int rascindex, int type, float pos, NlTrack *Track);
void 	deleteTrackNode(int segindex, int rascindex, NlTrack *Track);
SegmentNode 	getTrackNode(int segindex, int rascindex, NlTrack *Track);
int 	getTotalTrackNodes(int segindex, NlTrack *Track);

#endif
