 /* libnltrack.
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


/*! \brief Add a Free Node
 *
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
int addFreeNode(float posX,  float posY, float posZ,
                 NlTrack *Track)
{
   Track->totalFreeNodes++;            
   Track->freeNodes = (FreeNode *) realloc(Track->freeNodes, Track->totalFreeNodes * sizeof(FreeNode) );  
   Track->frensize = Track->frensize + (Track->totalFreeNodes * 28);
   
   Track->freeNodes[ Track->totalFreeNodes - 1 ].posX = posX;
   Track->freeNodes[ Track->totalFreeNodes - 1 ].posY = posY;
   Track->freeNodes[ Track->totalFreeNodes - 1 ].posZ = posZ; 
   
   return Track->totalFreeNodes - 1;
}

/*! \brief Modify a Free Node
 *
 *	\param nodeindex is the index number of the node,
 *				 which is modified.
 *	\param posX is the x-axis position of the node
 *	\param posY is the y-axis position of the node
 *	\param posZ is the z-axis position of the node
 *	\param Track a pointer to the NlTrack
 *
 *	\return nodeindex will contain the index number
 *					of the modified node
 *
 *  This function will modify a free
 *	node.
 */
void modifyFreeNode(int nodeindex, float posX,  float posY, float posZ,
                 NlTrack *Track)
{
   Track->freeNodes[ nodeindex ].posX = posX;
   Track->freeNodes[ nodeindex ].posY = posY;
   Track->freeNodes[ nodeindex ].posZ = posZ; 
}

/*! \brief Delete a Free Node
 *
 *	\param nodeindex is the index number of the node,
 *				 which is deleted.
 *	\param Track a pointer to the NlTrack
 *
 *
 *  This function will delete a free
 *	node.
 */
void deleteFreeNode(int nodeindex, NlTrack *Track)
{
    int i;
    for(i = nodeindex; i<=Track->totalFreeNodes-1; i++) {     
       Track->freeNodes[ i ] = Track->freeNodes[ i + 1 ];
   }

   Track->totalFreeNodes--;            
   //Track->freeNodes = (FreeNode *) realloc(Track->freeNodes, Track->totalFreeNodes * sizeof(FreeNode) );
   Track->frensize = Track->frensize + (Track->totalFreeNodes * 28);

   for( i = 0; i<=Track->totalFreeNodes; i++) {
       if(Track->tubes[i].n1type == NODE_FREE || Track->tubes[i].n2type == NODE_FREE) {
           int nodeIndex;
           if(Track->tubes[i].n1type == NODE_FREE) nodeIndex = Track->tubes[i].n1index;
           if(Track->tubes[i].n2type == NODE_FREE) nodeIndex = Track->tubes[i].n2index;
          if(nodeIndex > nodeindex) nodeIndex -= 1;
          if(Track->tubes[i].n1type == NODE_FREE) Track->tubes[i].n1index = nodeIndex;
          if(Track->tubes[i].n2type == NODE_FREE) Track->tubes[i].n2index = nodeIndex;
     }
   }
}

/*! \brief Get a Free Node
 *
 *	\param nodeindex is the index number of the node,
 *				 which we like to get.
 *	\param Track a pointer to the NlTrack
 *
 *	\return FreeNode will contain the free node datas.
 *
 *  This function will get a free
 *	node.
 */
FreeNode getFreeNode(int nodeindex, NlTrack *Track)
{
    FreeNode freeNode;
    freeNode = Track->freeNodes[ nodeindex ];   
    return freeNode;
}

/*! \brief Get the number of the free nodes
 *				
 *	\param Track a pointer to the NlTrack
 *
 *	\return will contain the number of the free nodes
 *
 *  This function will get the number of the free nodes.
 *
 */
int getTotalFreeNodes(NlTrack *Track)
{
    return Track->totalFreeNodes;
}


