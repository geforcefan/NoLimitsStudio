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

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libnltrack.h"

/* 
 * Swap bits 
 */
void swap( char* v1, char* v2 )
{
	char tmp = *v1;
	*v1 = *v2;
	*v2 = tmp;
}

/* 
 * Swap Endian from float type
 */
void SwapEndianFloat( float* val )
{
	char* ptr = (char*)val;
	swap( &ptr[0], &ptr[3] );
	swap( &ptr[1], &ptr[2] );
} 

/* 
 * Swap Endian from int type
 */
void SwapEndianInt( int* val )
{
	char* ptr = (char*)val;
	swap( &ptr[0], &ptr[3] );
	swap( &ptr[1], &ptr[2] );
} 

/* 
 * Byte to Int
 */
int byteInt ( char data[3], int endian ) 
{	
	int des;
	
	if(endian == LITTLE_ENDIAN_NL) {
		des = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
	}
	if(endian == BIG_ENDIAN_NL) {
		des = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
		SwapEndianInt(&des);
	}

	return des;
}

/* 
 * Byte from FILE to Float
 */
float byteFloat ( FILE *file, int endian ) 
{	
	float des;

	if(endian == LITTLE_ENDIAN_NL) {
		fread(&des, 4, 1, file);
	}
	if(endian == BIG_ENDIAN_NL) {
		fread(&des, 4, 1, file);
		SwapEndianFloat(&des);
	}

	return des;
}

/* 
 * File Length
 */
long filelength(int f)
{
    struct stat buf;

    fstat(f, &buf);

    return(buf.st_size);
}

/* 
 * Read Bytes
 */
char* ReadBytes( FILE *file, size_t length )
{
		char *bytes = (char*) malloc( length + 1 );
		char byte;
		int i=0;

		for(;i<=length-1; i++) {
			fread(&byte, 1, 1, file);
			bytes[i]=byte;
		}

		bytes[i] = 0;

    return bytes;
}

/* 
 * Write Bytes
 */
void WriteBytes( FILE *file, char* data, size_t length )
{
		char byte;
		int i=0;

		for(;i<=length-1; i++) {
			byte = data[i];
			fwrite(&byte, 1, 1, file);
		}
}

/* 
 * Read Byte
 */
char ReadByte( FILE *file )
{
    char byte;
    fread(&byte, 1, 1, file);
    return byte;
}

/* 
 * Write Byte
 */
void WriteByte( FILE *file, char byte )
{
		char *bytes = (char*) malloc( 2 );
		bytes[0] = byte;
		WriteBytes(file, bytes, 1);
}

/* 
 * Read Bool
 */
int ReadBool( FILE *file )
{
		char des = ReadByte(file);

		if( des == 0xFFFFFFFF )
    	return 1;
		else
			return 0;
}

/* 
 * Write Bool
 */
void WriteBool( FILE *file, int state )
{
		if( state == 1 )
    	WriteByte(file, 0xFF);
		else
    	WriteByte(file, 0x00);
}

/* 
 * Read Key
 */
int ReadKey( FILE *file )
{
	char *des;
	des = ReadBytes(file, 4);

	if(!strcmp(des, "NLIM")!=0)
		return NLIM;

	if(!strcmp(des, "TRCK")!=0)
		return TRCK;
 
	if(!strcmp(des, "INFO")!=0)
		return INFO;
 
	if(!strcmp(des, "BEZR")!=0)
		return BEZR;
 
	if(!strcmp(des, "SEGM")!=0)
		return SEGM;
  
	if(!strcmp(des, "SUPP")!=0)
		return SUPP;
  
	if(!strcmp(des, "FUND")!=0)
		return FUND;
  
	if(!strcmp(des, "RASC")!=0)
		return RASC;
  
	if(!strcmp(des, "FREN")!=0)
		return FREN;
  
	if(!strcmp(des, "TUBE")!=0)
		return TUBE;
  
	if(!strcmp(des, "CATW")!=0)
		return CATW;
 
	if(!strcmp(des, "SCEN")!=0)
		return SCEN;
  
	if(!strcmp(des, "SCOB")!=0)
		return SCOB;
 
	if(!strcmp(des, "TERA")!=0)
		return TERA;
 
	if(!strcmp(des, "ENVC")!=0)
		return ENVC;
  
	return 0;
}

/* 
 * Write Key
 */
void WriteKey( FILE *file, int key )
{
	if(key == NLIM)
		WriteBytes(file, "NLIM", 4);

	if(key == TRCK)
		WriteBytes(file, "TRCK", 4);
 
	if(key == INFO)
		WriteBytes(file, "INFO", 4);
 
	if(key == BEZR)
		WriteBytes(file, "BEZR", 4);
 
	if(key == SEGM)
		WriteBytes(file, "SEGM", 4);
  
	if(key == SUPP)
		WriteBytes(file, "SUPP", 4);
  
	if(key == FUND)
		WriteBytes(file, "FUND", 4);
  
	if(key == RASC)
		WriteBytes(file, "RASC", 4);
  
	if(key == FREN)
		WriteBytes(file, "FREN", 4);
  
	if(key == TUBE)
		WriteBytes(file, "TUBE", 4);
  
	if(key == CATW)
		WriteBytes(file, "CATW", 4);
 
	if(key == SCEN)
		WriteBytes(file, "SCEN", 4);
  
	if(key == SCOB)
		WriteBytes(file, "SCOB", 4);
 
	if(key == TERA)
		WriteBytes(file, "TERA", 4);
 
	if(key == ENVC)
		WriteBytes(file, "ENVC", 4);
}


