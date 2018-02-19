//
// Created by John Skinner on 2/11/18.
//

#include <fstream>
#include <cstdio>

#include "NRRDWriter.h"
NRRDWriter::NRRDWriter():m_nrrdFile(NULL)
{

}
bool NRRDWriter::write(const std::string filePath, const int dims,
                       const int* dimSize, const float* originxyz, const float* spacing,
                       const float* dxdydzVector, const float *voxels)
{
    bool success = openAndWritePrefix(filePath,dims,dimSize,originxyz,spacing,dxdydzVector);
    if (success)
    {
        success = writeAndCloseData(dimSize,voxels);
    }
    if (success)
    {
        m_nrrdFile->close();
    }
    return success;
}
bool NRRDWriter::write(std::string filePath, int dims, int* dimSize, float* originxyz, float* spacing, float* dxdydzVector, short *voxels)
{
    bool success = openAndWritePrefix(filePath,dims,dimSize,originxyz,spacing,dxdydzVector);
    if (success)
    {
        success = writeAndCloseData(dimSize,voxels);
    }
    if (success)
    {
        m_nrrdFile->close();
    }
    return success;
}
bool NRRDWriter::openAndWritePrefix(const std::string filePath, const int dims, const int* dimSize,
                                    const float* originxyz, const float* spacing, const float* dxdydzVector)
{

    bool success = false;
    char buff[MAXNRRDLINELENGTH];
    m_nrrdFile = new std::ofstream();
    m_nrrdFile->open(filePath,std::ios::out | std::ios::binary | std::ios::trunc);

    if (m_nrrdFile->is_open())
    {
        m_nrrdFile->seekp(0);
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
            // record 2 new-lines to indicate the start of the data
            snprintf(buff,MAXNRRDLINELENGTH,"space origin: (%f,%f,%f)\n\n",originxyz[0],
                     originxyz[1],
                     originxyz[2]);
            success = writeStringForBinary(buff);
        }
    }
    else
    {
        return false;
    }
    return success;
}
bool NRRDWriter::writeAndCloseData(const int* dimSize,const short* voxels)
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
                char const * icv = reinterpret_cast<char const *>(&v);
                m_nrrdFile->write(icv,4);
            }
        }
    }
    if (m_nrrdFile->good())
    {
        m_nrrdFile->close();
    }
    return m_nrrdFile->good();
}
bool NRRDWriter::writeAndCloseData(const int* dimSize,const float* voxels)
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
                char const * icv = reinterpret_cast<char const *>(&v);

                m_nrrdFile->write(icv,4);
            }
        }
    }
    if (m_nrrdFile->good())
    {
        m_nrrdFile->close();
    }
    return m_nrrdFile->good();
}

bool NRRDWriter::writeStringForBinary(std::string line) {
    size_t len = line.length();
    strncpy(t_writeBuffer,line.c_str(),len);
    m_nrrdFile->write(t_writeBuffer,len);
    return m_nrrdFile->good();

}