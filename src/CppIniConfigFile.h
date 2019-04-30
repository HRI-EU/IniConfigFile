/*
 *  INI config file parser
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


#ifndef CPPINICONFIGFILE_H
#define CPPINICONFIGFILE_H

#include <IniConfigFile.h>

#include <string>


/*!
 * \brief Define the CppIniConfigFile class ontop of the IniConfigFile
 */
class CppIniConfigFile
{
    private:
    IniConfigFile *ini;                 /**< Instance pointer */
    std::string fileName;               /**< Store the ini file name */

    public:

    /*!
     * \brief Constructor
     *
     * \param filename Ini filename
     *
     * This method construct a new CppIniConfigFile object
     */
    CppIniConfigFile(
    const std::string
    &filename )
    {
        ini = IniConfigFile_new();
        ANY_REQUIRE( ini );

        fileName = filename;

        IniConfigFile_init( ini, fileName.c_str());
    }

    /*!
     * \brief Destructor
     *
     * This method destruct the CppIniConfigFile object
     */
    ~CppIniConfigFile(
    void )
    {
        ANY_REQUIRE( ini );

        IniConfigFile_clear( ini );
        IniConfigFile_delete( ini );
    }

    /*!
     * \brief Get a double
     *
     * \param section the name of the section to search for
     * \param key the name of the entry to find the value of
     * \param defValue the default value in the event of a failed read
     *
     * This method return the corresponding key double value from the requested section. If the value doesn't
     * exist it will return the specified default value
     *
     * \code
     *  IniConfigFile myIniFile( "myConfig.ini" );
     *  double myDoubleValue = 0.0;
     *
     *  myDoubleValue = myIniFile.getDouble( "MySection", "DoubleValue", 0.0 );
     *
     *  ANY_LOG( 0, "myDoubleValue is %f", ANY_LOG_INFO, myDoubleValue );
     * \endcode
     *
     * \return The value located at Key
     */
    double get( const std::string
    &section,
    const std::string
    &key,
    double defValue = 0.0 ) const
    {
        return IniConfigFile_getDouble( ini, section.c_str(), key.c_str(), defValue );
    }

    /*!
     * \brief Get a long
     *
     * \param section the name of the section to search for
     * \param key the name of the entry to find the value of
     * \param defValue the default value in the event of a failed read
     *
     * This method return the corresponding key long value from the requested section. If the value doesn't
     * exist it will return the specified default value
     *
     * \code
     *  IniConfigFile myIniFile( "myConfig.ini" );
     *  long myLongValue = 0;
     *
     *  myLongValue = myIniFile.getLong( "MySection", "LongValue", 0 );
     *
     *  ANY_LOG( 0, "myLongValue is %ld", ANY_LOG_INFO, myLongValue );
     * \endcode
     *
     * \return The value located at Key
     */
    long get( const std::string
    &section,
    const std::string
    &key,
    long defValue = 0 ) const
    {
        return IniConfigFile_getLong( ini, section.c_str(), key.c_str(), defValue );
    }

    /*!
     * \brief Get a int
     *
     * \param section the name of the section to search for
     * \param key the name of the entry to find the value of
     * \param defValue the default value in the event of a failed read
     *
     * This method return the corresponding key int value from the requested section. If the value doesn't
     * exist it will return the specified default value
     *
     * \code
     *  IniConfigFile myIniFile( "myConfig.ini" );
     *  int myIntValue = 0;
     *
     *  myIntValue = myIniFile.getInt( "MySection", "IntValue", 0 );
     *
     *  ANY_LOG( 0, "myIntValue is %d", ANY_LOG_INFO, myIntValue );
     * \endcode
     *
     * \return The value located at Key
     */
    int get( const std::string
    &section,
    const std::string
    &key,
    int defValue = 0 ) const
    {
        return IniConfigFile_getLong( ini, section.c_str(), key.c_str(), (long)defValue );
    }

    /*!
     * \brief Get a string
     *
     * \param section the name of the section to search for
     * \param key the name of the entry to find the value of
     * \param defValue the default value in the event of a failed read
     *
     * This method return the corresponding key string value from the requested section. If the value doesn't
     * exist it will return the specified default value
     *
     * \code
     *  IniConfigFile myIniFile( "myConfig.ini" );
     *  std::string myString;
     *
     *  myString = myIniFile.getString( "MySection", "StringValue", "" );
     *
     *  ANY_LOG( 0, "myString is %s", ANY_LOG_INFO, myString.c_str() );
     * \endcode
     *
     * \return The value located at Key
     */
    std::string
    get(
    const std::string
    &section,
    const std::string
    &key,
    const std::string
    &defValue = "" ) const
    {
        char buffer[INICONFIGFILE_BUFFERSIZE];

        IniConfigFile_getString( ini, section.c_str(), key.c_str(), defValue.c_str(), buffer,
                                 INICONFIGFILE_BUFFERSIZE );

        return buffer;
    }