/* 
 * Read Int
 */
int ReadInt( FILE *file )
{
	char *des;
	des = ReadBytes(file, 4);

	int temp = byteInt( des, BIG_ENDIAN_NL );

	return temp;  
}

/* 
 * Write Int
 */
void WriteInt( FILE *file, int value )
{
	int temp = value;
	SwapEndianInt(&temp);

	fwrite(&temp, 1, 4, file);
}

/* 
 * Read Float
 */
float ReadFloat( FILE *file  )
{
	float temp = byteFloat( file, BIG_ENDIAN_NL );

	return temp;  
}

/* 
 * Write Float
 */
void WriteFloat( FILE *file, float value )
{
	float temp = value;
	SwapEndianFloat(&temp);

	fwrite(&temp, 1, 4, file);
}

/* 
 * Read Null
 */
void ReadNull( FILE *file , size_t length )
{
	char *des;
	des = ReadBytes(file, length);
}

/* 
 * Write Null
 */
void WriteNull( FILE *file , size_t length )
{
	int i;
	for(i=0; i < length; i++)
		WriteByte(file, 0x00);
}

/* 
 * Read String
 */
char *ReadString( FILE *file, size_t length )
{
	char *des;

	if( length > 0)
		des = ReadBytes(file, length);
	else
		des = "";

	return des;  
}

/* 
 * Write String
 */
void *WriteString( FILE *file, char *string )
{
	int i;
	int size = strlen(string);
	WriteInt(file, size);

	for(i=0; i < size; i++)
		WriteByte(file, string[i]);
}

/* 
 * Read Color
 */
Color ReadColor( FILE *file )
{
	char *des;
    Color color;
    unsigned char a = ReadByte(file);
    unsigned char b = ReadByte(file);
    unsigned char g = ReadByte(file);
    unsigned char r = ReadByte(file);

    color.a = a;
    color.b = b;
    color.g = g;
    color.r = r;

	return color;  
}

/* 
 * Write Color
 */
void WriteColor( FILE *file, Color color )
{
	WriteByte(file, color.a);
    WriteByte(file, color.b);
	WriteByte(file, color.g);
    WriteByte(file, color.r);
}

/* 
 * NlSave
 */
