 /* libnltrack
    Copyright (C) 2008 Ercan Akyürek <ercan.akyuerek@googlemail.com>

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

#include "libnltrack.h"
#include <math.h>


int addSegmentFromStruct(SEGMSegment Segment, int bezierindex, NlTrack *Track)
{
    Track->totalSegments++;
    Track->segmsize = Track->segmsize + (Track->totalSegments * 37);
    Track->segs = (SEGMSegment*) realloc(Track->segs, Track->totalSegments * sizeof(SEGMSegment) );

    Track->segs[ bezierindex - 1 ] = Segment;

    int typesize;
    switch(Segment.type)
    {
     case SEGMENT_NORMAL:
          typesize = 0;
     break;
     case SEGMENT_STATION:
          typesize = 126;
     break;
     case SEGMENT_LIFT:
          typesize = 36;
     break;
     case SEGMENT_BREAK:
          typesize = 50;
     break;
     case SEGMENT_TRANSPORT:
          typesize = 28;
     break;
    }


    Track->rascsize = Track->rascsize + (Track->totalSegments * 4);
    Track->totalSegment = Track->totalSegments;
    Track->seg = (SegmentNodes *) realloc(Track->seg, Track->totalSegment * sizeof(SegmentNodes) );

    Track->seg[bezierindex - 1].totalRASCs = 0;
    Track->segs[ bezierindex - 1 ].size = typesize;
}

int addSegment( int segmenttype, int bezierindex, int individualTrackColor,
                                     Color trackSpineColor, Color trackRailColor, Color trackCrosstieColor,
									 int useTunnel, int flags, NlTrack *Track)
{
    Track->totalSegments++;
    Track->segmsize = Track->segmsize + (Track->totalSegments * 37);
    Track->segs = (SEGMSegment*) realloc(Track->segs, Track->totalSegments * sizeof(SEGMSegment) );

    Track->segs[ bezierindex - 1 ].type = segmenttype;
    Track->segs[ bezierindex - 1 ].individualTrackColor = individualTrackColor;
    Track->segs[ bezierindex - 1 ].trackSpineColor = trackSpineColor;
    Track->segs[ bezierindex - 1 ].trackRailColor = trackRailColor;
    Track->segs[ bezierindex - 1 ].trackCrosstieColor = trackCrosstieColor;
    Track->segs[ bezierindex - 1 ].useTunnel = useTunnel;
    Track->segs[ bezierindex - 1 ].flags = flags;
					
    int typesize;
    switch(segmenttype)
    {
     case SEGMENT_NORMAL:
          typesize = 0;
     break;
     case SEGMENT_STATION:
          typesize = 126;
     break;
     case SEGMENT_LIFT:
          typesize = 36;
     break;
     case SEGMENT_BREAK:
          typesize = 50;
     break;
     case SEGMENT_TRANSPORT:
          typesize = 28;
     break;
    }
    

    Track->rascsize = Track->rascsize + (Track->totalSegments * 4);
    Track->totalSegment = Track->totalSegments;
    Track->seg = (SegmentNodes *) realloc(Track->seg, Track->totalSegment * sizeof(SegmentNodes) );
				
    Track->seg[bezierindex - 1].totalRASCs = 0;
    Track->segs[ bezierindex - 1 ].size = typesize;
}

void modifySegment( int segmenttype, int bezierindex, int individualTrackColor,
                                     Color trackSpineColor, Color trackRailColor, Color trackCrosstieColor,
									 int useTunnel, int flags, NlTrack *Track)
{
    Track->segs[ bezierindex - 1 ].type = segmenttype;
    Track->segs[ bezierindex - 1 ].individualTrackColor = individualTrackColor;
    Track->segs[ bezierindex - 1 ].trackSpineColor = trackSpineColor;
    Track->segs[ bezierindex - 1 ].trackRailColor = trackRailColor;
    Track->segs[ bezierindex - 1 ].trackCrosstieColor = trackCrosstieColor;
    Track->segs[ bezierindex - 1 ].useTunnel = useTunnel;
    Track->segs[ bezierindex - 1 ].flags = flags;
					
    int typesize;
    switch(segmenttype)
    {
     case SEGMENT_NORMAL:
          typesize = 0;
     break;
     case SEGMENT_STATION:
          typesize = 126;
     break;
     case SEGMENT_LIFT:
          typesize = 36;
     break;
     case SEGMENT_BREAK:
          typesize = 50;
     break;
     case SEGMENT_TRANSPORT:
          typesize = 28;
     break;
    }
			
    Track->segs[ bezierindex - 1 ].size = typesize;
}

SEGMSegment getSegment( int bezierindex, NlTrack *Track)
{
   SEGMSegment segmSegment;
   segmSegment = Track->segs[ bezierindex - 1 ];       
   return segmSegment;         
}

void deleteSegment( int segmenttype, NlTrack *Track)
{
    int i;
    for(i = segmenttype; i<=Track->totalSegments-1; i++) {
       Track->segs[ i ] = Track->segs[ i + 1 ];			
       Track->seg[ i ] = Track->seg[ i + 1 ];
    }

    Track->totalSegments--;
    Track->segmsize = Track->segmsize + (Track->totalSegments * 37);
    Track->segs = (SEGMSegment*) realloc(Track->segs, Track->totalSegments * sizeof(SEGMSegment) );
    			
    Track->rascsize = Track->rascsize + (Track->totalSegments * 4);
    Track->totalSegment = Track->totalSegments;
    Track->seg = (SegmentNodes *) realloc(Track->seg, Track->totalSegment * sizeof(SegmentNodes) );
}

int getTotalSegments(NlTrack *Track)
{
    return Track->totalSegments;
}

void setSegmentStation(int bezierindex, 
                       float waitAvg,
                       float waitMin,
                       float waitMax,
                       float waitDev,
                       char entranceSide,
                       char stationType,
                       int sync,
                       char stationHidden,
                       float mainBrakeDecel,
                       char numPasses,
                       char reverseCableLift,
                       int hideBreaks,
                       int usePreBrake,
                       float preBrakeSpeedLimit,
                       float preBrakeDecel,
                       float preBrakeHyster,
                       int useTransportation,
                       float transportSpeed,
                       float transportAccel,
                       float transportDecel,
                       char transportType,
                       float launchAccel,
                       int useLaunch,
                       NlTrack *Track) 
{
    Track->segs[ bezierindex - 1 ].station.waitAvg = waitAvg;
    Track->segs[ bezierindex - 1 ].station.waitMin = waitMin;
    Track->segs[ bezierindex - 1 ].station.waitMax = waitMax;
    Track->segs[ bezierindex - 1 ].station.waitDev = waitDev;
    Track->segs[ bezierindex - 1 ].station.entranceSide = entranceSide;
    Track->segs[ bezierindex - 1 ].station.stationType = stationType;
    Track->segs[ bezierindex - 1 ].station.sync = sync;
    Track->segs[ bezierindex - 1 ].station.stationHidden = stationHidden;
    Track->segs[ bezierindex - 1 ].station.mainBrakeDecel = mainBrakeDecel;
    Track->segs[ bezierindex - 1 ].station.numPasses = numPasses;
    Track->segs[ bezierindex - 1 ].station.reverseCableLift = reverseCableLift;
    Track->segs[ bezierindex - 1 ].station.hideBreaks = hideBreaks;
    Track->segs[ bezierindex - 1 ].station.usePreBrake = usePreBrake;
    Track->segs[ bezierindex - 1 ].station.preBrakeSpeedLimit = preBrakeSpeedLimit;
    Track->segs[ bezierindex - 1 ].station.preBrakeDecel = preBrakeDecel;
    Track->segs[ bezierindex - 1 ].station.preBrakeHyster = preBrakeHyster;
    Track->segs[ bezierindex - 1 ].station.useTransportation = useTransportation;
    Track->segs[ bezierindex - 1 ].station.transportSpeed = transportSpeed;
    Track->segs[ bezierindex - 1 ].station.transportAccel = transportAccel;
    Track->segs[ bezierindex - 1 ].station.transportDecel = transportDecel;
    Track->segs[ bezierindex - 1 ].station.transportType = transportType;
    Track->segs[ bezierindex - 1 ].station.launchAccel = launchAccel;
    Track->segs[ bezierindex - 1 ].station.useLaunch = useLaunch;     
}

NLSegmentStation getSegmentStation( int bezierindex, NlTrack *Track)
{
   NLSegmentStation segment;
   segment = Track->segs[ bezierindex - 1 ].station;       
   return segment;         
}

void setSegmentLift(int bezierindex, 
										char liftStyle,
										float speed, float accel, float decel,
										int railingLeft, int railingRight,
										int flags,
                    NlTrack *Track) 
{
    Track->segs[ bezierindex - 1 ].lift.liftStyle = liftStyle;
    Track->segs[ bezierindex - 1 ].lift.speed = speed;
    Track->segs[ bezierindex - 1 ].lift.accel = accel;
    Track->segs[ bezierindex - 1 ].lift.decel = decel;
    Track->segs[ bezierindex - 1 ].lift.railingLeft = railingLeft;
    Track->segs[ bezierindex - 1 ].lift.railingRight = railingRight;
    Track->segs[ bezierindex - 1 ].lift.flags = flags;
}

SegmentLift getSegmentLift( int bezierindex, NlTrack *Track)
{
   SegmentLift segment;
   segment = Track->segs[ bezierindex - 1 ].lift;       
   return segment;         
}

void setSegmentTransport(int bezierindex, 
												float speed, float accel, float decel,
												char transportStyle,
												int railingLeft, int railingRight,
												char speedUpPasses, char speedDown,
												float speedDownSpeedLimit,                      
                      	NlTrack *Track) 
{
    Track->segs[ bezierindex - 1 ].transport.speed = speed;
    Track->segs[ bezierindex - 1 ].transport.accel = accel;
    Track->segs[ bezierindex - 1 ].transport.decel = decel;
    Track->segs[ bezierindex - 1 ].transport.transportStyle = transportStyle;
    Track->segs[ bezierindex - 1 ].transport.railingLeft = railingLeft;
    Track->segs[ bezierindex - 1 ].transport.railingRight = railingRight;
    Track->segs[ bezierindex - 1 ].transport.speedUpPasses = speedUpPasses;
    Track->segs[ bezierindex - 1 ].transport.speedDown = speedDown;
    Track->segs[ bezierindex - 1 ].transport.speedDownSpeedLimit = speedDownSpeedLimit;
}

SegmentTransport getSegmentTransport( int bezierindex, NlTrack *Track)
{
   SegmentTransport segment;
   segment = Track->segs[ bezierindex - 1 ].transport;       
   return segment;         
}

void setSegmentBreak(int bezierindex, 
										float speedLimit, float decel, float hyster,
										float triggerPos,
										int blockSegment, int useTransport,
										float transportSpeed, float transportAccel, float transportDecel,
										int railingLeft, int railingRight, int completeStop,
										char stopWaitTime,
										int transportType,
										int launch,
										float launchAccel,
										int newTriggerBehaviour,                       
                    NlTrack *Track) 
{
    Track->segs[ bezierindex - 1 ].breaks.speedLimit = speedLimit;
    Track->segs[ bezierindex - 1 ].breaks.decel = decel;
    Track->segs[ bezierindex - 1 ].breaks.hyster = hyster;
    Track->segs[ bezierindex - 1 ].breaks.triggerPos = triggerPos;
    Track->segs[ bezierindex - 1 ].breaks.blockSegment = blockSegment;
    Track->segs[ bezierindex - 1 ].breaks.useTransport = useTransport;
    Track->segs[ bezierindex - 1 ].breaks.transportSpeed = transportSpeed;
    Track->segs[ bezierindex - 1 ].breaks.transportAccel = transportAccel;
    Track->segs[ bezierindex - 1 ].breaks.transportDecel = transportDecel;
    Track->segs[ bezierindex - 1 ].breaks.railingLeft = railingLeft;
    Track->segs[ bezierindex - 1 ].breaks.railingRight = railingRight;
    Track->segs[ bezierindex - 1 ].breaks.completeStop = completeStop;
    Track->segs[ bezierindex - 1 ].breaks.stopWaitTime = stopWaitTime;
    Track->segs[ bezierindex - 1 ].breaks.transportType = transportType;
    Track->segs[ bezierindex - 1 ].breaks.launch = launch;
    Track->segs[ bezierindex - 1 ].breaks.launchAccel = launchAccel;
    Track->segs[ bezierindex - 1 ].breaks.newTriggerBehaviour = newTriggerBehaviour;
}

SegmentBreak getSegmentBreak( int bezierindex, NlTrack *Track)
{
   SegmentBreak segment;
   segment = Track->segs[ bezierindex - 1 ].breaks;       
   return segment;         
}

Vertex getPosSegmentNode( int index, int segindex, NlTrack *Track )
{ 
	Vertex vertex;

	float t = Track->seg[segindex].data[index].pos;
	t = 1.0f - t;

	float it = 1.0f-t;

	float b0 = t*t*t;
	float b1 = 3*t*t*it;
	float b2 = 3*t*it*it;
	float b3 =  it*it*it;

	float x = b0 * Track->beziers[ segindex ].posX  +
				    b1 * Track->beziers[ segindex ].cp2X +
				    b2 * Track->beziers[ segindex + 1 ].cp1X +
				    b3 * Track->beziers[ segindex + 1 ].posX ;

	float y = b0 * Track->beziers[ segindex ].posY  +
				    b1 * Track->beziers[ segindex ].cp2Y +
				    b2 * Track->beziers[ segindex + 1 ].cp1Y +
				    b3 * Track->beziers[ segindex + 1 ].posY ;

	float z = b0 * Track->beziers[ segindex ].posZ  +
				    b1 * Track->beziers[ segindex ].cp2Z +
				    b2 * Track->beziers[ segindex + 1 ].cp1Z +
				    b3 * Track->beziers[ segindex + 1 ].posZ ;

	vertex.x = x;
	vertex.y = y ;
	vertex.z = z ;	

	return vertex;
}

