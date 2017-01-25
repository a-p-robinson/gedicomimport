import dicom

ds = dicom.read_file("TOMOLiver_EM001_DS.dcm")

newHeight = '-344.947394'

ds[0x54,0x22].value[0][0x20,0x32].value[1] = newHeight

print ds

print ds[0x11,0x1012].value
ds[0x11,0x1012].value = 'EM_test2'
print ds[0x11,0x1012].value

print ds[0x11,0x1030].value
ds[0x11,0x1030].value = 'EM_test2'
print ds[0x11,0x1030].value

print ds[0x11,0x1050].value
ds[0x11,0x1050].value = 'EM_test2'
print ds[0x11,0x1050].value


# print ds[0x8,0x103e].value
# ds[0x8,0x103e].value = 'EM_test'
# print ds[0x8,0x103e].value


# #(0011, 1012) [Dataset Name]                      LO: 'TOMO Liver_EM'
# #(0008, 103e) Series Description                  LO: 'TOMO Liver'

#(0011, 1012) [Dataset Name]                      LO: 'TOMO Liver_EM'
#(0011, 1030) [Picture Object Name]               LO: 'TOMO Liver_EM'
#(0011, 1050) [Where Object Name]                 LO: 'TOMO Liver_EM'



ds.save_as("test_all2.dcm")
