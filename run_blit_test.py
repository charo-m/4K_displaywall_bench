#!/usr/bin/env python

import os
import sys, getopt
import subprocess
import platform
import csv

local_cwd = os.path.dirname(os.path.realpath(__file__))
print local_cwd
os.chdir(local_cwd)
distname, version, Id = platform.linux_distribution()
print distname, version

csvfile = 'blit_report.csv'
num_images = 5
try:
  opts, args = getopt.getopt(sys.argv[1:], "hf:n:", ["csvfile=","num_images="])
except getopt.GetoptError:
  print 'run_test.py --csvfile/-f <outputfile> --images/-n <num images>'
  sys.exit(2)
for opt, arg in opts:
  if opt == '-h':
    print 'run_test.py --csvfile/-f <outputfile> --images/-n <num images>'
    sys.exit()
  elif opt in ("-f", "--csvfile"):
    csvfile = arg
  elif opt in ("-n", "--num_images"):
    num_images = arg

print str(num_images) + " images"


rows = []
append_to_existing_data = False
if os.path.isfile(csvfile):
  with open(csvfile, 'rb') as cf:
    reader = csv.reader(cf)
    for row in reader:
      rows.append(row)
  cf.close()
  if len(rows) != 0:
    append_to_existing_data = True

with open(csvfile, 'wb') as cf:
  writer = csv.writer(cf)
  column_label = "FPS " + distname + version
  if append_to_existing_data:
    row = rows[0]
    row.append(column_label)
    writer.writerow(row)
  else:
    writer.writerow([str(num_images) + " images", column_label])

  images = []
  for x in range(0, int(num_images)):
    images.extend(["-i", "./images/bubble4K.png",])


#num images, regular mode
  if len(images):
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-mipmap", "true", "-layout", "true",
                                      "-testmode", "fps",
                                      ] + images, cwd=local_cwd)
  else:
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-testmode", "fps",
                                      ], cwd=local_cwd)


  if append_to_existing_data:
    row = rows[1]
    row.append(data)
    writer.writerow(row)
  else:
    writer.writerow(["no blit", data])

#num images, blit entire window
  if len(images):
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-mipmap", "true", "-layout", "true",
                                      "-testmode", "fps",
                                      "-blit"
                                      ] + images, cwd=local_cwd)
  else:
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-testmode", "fps",
                                      "-blit"
                                      ], cwd=local_cwd)
  if append_to_existing_data:
    row = rows[2]
    row.append(data)
    writer.writerow(row)
  else:
    writer.writerow(["blit full", data])

#num images, blit 2/3
  if len(images):
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-mipmap", "true", "-layout", "true",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "0", "0", "7680", "2160"
                                      ] + images, cwd=local_cwd)
  else:
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "0", "0", "7680", "2160"
                                      ], cwd=local_cwd)
  if append_to_existing_data:
    row = rows[3]
    row.append(data)
    writer.writerow(row)
  else:
    writer.writerow(["blit 2/3", data])

#num images, blit 1/2
  if len(images):
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-mipmap", "true", "-layout", "true",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "2892", "0", "5785", "2160"
                                      ] + images, cwd=local_cwd)
  else:
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "2892", "0", "5785", "2160"
                                      ], cwd=local_cwd)
  if append_to_existing_data:
    row = rows[3]
    row.append(data)
    writer.writerow(row)
  else:
    writer.writerow(["blit 1/2", data])

#num images, blit 1/3
  if len(images):
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-mipmap", "true", "-layout", "true",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "3840", "0", "3840", "2160"
                                      ] + images, cwd=local_cwd)
  else:
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "3840", "0", "3840", "2160"
                                      ], cwd=local_cwd)
  if append_to_existing_data:
    row = rows[4]
    row.append(data)
    writer.writerow(row)
  else:
    writer.writerow(["blit 1/3", data])

#num images, blit 1/4
  if len(images):
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-mipmap", "true", "-layout", "true",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "3840", "0", "2892", "2160"
                                      ] + images, cwd=local_cwd)
  else:
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "3840", "0", "2892", "2160"
                                      ], cwd=local_cwd)
  if append_to_existing_data:
    row = rows[5]
    row.append(data)
    writer.writerow(row)
  else:
    writer.writerow(["blit 1/4", data])

#num images, 1/5
  if len(images):
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-mipmap", "true", "-layout", "true",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "3840", "0", "2314", "2160"
                                      ] + images, cwd=local_cwd)
  else:
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "3840", "0", "2314", "2160"
                                      ], cwd=local_cwd)
  if append_to_existing_data:
    row = rows[6]
    row.append(data)
    writer.writerow(row)
  else:
    writer.writerow(["blit 1/5", data])

#num images, 1/6
  if len(images):
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-mipmap", "true", "-layout", "true",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "3840", "0", "1928", "2160"
                                      ] + images, cwd=local_cwd)
  else:
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "3840", "0", "1928", "2160"
                                      ], cwd=local_cwd)
  if append_to_existing_data:
    row = rows[7]
    row.append(data)
    writer.writerow(row)
  else:
    writer.writerow(["blit 1/6", data])

#num images, 1/12
  if len(images):
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-mipmap", "true", "-layout", "true",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "3840", "1080", "1920", "1080"
                                      ] + images, cwd=local_cwd)
  else:
    data = subprocess.check_output(
           ["./4K_displaywall_bench", "-width", "11520", "-height", "2160",
                                      "-testmode", "fps",
                                      "-blit",
                                      "-blit_rect", "3840", "1080", "1920", "1080"
                                      ], cwd=local_cwd)
  if append_to_existing_data:
    row = rows[7]
    row.append(data)
    writer.writerow(row)
  else:
    writer.writerow(["blit 1/12", data])


print csvfile, " has been written with test data."
