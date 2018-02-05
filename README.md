A simple set of utilies for recording and viewing NRRD files that contains patial plane information of the image aloing with the image pixels.
The initial format varient supported by the viewer is like the following subsequent lines as an example:

NRRD0004
type: float
dimension: 3
space dimension: 3
sizes: 475 1250 190
space directions: (1,0,0) (0,1,0) (0,0,1)
kinds: domain domain domain
endian: little
encoding: raw
space origin: (0,0,0)

Where the example has dimension of 3, the viewer will also suport dimension of 2.
Where the pixel type of the example, the viewer will also support signed shorts.


