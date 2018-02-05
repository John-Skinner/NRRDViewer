package nrrd;

import java.io.*;
import java.nio.ByteOrder;
import java.nio.ByteBuffer;
public class NRRDWriter
{
    /**
     * write records a NRRD file in raw format containing floats (even though shorts are passed)
     * for either 2 dimensional images or 3-dimensional volumes.  In either case, the geometic coordinate
     * information is in 3-dimensional space, so that the origin coordinate and basis vectors
     * must be  3 dimensional.  Spacing and dimensions are either 2 or 3 dimensional.
     *
     * @param filePath  Where the nrrd file is written.
     * @param originxyz The origin in 3-dimensional space for the origin of image/volume.
     * @param spacing The coordinate spacing between image pixels, which must be 3-dimensional (use any value
     *                for the third dimension).
     * @param dx_dy_dzVector  The 3-dimensional displacement vectors in the  following order:
     *                        [0] = dx of row
     *                        [1] = dy of row
     *                        [2] = dz of row
     *                        [3] = dx of column
     *                        [4] = dy of column
     *                        [5] = dz of column
     *                        [6] = dx of next slice
     *                        [7] = dy of next slice
     *                        [8] = dz of next slice
     *
     * @param voxels The 2 or 3 dimensional volume of voxels indexed as follows:
     *                                       value(x,y,z) = voxels[dims[0]*dims[1]*z+dims[0]*ay+x];
     *
     * @param dims The dimensions of the image which must be  3-dimensional.  One can make the third dimension size 1 for images
     *             @return return of true indicates write succeeded.  false indicate either argument error or write failed.
     */
    public boolean write(String filePath, float[] originxyz, float[] spacing, float[] dx_dy_dzVector, short[] voxels, int[] dims)
    {
        ByteBuffer byteBuf  = ByteBuffer.allocate(4);
        StringBuilder formatter = new StringBuilder();
        byteBuf.order(ByteOrder.LITTLE_ENDIAN);
        int[] address = new int[3];
        FileOutputStream out;
        BufferedOutputStream bos = null;
        try
        {

            out = new FileOutputStream(new File(filePath));


            bos = new BufferedOutputStream(out);

            /*
             * Everything written out has to be written with write() as byte to allow the binary pixel representation
             * to also be written in binary form.
             */
            bos.write("NRRD0004\n".getBytes());
            bos.write("type: float\n".getBytes());
            String dimString = "dimension: " + dims.length + "\n";
            String spaceDimension = "space " + dimString;
            bos.write(dimString.getBytes());
            bos.write(spaceDimension.getBytes());
            formatter.setLength(0);

            formatter.append("sizes: ");
            for (int i = 0;i < 3;i++)
            {
                formatter.append(dims[i]);
                if (i < 2)
                {
                    formatter.append(" ");
                }
            }
            formatter.append("\n");
            bos.write(formatter.toString().getBytes());
            formatter.setLength(0);
            formatter.append("space directions:");
            for(int i = 0;i < 9;i+=3)
            {
                formatter.append(" (");
                formatter.append(dx_dy_dzVector[i]);
                formatter.append(",");
                formatter.append(dx_dy_dzVector[i+1]);
                formatter.append(",");
                formatter.append(dx_dy_dzVector[i+2]);
                formatter.append(")");
                if (i < 8)
                {
                    formatter.append(" ");
                }
            }
            formatter.append("\n");
            bos.write(formatter.toString().getBytes());

            formatter.setLength(0);

            formatter.append("kinds: ");
            for (int i = 0;i < 3;i++)
            {
                formatter.append("domain");
                if (i < dims.length - 1)
                {
                    formatter.append(" ");
                }
            }
            formatter.append("\n");
            bos.write(formatter.toString().getBytes());
            bos.write("endian: little\n".getBytes());
            bos.write("encoding: raw\n".getBytes());
            formatter.setLength(0);
            formatter.append("space origin: (");
            for (int i = 0;i < 3;i++)
            {
                formatter.append(originxyz[i]);
                if (i < 2)
                {
                    formatter.append(",");
                }
            }
            formatter.append(")\n");
            bos.write(formatter.toString().getBytes());
            bos.write("\n".getBytes());
            for (int iz = 0; iz < dims[2]; iz++)
            {
                address[2] = iz;
                for (int iy = 0; iy < dims[1]; iy++)
                {
                    address[1] = iy;
                    for (int ix = 0; ix < dims[0]; ix++)
                    {
                        address[0] = ix;
                        float v = voxels[dims[0]*dims[1]*address[2]+dims[0]*address[1]+address[0]];
                        byteBuf.asFloatBuffer().put(v);
                        bos.write(byteBuf.array(),0,4);

                    }
                }
            }

            bos.close();


        } catch (IOException exc)
        {
            if (bos != null)
            {
                try
                {
                    bos.close();
                } catch (IOException e)
                {
                    e.printStackTrace();
                }
            }

            return false;
        }
        finally
        {
            if (bos != null)
            {
                try
                {
                    bos.close();
                } catch (IOException e)
                {
                    e.printStackTrace();
                    return false;
                }
            }
            return true;
        }
    }

}

