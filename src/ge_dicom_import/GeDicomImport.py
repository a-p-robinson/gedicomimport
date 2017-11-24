"""GeDicomImport.py
 30/01/17
 andrew.robinson@npl.co.uk

 Import data into GE Xeleris system based on an exisiting GE Dicom file

 Usage:
  python GeDicomImport -d new_dataset_name original_dicom_filename new_dicom_filename
  (python GeDicomImport -h for full list of options)

 Requirements:
  [pydicom <v1.0] sudo pip install pydicom
  http://pydicom.readthedocs.io/en/stable/


 Requires python 2.7 currently

 Reformatted partially by EJP
"""

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

    print("Reading Interfile: " + filename)
    dimensions, dataFile, dtype = readInterFileHeader(filename)
    words = re.split("(/)", filename)
    datafilename = ''.join(words[0:-1]) + dataFile

    print("Matrix: ")
    print(dimensions)
    print("Data Type: ")
    print(dtype)
    print("Binary data file: " + datafilename)

    return readInterFileData(datafilename, dimensions, dtype)
#----------------------

#----------------------
# Change the dataname and UID of the dicom file
def changeDataName(datasetname, uid_offset=10):

    ############################################################################################################################################################
    # [EM1] (0008, 0013) Instance Creation Time              TM: '162936.0000' -> +2                                                                           #
    # [EM1] (0008, 0018) SOP Instance UID                    UI: 1.2.840.113619.2.280.2.1.13072016173700324.423137180 -> .3 (or what ever is clear from SC)    #
    # [EM1] (0011, 1012) [Dataset Name]                      LO: 'TOMO Liver_EM'                                                                               #
    # [EM1] (0011, 1030) [Picture Object Name]               LO: 'TOMO Liver_EM'                                                                               #
    # [EM1] (0011, 1050) [Where Object Name]                 LO: 'TOMO Liver_EM'                                                                               #
    # [EM1] (0033, 1107) [Orig SOP Instance UID]             LO: '1.2.840.113619.2.280.2.1.13072016173700324.423137180' - > .3 (or what ever is clear from SC) #
    ############################################################################################################################################################

    #new_uid = ds[0x08,0x0018].value + '.' + str(uid_offset + randint(0,9)) # Add random number to UID
    new_uid = ds[0x08,0x0018].value + '.' + str(uid_offset)

    print("\n|Dataset Name|")

    print(ds[0x08,0x0018].value + " --> " + new_uid)
    print(ds[0x33,0x1107].value + " --> " + new_uid)

    print(ds[0x11,0x1012].value + " --> " + datasetname)
    print(ds[0x11,0x1030].value + " --> " + datasetname)
    print(ds[0x11,0x1050].value + " --> " + datasetname)

    # Change UIDs
    ds[0x08,0x0018].value = new_uid
    ds[0x33,0x1107].value = new_uid

    # Change datset name
    ds[0x11,0x1012].value = datasetname
    ds[0x11,0x1050].value = datasetname
    ds[0x11,0x1030].value = datasetname
#----------------------

#----------------------
# Change the energy window of the dicom file
#  Energy window number can be set manually to avoid overlapping
def changeEnergyWindow(energy, energy_number=7):

    ###########################################################################################
    # [EM1] (0011, 1016) [Energy Number]                     SL: 1 -> 4 (or whatever is free) #
    # [EM1] (0028, 1050) Window Center                       DS: '22.500000'  -> ???          #
    # [EM1] (0028, 1051) Window Width                        DS: '45.000000'  -> ???          #
    # [EM1] (0054, 0014) Energy Window Lower Limit           DS: '39.84'      -> low          #
    # [EM1] (0054, 0015) Energy Window Upper Limit           DS: '126.16'     -> hi           #
    # [EM1] (0054, 0018) Energy Window Name                  SH: 'Y90_EM'     -> new          #
    ###########################################################################################

    print("\n|Energy Window|")

    print(str(ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0014].value) + " --> " + energy[0])
    print(str(ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0015].value) + " --> " + energy[1])
    print(ds[0x54,0x12].value[0][0x54,0x18].value + " --> " + ds[0x54,0x12].value[0][0x54,0x18].value + "-" + str(energy_number))
    print(str(ds[0x11,0x1016].value) + " --> " + str(energy_number))

    ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0014].value = float(energy[0])
    ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0015].value = float(energy[1])
    ds[0x54,0x12].value[0][0x54,0x18].value += "-" + str(energy_number)
    ds[0x11,0x1016].value = int(energy_number)
#----------------------

#----------------------
# Change the pixel data of the dicom file
def changePixelData(interfile):

    print("\n|Pixel Data|")
    newPixelData = readIFmatrix(interfile)

    # Swap the array data
    ds.PixelData = newPixelData.tostring() # Have to write as raw data
#----------------------

def main():
    #---------------------------------------------------
    # Parse the arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("dicomfile", help="Original GE Xelris DICOM file")
    parser.add_argument("outputfile", help="Modified DICOM file")

    parser.add_argument("-d", "--datasetname", help="New dataset name")
    parser.add_argument("-e", "--energywindow", help="Low and High energy window values (keV)", nargs = 2)
    parser.add_argument("-i", "--interfile", help="Interfile to replace pixel data with")
    parser.add_argument("-u", "--uid", help="Specify file specific UID (single number)")

    args = parser.parse_args()
    #---------------------------------------------------

    #---------------------------------------------------
    # Process the file
    print("\nReading DICOM file: " + args.dicomfile)
    ds = dicom.read_file(args.dicomfile)

    # Change the dataset name
    if args.uid:
        changeDataName(args.datasetname, args.uid)
    else:
        changeDataName(args.datasetname)

    # Modifiy the energy window if requested
    if args.energywindow and args.uid:
        changeEnergyWindow(args.energywindow, args.uid)
    elif args.energywindow:
        changeEnergyWindow(args.energywindow)

    # Change the pixel data if supplied
    if args.interfile:
        changePixelData(args.interfile)

    # Save file
    print("\nSaving file: " + args.outputfile)
    ds.save_as(args.outputfile)
    #---------------------------------------------------

if __name__ == "__main__":
    main()
