/*
 *  Show value of
 *
 *  Copyright (C)
 *  Honda Research Institute Europe GmbH
 *  Carl-Legien-Str. 30
 *  63073 Offenbach/Main
 *  Germany
 *
 *  UNPUBLISHED PROPRIETARY MATERIAL.
 *  ALL RIGHTS RESERVED.
 *
 */


#include <stdio.h>
#include <stdlib.h>

#include <Any.h>

#include <minIni.h>
#include <IniConfigFile.h>


int main( void )
{

    ANY_LOG( 0, "INI_BUFFERSIZE=%d", ANY_LOG_INFO, INI_BUFFERSIZE );
    ANY_LOG( 0, "INICONFIGFILE_BUFFERSIZE=%d", ANY_LOG_INFO, INICONFIGFILE_BUFFERSIZE );

    return( EXIT_SUCCESS );
}


/* EOF */
