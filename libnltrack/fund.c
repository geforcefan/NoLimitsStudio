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

/*! \brief Add a Fundament Node
 *
 *	\param useSizeValue contains the value, whether their own size to be used
 *	\param size is the size of the support, if useSizeValue is 1
 *	\param useRotationValue contains the value, whether their own rotation to be used
 *	\param rotation is the x-axis position of the node
 *	\param posX is the rotation of the support in radian, if rotation is 1.
 *					To convert from degree to radian,
 *					please use toRad().
 *	\param posX is the x-axis position of the node
 *	\param posY is the y-axis position of the node
 *	\param posZ is the z-axis position of the node
 *	\param Track a pointer to the NlTrack
 *
 *	\return nodeindex will contain the index number
 *					of the added node
 *
 *  This function will add a new free
 *	node.
 */
int addFundNode(	int useSizeValue,
									float size,
									int useRotationValue,
									float rotation,
									char fundType,
									int flags,
									float posX,  
									float posY, 
									float posZ,
                 	NlTrack *Track)
{
		Track->totalFundNodes++;            
		Track->fundNodes = (FundNode *) realloc(Track->fundNodes, Track->totalFundNodes * sizeof(FundNode) );  
		Track->fundsize = Track->fundsize + (Track->totalFundNodes * 92);
		 
		Track->fundNodes[ Track->totalFundNodes - 1 ].posX = posX;
		Track->fundNodes[ Track->totalFundNodes - 1 ].posY = posY;
		Track->fundNodes[ Track->totalFundNodes - 1 ].posZ = posZ; 

		Track->fundNodes[ Track->totalFundNodes - 1 ].useSizeValue = useSizeValue;
		Track->fundNodes[ Track->totalFundNodes - 1 ].size = size;
		Track->fundNodes[ Track->totalFundNodes - 1 ].useRotationValue = useRotationValue;
		Track->fundNodes[ Track->totalFundNodes - 1 ].rotation = rotation;
		Track->fundNodes[ Track->totalFundNodes - 1 ].fundType = fundType;
		Track->fundNodes[ Track->totalFundNodes - 1 ].flags = flags;
   
   return Track->totalFundNodes - 1;
}

FundNode getFundNode(int fundindex,
                 	NlTrack *Track)
{
		FundNode fundnode; 
	  fundnode = Track->fundNodes[ fundindex ];
   	return fundnode;
}

int modifyFundNode(int fundindex,
									int useSizeValue,
									float size,
									int useRotationValue,
									float rotation,
									char fundType,
									int flags,
									float posX,  
									float posY, 
									float posZ,
                 	NlTrack *Track)
{	 
		Track->fundNodes[ fundindex ].posX = posX;
		Track->fundNodes[ fundindex ].posY = posY;
		Track->fundNodes[ fundindex ].posZ = posZ; 

		Track->fundNodes[ fundindex ].useSizeValue = useSizeValue;
		Track->fundNodes[ fundindex ].size = size;
		Track->fundNodes[ fundindex ].useRotationValue = useRotationValue;
		Track->fundNodes[ fundindex ].rotation = rotation;
		Track->fundNodes[ fundindex ].fundType = fundType;
		Track->fundNodes[ fundindex ].flags = flags;
}

void deleteFundNode( int fundindex, NlTrack *Track)
{
		 int i;
		 for(i = fundindex; i<=Track->totalFundNodes-1; i++) {
		    Track->fundNodes[ i ] = Track->fundNodes[ i + 1 ];       
		 }

		Track->totalFundNodes--;            
        //Track->fundNodes = (FundNode *) realloc(Track->fundNodes, Track->totalFundNodes * sizeof(FundNode) );
        Track->fundsize = Track->fundsize + (Track->totalFundNodes * 92);

        for( i = 0; i<=Track->totalTubes; i++) {
            if(Track->tubes[i].n1type == NODE_FUND || Track->tubes[i].n2type == NODE_FUND) {
                int nodeIndex;
                if(Track->tubes[i].n1type == NODE_FUND) nodeIndex = Track->tubes[i].n1index;
                if(Track->tubes[i].n2type == NODE_FUND) nodeIndex = Track->tubes[i].n2index;
               if(nodeIndex > fundindex) nodeIndex -= 1;
               if(Track->tubes[i].n1type == NODE_FUND) Track->tubes[i].n1index = nodeIndex;
               if(Track->tubes[i].n2type == NODE_FUND) Track->tubes[i].n2index = nodeIndex;
          }
        }
}

int getTotalFundNodes(NlTrack *Track)
{
     return Track->totalFundNodes;
}

int getFundamentSize(int fundindex, NlTrack *Track) {
    int size = 0;
    int i=0;

    for( i = 0; i<=Track->totalTubes; i++) {
        if(Track->tubes[i].n1type == NODE_FUND || Track->tubes[i].n2type == NODE_FUND) {
            int nodeIndex;
            if(Track->tubes[i].n1type == NODE_FUND) nodeIndex = Track->tubes[i].n1index;
            if(Track->tubes[i].n2type == NODE_FUND) nodeIndex = Track->tubes[i].n2index;
            if(nodeIndex != fundindex) continue;

            int currentSize = 0;

            if(Track->tubes[i].tubeType == TUBE_MEDIUM) currentSize = 1;
            if(Track->tubes[i].tubeType == TUBE_LARGE) currentSize = 2;
            if(Track->tubes[i].tubeType == TUBE_X_LARGE) currentSize = 3;
            if(Track->tubes[i].tubeType == TUBE_XX_LARGE) currentSize = 3;

            if(currentSize > size) size = currentSize;
        }
    }
    return size;
}
