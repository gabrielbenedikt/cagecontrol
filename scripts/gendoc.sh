#!/bin/sh

echo "Calling doxygen\n"
doxygen doxygen_cagecontrol

cd ../doc/latex

for i in *.dot; do
  echo "converting $i to ps"
  dot -Tps2 ${i%%.*}.dot -o ${i%%.*}.ps
  echo "converting ${i%%.*}.ps to pdf"
  ps2pdf ${i%%.*}.ps
done

echo "Generating single pdf"
make

echo "Copying pdf to docdir"
cp refman.pdf ../cagecontrol_doc.pdf

echo "generating pdf pax"
cd ..
pdfannotextractor cagecontrol_doc.pdf
#
# Broken
#
#cd ../html
#echo "Generating QT helpfile"
#qhelpgenerator index.qhp -o ../cagecontrol_help.qch
#cd ..
#qcollectiongenerator cagecontrol_help.qhcp -o cagecontrol_help.qhc
