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


/*! \brief Add a Support
 *	\param n1type is the type of the first node.
 *				 There is few Pre-defined values:<br>
 *				 #NODE_FUND for a fundament node<br>
 *				 #NODE_FREE for a free node<br>
 *				 #NODE_SEGMENT for a segment node
 *	\param n1index is the index number of the first node
 *	\param n1segIndex is needed, when n1type is #NODE_SEGMENT.
 *				Its the index number of the segment, where the first node is added.
 *	\param n2type is the type of the second node.
 *				 There is few Pre-defined values:<br>
 *				 #NODE_FUND for a fundament node<br>
 *				 #NODE_FREE for a free node<br>
 *				 #NODE_SEGMENT for a segment node
 *	\param n2index is the index number of the second node
 *	\param n2segIndex is needed, when n2type is #NODE_SEGMENT.
 *				Its the index number of the segment, where the second node is added.
 *
 *	\param tubeType is the tube type. There is few Pre-define values:
 *				#TUBE_MEDIUM medium sized tube<br>
 *				#TUBE_LARGE large sized tube<br>
 *				#TUBE_SMALL small sized tube<br>
 *				#TUBE_L_BEAM L Beam<br>
 *				#TUBE_H_BEAM H Beam<br>
 *				#TUBE_X_WOOD X Wood<br>
 *				#TUBE_XX_LARGE XX Large<br>
 *				#TUBE_4_4_WOOD_BEAM 4x4 Wood Beam<br>
 *				#TUBE_1_4_WOOD_BEAM 1x4 Wood Beam<br>
 *				#TUBE_2_2_WOOD_BEAM 2x2 Wood Beam<br>
 *
 *	\param flags of the support.
 *	
 *	\param rotation a is the support rotation in radian. 
 *					To convert from degree to radian,
 *					please use toRad().
 *	
 *	\param Track a pointer to the NlTrack
 *
 *	\return supportindex will contain the index number
 *					of the added support
 *
 *  This function will add a new support.
 *	node.
 */
int addSupport( int n1type, int n1index, int n1segIndex,
                int n2type, int n2index, int n2segIndex,
                int tubeType, int flags, int rotation, NlTrack *Track)
{
   Track->totalTubes++;            
   Track->tubes = (Tube *) realloc(Track->tubes, Track->totalTubes * sizeof(Tube) );  
   Track->tubesize = Track->tubesize + (Track->totalTubes * 48);  
   
   Track->tubes[Track->totalTubes - 1].n1type = n1type;
   Track->tubes[Track->totalTubes - 1].n1index = n1index;
   Track->tubes[Track->totalTubes - 1].n1segIndex = n1segIndex;
                    
   Track->tubes[Track->totalTubes - 1].n2type = n2type;
   Track->tubes[Track->totalTubes - 1].n2index = n2index;
   Track->tubes[Track->totalTubes - 1].n2segIndex = n2segIndex;
                    
   Track->tubes[Track->totalTubes - 1].tubeType = tubeType;
   Track->tubes[Track->totalTubes - 1].flags = flags;
   Track->tubes[Track->totalTubes - 1].rotation = rotation;      
   
   return Track->totalTubes - 1;
}

void modifySupport(int suppindex, int tubeType, int flags, int rotation, NlTrack *Track)
{
   Track->tubes[suppindex].tubeType = tubeType;
   Track->tubes[suppindex].flags = flags;
   Track->tubes[suppindex].rotation = rotation;   
}

void deleteSupport( int suppindex, NlTrack *Track)
{
    int i;
    for(i = suppindex; i<=Track->totalTubes-1; i++) {
       Track->tubes[ i ] = Track->tubes[ i + 1 ];       
    }
     
   Track->totalTubes--;            
   Track->tubes = (Tube *) realloc(Track->tubes, Track->totalTubes * sizeof(Tube) );  
   Track->tubesize = Track->tubesize + (Track->totalTubes * 48);
}

Tube getSupport( int suppindex, NlTrack *Track)
{
   Tube tube;
   tube = Track->tubes[ suppindex ];       
   return tube;         
}

int getTotalSupport(NlTrack *Track)
{
    return Track->totalTubes;
}

