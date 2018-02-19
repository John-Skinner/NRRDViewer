//
// Created by John Skinner on 2/18/18.
//

#include <NRRDWriter.h>


int main(int argc, char *argv)
{
    /*
     * Create a circle test image.
     */

    float spacing[] = {
            2.0f,4.0f,3.0f
    };
    float dxdydz[] = {
            1.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,0.0f,1.0f
    };
    float origin[] = {
            10f,20f,30f
    };
    float origin2[] = {
            20f,20f,30f
    };
    int dimx = 50;
    int dimy = 60;
    int dims[] = new int[3];
    dims[0] = dimx;
    dims[1] = dimy;
    dims[2] = 1;
    short* pixels = new short[dimx*dimy];
    float v = 0.0f;
    for (int iy = 0;iy < dimy;iy++)
    {
        for (int ix = 0;ix < dimx;ix++)
        {
            double  dist = Math.hypot(ix-dimx/2,iy-dimy/2);
            if (dist < 25)
            {
                v = 100f;
            }
            else
            {
                v = 200f;
            }
            if ((iy == 0) || (iy == dimy-1))
            {
                v = 1000;
            }
            if ((ix == 0) || (ix == dimx-1))
            {
                v = 1000;
            }
            pixels[dimx*iy+ix] = (short) v;
        }
    }
    pixels[dimx*20+10] = 0;
    NRRDWriter writer;
    writer.write("/tmp/test3.nrrd",origin,spacing,dxdydz,pixels);


}
