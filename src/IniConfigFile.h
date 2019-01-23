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


/*!
 * \page IniConfigFile_About INI config file parser
 *
 * The IniConfigFile library is a wrapper for the minIni library found at URL
 * http://www.compuphase.com/minini.htm to provide full compatibility with the
 * HRI-EU coding conventions, plus some additional functionality and a C++ class
 * of support.
 *
 * IniConfigFile is a programmer's library to read and write "INI" files.
 * The library takes little resources and provides functionality
 * for reading, writing and deleting keys from an INI file.
 *
 * <h2>File syntax</h2>
 *
 * INI files have a simple syntax with name/value pairs in a plain text file.
 * The name must be unique (per section) and the value must fit
 * on a single line. INI files are commonly separated into sections -- in
 * IniConfigFile, this is optional. A section is a name between square
 * brackets, like "[Network]" in the example below:
 *
 * \code
 * [Network]
 * # This is a comment
 * hostname=My Computer
 * address=dhcp
 * dns = 192.168.1.1
 *
 * [Network1]
 * # This is a comment
 * hostname=My Computer
 * address=10.0.0.5
 * dns = 10.0.0.1
 * \endcode
 *
 * In the API and in this documentation, the "name" for a setting is denoted
 * as the key for the setting. The key and the value are separated
 * by an equal sign ("="). The library supports also the colon (":") as an
 * alternative to the equal sign for the key/value delimiter.
 *
 * Leading a trailing spaces around values or key names are removed. If you
 * need to include leading and/or trailing spaces in a value, put the
 * value between double quotes. The IniConfigFile_getString() function strips
 * off the double quotes from the returned value.
 * Function IniConfigFile_putString() adds double quotes if the value to
 * write contains trailing white space (or special characters).
 *
 * The library ignores spaces around the "=" or ":" delimiters, but it does
 * not ignore spaces between the brackets in a section name. In
 * other words, it is best not to put spaces behind the opening bracket
 * "[" or before the closing bracket "]" of a section name.
 *
 * Comments in the INI must start with a semicolon (";") or a hash character
 * ("#"), and run to the end of the line. A comment can be a line
 * of its own, or it may follow a key/value pair (the "#" character and
 * trailing comments are extensions of this library).
 *
 * <h2>Multi-tasking / Multi-threading</h2>
 *
 * The library does not have any global variables and it does not use any
 * dynamically allocated memory. Yet, the library should not be
 * considered "thread-safe" or re-entrant, because it implicitly uses a
 * particular shared resource: the file system.
 *
 * Multiple tasks/threads reading from an INI file do not pose a problem.
 * However, when one task is writing to an INI file, no other tasks
 * should access this INI file --neither for reading, nor for writing.
 * It might be easier, in the implementation, to serialize all accesses of
 * the INI file.
 *
 * The first advise in protecting resources from concurrent access in a
 * multi-tasking/threading environment is to avoid sharing resources
 * between tasks. If only a single task uses a resource, no semaphore
 * protection is necessary and no priority inversion or deadlock problems
 * can occur. This advise also applies to the library. If possible, make a
 * single task the "owner" of the INI file and create a client/server
 * architecture for other tasks to query and adjust settings.
 *
 * If the INI file must be shared between tasks (and at least one of the
 * tasks writes to the INI file), you need to write wrappers around the
 * functions of the minIni library that block on a mutex or binary semaphore.
 *
 * \note The library uses temporary files when writing/removing keys.
 *       All the temporary filenames start with a tilde (~).
 */

#ifndef INICONFIGFILE_H
#define INICONFIGFILE_H

/*!
 * \brief Defines the max value stored by a key
 */
#define INICONFIGFILE_BUFFERSIZE  4096

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * \brief IniConfigFile definition
 */
typedef struct IniConfigFile
{
    unsigned long valid;   /**< Object validity */
    const char *fileName;  /**< Pointer to the ini filename */
}
IniConfigFile;

