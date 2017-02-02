###########################################################################################
# GeDicomImport.py                                                                        #
# 30/01/17                                                                                #
# andrew.robinson@npl.co.uk                                                               #
#                                                                                         #
# Import data into GE Xeleris system based on an exisiting GE Dicom file                  #
#                                                                                         #
# Usage:                                                                                  #
#  python GeDicomImport -d new_dataset_name original_dicom_filename new_dicom_filename    #
#  (python GeDicomImport -h for full list of options)                                      #
#                                                                                         #
# Requirements:                                                                           #
#  [pydicom] sudo pip install pydicom                                                     #
#   http://pydicom.readthedocs.io/en/stable/                                              #
###########################################################################################

import dicom
import argparse
import re
import numpy as np
from random import randint

#----------------------
# Read the data filename, matrix dimensions and data type from an interfile header
def readInterFileHeader(inputFileName):
    inFile = open(inputFileName, 'r')
    for line in inFile:
        if re.search("matrix size \[1\]", line):
            line = line.rstrip('\n\r')
            words = re.split("=", line)
            x = words[1]
        if re.search("matrix size \[2\]", line):
            line = line.rstrip('\n\r')
            words = re.split("=", line)
            y =  words[1]
        if re.search("number of projections", line):
            line = line.rstrip('\n\r')
            words = re.split("=", line)
            z =  words[1]
        if re.search("name of data file", line):
            line = line.rstrip('\n\r')
            words = re.split("=", line)
            dataFile =  words[1]
        if re.search("number format", line):
            line = line.rstrip('\n\r')
            words = re.split("=", line)
            if re.match("unsigned integer", words[1]):
                dtype = np.uint16
            if re.match("signed integer", words[1]):
                dtype = np.int16

    inFile.close()

    return [int(x),int(y),int(z)], dataFile, dtype
#----------------------

#----------------------
# Read the binary data from an interfile into a numpy array
def readInterFileData(dataFileName, PixelDims, dtype):

    ArrayRaw = np.fromfile(dataFileName,dtype=dtype)
    ArrayRaw3D = ArrayRaw.reshape(PixelDims, order='C') # Reshape the array into 3D
    #ArrayRaw3D = ArrayRaw3D[:,:,::-1]                        # Flip the array in Z
    #ArrayRaw3D = numpy.rot90(ArrayRaw3D)                     # Rotate 90deg CCW

    return ArrayRaw3D
#----------------------

#----------------------
# Read in the matrix data from an interfile
def readIFmatrix(filename):

    print "Reading Interfile: " + filename
    dimensions, dataFile, dtype = readInterFileHeader(filename)
    words = re.split("(/)", filename)
    datafilename = ''.join(words[1:-1]) + dataFile

    print "Matrix: "
    print dimensions
    print "Data Type: " 
    print dtype
    print "Binary data file: " + datafilename
    
    return readInterFileData(datafilename, dimensions, dtype)
#----------------------

#----------------------
# Change the dataname and UID of the dicom file
def changeDataName(dicomfile):
    return 1
#----------------------

#----------------------
# Change the energy window of the dicom file
def changeEnergyWindow(dicomfile):
    return 1
#----------------------

#----------------------
# Change the pixel data of the dicom file
def changePixelData(dicomfile):
    return 1
#----------------------

#---------------------------------------------------
# Parse the arguments
parser = argparse.ArgumentParser()
parser.add_argument("dicomfile", help="Original GE Xelris DICOM file")
parser.add_argument("outputfile", help="Modified DICOM file")

parser.add_argument("-d", "--datasetname", help="New dataset name")
parser.add_argument("-e", "--energywindow", help="Low and High energy window values (keV)", nargs = 2)
parser.add_argument("-i", "--interfile", help="Interfile to replace pixel data with")

args = parser.parse_args()
print args

#original_file = args.
#patient = args.patient
#---------------------------------------------------


newPixelData = readIFmatrix("/home/apr/Analysis/GeDicomImport/TestData/InterFile/EM1_LEHR-SinglesXZ.hdr")

# File to read in
ds = dicom.read_file("/home/apr/Analysis/GeDicomImport/TestData/CATIE_90Y/TOMOLiver_EM001_DS.dcm")


# Dump the data to the screen
#print ds
#print ds.pixel_array

