/*
 *  INI file parser
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


#include <Any.h>

#include <minIni.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined(__windows__)

#include <strings.h>

#endif

#define IniConfigFile_openRead( filename, file )   ((*(file) = fopen((filename),"rt")) != NULL)
#define IniConfigFile_openWrite( filename, file )  ((*(file) = fopen((filename),"wt")) != NULL)
#define IniConfigFile_close( file )                fclose(*(file))
#define IniConfigFile_read( buffer, size, file )   fgets((buffer),(size),*(file))
#define IniConfigFile_write( buffer, file )        fputs((buffer),*(file))
#define IniConfigFile_rename( source, dest )       rename((source),(dest))
#define IniConfigFile_remove( filename )           remove(filename)
#define IniConfigFile_rewind( file )               rewind(*(file))


#include <IniConfigFile.h>

#if !defined INICONFIGFILE_LINETERM
#define INICONFIGFILE_LINETERM    "\n"
#endif
#if !defined INICONFIGFILE_FILETYPE
#define INICONFIGFILE_FILETYPE    FILE*
#endif

#define INICONFIGFILE_VALID     0x26aec137
#define INICONFIGFILE_INVALID   0xb00db00f


/*
 * Public functions
 */

IniConfigFile *IniConfigFile_new( void )
{
    return ( ANY_TALLOC( IniConfigFile ) );
}


bool IniConfigFile_init( IniConfigFile *self, const char *fileName )
{
    bool retVal = false;

    ANY_REQUIRE( self );
    ANY_REQUIRE( fileName );

    self->valid = INICONFIGFILE_INVALID;
    self->fileName = Any_strdup( (char*)fileName );

    if( !self->fileName )
    {
        goto out;
    }

    retVal = true;

    self->valid = INICONFIGFILE_VALID;

    out:

    return retVal;
}


int IniConfigFile_getString( const IniConfigFile *self, const char *section, const char *key,
                             const char *defValue, char *buffer, int bufferSize )
{
    ANY_REQUIRE( self );
    ANY_REQUIRE( self->valid == INICONFIGFILE_VALID );
    ANY_REQUIRE( buffer );
    ANY_REQUIRE( bufferSize > 0 );
    ANY_REQUIRE( key );
    ANY_REQUIRE( self->fileName );

    return ini_gets( section, key, defValue, buffer, bufferSize, self->fileName );
}


long IniConfigFile_getLong( const IniConfigFile *self, const char *section, const char *key, long defValue )
{
    ANY_REQUIRE( self );
    ANY_REQUIRE( self->valid == INICONFIGFILE_VALID );

    return ini_getl( section, key, defValue, self->fileName );
}


int IniConfigFile_getInt( const IniConfigFile *self, const char *section, const char *key, int defValue )
{
    char buff[64];
    int len = 0;

    ANY_REQUIRE( self );
    ANY_REQUIRE( self->valid == INICONFIGFILE_VALID );

    len = ini_gets( section,
                    key,
                    "",
                    buff,
                    64,
                    self->fileName );

    return ( len == 0 ? defValue : atoi( buff ) );
}


double IniConfigFile_getDouble( const IniConfigFile *self, const char *section, const char *key, double defValue )
{
    char buff[64];
    int len = 0;

    ANY_REQUIRE( self );
    ANY_REQUIRE( self->valid == INICONFIGFILE_VALID );

    len = ini_gets( section,
                    key,
                    "",
                    buff,
                    64,
                    self->fileName );

    return ( len == 0 ? defValue : strtod( buff, NULL ) );
}


int IniConfigFile_getSection( const IniConfigFile *self, int idx, char *buffer, int bufferSize )
{
    ANY_REQUIRE( self );
    ANY_REQUIRE( self->valid == INICONFIGFILE_VALID );
    ANY_REQUIRE( buffer );
    ANY_REQUIRE( bufferSize > 0 );
    ANY_REQUIRE( idx >= 0 );

    return ini_getsection( idx, buffer, bufferSize, self->fileName );
}


int IniConfigFile_getKey( const IniConfigFile *self, const char *section, int idx, char *buffer, int bufferSize )
{
    ANY_REQUIRE( self );
    ANY_REQUIRE( self->valid == INICONFIGFILE_VALID );
    ANY_REQUIRE( buffer );
    ANY_REQUIRE( bufferSize > 0 );
    ANY_REQUIRE( idx >= 0 );
    ANY_REQUIRE( self->fileName );

    return ini_getkey( section, idx, buffer, bufferSize, self->fileName );
}


int IniConfigFile_putString( const IniConfigFile *self, const char *section, const char *key, const char *value )
{
    ANY_REQUIRE( self );
    ANY_REQUIRE( self->valid == INICONFIGFILE_VALID );
    ANY_REQUIRE( self->fileName );

    return ini_puts( section, key, value, self->fileName );
}

int IniConfigFile_putLong( const IniConfigFile *self, const char *section, const char *key, long value )
{
    ANY_REQUIRE( self );
    ANY_REQUIRE( self->valid == INICONFIGFILE_VALID );
    ANY_REQUIRE( self->fileName );

    return ini_putl( section, key, value, self->fileName );
}


int IniConfigFile_putInt( const IniConfigFile *self, const char *section, const char *key, int value )
{
    char str[32];

    ANY_REQUIRE( self );
    ANY_REQUIRE( self->valid == INICONFIGFILE_VALID );
    ANY_REQUIRE( self->fileName );

    Any_snprintf( str, 32, "%d", value );

    return IniConfigFile_putString( self, section, key, str );
}


int IniConfigFile_putDouble( const IniConfigFile *self, const char *section, const char *key, double value )
{
    char str[32];

    ANY_REQUIRE( self );
    ANY_REQUIRE( self->valid == INICONFIGFILE_VALID );
    ANY_REQUIRE( self->fileName );

    Any_snprintf( str, 32, "%e", value );

    return IniConfigFile_putString( self, section, key, str );
}


void IniConfigFile_removeKey( const IniConfigFile *self, const char *section, const char *key )
{
    IniConfigFile_putString( self, section, key, NULL);
}


void IniConfigFile_clear( IniConfigFile *self )
{
    ANY_REQUIRE( self );
    ANY_REQUIRE( self->valid == INICONFIGFILE_VALID );
    ANY_REQUIRE( self->fileName );

    self->valid = INICONFIGFILE_INVALID;

    ANY_FREE( (char*)self->fileName );
    self->fileName = NULL;
}


void IniConfigFile_delete( IniConfigFile *self )
{
    ANY_REQUIRE( self );

    ANY_FREE( self );
}
