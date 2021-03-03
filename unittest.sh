#!/bin/bash
#
#  launches the unit testing
#
#  Copyright (C)
#  Honda Research Institute Europe GmbH
#  Carl-Legien-Str. 30
#  63073 Offenbach/Main
#  Germany
#
#  UNPUBLISHED PROPRIETARY MATERIAL.
#  ALL RIGHTS RESERVED.
#
#


#----------------------------------------------------------------------------
# Setup
#----------------------------------------------------------------------------


source ${TOOLBOSCORE_ROOT}/include/Unittest.bash


if [[ ! -e ./install/BashSrc ]]
then
    BST.py --shellfiles

    source ./install/BashSrc
fi


CWD=$(pwd)

export LD_LIBRARY_PATH=${CWD}/lib/${MAKEFILE_PLATFORM}:${LD_LIBRARY_PATH}


#----------------------------------------------------------------------------
# Unittests
#----------------------------------------------------------------------------


cd ${CWD}/test && runTest ./${MAKEFILE_PLATFORM}/ReadFile


# EOF
