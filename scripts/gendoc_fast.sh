#!/bin/sh

echo "Calling doxygen\n"
doxygen doxygen_cagecontrol

cd ../doc/latex

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