int NlSave( char *filename, NlTrack *Track) 
{
	FILE *file;
	int i=0, j=0;

	/* NL-DATAS */
	int key;
	int size, size2;
	int len;

  if( (file = fopen(filename, "wb" )) == NULL )  return 0;

	/*
	 * NLIM
	 */
	WriteKey(file, NLIM);
	WriteInt(file, Track->nlimsize);

	WriteByte(file, Track->major);
	WriteByte(file, Track->minor);
	WriteByte(file, Track->revision);
	WriteByte(file, Track->markup);
  WriteByte(file, Track->reserved);
	WriteBool(file, Track->use16friction);
	WriteNull(file, 14);		
    WriteInt(file, Track->sizeX?5:4);

	/*
	 * TRCK
	 */
	WriteKey(file, TRCK);	
  WriteInt(file, Track->trcksize);

  WriteInt(file, Track->style);
  WriteInt(file, Track->numTrains);
  WriteInt(file, Track->numCars);

	for (i = 0; i < Track->numTrains; i++)
	{
		WriteBool(file, Track->train[i].useIndividualColor);
		WriteNull(file, 7);

		WriteString(file, Track->train[i].textureFilename );

		WriteColor(file, Track->train[i].seatColor);
		WriteColor(file, Track->train[i].restraintColor);
		WriteColor(file, Track->train[i].trainColor);
		WriteColor(file, Track->train[i].gearColor );
	}

  WriteColor(file, Track->trackSpineColor);
  WriteColor(file, Track->trackRailColor);
  WriteColor(file, Track->trackCrosstieColor);
  WriteColor(file, Track->supportsColor);
  WriteColor(file, Track->trainSeatColor);
  WriteColor(file, Track->trainRestraintColor);
  WriteColor(file, Track->trainColor);
  WriteColor(file, Track->trainGearColor);
  WriteByte(file, Track->reserved0);
  WriteByte(file, Track->reserved1);
  WriteByte(file, Track->reserved2);
  WriteByte(file, Track->reserved3);
  WriteInt(file, Track->trackMode);
  WriteBool(file, Track->useTunnelColor);
  WriteColor(file, Track->tunnelColor);
	WriteNull(file, 3);
	WriteInt(file, 4);		

	/*
	 * INFO
	 */
	WriteKey(file, INFO);
	WriteInt(file, Track->infosize );
	WriteNull(file, 16);
	WriteString(file, Track->authorComments);
	WriteInt(file, 0);	

	/*
	 * BEZR
	 */
	WriteKey(file, BEZR);
	WriteInt(file, Track->bezrsize);

	WriteNull(file, 16);
	WriteInt(file, Track->numBeziers);

	for (i = 0; i < Track->numBeziers; i++)
	{

		WriteFloat(file, Track->beziers[i].posX);
		WriteFloat(file, Track->beziers[i].posY);
		WriteFloat(file, Track->beziers[i].posZ);
	 	WriteFloat(file, Track->beziers[i].cp1X);
		WriteFloat(file, Track->beziers[i].cp1Y);
		WriteFloat(file, Track->beziers[i].cp1Z);
		WriteFloat(file, Track->beziers[i].cp2X);
		WriteFloat(file, Track->beziers[i].cp2Y);
		WriteFloat(file, Track->beziers[i].cp2Z);
		WriteFloat(file, Track->beziers[i].roll);
		WriteBool(file, Track->beziers[i].continuesRoll);
		WriteBool(file, Track->beziers[i].equalDistanceCP);
		WriteBool(file, Track->beziers[i].relativeRoll);
		WriteNull(file, 17);
	}

	WriteNull(file, 16);
	WriteInt(file, 0);	

	/*
	 * SEGM
	 */
	WriteKey(file, SEGM);
	WriteInt(file, Track->segmsize);

	WriteInt(file, Track->totalSegments);
	WriteBool(file, Track->trackClosedCircuit );
	WriteNull(file, 16);


	for (i = 0; i < Track->totalSegments; i++)
	{
		WriteInt(file, Track->segs[i].type);
		WriteBool(file, Track->segs[i].individualTrackColor);
		WriteColor(file, Track->segs[i].trackSpineColor);
		WriteColor(file, Track->segs[i].trackRailColor);
        WriteColor(file, Track->segs[i].trackCrosstieColor);
		WriteBool(file, Track->segs[i].useTunnel);
		WriteInt(file, Track->segs[i].flags);
		WriteNull(file, 11);

		switch (Track->segs[i].type)
		{
			case SEGMENT_NORMAL:
				WriteInt(file, Track->segs[i].size);
			break;

			case SEGMENT_STATION:
				WriteInt(file, Track->segs[i].size);

				WriteFloat(file, Track->segs[i].station.waitAvg);
				WriteFloat(file, Track->segs[i].station.waitMin);
				WriteFloat(file, Track->segs[i].station.waitMax);
				WriteFloat(file, Track->segs[i].station.waitDev);
				WriteByte(file, Track->segs[i].station.entranceSide);
				WriteByte(file, Track->segs[i].station.stationType);
				WriteBool(file, Track->segs[i].station.sync);
				WriteByte(file, Track->segs[i].station.stationHidden);
				WriteFloat(file, Track->segs[i].station.mainBrakeDecel);
				WriteByte(file, Track->segs[i].station.numPasses);
				WriteByte(file, Track->segs[i].station.reverseCableLift);
				WriteBool(file, Track->segs[i].station.hideBreaks);
				WriteNull(file, 1);
				WriteBool(file, Track->segs[i].station.usePreBrake);
				WriteFloat(file, Track->segs[i].station.preBrakeSpeedLimit);
				WriteNull(file, 4);
				WriteFloat(file, Track->segs[i].station.preBrakeDecel);
				WriteFloat(file, Track->segs[i].station.preBrakeHyster);
				WriteNull(file, 4);
				WriteBool(file, Track->segs[i].station.useTransportation);
				WriteFloat(file, Track->segs[i].station.transportSpeed);
				WriteFloat(file, Track->segs[i].station.transportAccel);
				WriteFloat(file, Track->segs[i].station.transportDecel);
				WriteNull(file, 3);
				WriteByte(file, Track->segs[i].station.transportType);
				WriteFloat(file, Track->segs[i].station.launchAccel);
				WriteBool(file, Track->segs[i].station.useLaunch);

				WriteNull(file, 55);
			break;

			case SEGMENT_LIFT:
				WriteInt(file, Track->segs[i].size);

				WriteNull(file, 3);
				WriteByte(file, Track->segs[i].lift.liftStyle);
				WriteFloat(file, Track->segs[i].lift.speed);
				WriteFloat(file, Track->segs[i].lift.accel);
				WriteFloat(file, Track->segs[i].lift.decel);
				WriteNull(file, 4);
				WriteBool(file, Track->segs[i].lift.railingLeft);
				WriteBool(file, Track->segs[i].lift.railingRight);
				WriteInt(file, Track->segs[i].lift.flags);
				WriteNull(file,10);
			break;

			case SEGMENT_TRANSPORT:
				WriteInt(file, Track->segs[i].size);

				WriteFloat(file, Track->segs[i].transport.speed);
				WriteFloat(file, Track->segs[i].transport.accel);
				WriteFloat(file, Track->segs[i].transport.decel);
				WriteNull(file, 3);
				WriteByte(file, Track->segs[i].transport.transportStyle);
				WriteBool(file, Track->segs[i].transport.railingLeft);
				WriteBool(file, Track->segs[i].transport.railingRight);
				WriteByte(file, Track->segs[i].transport.speedUpPasses);
				WriteByte(file, Track->segs[i].transport.speedDown);
				WriteFloat(file, Track->segs[i].transport.speedDownSpeedLimit);
				WriteNull(file, 4);
			break;

			case SEGMENT_BREAK:
				WriteInt(file, Track->segs[i].size);

				WriteFloat(file, Track->segs[i].breaks.speedLimit);
				WriteFloat(file, Track->segs[i].breaks.decel);
				WriteFloat(file, Track->segs[i].breaks.hyster);
				WriteNull(file, 4);
				WriteFloat(file, Track->segs[i].breaks.triggerPos);
				WriteBool(file, Track->segs[i].breaks.blockSegment);
				WriteBool(file, Track->segs[i].breaks.useTransport);
				WriteFloat(file, Track->segs[i].breaks.transportSpeed);
				WriteFloat(file, Track->segs[i].breaks.transportAccel);
				WriteFloat(file, Track->segs[i].breaks.transportDecel);
				WriteBool(file, Track->segs[i].breaks.railingLeft);
				WriteBool(file, Track->segs[i].breaks.railingRight);
				WriteBool(file, Track->segs[i].breaks.completeStop);
				WriteByte(file, Track->segs[i].breaks.stopWaitTime);
				WriteInt(file, Track->segs[i].breaks.transportType);
				WriteBool(file, Track->segs[i].breaks.launch);
				WriteFloat(file, Track->segs[i].breaks.launchAccel);
				WriteBool(file, Track->segs[i].breaks.newTriggerBehaviour);
				WriteNull(file, 2);

			break;

			default:
				WriteInt(file, Track->segs[i].size);
			break;
		}
	}
	WriteNull(file, 16);
	WriteInt(file, 0);	

	/*
	 * SUPP
	 */
	WriteKey(file, SUPP);
	WriteInt(file, Track->suppsize);
    WriteNull(file, 32);
	WriteInt(file, 5);	

	/*
	 * FUND
	 */
	WriteKey(file, FUND);
	WriteInt(file, Track->fundsize);

	WriteNull(file, 16);
	WriteInt(file, Track->totalFundNodes);

	for (i = 0; i < Track->totalFundNodes; i++)
	{
		WriteBool(file, Track->fundNodes[i].useSizeValue);
		WriteFloat(file, Track->fundNodes[i].size);
		WriteBool(file, Track->fundNodes[i].useRotationValue);
		WriteFloat(file, Track->fundNodes[i].rotation);
		WriteByte(file, Track->fundNodes[i].fundType);
		WriteInt(file, Track->fundNodes[i].flags);
		WriteNull(file, 1);
	 	WriteFloat(file, Track->fundNodes[i].posX);
        WriteFloat(file, Track->fundNodes[i].posY);
        WriteFloat(file, Track->fundNodes[i].posZ);
        WriteColor(file, Track->fundNodes[i].color);
        WriteNull(file, 60);
	}
	WriteNull(file, 16);
	WriteInt(file, 0);

	/*
	 * RASC
	 */
	WriteKey(file, RASC);
	WriteInt(file, Track->rascsize);
	WriteNull(file, 16);
	WriteInt(file, Track->totalSegment);
				
	for (i = 0; i < Track->totalSegment; i++)
	{
        WriteInt(file, Track->seg[i].totalRASCs);
		for (j = 0; j < Track->seg[i].totalRASCs; j++)
		{
			WriteInt(file,Track->seg[i].data[j].type );
			WriteFloat(file, Track->seg[i].data[j].pos);
			WriteNull(file, 16);
		}	
	}
	
	WriteNull(file, 16);
	WriteInt(file, 0);

	/*
	 * FREN
	 */
	WriteKey(file, FREN);
	WriteInt(file, Track->frensize);
	WriteNull(file, 16);
	WriteInt(file, Track->totalFreeNodes);

	for (i = 0; i < Track->totalFreeNodes; i++)
	{
		WriteFloat(file, Track->freeNodes[i].posX);
		WriteFloat(file, Track->freeNodes[i].posY);
		WriteFloat(file, Track->freeNodes[i].posZ);
		WriteInt(file, Track->freeNodes[i].flags);
		WriteNull(file, 12);
	}

	WriteNull(file, 16);
	WriteInt(file, 0);	

	/*
	 * TUBE
	 */
	WriteKey(file, TUBE);
	WriteInt(file, Track->tubesize);

	WriteNull(file, 16);
	WriteInt(file, Track->totalTubes);

	for (i = 0; i < Track->totalTubes; i++)
	{
		WriteInt(file, Track->tubes[i].n1type);
		WriteInt(file, Track->tubes[i].n1segIndex);
		WriteInt(file, Track->tubes[i].n1index);
		WriteNull(file, 4);
		WriteInt(file, Track->tubes[i].n2type);
		WriteInt(file, Track->tubes[i].n2segIndex);
		WriteInt(file, Track->tubes[i].n2index);
		WriteNull(file, 4);
		WriteInt(file, Track->tubes[i].tubeType);
        WriteInt(file, Track->tubes[i].flags);
        WriteFloat(file, Track->tubes[i].rotation);
        WriteColor(file, Track->tubes[i].color);
	}

	WriteNull(file, 16);
	WriteInt(file, 0);	

	/*
	 * CATW
	 */
    WriteKey(file, CATW);
    WriteInt(file, Track->catwsize);

    WriteNull(file, 16);
    WriteInt(file, Track->totalCatwalks);

    for (i = 0; i < Track->totalCatwalks; i++)
    {
	 WriteInt(file, Track->catwalks[i].fn1index);
	 WriteInt(file, Track->catwalks[i].fn2index);
	 WriteInt(file,Track->catwalks[i].fn3index );
	 WriteInt(file, Track->catwalks[i].fn4index);
	 WriteInt(file, Track->catwalks[i].flags);
	 WriteInt(file, Track->catwalks[i].catwalkType);
	 WriteInt(file, Track->catwalks[i].skipByteCount);

	 for (j = 0; j < Track->catwalks[i].skipByteCount; j++)
	 {
         WriteByte(file, Track->catwalks[i].skipBytes[j].reserved);
     }
    }

    WriteNull(file, 16);
    WriteInt(file, 0);

     /*
      * SCEN
      */
     WriteKey(file, SCEN);
     WriteInt(file, Track->scensize );

     WriteNull(file, 16);
     WriteInt(file, Track->totalTrees);

     for (i = 0; i < Track->totalTrees; i++)
     {
	      WriteFloat(file, Track->trees[i].posX);
	      WriteFloat(file,Track->trees[i].posY );
	      WriteFloat(file, Track->trees[i].posZ);
	      WriteInt(file, Track->trees[i].treeType);
	      WriteFloat(file, Track->trees[i].rotation);
	      WriteNull(file, 44);
     }

     WriteNull(file, 16);
     WriteInt(file, Track->chunkscen);

     /*
      * SCOB
      */
     WriteKey(file, SCOB);
     WriteInt(file, Track->scobsize);
  
     WriteNull(file, 16);
     WriteInt(file, Track->totalObjects);

     for (i = 0; i < Track->totalObjects; i++)
     {
     WriteString(file, Track->objects[i].dummyString );
     WriteString(file, Track->objects[i].objectFilename);

	      WriteFloat(file, Track->objects[i]._11);
	      WriteFloat(file, Track->objects[i]._12);
	      WriteFloat(file, Track->objects[i]._13);
	      WriteFloat(file, Track->objects[i]._14);
	      WriteFloat(file, Track->objects[i]._21);
	      WriteFloat(file, Track->objects[i]._22);
	      WriteFloat(file, Track->objects[i]._23);
	      WriteFloat(file, Track->objects[i]._24);
	      WriteFloat(file, Track->objects[i]._31);
	      WriteFloat(file, Track->objects[i]._32);
	      WriteFloat(file, Track->objects[i]._33);
	      WriteFloat(file, Track->objects[i]._34);
	      WriteFloat(file, Track->objects[i]._41);
	      WriteFloat(file, Track->objects[i]._42);
	      WriteFloat(file, Track->objects[i]._43);
	      WriteFloat(file, Track->objects[i]._44);
          WriteNull(file, 16);
     }

     WriteNull(file, 16);
     WriteInt(file, 0);
     
      /*
      * CSTM
      */    
      WriteBytes(file, "CSTM", 4);
      WriteInt(file, 14);
      WriteInt(file, 1);
      WriteBytes(file, "vrtx", 4);
      WriteInt(file, 8);   
      WriteInt(file, 0);
      WriteInt(file, 0);
            
      /*
       * ENVC
       */
       WriteKey(file, ENVC);
       WriteInt(file, Track->envcsize);

       WriteNull(file, 16);
       WriteString(file, Track->envFilename);
       WriteString(file, Track->reservedString);
       WriteNull(file, 16);
       WriteInt(file, 0);        

      /*
       * TERA
       */
            if( Track->sizeX )
            {
                WriteKey(file, TERA);
                WriteInt(file, Track->terasize);

				WriteInt(file,Track->sizeX);
				WriteInt(file,Track->sizeZ);
				WriteFloat(file,Track->scaleX);
				WriteFloat(file,Track->scaleZ);

				WriteString(file, Track->groundTextureFilename);

				WriteFloat(file, Track->numGroundTextureRepeats);

				WriteString(file, Track->detailTextureFilename);

				WriteFloat(file, Track->numDetailTextureRepeats);
				WriteBool(file, Track->enableWater);
			 	WriteFloat(file, Track->seaLevel);

				WriteString(file, Track->waterTextureFilename);

				WriteFloat(file, Track->numWaterTextureRepeats);
				WriteByte(file, Track->waterReflectiveness);
				WriteBool(file, Track->useWaves);
				WriteFloat(file, Track->waveScale);
				WriteNull(file, 10);

				int z, x;

				for (z = 0; z < Track->sizeZ; z++)
				{
					for (x = 0; x < Track->sizeX; x++)
					{
						 WriteFloat(file, Track->terrainHeight[z][x]);
					}
				}
				WriteNull(file, 16);
				WriteInt(file, 0);
			}

	fclose(file);
}

