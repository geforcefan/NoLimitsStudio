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


int addTrackNode(int segindex, int type, float pos,
                 NlTrack *Track)
{
   Track->seg[segindex].totalRASCs++;            
   Track->seg[segindex].data = (SegmentNode *) realloc(Track->seg[segindex].data, Track->seg[segindex].totalRASCs * sizeof(SegmentNode) );  
   Track->frensize = Track->frensize + (Track->seg[segindex].totalRASCs * 24);
   
   Track->seg[segindex].data[ Track->seg[ segindex ].totalRASCs - 1 ].type = type;
   Track->seg[segindex].data[ Track->seg[ segindex ].totalRASCs - 1 ].pos = pos;
   
   return Track->seg[segindex].totalRASCs - 1;
}

void modifyTrackNode(int segindex, int rascindex, int type, float pos,
                 NlTrack *Track)
{
   Track->seg[segindex].data[ rascindex ].type = type;
   Track->seg[segindex].data[ rascindex ].pos = pos;
}

void deleteTrackNode(int segindex, int rascindex, NlTrack *Track)
{
    int i;
    for(i = rascindex; i<=Track->seg[segindex].totalRASCs-1; i++) {     
       Track->seg[segindex].data[ i ] = Track->seg[segindex].data[ i + 1 ];
   }

   Track->seg[segindex].totalRASCs--;            
   Track->seg[segindex].data = (SegmentNode *) realloc(Track->seg[segindex].data, Track->seg[segindex].totalRASCs * sizeof(SegmentNode) );  
   Track->frensize = Track->frensize + (Track->seg[segindex].totalRASCs * 24);
}

SegmentNode getTrackNode(int segindex, int rascindex, NlTrack *Track)
{
    SegmentNode trackNode;
    trackNode = Track->seg[segindex].data[ rascindex ];   
    return trackNode;
}

int getTotalTrackNodes(int segindex, NlTrack *Track)
{
    return  Track->seg[segindex].totalRASCs;
}


