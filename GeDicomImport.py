#########################################################################
# GeDicomImport.py                                                      #
# 30/01/17                                                              #
# andrew.robinson@npl.co.uk                                             #
#                                                                       #
# Modify an export GE Xelris Dicom file to allow it to be read back in. #
#                                                                       #
#########################################################################

import dicom # http://pydicom.readthedocs.io/en/stable/ref_guide.html

# File to read in
ds = dicom.read_file("/home/apr/Analysis/GeDicomImport/TestData/CATIE_90Y/TOMOLiver_EM001_DS.dcm")
#ds = dicom.read_file("/home/apr/Analysis/GeDicomImport/TestData/CATIE_90Y/TOMOLiver_SC1001_DS.dcm")
#ds = dicom.read_file("/home/apr/Analysis/GeDicomImport/TestData/CATIE_90Y/TOMOLiver_SC2001_DS.dcm")

# Dump the data to the screen
print ds

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

# Time
time_offset = 30.0
ds[0x08,0x0013].value = str(float(ds[0x08,0x0013].value) + time_offset)

# UID
uid_offset = 8
ds[0x08,0x0018].value += '.'
ds[0x08,0x0018].value += str(uid_offset)
ds[0x33,0x1107].value += '.'
ds[0x33,0x1107].value += str(uid_offset)

# Description
data_description = 'TOMO Liver_NewE'
ds[0x11,0x1012].value = data_description
ds[0x11,0x1050].value = data_description
ds[0x11,0x1030].value = data_description

# Energy Number
ds[0x11,0x1016].value = uid_offset
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

#----------------------------------------
# Modify the energy window
##################################################################################
# [EM1] (0028, 1050) Window Center                       DS: '22.500000'  -> ??? #
# [EM1] (0028, 1051) Window Width                        DS: '45.000000'  -> ??? #
# [EM1] (0054, 0014) Energy Window Lower Limit           DS: '39.84'      -> 200 #
# [EM1] (0054, 0015) Energy Window Upper Limit           DS: '126.16'     -> 300 #
# [EM1] (0054, 0018) Energy Window Name                  SH: 'Y90_EM'     -> new #
##################################################################################

print "-----------------"
print 'Values to change:'
print "-----------------"
print ds[0x28,0x1050].value
print ds[0x28,0x1051].value
print ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0014].value
print ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0015].value
print ds[0x54,0x12].value[0][0x54,0x18].value

lower_e = 100.0
upper_e = 200.0
energy_name = 'Y90_test'

ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0014].value = lower_e
ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0015].value = upper_e
ds[0x54,0x12].value[0][0x54,0x18].value = energy_name

print "-----------------"
print 'New Values:'
print "-----------------"
print ds[0x28,0x1050].value
print ds[0x28,0x1051].value
print ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0014].value
print ds[0x54,0x12].value[0][0x54,0x13].value[0][0x54,0x0015].value
print ds[0x54,0x12].value[0][0x54,0x18].value

#----------------------------------------


# Save file
ds.save_as("test_newE.dcm")


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




