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

/*! \brief Add a bezier
 *	\param posX is the x-axis position 
 *					of the bezier
 *					
 *	\param posY is the y-axis position 
 *					of the bezier
 *					
 *	\param posZ is the z-axis position 
 *					of the bezier
 *					
 *	\param cp1X is the x-axis position 
 *					of the first controll point
 *					
 *	\param cp1Y is the y-axis position 
 *					of the first controll point
 *					
 *	\param cp1Z is the z-axis position 
 *					of the first controll point
 *					
 *	\param cp2X is the x-axis position 
 *					of the second controll point
 *					
 *	\param cp2Y is the y-axis position 
 *					of the second controll point
 *					
 *	\param cp2Z is the z-axis position 
 *					of the second controll point
 *					
 *	\param roll is the banking in radian. 
 *					To convert from degree to radian,
 *					please use toRad().
 *		
 *	\param continuesRoll is the controll roll 
 *					attribute of the bezier. 1 for true and 
 *					0 for false.
 *	
 *	\param equalDistanceCP Will determine that the 
 *					controll points are equal. 1 for true and 
 *					0 for false.
 *	
 *	\param relativeRoll is the relative roll 
 *					attribute of the bezier. 1 for true and 
 *					0 for false.
 *	
 *	\param Track a pointer to the NlTrack
 *
 *	\return bezierindex will contain the index number
 *					of the added bezier
 *
 *  This function will add a new bezier.
 *  If you call this function, you'll have to add a 
 *	new segment with addSegment() otherwise
 *	a segment error will occur.
 */
int addBezier(float posX,	float posY,	float posZ,
	           float cp1X,	float cp1Y,	float cp1Z,
	           float cp2X,	float cp2Y, float cp2Z,
	           float roll, int continuesRoll, int equalDistanceCP, int relativeRoll,
               NlTrack *Track)
{
   Track->numBeziers++;            
   Track->beziers = (Bezier *) realloc(Track->beziers, Track->numBeziers * sizeof(Bezier) );  
   Track->bezrsize = Track->bezrsize + (Track->numBeziers * 60);
   
   Track->beziers[ Track->numBeziers - 1 ].posX = posX;
   Track->beziers[ Track->numBeziers - 1 ].posY = posY;
   Track->beziers[ Track->numBeziers - 1 ].posZ = posZ;
   Track->beziers[ Track->numBeziers - 1 ].cp1X = cp1X;
   Track->beziers[ Track->numBeziers - 1 ].cp1Y = cp1Y;
   Track->beziers[ Track->numBeziers - 1 ].cp1Z = cp1Z;
   Track->beziers[ Track->numBeziers - 1 ].cp2X = cp2X;
   Track->beziers[ Track->numBeziers - 1 ].cp2Y = cp2Y;
   Track->beziers[ Track->numBeziers - 1 ].cp2Z = cp2Z;
   Track->beziers[ Track->numBeziers - 1 ].roll = roll;
   Track->beziers[ Track->numBeziers - 1 ].continuesRoll = continuesRoll;
   Track->beziers[ Track->numBeziers - 1 ].equalDistanceCP = equalDistanceCP;
   Track->beziers[ Track->numBeziers - 1 ].relativeRoll = relativeRoll;
					
   return  Track->numBeziers - 1 ;
}

/*! \brief Modify a bezier
 *				
 *	\param bezierindex is the index value 
 *					of the modified bezier
 *				
 *	\param posX is the x-axis position 
 *					of the bezier
 *					
 *	\param posY is the y-axis position 
 *					of the bezier
 *					
 *	\param posZ is the z-axis position 
 *					of the bezier
 *					
 *	\param cp1X is the x-axis position 
 *					of the first controll point
 *					
 *	\param cp1Y is the y-axis position 
 *					of the first controll point
 *					
 *	\param cp1Z is the z-axis position 
 *					of the first controll point
 *					
 *	\param cp2X is the x-axis position 
 *					of the second controll point
 *					
 *	\param cp2Y is the y-axis position 
 *					of the second controll point
 *					
 *	\param cp2Z is the z-axis position 
 *					of the second controll point
 *					
 *	\param roll is the banking in radian. 
 *					To convert from degree to radian,
 *					please use toRad().
 *		
 *	\param continuesRoll is the controll roll 
 *					attribute of the bezier. 1 for true and 
 *					0 for false.
 *	
 *	\param equalDistanceCP Will determine that the 
 *					controll points are equal. 1 for true and 
 *					0 for false.
 *	
 *	\param relativeRoll is the relative roll 
 *					attribute of the bezier. 1 for true and 
 *					0 for false.
 *	
 *	\param Track a pointer to the NlTrack
 *
 *
 *  This function will modify a bezier.
 *
 */
void modifyBezier( int bezierindex, float posX,	float posY,	float posZ,
	           			float cp1X,	float cp1Y,	float cp1Z,
	           			float cp2X,	float cp2Y, float cp2Z,
	           			float roll, int continuesRoll, int equalDistanceCP, int relativeRoll,
               		NlTrack *Track)
{
   Track->beziers[ bezierindex ].posX = posX;
   Track->beziers[ bezierindex ].posY = posY;
   Track->beziers[ bezierindex ].posZ = posZ;
   Track->beziers[ bezierindex ].cp1X = cp1X;
   Track->beziers[ bezierindex ].cp1Y = cp1Y;
   Track->beziers[ bezierindex ].cp1Z = cp1Z;
   Track->beziers[ bezierindex ].cp2X = cp2X;
   Track->beziers[ bezierindex ].cp2Y = cp2Y;
   Track->beziers[ bezierindex ].cp2Z = cp2Z;
   Track->beziers[ bezierindex ].roll = roll;
   Track->beziers[ bezierindex ].continuesRoll = continuesRoll;
   Track->beziers[ bezierindex ].equalDistanceCP = equalDistanceCP;
   Track->beziers[ bezierindex ].relativeRoll = relativeRoll;
}

/*! \brief Delete a bezier
 *				
 *	\param bezierindex is the index value 
 *					of the deleted bezier		
 *	
 *	\param Track a pointer to the NlTrack
 *
 *  This function will delete a bezier.
 *
 */
void deleteBezier( int bezierindex, NlTrack *Track)
{
    int i;
    for(i = bezierindex; i<=Track->numBeziers-1; i++) {
       Track->beziers[ i ] = Track->beziers[ i + 1 ];       
    }
     
   Track->numBeziers--;            
   Track->beziers = (Bezier *) realloc(Track->beziers, Track->numBeziers * sizeof(Bezier) );  
   Track->bezrsize = Track->bezrsize + (Track->numBeziers * 60);         
}

/*! \brief Get a bezier
 *				
 *	\param bezierindex is the index value 
 *					of the deleted bezier		
 *	
 *	\param Track a pointer to the NlTrack
 *
 *	\return Bezier will contain the bezier datas
 *
 *  This function will get a bezier.
 *
 */
Bezier getBezier( int bezierindex, NlTrack *Track ) 
{
   Bezier bezier;
   bezier = Track->beziers[ bezierindex ];
   return bezier;
}

/*! \brief Get the number of the beziers
 *				
 *	\param Track a pointer to the NlTrack
 *
 *	\return will contain the number of the beziers
 *
 *  This function will get the number of the beziers.
 *
 */
int getNumBeziers(NlTrack *Track)
{
    return Track->numBeziers;
}

