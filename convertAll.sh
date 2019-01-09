#!/bin/bash

#
# Script to convert a directory of sequential dicom files using SiemensDicomImport.py
#

dicomfile="Manc_central_UHD2i21ssG10_DICOM/PETACUHD20010mm001_PT"
newname="test"
#interfile="it2ss21/y90_series_sinogram-WB-PSFTOF_it2_s21_g12_000_integer.hdr"
interfile="it2ss21/y90_series_sinogram-WB-PSFTOF_it2_s21_g12_000_ctm.v.hdr"
slices=88

for i in $(seq -f "%03g" 1 $slices);
do
    j=`expr $i - 1`
    #echo $i $j;
    python SiemensDicomImport.py $dicomfile$i.dcm $newname -i $interfile -d $newname -s $j
done
