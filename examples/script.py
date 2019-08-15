from tempfile import mkstemp
from shutil import move
from os import fdopen, remove, system
import os
import time


def replace(file_path, pattern, subst):
    #Create temp file
	fh, abs_path = mkstemp()
	with fdopen(fh,'w') as new_file:
		with open(file_path) as old_file:
			for line in old_file:
				line_splitted = line.split(pattern)
				if len(line_splitted) == 1:
					new_file.write(line)
				else:
					new_file.write(line_splitted[0] + pattern + str(subst) + ";\n")
	#Remove original file
	remove(file_path)
	#Move new file
	move(abs_path, file_path)


for nbids in [90]:
	for alpha in [0.0, 0.5, 1.0, 0.25, 0.75]:
		#alpha = float(a) / 20;
		#alpha = round(alpha, 6)
		#share = round(share, 6)
		replace("cpp/AsymetricDoubleAuction.cpp", "alpha = ", alpha)
		replace("cpp/AsymetricDoubleAuction.cpp", "nbids = ", nbids)
		#replace("cpp/AsymetricDoubleAuction.cpp", "share = ", share)
		system("make")
		#system("./AsymetricDoubleAuction >> Output_Nbids.txt")
		system("./AsymetricDoubleAuction")
		print("alpha = " + str(alpha) + " nbids = " + str(nbids) + "\n")
