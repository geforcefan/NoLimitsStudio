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


/*! \brief Initialize an new NlTrack
 *	\param Track a pointer to the NlTrack
 *
 *  This function will initialize
 * 	the NlTrack with empty datas
 * 	like the number of cars.
 */
int initNewNlTrack(NlTrack *Track)
{
        Color colorwhite = createColorARGB(255, 255, 255, 255);
        
        int i;
    
				Track->nlimsize = 0;
        Track->major = 1;
      	Track->minor = 6;
       	Track->revision = 0;
      	Track->markup = 0;
      	Track->reserved = 0;
      	Track->use16friction = 1;
          
				Track->trcksize = 88;

      	Track->style = 1;
      	Track->numTrains = 32;
      	Track->numCars = 6;

				Track->train = (Train*) malloc( Track->numTrains * sizeof(Train) );

        for (i = 0; i <= Track->numTrains-1; i++)
        {
        	Track->train[i].useIndividualColor = 0;
        	Track->train[i].textureFilename = "";

         	Track->train[i].seatColor = colorwhite;
         	Track->train[i].restraintColor = colorwhite;
         	Track->train[i].trainColor = colorwhite;
        	Track->train[i].gearColor = colorwhite;
         }

         Track->trackSpineColor = colorwhite;
         Track->trackRailColor = colorwhite;
         Track->trackCrosstieColor = colorwhite;
         Track->supportsColor = colorwhite;
         Track->trainSeatColor = colorwhite;
         Track->trainRestraintColor = colorwhite;
         Track->trainColor = colorwhite;
         Track->trainGearColor = colorwhite;
         Track->reserved0 = 0;
         Track->reserved1 = 0;
         Track->reserved2 = 0;
         Track->reserved3 = 0;
         Track->trackMode = MODE_NORMAL;
         Track->useTunnelColor = 1;
         Track->tunnelColor = colorwhite;
         
         Track->infosize = 39;
         Track->authorComments = "";
         
         Track->bezrsize = 100;
         Track->numBeziers = 1;
         Track->beziers = (Bezier*) malloc( 1 * sizeof(Bezier) );
 
         Track->beziers[ 0 ].posX = 0;
         Track->beziers[ 0 ].posY = 1;
         Track->beziers[ 0 ].posZ = 0;
         Track->beziers[ 0 ].cp1X = 0;
         Track->beziers[ 0 ].cp1Y = 1;
         Track->beziers[ 0 ].cp1Z = 10;
         Track->beziers[ 0 ].cp2X = 0;
         Track->beziers[ 0 ].cp2Y = 1;
         Track->beziers[ 0 ].cp2Z = -10;
         Track->beziers[ 0 ].roll = 0;
         Track->beziers[ 0 ].continuesRoll = 0;
         Track->beziers[ 0 ].equalDistanceCP = 0;
         Track->beziers[ 0 ].relativeRoll = 0;
            
         Track->segmsize = 41;
         Track->totalSegments = 0;
         Track->trackClosedCircuit = 0;         
         
         Track->suppsize = 36;

         Track->fundsize = 40;
         Track->totalFundNodes = 0;
         Track->fundNodes = (FundNode*) malloc( 0 * sizeof(FundNode) );

         Track->rascsize = 40;
         Track->totalSegment = 0;
         Track->seg = (SegmentNodes *) malloc( 0 * sizeof(SegmentNodes) );

         Track->frensize = 40;
         Track->totalFreeNodes = 0;
         Track->freeNodes = (FreeNode *) malloc( 0 * sizeof(FreeNode) );
         
         Track->tubesize = 40;
         Track->totalTubes = 0;
         Track->tubes = (Tube*) malloc( 0 * sizeof(Tube) ); 
         
         Track->catwsize = 40;
         Track->totalCatwalks = 0;
         Track->catwalks = (Catwalk*) malloc( 0 * sizeof(Catwalk) );

         Track->scensize = 88;
         Track->totalTrees = 0;
         Track->trees = (Tree *) malloc( 0 * sizeof(Tree) );    
         
         Track->scobsize = 40;
         Track->totalObjects = 0;
         Track->objects = (SCOBObject*) malloc( 0 * sizeof(SCOBObject) );   
         
         Track->envcsize = 44;
         Track->envFilename = "";
         Track->reservedString = "";             
} 

void setCoasterStyle(int style, NlTrack *Track)
{
     Track->style = style;
}

int getCoasterStyle(NlTrack *Track)
{
     return Track->style;
}

void setMajor( int major, NlTrack *Track )
{
     Track->major = major;     
}

int getMajor(NlTrack *Track )
{
     return Track->major;     
}

void setMinor( int minor, NlTrack *Track )
{
     Track->minor = minor;     
}

int getMinor( NlTrack *Track )
{
     return Track->minor;     
}

void setRevision( int revision, NlTrack *Track )
{
     Track->revision = revision;     
}

int getRevision(NlTrack *Track )
{
     return Track->revision;     
}

void setMarkup( int markup, NlTrack *Track )
{
     Track->markup = markup;     
}

int getMarkup(NlTrack *Track )
{
     return Track->markup;     
}

void setUse16friction( int use16friction, NlTrack *Track )
{
     Track->use16friction = use16friction;
}

int getUse16friction(NlTrack *Track )
{
     return Track->use16friction;
}