    /*!
     * \brief Get a requested section
     *
     * \param idx the zero-based sequence number of the section to return
     *
     * This method return in specified buffer the section name at the index idx. If any section at the given index is found than the
     * method return emtpy string. Using this function would be easy to create a section iterator
     *
     * \code
     *  IniConfigFile myIniFile( "myConfig.ini" );
     *  std::string section;
     *  int idx = 0;
     *
     *  for( idx = 0; ; idx++ )
     *  {
     *    section = ini.getSection( idx );
     *
     *   if ( section.empty() )
     *   {
     *     break;
     *   }
     *
     *   ANY_LOG( 0, "Found section [%s]", ANY_LOG_INFO, section.c_str() );
     * }
     * \endcode
     *
     * \return A string representing the found section name
     *
     * \see getKey()
     */
    std::string getSection( int idx ) const
    {
        char buffer[INICONFIGFILE_BUFFERSIZE];

        IniConfigFile_getSection( ini, idx, buffer, INICONFIGFILE_BUFFERSIZE );

        return buffer;
    }

    /*!
     * \brief Return a requested key from a section
     *
     * \param section the name of the section to browse through, or NULL to browse through the keys outside any section
     * \param idx the zero-based sequence number of the key to return
     *
     * This method return in specified buffer the key name at the index idx in the given section. If any key at the given section/index painr is found than the
     * method return empty string. Using this function would be easy to create a key iterator in a section
     *
     * \code
     *  IniConfigFile myIniFile( "myConfig.ini" );
     *  std::string key;
     *  int idx = 0;
     *
     *  for( idx = 0; ; idx++ )
     *  {
     *    key = ini.getKey( "MySection", idx );
     *
     *   if ( key.empty() )
     *   {
     *     break;
     *   }
     *
     *   ANY_LOG( 0, "Found key %s", ANY_LOG_INFO, section.c_str() );
     * }
     * \endcode
     *
     * \return A string representing the found key name
     *
     * \see getSection()
     * \see removeKey()
     */
    std::string
    getKey(
    const std::string
    &section,
    int idx ) const
    {
        char buffer[INICONFIGFILE_BUFFERSIZE];

        IniConfigFile_getKey( ini, section.c_str(), idx, buffer, INICONFIGFILE_BUFFERSIZE );

        return buffer;
    }

    /*!
     * \brief Write a long value using the specified key into a section
     *
     * \param section     the name of the section to write the value in
     * \param key         the name of the entry to write, or NULL to erase all keys in the section
     * \param value       the value to write
     *
     * This function write a long key value into a given Section.
     *
     * \code
     *  IniConfigFile myIniFile( "myConfig.ini" );
     *  bool status = false;
     *
     *  status = myIniFile.put( "MySection", "LongValue", 1234567 );
     *
     *  if ( status == false )
     *  {
     *    ANY_LOG( 0, "Unable to write a long key", ANY_LOG_ERROR );
     *  }
     * \endcode
     *
     * \return true if successful, false otherwise
     *
     * \see removeKey()
     */
    bool put( const std::string
    &section,
    const std::string
    &key,
    long value ) const
    {
        return IniConfigFile_putLong( ini, section.c_str(), key.c_str(), value );
    }

    /*!
     * \brief Write a double value using the specified key into a section
     *
     * \param section     the name of the section to write the value in
     * \param key         the name of the entry to write, or NULL to erase all keys in the section
     * \param value       the value to write
     *
     * This function write a double key value into a given Section.
     *
     * \code
     *  IniConfigFile myIniFile( "myConfig.ini" );
     *  bool status = false;
     *
     *  status = myIniFile.put( "MySection", "DoubleValue", 1234567.1234567 );
     *
     *  if ( status == false )
     *  {
     *    ANY_LOG( 0, "Unable to write a double key", ANY_LOG_ERROR );
     *  }
     * \endcode
     *
     * \return true if successful, false otherwise
     *
     * \see removeKey()
     */
    bool put( const std::string
    &section,
    const std::string
    &key,
    double value ) const
    {
        return IniConfigFile_putDouble( ini, section.c_str(), key.c_str(), value );
    }

    /*!
     * \brief Write a string value using the specified key into a section
     *
     * \param section     the name of the section to write the value in
     * \param key         the name of the entry to write, or NULL to erase all keys in the section
     * \param value       the value to write
     *
     * This function write a string key value into a given Section.
     *
     * \code
     *  IniConfigFile myIniFile( "myConfig.ini" );
     *  std::string myString = "Hello World!!!";
     *  bool status = false;
     *
     *  status = myIniFile.put( "MySection", "StringValue", myString );
     *
     *  if ( status == false )
     *  {
     *    ANY_LOG( 0, "Unable to write a string key", ANY_LOG_ERROR );
     *  }
     * \endcode
     *
     * \return true if successful, false otherwise
     *
     * \see removeKey()
     */
    bool put( const std::string
    &section,
    const std::string
    &key,
    const std::string
    &value ) const
    {
        return IniConfigFile_putString( ini, section.c_str(), key.c_str(), value.c_str());
    }

    /*!
     * \brief Remove the requested key from the given section
     *
     * \param section     the name of the section to write the string in
     * \param key         the name of the entry to write, or NULL to erase all keys in the section
     *
     * This function remove the requested key from the given section
     *
     * \code
     *  IniConfigFile myIniFile( "myConfig.ini" );
     *
     *  myIniFile.put( "MySection", "MyDoubleKey", 0.0 );
     *
     *  [ ... ]
     *
     *  myIniFile.removeKey( "MySection", "MyDoubleKey" );
     * \endcode
     *
     * \return Nothing
     *
     * \see getKey()
     */
    void removeKey( const std::string
    &section,
    const std::string
    &key ) const
    {
        return IniConfigFile_removeKey( ini, section.c_str(), key.c_str());
    }
};

#endif /* CPPINICONFIGFILE_H */