/*!
 * \brief Allocate a new IniConfigFile instance
 *
 * This function allocate a new IniConfigFile instance
 *
 * \code
 *   IniConfigFile *myIniFile = NULL;
 *
 *   myIniFile = IniConfigFile_new();
 *   ANY_REQUIRE( myIniFile );
 * \endcode
 *
 * \return A new IniConfigFile instance, NULL on error
 *
 * \see IniConfigFile_init()
 * \see IniConfigFile_clear()
 * \see IniConfigFile_delete()
 */
IniConfigFile *IniConfigFile_new( void );

/*!
 * \brief Initialize a new IniConfigFile instance
 *
 * \param self        Pointer to the IniConfigFile
 * \param fileName    Pointer to a filename
 *
 * This function initialize a new IniConfigFile instance
 *
 * \code
 *  IniConfigFile *myIniFile = NULL;
 *
 *  myIniFile = IniConfigFile_new();
 *  ANY_REQUIRE_MSG( myIniFile, "Unable to create a new IniConfigFile" );
 *
 *  IniConfigFile_init( myIniFile, "myConfig.ini" );
 * \endcode
 *
 * \return Returns true on success, false otherwise
 *
 * \see IniConfigFile_new()
 * \see IniConfigFile_clear()
 * \see IniConfigFile_delete()
 */
bool IniConfigFile_init( IniConfigFile *self, const char *fileName );


/*!
 * \brief Get a int
 *
 * \param self        Pointer to the IniConfigFile
 * \param section     the name of the section to search for
 * \param key         the name of the entry to find the value of
 * \param defValue    the default value in the event of a failed read
 *
 * This function return the corresponding key int value from the requested section. If the value doesn't
 * exist it will return the specified default value
 *
 * \code
 *  IniConfigFile *myIniFile = NULL;
 *  int myIntValue = 0L;
 *
 *  myIniFile = IniConfigFile_new();
 *  ANY_REQUIRE_MSG( myIniFile, "Unable to create a new IniConfigFile" );
 *
 *  IniConfigFile_init( myIniFile, "myConfig.ini" );
 *
 *  myIntValue = IniConfigFile_getInt( myIniFile, "MySection", "IntValue", 0 );
 *
 *  ANY_LOG( 0, "myIntValue is %d", ANY_LOG_INFO, myIntValue );
 * \endcode
 *
 * \return The value located at Key
 *
 * \see IniConfigFile_removeKey()
 * \see IniConfigFile_getString()
 * \see IniConfigFile_getLong()
 * \see IniConfigFile_getDouble()
 */
int IniConfigFile_getInt( const IniConfigFile *self, const char *section, const char *key, int defValue );

/*!
 * \brief Get a long
 *
 * \param self        Pointer to the IniConfigFile
 * \param section     the name of the section to search for
 * \param key         the name of the entry to find the value of
 * \param defValue    the default value in the event of a failed read
 *
 * This function return the corresponding key long value from the requested section. If the value doesn't
 * exist it will return the specified default value
 *
 * \code
 *  IniConfigFile *myIniFile = NULL;
 *  long myLongValue = 0L;
 *
 *  myIniFile = IniConfigFile_new();
 *  ANY_REQUIRE_MSG( myIniFile, "Unable to create a new IniConfigFile" );
 *
 *  IniConfigFile_init( myIniFile, "myConfig.ini" );
 *
 *  myLongValue = IniConfigFile_getLong( myIniFile, "MySection", "LongValue", 0L );
 *
 *  ANY_LOG( 0, "myLongValue is %ld", ANY_LOG_INFO, myLongValue );
 * \endcode
 *
 * \return The value located at Key
 *
 * \see IniConfigFile_removeKey()
 * \see IniConfigFile_getString()
 * \see IniConfigFile_getInt()
 * \see IniConfigFile_getDouble()
 */
long IniConfigFile_getLong( const IniConfigFile *self, const char *section, const char *key, long defValue );

