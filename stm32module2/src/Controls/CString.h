#ifndef MY_CString_H_
#define MY_CString_H_

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "vector.hpp"
#include "time.h"
#include "sys/time.h"
/*
 * The next line is used because Codewarrior has a conflict with
 * the STL CString. Make sure to put the #include of this file
 * AFTER all the system includes.
 */
//#define CString CString

class StringIndexOutOfBounds { };

class CString
{
  public:
	CString();
    CString( const char *cstring );               // Constructor
    CString( char * cCString, uint16_t len );
    CString( const CString & str );                     // Copy constructor
    ~CString( )                                        // Destructor
    {
    	if (buffer!=NULL)
    	{
    		delete(buffer);
    		//free(buffer);
    		buffer = NULL;
    	}

      }
    CString( double val );
    CString( uint8_t val );

    void SetAt(size_t pos, char val);

    const CString & operator= ( const CString & rhs );  // Copy
    const CString & operator+=( const CString & rhs );  // Append

	CString operator+( const CString & rhs );	//Append object
	CString operator+( const char *cstr );		//Append cstring
	CString operator+( double val );

	int IndexOf(const char* str);

	CString Substring(size_t startIndex,size_t size);

	int16_t find(char znak, uint16_t start );
	bool Contains(const char* str);

    const char *c_str( ) const;        // Return C-style CString
    size_t length( ) const               // Return CString length
      { return strLength; }
    void Clear();
    char   operator[]( uint16_t k ) const; // Accessor operator[]
    //char & operator[]( uint16_t k );       // Mutator  operator[]

    enum { MAX_LENGTH = 2000 };  // Maximum length for input CString

#ifdef _AVR_
	CString & Add_P(PGM_P pgstr);	//append from program memory
	CString & Format_P(PGM_P reg, ... );
#endif
	CString & Add(float val,int afterpoint);		//append converted double
	CString & Add(double val);
	CString & Add(const char *str);
	CString & Add(unsigned long int val);
	CString & Add(const CString & val);
	CString & Add(uint8_t val);
	CString & Add(int8_t val);
	CString & Add(char val);
	
	static CString Format(const char *reg, ... );
	void RemoveAt(size_t pos,size_t pLength);

	void CopyTo(char *targetBuffer,uint16_t len); //copy to target buffer

	//vytvori seznam stringu dle oddelovace delimiter
	Vector<CString*>* Split(const char delimiter);

	void PadRight(uint16_t totalLenght);

	static CString ToString(time_t ptime);

	friend bool operator==( const CString & lhs, const CString & rhs );    // Compare ==
	friend bool operator!=( const CString & lhs, const CString & rhs );    // Compare !=
	friend bool operator< ( const CString & lhs, const CString & rhs );    // Compare <
	friend bool operator<=( const CString & lhs, const CString & rhs );    // Compare <=
	friend bool operator> ( const CString & lhs, const CString & rhs );    // Compare >
	friend bool operator>=( const CString & lhs, const CString & rhs );    // Compare >=


  private:
    char *buffer;                  // storage for characters
    size_t strLength;                 // length of CString (# of characters)
    size_t bufferLength;              // capacity of buffer
};

//ostream & operator<<( ostream & out, const CString & str );    // Output
//istream & operator>>( istream & in, CString & str );           // Input
//istream & getline( istream & in, CString & str );              // Read line

bool operator==( const CString & lhs, const CString & rhs );    // Compare ==
bool operator!=( const CString & lhs, const CString & rhs );    // Compare !=
bool operator< ( const CString & lhs, const CString & rhs );    // Compare <
bool operator<=( const CString & lhs, const CString & rhs );    // Compare <=
bool operator> ( const CString & lhs, const CString & rhs );    // Compare >
bool operator>=( const CString & lhs, const CString & rhs );    // Compare >=

#endif
