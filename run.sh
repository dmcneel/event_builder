#!/bin/bash

for RUN in {200..213..1}
do
  
   root -q -b -l "allcores.C($RUN)"
    






cp gen.root dp1/gen_run${RUN}.root
echo ----Done with Processing Run Number ${RUN}----
rm gen.root
done