/*!
 * \brief Get a double
 *
 * \param self        Pointer to the IniConfigFile
 * \param section     the name of the section to search for
 * \param key         the name of the entry to find the value of
 * \param defValue    the default value in the event of a failed read
 *
 * This function return the corresponding key double value from the requested section. If the value doesn't
 * exist it will return the specified default value
 *
 * \code
 *  IniConfigFile *myIniFile = NULL;
 *  double myDoubleValue = 0.0;
 *
 *  myIniFile = IniConfigFile_new();
 *  ANY_REQUIRE_MSG( myIniFile, "Unable to create a new IniConfigFile" );
 *
 *  IniConfigFile_init( myIniFile, "myConfig.ini" );
 *
 *  myDoubleValue = IniConfigFile_getDouble( myIniFile, "MySection", "DoubleValue", 0.0 );
 *
 *  ANY_LOG( 0, "myDoubleValue is %f", ANY_LOG_INFO, myDoubleValue );
 * \endcode
 *
 * \return The value located at Key
 *
 * \see IniConfigFile_removeKey()
 * \see IniConfigFile_getString()
 * \see IniConfigFile_getInt()
 * \see IniConfigFile_getLong()
 */
double IniConfigFile_getDouble( const IniConfigFile *self, const char *section, const char *key, double defValue );

/*!
 * \brief Get a string
 *
 * \param self        Pointer to the IniConfigFile
 * \param section     the name of the section to search for
 * \param key         the name of the entry to find the value of
 * \param defValue    default string in the event of a failed read
 * \param buffer      a pointer to the buffer to copy into
 * \param bufferSize  the maximum number of characters to copy
 *
 * This function return the corresponding key string value from the requested section. If the value doesn't
 * exist it will return the specified default value
 *
 * \code
 *  IniConfigFile *myIniFile = NULL;
 *  char myString[128];
 *  int myStringSize = 0;
 *
 *  myIniFile = IniConfigFile_new();
 *  ANY_REQUIRE_MSG( myIniFile, "Unable to create a new IniConfigFile" );
 *
 *  IniConfigFile_init( myIniFile, "myConfig.ini" );
 *
 *  myStringSize = IniConfigFile_getString( myIniFile, "MySection", "StringValue", myString, 128 );
 *
 *  if ( myStringSize > 0 )
 *  {
 *    ANY_LOG( 0, "myString is '%s'", ANY_LOG_INFO, myString );
 *  }
 * \endcode
 *
 * \return The number of characters copied into the supplied buffer
 *
 * \see IniConfigFile_removeKey()
 * \see IniConfigFile_getDouble()
 * \see IniConfigFile_getInt()
 * \see IniConfigFile_getLong()
 */
int IniConfigFile_getString( const IniConfigFile *self, const char *section, const char *key,
                             const char *defValue, char *buffer, int bufferSize );

/*!
 * \brief Write a long value using the specified key into a section
 *
 * \param self        Pointer to the IniConfigFile
 * \param section     the name of the section to write the value in
 * \param key         the name of the entry to write, or NULL to erase all keys in the section
 * \param value       the value to write
 *
 * This function write a long key value into a given Section.
 *
 * \code
 *  IniConfigFile *myIniFile = NULL;
 *  int status = 0;
 *
 *  myIniFile = IniConfigFile_new();
 *  ANY_REQUIRE_MSG( myIniFile, "Unable to create a new IniConfigFile" );
 *
 *  IniConfigFile_init( myIniFile, "myConfig.ini" );
 *
 *  status = IniConfigFile_putInt( myIniFile, "MySection", "IntValue", 1234567 );
 *
 *  if ( !status )
 *  {
 *    ANY_LOG( 0, "Unable to write a int key", ANY_LOG_ERROR );
 *  }
 * \endcode
 *
 * \return 1 if successful, otherwise 0
 *
 * \see IniConfigFile_removeKey()
 * \see IniConfigFile_putString()
 * \see IniConfigFile_putLong()
 * \see IniConfigFile_putDouble()
 */
int IniConfigFile_putInt( const IniConfigFile *self, const char *section, const char *key, int value );

