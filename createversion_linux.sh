#!/bin/bash
echo  "#ifndef VERSION_H" > version.h
echo  "#define VERSION_H" >> version.h
echo  "" >> version.h
echo  "/*!" >> version.h
echo  " * \\file version.h" >> version.h
echo  " * \\brief This file contains information about the code version" >> version.h
echo  " * \\author Peter Schiansky" >> version.h
echo  " * \\bug There are no known bugs." >> version.h
echo  " *" >> version.h
echo  " * The definitions in this file are used to fill the about-dialog with information about the code:" >> version.h
echo  " * The git commit description, the commit date and the builddate." >> version.h
echo  " */" >> version.h
echo  "" >> version.h
echo  "/*!" >> version.h
echo  " * \\def VERSION_GIT" >> version.h
echo  " * \\brief The git commit description" >> version.h
echo  " */" >> version.h
echo  "#define VERSION_GIT \"$(git describe)"\" >> version.h
echo  "/*!" >> version.h
echo  "* \\def VERSION_GIT_DATE" >> version.h
echo  "* \\brief The date of the git commit" >> version.h
echo  "*/" >> version.h
echo  "#define VERSION_GIT_DATE $(git show -s --format=%cd --date=format:%Y%m%d%H%M)" >> version.h
echo  "/*!" >> version.h
echo  " * \\def VERSION_BUILD_DATE" >> version.h
echo  " * \\brief The builddate" >> version.h
echo  " */" >> version.h
echo  "#define VERSION_BUILD_DATE $(date +%Y%m%d%H%M)" >> version.h
echo  "" >> version.h
echo  "#endif" >> version.h
