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

void setNumTrains( int numTrains, NlTrack *Track )
{
     Track->numTrains = numTrains;
}

int getNumTrains(NlTrack *Track )
{
     return Track->numTrains;
}

void setNumCars( int numCars, NlTrack *Track )
{
     Track->numCars = numCars;
}

int getNumCars(NlTrack *Track )
{
     return Track->numCars;
}

int setTrainProperty( int trainindex, 
											char *textureFilename,
											int useIndividualColor,
											Color seatColor, 
											Color restraintColor, 
											Color trainColor, 
											Color gearColor, 
											NlTrack *Track )
{
			Track->train[trainindex].textureFilename = textureFilename;
			Track->train[trainindex].useIndividualColor = useIndividualColor;
			Track->train[trainindex].seatColor = seatColor;
			Track->train[trainindex].restraintColor = restraintColor;
			Track->train[trainindex].trainColor = trainColor;
			Track->train[trainindex].gearColor = gearColor;
}


Train getTrainProperty( int trainindex, 
											NlTrack *Track )
{
			Train train;

			train.textureFilename = Track->train[trainindex].textureFilename;
			train.useIndividualColor = Track->train[trainindex].useIndividualColor;
			train.seatColor = Track->train[trainindex].seatColor;
			train.restraintColor = Track->train[trainindex].restraintColor;
			train.trainColor = Track->train[trainindex].trainColor;
			train.gearColor = Track->train[trainindex].gearColor;

			return train;
}

Color getTrackSpineColor(NlTrack *Track ) 
{
			return Track->trackSpineColor;
}

Color getTrackRailColor(NlTrack *Track ) 
{
			return Track->trackRailColor;
}

Color getTrackCrosstieColor(NlTrack *Track ) 
{
			return Track->trackCrosstieColor;
}

Color getTrackSupportsColor(NlTrack *Track ) 
{
			return Track->supportsColor;
}

Color getTrackTrainSeatColor(NlTrack *Track ) 
{
			return Track->trainSeatColor;
}

Color getTrackTrainRestraintColor(NlTrack *Track ) 
{
			return Track->trainRestraintColor;
}

Color getTrackTrainColor(NlTrack *Track ) 
{
			return Track->trainColor;
}

Color getTrackTrainGearColor(NlTrack *Track ) 
{
			return Track->trainGearColor;
}

Color getTrackTunnelColor(NlTrack *Track ) 
{
			return Track->tunnelColor;
}

int getTrackUseTunnelColor(NlTrack *Track ) 
{
			return Track->useTunnelColor;
}

void setTrackSpineColor(NlTrack *Track, Color trackSpineColor) 
{
		 Track->trackSpineColor = trackSpineColor;
}

void setTrackRailColor(NlTrack *Track, Color trackRailColor) 
{
		 Track->trackRailColor = trackRailColor;
}

void setTrackCrosstieColor(NlTrack *Track, Color trackCrosstieColor) 
{
		 Track->trackCrosstieColor = trackCrosstieColor;
}

void setTrackSupportsColor(NlTrack *Track, Color supportsColor) 
{
		 Track->supportsColor = supportsColor;
}

void setTrackTrainSeatColor(NlTrack *Track, Color trainSeatColor) 
{
		 Track->trainSeatColor = trainSeatColor;
}

void setTrackTrainRestraintColor(NlTrack *Track, Color trainRestraintColor) 
{
		 Track->trainRestraintColor = trainRestraintColor;
}

void setTrackTrainColor(NlTrack *Track, Color trainColor) 
{
		 Track->trainColor = trainColor;
}

void setTrackTrainGearColor(NlTrack *Track, Color trainGearColor) 
{
		 Track->trainGearColor = trainGearColor;
}

void setTrackTunnelColor(NlTrack *Track, Color tunnelColor) 
{
		 Track->tunnelColor = tunnelColor;
}

void setTrackUseTunnelColor(NlTrack *Track, int useTunnelColor) 
{
		 Track->useTunnelColor = useTunnelColor;
}

/*
 * Get the string of a coaster type
 */
char *NlGetCoasterType(int type) 
{
	char *coastertype;

	switch( type ) 
	{
		case CLASSIC_STEEL_LOOPING :
			coastertype = "Classic Steel Looping";
		break;

		case CORKSCREW_COASTER :
			coastertype = "Corkscrew Coaster";
		break;

		case INVERTED_COASTER_2 :
			coastertype = "Inverted Coaster 2-Seat";
		break;

		case TWISTED_STEEL_COASTER :
			coastertype = "Twisted Steel Coaster";
		break;

		case INVERTED_COASTER_4 :
			coastertype = "Inverted Coaster 4-Seat";
		break;

		case HYPER_COASTER :
			coastertype = "Hyper Coaster";
		break;

		case FLOORLESS_TWISTED_STEEL_COASTER :
			coastertype = "Floorless Twisted Steel Coaster";
		break;

		case STANDUP_TWISTED_STEEL_COASTER :
			coastertype = "Stand-Up Twisted Steel Coaster";
		break;

		case HYPER_COASTER_4 :
			coastertype = "Hyper Coaster 4-Seat";
		break;

		case WOODEN_COASTER_TRAILERED_2 :
			coastertype = "Wooden Coaster 2-Seat (Trailered)";
		break;

		case WOODEN_COASTER_CLASSIC_4 :
			coastertype = "Wooden Coaster 4-Seat (Classic)";
		break;

		case WOODEN_COASTER_CLASSIC_6 :
			coastertype = "Wooden Coaster 6-Seat (Classic)";
		break;

		case WOODEN_COASTER_TRAILERED_4 :
			coastertype = "Wooden Coaster 4-Seat (Trailered)";
		break;

		case LIM_LAUNCHED_COASTER :
			coastertype = "Lim Launched Coaster";
		break;

		case INVERTED_FACE_TO_FACE_COASTER :
			coastertype = "Inverted Face-to-Face Coaster";
		break;

		case INVERTED_IMPULSE_COASTER :
			coastertype = "Inverted Impulse Coaster";
		break;

		case VEKOMA_FLYING_DUTCHMAN :
			coastertype = "Vekoma Flying Dutchman";
		break;

		case MAURER_SOEHNE_SPINNING_COASTER :
			coastertype = "Maurer S�hne Spinning Coaster";
		break;

		case GERSTLAUER_EURO_FIGHTER :
			coastertype = "Gerstlauer Euro-Fighter";
		break;

		case VEKOMA_MOTORBIKE_COASTER :
			coastertype = "Vekoma Motorbike Coaster";
		break;

		case GERSTLAUER_BOBSLED_COASTER :
			coastertype = "Gerstlauer Bobsled Coaster";
		break;

		case GERSTLAUER_SPINNING_COASTER :
			coastertype = "Gerstlauer Spinning Coaster";
		break;

		case GERSTLAUER_EURO_FIGHTER_2 :
			coastertype = "Gerstlauer Euro-Fighter 2";
		break;

		case STEEL_LOOPING_COASTER :
			coastertype = "Steel Looping Coaster (Modern)";
		break;

		case VERKOMA_MINE_TRAIN_COASTER :
			coastertype = "Vekoma Minetrain Coaster";
		break;

		case VERKOMA_MINE_TRAIN_COASTER_LOC :
			coastertype = "Vekoma Minetrain Coaster with locomotive";
		break;

		case MAURER_SOEHNE_X_CAR_COASTER  :
			coastertype = "Maurer S�hne X-Car Coaster";
		break;

		default :
			coastertype = "Unknown";
		break;
	}

	return coastertype;
}
