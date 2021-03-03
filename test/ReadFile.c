/*
 *  Test program reading an existing INI file
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

#include <IniConfigFile.h>


int main( void )
{
    IniConfigFile *ini = (IniConfigFile*)NULL;
    ini = IniConfigFile_new();

    IniConfigFile_init( ini, "Example.ini" );

    int foo = IniConfigFile_getInt( ini, "Example", "foo", -1 );

    IniConfigFile_clear( ini );
    IniConfigFile_delete( ini );

    ANY_TRACE( 0, "%d", foo );

    int status = foo == 42 ? EXIT_SUCCESS : EXIT_FAILURE;

    return( status );
}


/* EOF */