# Clone EM with a differnet 'ID'
############################################################################################################################################################
# [EM1] (0008, 0013) Instance Creation Time              TM: '162936.0000' -> +2                                                                           #
# [EM1] (0008, 0018) SOP Instance UID                    UI: 1.2.840.113619.2.280.2.1.13072016173700324.423137180 -> .3 (or what ever is clear from SC)    #
# [EM1] (0011, 1012) [Dataset Name]                      LO: 'TOMO Liver_EM'                                                                               #
# [EM1] (0011, 1016) [Energy Number]                     SL: 1 -> 4 (or whatevr is free)                                                                   #
# [EM1] (0011, 1030) [Picture Object Name]               LO: 'TOMO Liver_EM'                                                                               #
# [EM1] (0011, 1050) [Where Object Name]                 LO: 'TOMO Liver_EM'                                                                               #
# [EM1] (0033, 1107) [Orig SOP Instance UID]             LO: '1.2.840.113619.2.280.2.1.13072016173700324.423137180' - > .3 (or what ever is clear from SC) #
############################################################################################################################################################

print "-----------------"
print 'Values to change:'
print "-----------------"

print ds[0x08,0x0013].value
print ds[0x08,0x0018].value
print ds[0x11,0x1012].value
print ds[0x11,0x1016].value
print ds[0x11,0x1030].value
print ds[0x11,0x1050].value
print ds[0x33,0x1107].value

#----------------------------------------
# Change Values:

# # Time
# time_offset = 40.0
# ds[0x08,0x0013].value = str(float(ds[0x08,0x0013].value) + time_offset + randint(0,9))

# UID
uid_offset = 7
ds[0x08,0x0018].value += '.'
ds[0x08,0x0018].value += str(uid_offset)
ds[0x33,0x1107].value += '.'
ds[0x33,0x1107].value += str(uid_offset)

# Description
data_description = 'TOMO Liver_NameUID'
ds[0x11,0x1012].value = data_description
ds[0x11,0x1050].value = data_description
ds[0x11,0x1030].value = data_description

# # Energy Number
# ds[0x11,0x1016].value = uid_offset
#----------------------------------------

print "-----------------"
print 'New Values:'
print "-----------------"

print ds[0x08,0x0013].value
print ds[0x08,0x0018].value
print ds[0x11,0x1012].value
print ds[0x11,0x1016].value
print ds[0x11,0x1030].value
print ds[0x11,0x1050].value
print ds[0x33,0x1107].value

# #----------------------------------------
# # Modify the energy window
# ##################################################################################
# # [EM1] (0028, 1050) Window Center                       DS: '22.500000'  -> ??? #
# # [EM1] (0028, 1051) Window Width                        DS: '45.000000'  -> ??? #
# # [EM1] (0054, 0014) Energy Window Lower Limit           DS: '39.84'      -> 200 #
# # [EM1] (0054, 0015) Energy Window Upper Limit           DS: '126.16'     -> 300 #
# # [EM1] (0054, 0018) Energy Window Name                  SH: 'Y90_EM'     -> new #
# ##################################################################################

# print "-----------------"
# print 'Values to change:'
# print "-----------------"
# print ds[0x28,0x1050].value
# print ds[0x28,0x1051].value
# print ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0014].value
# print ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0015].value
# print ds[0x54,0x12].value[0][0x54,0x18].value

# lower_e = 20.0
# upper_e = 40.0
# energy_name = 'Y90_test2'

# ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0014].value = lower_e
# ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0015].value = upper_e
# ds[0x54,0x12].value[0][0x54,0x18].value = energy_name

# print "-----------------"
# print 'New Values:'
# print "-----------------"
# print ds[0x28,0x1050].value
# print ds[0x28,0x1051].value
# print ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0014].value
# print ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0015].value
# print ds[0x54,0x12].value[0][0x54,0x18].value

# #----------------------------------------

# #----------------------------------------
# # Swap the array data
# ds.PixelData = newPixelData.tostring() # Have to write as raw data
# #----------------------------------------

#----------------------------------------
# Save file
ds.save_as("test_name_uid.dcm")
#----------------------------------------

# ds[0x11,0x1012].value = 'EM_test2'
# print ds[0x11,0x1012].value

# print ds[0x11,0x1030].value
# ds[0x11,0x1030].value = 'EM_test2'
# print ds[0x11,0x1030].value

# print ds[0x11,0x1050].value
# ds[0x11,0x1050].value = 'EM_test2'
# print ds[0x11,0x1050].value


# # print ds[0x8,0x103e].value
# # ds[0x8,0x103e].value = 'EM_test'
# # print ds[0x8,0x103e].value


# # #(0011, 1012) [Dataset Name]                      LO: 'TOMO Liver_EM'
# # #(0008, 103e) Series Description                  LO: 'TOMO Liver'

# #(0011, 1012) [Dataset Name]                      LO: 'TOMO Liver_EM'
# #(0011, 1030) [Picture Object Name]               LO: 'TOMO Liver_EM'
# #(0011, 1050) [Where Object Name]                 LO: 'TOMO Liver_EM'