/*!
 * \brief Write a long value using the specified key into a section
 *
 * \param self        Pointer to the IniConfigFile
 * \param section     the name of the section to write the value in
 * \param key         the name of the entry to write, or NULL to erase all keys in the section
 * \param value       the value to write
 *
 * This function write a long key value into a given Section.
 *
 * \code
 *  IniConfigFile *myIniFile = NULL;
 *  int status = 0;
 *
 *  myIniFile = IniConfigFile_new();
 *  ANY_REQUIRE_MSG( myIniFile, "Unable to create a new IniConfigFile" );
 *
 *  IniConfigFile_init( myIniFile, "myConfig.ini" );
 *
 *  status = IniConfigFile_putLong( myIniFile, "MySection", "LongValue", 1234567 );
 *
 *  if ( !status )
 *  {
 *    ANY_LOG( 0, "Unable to write a long key", ANY_LOG_ERROR );
 *  }
 * \endcode
 *
 * \return 1 if successful, otherwise 0
 *
 * \see IniConfigFile_removeKey()
 * \see IniConfigFile_putString()
 * \see IniConfigFile_putInt()
 * \see IniConfigFile_putDouble()
 */
int IniConfigFile_putLong( const IniConfigFile *self, const char *section, const char *key, long value );

/*!
 * \brief Write a double value using the specified key into a section
 *
 * \param self        Pointer to the IniConfigFile
 * \param section     the name of the section to write the value in
 * \param key         the name of the entry to write, or NULL to erase all keys in the section
 * \param value       the value to write
 *
 * This function write a long key value into a given Section.
 *
 * \code
 *  IniConfigFile *myIniFile = NULL;
 *  int status = 0;
 *
 *  myIniFile = IniConfigFile_new();
 *  ANY_REQUIRE_MSG( myIniFile, "Unable to create a new IniConfigFile" );
 *
 *  IniConfigFile_init( myIniFile, "myConfig.ini" );
 *
 *  status = IniConfigFile_putDouble( myIniFile, "MySection", "DoubleValue", 1234567.123456 );
 *
 *  if ( !status )
 *  {
 *    ANY_LOG( 0, "Unable to write a double key", ANY_LOG_ERROR );
 *  }
 * \endcode
 *
 * \return 1 if successful, otherwise 0
 *
 * \see IniConfigFile_removeKey()
 * \see IniConfigFile_putString()
 * \see IniConfigFile_putInt()
 * \see IniConfigFile_putLong()
 */
int IniConfigFile_putDouble( const IniConfigFile *self, const char *section, const char *key, double value );

/*!
 * \brief Write a string value using the specified key into a section
 *
 * \param self        Pointer to the IniConfigFile
 * \param section     the name of the section to write the string in
 * \param key         the name of the entry to write, or NULL to erase all keys in the section
 * \param value       a pointer to the buffer the string, or NULL to erase the key
 *
 * This function write a long key value into a given Section.
 *
 * \code
 *  IniConfigFile *myIniFile = NULL;
 *  int status = 0;
 *
 *  myIniFile = IniConfigFile_new();
 *  ANY_REQUIRE_MSG( myIniFile, "Unable to create a new IniConfigFile" );
 *
 *  IniConfigFile_init( myIniFile, "myConfig.ini" );
 *
 *  status = IniConfigFile_putLong( myIniFile, "MySection", "StringValue", myStringValue );
 *
 *  if ( !status )
 *  {
 *    ANY_LOG( 0, "Unable to write a string key", ANY_LOG_ERROR );
 *  }
 * \endcode
 *
 * \return 1 if successful, otherwise 0
 *
 * \see IniConfigFile_removeKey()
 * \see IniConfigFile_putDouble()
 * \see IniConfigFile_putInt()
 * \see IniConfigFile_putLong()
 */
int IniConfigFile_putString( const IniConfigFile *self, const char *section, const char *key, const char *value );

