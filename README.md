## `GeDicomImport.py` ##

This produces a Xeleris compatible DICOM with provided pixel data and a few
required changes to header fields.

Takes as arguments:

+ An original DICOM file to copy and modify.
+ The name of the new DICOM file to write.
+ A new name of the dataset to put in the header.

Optionally:

+ -e [energywindow]; New energy window bounds
+ -i [interfile]; A new interfile to replace the pixel data with
+ -u [uid]; A new UID for the file
+ -t [image type]: The type of the image - Reconstructed or projections. This
  defaults to projections for backwards compatibility.

Note that if you include multiple files with the same uid then Xeleris thinks
they are the same and only sees one.

Also, if the datasetname is the same, then a '1' is appended to one of the data
sets - making them hard to distinguish.
