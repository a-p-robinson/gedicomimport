# root2image #

This is a set of scripts and code to convert the projections in a root file into
interfiles and DICOMs. The code is all originally written by APR, this README
discusses the bits I have used and is my understanding of what they do.

The root file containing projections is produced using the `genP` code,
documented elsewhere.

## `projMacros.C` ##

This contains a selection of root macros to convert the projection data in a
root file to an interfile

I have only used some of these but I have listed all of the macros in the file
for completeness.

+ `analyseProj`; seems to just check that the given projection is fine, with no
  overflows.
+ `getTime`; fills a time histogram
+ `showSF`; shows the scatter fraction in the projection
+ `showProj`; shows the projections
+ `writeInter`; writes the given projection set to an interfile
+ `writeInterFloat`; presumably the same but outputs a float data type
+ `writeInter2`; same as `writeInter` but takes a header file as an argument to
  produce interfiles that are compatible with Xeleris. They may not actually be
  compatible.
  
### `writeInter` ###

This takes the following arguments:

+ The name of the projection (the name in the root file)
+ The type of the projection (NOT the data type, but e.g. 'singlesXZ',
  'sourceXZ')
+ The number of projections per head

This produces interfiles with the header defined from hard coded output in the
root macro. A DICOM image is also produced using the external program `medcon`.

### `writeInter2` ###

This takes the following arguments:

+ The name of the projection (the name in the root file)
+ The type of the projection (NOT the data type, but e.g. 'singlesXZ',
  'sourceXZ')
+ The name of the header file to use as the template
+ The ID of something to put in the header file
+ The number of projections per head

This produces interfiles with the header defined from hard coded output in the
root macro. A DICOM image is also produced using the external program `medcon`.
This DICOM image may need further processing with `GeDicomImport.py`.

## `GeDicomImport.py` ##

This produces a Xeleris compatible DICOM with provided pixel data and a few
required changes to header fields.

Takes as arguments:

+ An original DICOM file to copy and modify
+ The name of the new DICOM file to write

Optionally:

+ -d [datasetname]; A new data set name to put in the header
+ -e [energywindow]; New energy window bounds
+ -i [interfile]; A new interfile to replace the pixel data with
+ -u [uid]; A new UID for the file