/*!
 * \brief Remove the requested key from the given section
 *
 * \param self        Pointer to the IniConfigFile
 * \param section     the name of the section to write the string in
 * \param key         the name of the entry to write, or NULL to erase all keys in the section
 *
 * This function remove the requested key from the given section
 *
 * \code
 *  IniConfigFile_putDouble( myIniFile, "MySection", "MyDoubleKey", 0.0 );
 *
 *  [ ... ]
 *
 *  IniConfigFile_removeKey( myIniFile, "MySection", "MyDoubleKey" );
 * \endcode
 *
 * \return Nothing
 *
 * \see IniConfigFile_getKey()
*/
void IniConfigFile_removeKey( const IniConfigFile *self, const char *section, const char *key );

/*!
 * \brief Get a requested section
 *
 * \param self        Pointer to the IniConfigFile
 * \param idx         the zero-based sequence number of the section to return
 * \param buffer      a pointer to the buffer to copy into
 * \param bufferSize  the maximum number of characters to copy
 *
 * This function return in specified buffer the section name at the index idx. If any section at the given index is found than the
 * function return 0. Using this function would be easy to create a section iterator
 *
 * \code
 *  IniConfigFile *myIniFile = NULL;
 *  char section[64];
 *  int idx = 0;
 *
 *  myIniFile = IniConfigFile_new();
 *  ANY_REQUIRE_MSG( myIniFile, "Unable to create a new IniConfigFile" );
 *
 *  IniConfigFile_init( myIniFile, "myConfig.ini" );
 *
 *  for ( idx = 0; IniConfigFile_getSection( myIniFile, idx, section, 64 ); idx++)
 *   {
 *     ANY_LOG( 0, "Found section [%s]", ANY_LOG_INFO, section );
 *   }
 * \endcode
 *
 * \return The number of characters copied into the supplied buffer
 *
 * \see IniConfigFile_getKey()
 */
int IniConfigFile_getSection( const IniConfigFile *self, int idx, char *buffer, int bufferSize );

/*!
 * \brief Return a requested key from a section
 *
 * \param self        Pointer to the IniConfigFile
 * \param section     the name of the section to browse through, or NULL to browse through the keys outside any section
 * \param idx         the zero-based sequence number of the key to return
 * \param buffer      a pointer to the buffer to copy into
 * \param bufferSize  the maximum number of characters to copy
 *
 * This function return in specified buffer the key name at the index idx in the given section. If any key at the given section/index painr is found than the
 * function return 0. Using this function would be easy to create a key iterator in a section
 *
 * \code
 *  IniConfigFile *myIniFile = NULL;
 *  char key[64];
 *  int idx = 0;
 *
 *  myIniFile = IniConfigFile_new();
 *  ANY_REQUIRE_MSG( myIniFile, "Unable to create a new IniConfigFile" );
 *
 *  IniConfigFile_init( myIniFile, "myConfig.ini" );
 *
 *  for ( idx = 0; IniConfigFile_getKey( myIniFile, "MySection", idx, key, 64 ); idx++)
 *   {
 *     ANY_LOG( 0, "Found key [%s]", ANY_LOG_INFO, key );
 *   }
 * \endcode
 *
 * \return The number of characters copied into the supplied buffer
 *
 * \see IniConfigFile_getSection()
 * \see IniConfigFile_removeKey()
 */
int IniConfigFile_getKey( const IniConfigFile *self, const char *section, int idx, char *buffer, int bufferSize );

/*!
 * \brief Clear a IniConfigFile instance
 *
 * \param self Pointer to the IniConfigFile
 *
 * This function clear a IniConfigFile instance
 *
 * \return Nothing
 *
 * \see IniConfigFile_new();
 * \see IniConfigFile_init()
 * \see IniConfigFile_delete()
 */
void IniConfigFile_clear( IniConfigFile *self );

/*!
 * \brief Delete a IniConfigFile instance
 *
 * \param self Pointer to the IniConfigFile
 *
 * This function delete a IniConfigFile instance
 *
 * \return Nothing
 *
 * \see IniConfigFile_new();
 * \see IniConfigFile_init()
 * \see IniConfigFile_clear()
 */
void IniConfigFile_delete( IniConfigFile *self );


#if defined(__cplusplus)
}
#endif

#endif /* INICONFIGFILE_H */
