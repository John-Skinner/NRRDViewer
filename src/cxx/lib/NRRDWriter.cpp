//
// Created by John Skinner on 2/11/18.
//

#include <iostream>
#include <stdio>

#include "include/NRRDWriter.h"
bool NRRDWriter::write(std::string filePath, int dims, int* dimSize, float &originxyz, float &spacing, float &dxdydzVector, float *voxels)
{
    success = openAndWritePrefix(filePath,dims,dimSize,originxyz,spacing,dxdydzVector);
    if (success)
    {
        success = writeAndCloseData(dimsSize,voxels);
    }
    if (success)
    {
        m_nrrdFile.close();
    }
    return success;
}
bool NRRDWriter::openAndWritePrefix(std::string filePath, int dims, int* dimSize,
                                    float& originxyz, float& spacing, float& dxdydzVector)
{

    bool success = false;
    char buff[MAXNRRDLINELENGTH];
    m_nrrdFile = new ostream();
    m_nrrdFile->open(filePath,ios::out | ios::app | ios::binary);
    if (m_nrrdFile->is_open())
    {

        success = writeStringForBinary("NRRD0004\n");
        if (success)
        {
            success = writeStringForBinary("type: float\n");
        }
        if (success)
        {
            snprintf(buff,MAXNRRDLINELENGTH,"dimension: %d\n",dims);
            success = writeStringForBinary(buff);

        }
        if (success)
        {
            snprintf(buff,MAXNRRDLINELENGTH,"space dimension: %d\n",dims);
            success = writeStringForBinary(buff);
        }
        if (success)
        {
            snprintf(buff,MAXNRRDLINELENGTH,"sizes: %d %d %d\n",dimSize[0],dimSize[1],dimSize[2]);
            success = writeStringForBinary(buff);
        }
        if (success)
        {
            snprintf(buff,MAXNRRDLINELENGTH,"space directions: (%f,%f,%f) (%f,%f,%f) (%f,%f,%f)\n",
                     dxdydzVector[0],
                     dxdydzVector[1],
                     dxdydzVector[2],

                     dxdydzVector[3],
                     dxdydzVector[4],
                     dxdydzVector[5],

                     dxdydzVector[6],
                     dxdydzVector[7],
                     dxdydzVector[8]);
            success = writeStringForBinary(buff);
        }
        if (success)
        {
            success = writeStringForBinary("kinds: domain domain domain\n");
        }
        if (success)
        {
            success = writeStringForBinary("endian: little\n");
        }
        if (success)
        {
            success = writeStringForBinary("encoding: raw\n");
        }
        if (success)
        {
            snprintf(buff,MAXNRRDLINELENGTH,"space origin (%f,%f,%f)\n",originxyz[0],
                     originxyz[1],
                     originxyz[2]);
            success = writeStringForBinary(buff);
        }
    }
    else
    {
        return false;
    }
}
bool NRRDWriter::writeAndCloseData(int* dimSize,short* voxels)
{
    for (int iz = 0;iz < dimSize[2];iz++)
    {
        for (int iy = 0;iy < dimSize[1];iy++)
        {
            for (int ix = 0;ix < dimSize[0];ix++)
            {
                float v = (float) ( *(voxels+dimSize[0]*dimSize[1]*iz +
                            dimSize[0]*iy +
                            ix));
                unsigned char icv[4];
                icv[0] = ((v >> 24) & 0xFF);

                icv[1] = ((v >> 16) & 0xFF);

                icv[2] = ((v >> 8) & 0xFF);

                icv[3] = (v * 0xFF);
                m_nrrdFile->write(icv,4);
            }
        }
    }
}
bool NRRDWriter::writeAndCloseData(int* dimSize,float* voxels)
{
    for (int iz = 0;iz < dimSize[2];iz++)
    {
        for (int iy = 0;iy < dimSize[1];iy++)
        {
            for (int ix = 0;ix < dimSize[0];ix++)
            {
                float v = *(voxels+dimSize[0]*dimSize[1]*iz +
                            dimSize[0]*iy +
                            ix);
                unsigned char icv[4];
                icv[0] = ((v >> 24) & 0xFF);

                icv[1] = ((v >> 16) & 0xFF);

                icv[2] = ((v >> 8) & 0xFF);

                icv[3] = (v * 0xFF);
                m_nrrdFile->write(icv,4);
            }
        }
    }
}