/* 
 * NlOpen
 */
int NlOpen( char *filename, NlTrack *Track) 
{
	FILE *file;
	int i=0, j=0;

	/* NL-DATAS */
	int key;
	int size, size2;
	int len;

  if( (file = fopen(filename, "rb" )) == NULL )  return 0;

	while (ftell ( file ) < filelength (fileno ( file ))) 
	{
	  key = ReadKey(file);
  
		switch(key) {

			/*
			 * NLIM
			 */
			case NLIM:

		Track->nlimsize = ReadInt(file);

        Track->major = ReadByte(file);
      	Track->minor = ReadByte(file);
       	Track->revision = ReadByte(file);
      	Track->markup = ReadByte(file);
      	Track->reserved = ReadByte(file);
      	Track->use16friction = ReadBool(file);
      	ReadNull(file, 14);		

			break;	

			/*
			 * TRCK
			 */
			case TRCK: 
				Track->trcksize = ReadInt(file);

      	Track->style = ReadInt(file);
      	Track->numTrains = ReadInt(file);
      	Track->numCars = ReadInt(file);

		Track->train = (Train*) malloc( Track->numTrains * sizeof(Train) );

        for (i = 0; i <= Track->numTrains-1; i++)
        {
        	Track->train[i].useIndividualColor = ReadBool(file);
        	ReadNull(file, 7);

			len = ReadInt(file);
        	Track->train[i].textureFilename = ReadString(file, len );

         	Track->train[i].seatColor = ReadColor(file);
         	Track->train[i].restraintColor = ReadColor(file);
         	Track->train[i].trainColor = ReadColor(file);
        	Track->train[i].gearColor = ReadColor(file);
         }

				Track->trackSpineColor = ReadColor(file);
				Track->trackRailColor = ReadColor(file);
				Track->trackCrosstieColor = ReadColor(file);
				Track->supportsColor = ReadColor(file);
				Track->trainSeatColor = ReadColor(file);
				Track->trainRestraintColor = ReadColor(file);
				Track->trainColor = ReadColor(file);
				Track->trainGearColor = ReadColor(file);
				Track->reserved0 = ReadByte(file);
				Track->reserved1 = ReadByte(file);
				Track->reserved2 = ReadByte(file);
				Track->reserved3 = ReadByte(file);
				Track->trackMode = ReadInt(file);
				Track->useTunnelColor = ReadBool(file);
				Track->tunnelColor = ReadColor(file);
				ReadNull(file, 3);

			break;	

			/*
			 * INFO
			 */
			case INFO:

				Track->infosize = ReadInt(file);

	 			ReadNull(file, 16);

				len = ReadInt(file);
		   	Track->authorComments = ReadString(file, len);

			break;	

			/*
			 * BEZR
			 */
			case BEZR: 

				Track->bezrsize = ReadInt(file);

				ReadNull(file, 16);
				Track->numBeziers = ReadInt(file);

				Track->beziers = (Bezier*) malloc( Track->numBeziers * sizeof(Bezier) );

				for (i = 0; i < Track->numBeziers; i++)
				{
					Track->beziers[i].posX = ReadFloat(file);
					Track->beziers[i].posY = ReadFloat(file);
					Track->beziers[i].posZ = ReadFloat(file);
					Track->beziers[i].cp1X = ReadFloat(file);
					Track->beziers[i].cp1Y = ReadFloat(file);
					Track->beziers[i].cp1Z = ReadFloat(file);
					Track->beziers[i].cp2X = ReadFloat(file);
					Track->beziers[i].cp2Y = ReadFloat(file);
					Track->beziers[i].cp2Z = ReadFloat(file);
					Track->beziers[i].roll = ReadFloat(file);
					Track->beziers[i].continuesRoll = ReadBool(file);
					Track->beziers[i].equalDistanceCP = ReadBool(file);
					Track->beziers[i].relativeRoll = ReadBool(file);
					ReadNull(file, 17);
				}

        ReadNull(file, 16);

			break;	

			/*
			 * SEGM
			 */
			case SEGM:
				Track->segmsize = ReadInt(file);

				Track->totalSegments = ReadInt(file);
				Track->trackClosedCircuit = ReadBool(file);
				ReadNull(file, 16);

				Track->segs = (SEGMSegment*) malloc(Track->totalSegments * sizeof(SEGMSegment) );


				for (i = 0; i < Track->totalSegments; i++)
				{
					Track->segs[i].type = ReadInt(file);
					Track->segs[i].individualTrackColor = ReadBool(file);
					Track->segs[i].trackSpineColor = ReadColor(file);
					Track->segs[i].trackRailColor = ReadColor(file);
                    Track->segs[i].trackCrosstieColor = ReadColor(file);
					Track->segs[i].useTunnel = ReadBool(file);
					Track->segs[i].flags = ReadInt(file);
					ReadNull(file, 11);

					switch (Track->segs[i].type)
					{
							case SEGMENT_NORMAL:
									Track->segs[i].size = ReadInt(file);
									break;

						case SEGMENT_STATION:
									Track->segs[i].size = ReadInt(file);

									Track->segs[i].station.waitAvg = ReadFloat(file);
									Track->segs[i].station.waitMin = ReadFloat(file);
									Track->segs[i].station.waitMax = ReadFloat(file);
									Track->segs[i].station.waitDev = ReadFloat(file);
									Track->segs[i].station.entranceSide = ReadByte(file);
									Track->segs[i].station.stationType = ReadByte(file);
									Track->segs[i].station.sync = ReadBool(file);
									Track->segs[i].station.stationHidden = ReadByte(file);
									Track->segs[i].station.mainBrakeDecel = ReadFloat(file);
									Track->segs[i].station.numPasses = ReadByte(file);
									Track->segs[i].station.reverseCableLift = ReadByte(file);
									Track->segs[i].station.hideBreaks = ReadBool(file);
									ReadNull(file, 1);
									Track->segs[i].station.usePreBrake = ReadBool(file);
									Track->segs[i].station.preBrakeSpeedLimit = ReadFloat(file);
									ReadNull(file, 4);
									Track->segs[i].station.preBrakeDecel = ReadFloat(file);
									Track->segs[i].station.preBrakeHyster = ReadFloat(file);
									ReadNull(file, 4);
									Track->segs[i].station.useTransportation = ReadBool(file);
									Track->segs[i].station.transportSpeed = ReadFloat(file);
									Track->segs[i].station.transportAccel = ReadFloat(file);
									Track->segs[i].station.transportDecel = ReadFloat(file);
									ReadNull(file, 3);
									Track->segs[i].station.transportType = ReadByte(file);
									Track->segs[i].station.launchAccel = ReadFloat(file);
									Track->segs[i].station.useLaunch = ReadBool(file);

									ReadNull(file, 55);

									break;

							case SEGMENT_LIFT:
									Track->segs[i].size= ReadInt(file);

									ReadNull(file, 3);
									Track->segs[i].lift.liftStyle = ReadByte(file);
									Track->segs[i].lift.speed = ReadFloat(file);
									Track->segs[i].lift.accel = ReadFloat(file);
									Track->segs[i].lift.decel = ReadFloat(file);
									ReadNull(file, 4);
									Track->segs[i].lift.railingLeft = ReadBool(file);
									Track->segs[i].lift.railingRight = ReadBool(file);
									Track->segs[i].lift.flags = ReadInt(file);
									ReadNull(file, 10);

									break;

							case SEGMENT_TRANSPORT:
									Track->segs[i].size = ReadInt(file);

									Track->segs[i].transport.speed = ReadFloat(file);
									Track->segs[i].transport.accel = ReadFloat(file);
									Track->segs[i].transport.decel = ReadFloat(file);
									ReadNull(file, 3);
									Track->segs[i].transport.transportStyle = ReadByte(file);
									Track->segs[i].transport.railingLeft = ReadBool(file);
									Track->segs[i].transport.railingRight = ReadBool(file);
									Track->segs[i].transport.speedUpPasses = ReadByte(file);
									Track->segs[i].transport.speedDown = ReadByte(file);
									Track->segs[i].transport.speedDownSpeedLimit = ReadFloat(file);
									ReadNull(file, 4);

									break;
							case SEGMENT_BREAK:
									Track->segs[i].size = ReadInt(file);

									Track->segs[i].breaks.speedLimit = ReadFloat(file);
									Track->segs[i].breaks.decel = ReadFloat(file);
									Track->segs[i].breaks.hyster = ReadFloat(file);
									ReadNull(file, 4);
									Track->segs[i].breaks.triggerPos = ReadFloat(file);
									Track->segs[i].breaks.blockSegment = ReadBool(file);
									Track->segs[i].breaks.useTransport = ReadBool(file);
									Track->segs[i].breaks.transportSpeed = ReadFloat(file);
									Track->segs[i].breaks.transportAccel = ReadFloat(file);
									Track->segs[i].breaks.transportDecel = ReadFloat(file);
									Track->segs[i].breaks.railingLeft = ReadBool(file);
									Track->segs[i].breaks.railingRight = ReadBool(file);
									Track->segs[i].breaks.completeStop = ReadBool(file);
									Track->segs[i].breaks.stopWaitTime = ReadByte(file);
									Track->segs[i].breaks.transportType = ReadInt(file);
									Track->segs[i].breaks.launch = ReadBool(file);
									Track->segs[i].breaks.launchAccel = ReadFloat(file);
									Track->segs[i].breaks.newTriggerBehaviour = ReadBool(file);
									ReadNull(file, 2);

									break;

							default:
									Track->segs[i].size = ReadInt(file);
									break;
					}
				}
				ReadNull(file, 16);

			break;

			/*
			 * SUPP
			 */
			case SUPP:
				Track->suppsize = ReadInt(file);
      	        ReadNull(file, 32);
			break;	

			/*
			 * FUND
			 */
			case FUND:

				Track->fundsize = ReadInt(file);

		    ReadNull(file, 16);
		    Track->totalFundNodes = ReadInt(file);

				Track->fundNodes = (FundNode*) malloc( Track->totalFundNodes * sizeof(FundNode) );

		    for (i = 0; i < Track->totalFundNodes; i++)
		    {
					Track->fundNodes[i].useSizeValue = ReadBool(file);
					Track->fundNodes[i].size = ReadFloat(file);
					Track->fundNodes[i].useRotationValue = ReadBool(file);
					Track->fundNodes[i].rotation = ReadFloat(file);
					Track->fundNodes[i].fundType = ReadByte(file);
					Track->fundNodes[i].flags = ReadInt(file);
					ReadNull(file, 1);
					Track->fundNodes[i].posX = ReadFloat(file);
					Track->fundNodes[i].posY = ReadFloat(file);
					Track->fundNodes[i].posZ = ReadFloat(file);

                    Track->fundNodes[i].color = ReadColor(file);

                    ReadNull(file, 60);
		    }

				ReadNull(file, 16);

			break;	

			/*
			 * RASC
			 */
			case RASC: 
				Track->rascsize = ReadInt(file);
				ReadNull(file, 16);
				Track->totalSegment = ReadInt(file);
				Track->seg = (SegmentNodes *) malloc( Track->totalSegment * sizeof(SegmentNodes) );
				
					for (i = 0; i < Track->totalSegment; i++)
					{
                        Track->seg[i].totalRASCs = ReadInt(file);
						Track->seg[i].data = (SegmentNode *) malloc( Track->seg[i].totalRASCs * sizeof(SegmentNode) );

						for (j = 0; j < Track->seg[i].totalRASCs; j++)
						{
							Track->seg[i].data[j].type = ReadInt(file);
							Track->seg[i].data[j].pos = ReadFloat(file);
							ReadNull(file, 16);
						}	
					}
	
					ReadNull(file, 16);
					ReadNull(file, 4);
			break;
	
			/*
			 * FREN
			 */
			case FREN:

				Track->frensize = ReadInt(file);
				ReadNull(file, 16);
				Track->totalFreeNodes = ReadInt(file);
				Track->freeNodes = (FreeNode *) malloc( Track->totalFreeNodes * sizeof(FreeNode) );

				for (i = 0; i < Track->totalFreeNodes; i++)
				{
					Track->freeNodes[i].posX = ReadFloat(file);
					Track->freeNodes[i].posY = ReadFloat(file);
					Track->freeNodes[i].posZ = ReadFloat(file);
					Track->freeNodes[i].flags = ReadInt(file);
					ReadNull(file, 12);
				}

				ReadNull(file, 16);

			break;	

			/*
			 * TUBE
			 */
			case TUBE:

				Track->tubesize = ReadInt(file);

				ReadNull(file, 16);
				Track->totalTubes = ReadInt(file);

				Track->tubes = (Tube*) malloc( Track->totalTubes * sizeof(Tube) );

				for (i = 0; i < Track->totalTubes; i++)
				{
					Track->tubes[i].n1type = ReadInt(file);
					Track->tubes[i].n1segIndex = ReadInt(file);
					Track->tubes[i].n1index = ReadInt(file);
					ReadNull(file, 4);
					Track->tubes[i].n2type = ReadInt(file);
					Track->tubes[i].n2segIndex = ReadInt(file);
					Track->tubes[i].n2index = ReadInt(file);
					ReadNull(file, 4);
					Track->tubes[i].tubeType = ReadInt(file);
                    Track->tubes[i].flags = ReadInt(file);
                    Track->tubes[i].rotation = ReadFloat(file);
                    Track->tubes[i].color = ReadColor(file);
				}

				ReadNull(file, 16);

			break;	

			/*
			 * CATW
			 */
			case CATW:
				// Not implemented
				Track->catwsize = ReadInt(file);

				ReadNull(file, 16);
				Track->totalCatwalks = ReadInt(file);

				Track->catwalks = (Catwalk*) malloc( Track->totalCatwalks * sizeof(Catwalk) );

				for (i = 0; i < Track->totalCatwalks; i++)
				{
					Track->catwalks[i].fn1index = ReadInt(file);
					Track->catwalks[i].fn2index = ReadInt(file);
					Track->catwalks[i].fn3index = ReadInt(file);
					Track->catwalks[i].fn4index = ReadInt(file);
					Track->catwalks[i].flags = ReadInt(file);
					Track->catwalks[i].catwalkType = ReadInt(file);
					Track->catwalks[i].skipByteCount = ReadInt(file);

					Track->catwalks[i].skipBytes = (SkipByte*) malloc( Track->totalCatwalks * sizeof(SkipByte) );

					for (j = 0; j < Track->catwalks[i].skipByteCount; j++)
					{
							Track->catwalks[i].skipBytes[j].reserved = ReadByte(file);
					}
				}

				ReadNull(file, 16);

			break;	

			/*
			 * SCEN
			 */
			case SCEN:

				Track->scensize = ReadInt(file);

				ReadNull(file, 16);
				Track->totalTrees = ReadInt(file);

				Track->trees = (Tree *) malloc( Track->totalTrees * sizeof(Tree) );

				for (i = 0; i < Track->totalTrees; i++)
				{
					Track->trees[i].posX = ReadFloat(file);
					Track->trees[i].posY = ReadFloat(file);
					Track->trees[i].posZ = ReadFloat(file);
					Track->trees[i].treeType = ReadInt(file);
					Track->trees[i].rotation = ReadFloat(file);
					ReadNull(file, 44);
				}

				ReadNull(file, 16);
                Track->chunkscen = ReadInt(file);
			break;	

			/*
			 * SCOB
			 */
			case SCOB:

				Track->scobsize = ReadInt(file);
  
				ReadNull(file, 16);
				Track->totalObjects = ReadInt(file);

				Track->objects = (SCOBObject*) malloc( Track->totalObjects * sizeof(SCOBObject) );

				for (i = 0; i < Track->totalObjects; i++)
				{
					len = ReadInt(file); 
					Track->objects[i].dummyString = ReadString(file, len);

					len = ReadInt(file);
					Track->objects[i].objectFilename = ReadString(file, len);

					Track->objects[i]._11 = ReadFloat(file);
					Track->objects[i]._12 = ReadFloat(file);
					Track->objects[i]._13 = ReadFloat(file);
					Track->objects[i]._14 = ReadFloat(file);
					Track->objects[i]._21 = ReadFloat(file);
					Track->objects[i]._22 = ReadFloat(file);
					Track->objects[i]._23 = ReadFloat(file);
					Track->objects[i]._24 = ReadFloat(file);
					Track->objects[i]._31 = ReadFloat(file);
					Track->objects[i]._32 = ReadFloat(file);
					Track->objects[i]._33 = ReadFloat(file);
					Track->objects[i]._34 = ReadFloat(file);
					Track->objects[i]._41 = ReadFloat(file);
					Track->objects[i]._42 = ReadFloat(file);
					Track->objects[i]._43 = ReadFloat(file);
					Track->objects[i]._44 = ReadFloat(file);
					ReadNull(file, 16);
				}

				ReadNull(file, 16);

			break;	

			/*
			 * TERA
			 */
            case TERA:
				Track->terasize = ReadInt(file);

                Track->sizeX = ReadInt(file);
				Track->sizeZ = ReadInt(file);
				Track->scaleX = ReadFloat(file);
                Track->scaleZ = ReadFloat(file);

				len = ReadInt(file);
				Track->groundTextureFilename = ReadString(file, len);

				Track->numGroundTextureRepeats = ReadFloat(file);

				len = ReadInt(file);
				Track->detailTextureFilename = ReadString(file, len);

				Track->numDetailTextureRepeats = ReadFloat(file);
				Track->enableWater = ReadBool(file);
				Track->seaLevel = ReadFloat(file);

				len = ReadInt(file);
				Track->waterTextureFilename = ReadString(file, len);

				Track->numWaterTextureRepeats = ReadFloat(file);
				Track->waterReflectiveness = ReadByte(file);
				Track->useWaves = ReadBool(file);
				Track->waveScale = ReadFloat(file);
				ReadNull(file, 10);

				int z, x;

				for (z = 0; z < Track->sizeZ; z++)
				{
					for (x = 0; x < Track->sizeX; x++)
					{
						Track->terrainHeight[z][x] = ReadFloat(file);
					}
				}
 
				ReadNull(file, 16);

			break;	

			/*
			 * ENVC
			 */
			case ENVC:

				Track->envcsize = ReadInt(file);

				ReadNull(file, 16);

				len = ReadInt(file);
				Track->envFilename = ReadString(file, len);

				len = ReadInt(file);
				Track->reservedString = ReadString(file, len);

				ReadNull(file, 16);

			break;	

		}
	}

	fclose(file);

	return 1;
}

