#!/bin/bash
echo -E "#ifndef VERSION_H" > version.h
echo -E "#define VERSION_H" >> version.h
echo -E "" >> version.h
echo -E "/*!" >> version.h
echo -E " * \\file version.h" >> version.h
echo -E " * \\brief This file contains information about the code version" >> version.h
echo -E " * \\author Peter Schiansky" >> version.h
echo -E " * \\bug There are no known bugs." >> version.h
echo -E " *" >> version.h
echo -E " * The definitions in this file are used to fill the about-dialog with information about the code:" >> version.h
echo -E " * The git commit description, the commit date and the builddate." >> version.h
echo -E " */" >> version.h
echo -E "" >> version.h
echo -E "/*!" >> version.h
echo -E " * \\def VERSION_GIT" >> version.h
echo -E " * \\brief The git commit description" >> version.h
echo -E " */" >> version.h
echo -E "#define VERSION_GIT \"$(git describe)"\" >> version.h
echo -E "/*!" >> version.h
echo -E "* \\def VERSION_GIT_DATE" >> version.h
echo -E "* \\brief The date of the git commit" >> version.h
echo -E "*/" >> version.h
echo -E "#define VERSION_GIT_DATE $(git show -s --format=%cd --date=format:%Y%m%d%H%M)" >> version.h
echo -E "/*!" >> version.h
echo -E " * \\def VERSION_BUILD_DATE" >> version.h
echo -E " * \\brief The builddate" >> version.h
echo -E " */" >> version.h
echo -E "#define VERSION_BUILD_DATE $(date +%Y%m%d%H%M)" >> version.h
echo -E "" >> version.h
echo -E "#endif" >> version.h
