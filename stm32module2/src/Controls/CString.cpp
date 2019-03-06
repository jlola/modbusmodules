#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#ifdef _AVR_
#include <avr/pgmspace.h>
#include <stdarg.h>
#include "cppdef.h"
#endif
#include "CString.h"

//#include "snprintf.h"

CString::CString()
{
	this->bufferLength = 0;
	this->strLength = 0;
	this->buffer = NULL;
}

CString::CString( double val )
{
	this->bufferLength = 0;
	this->strLength = 0;
	this->buffer = NULL;
	Add(val);
}

CString::CString( uint8_t val )
{
	this->bufferLength = 0;
	this->strLength = 0;
	this->buffer = NULL;
	Add(val);
}

const char *CString::c_str( ) const       // Return C-style CString
{
	if (strLength==0) return NULL;
	char* rbuffer = new char[bufferLength];
	strncpy(rbuffer,buffer,strLength);
	rbuffer[strLength] = '\0';
	return rbuffer;
}

CString CString::ToString(time_t ptime)
{
	struct tm * timeinfo = localtime ( &ptime );

	CString result = CString::Format("%02d:%02d:%02d %02d.%02d.%04d\0",
			(int)timeinfo->tm_hour,(int)timeinfo->tm_min,(int)timeinfo->tm_sec,
			(int)timeinfo->tm_mday,(int)timeinfo->tm_mon,(int)timeinfo->tm_year+1900
			);
	return result;
}

bool CString::Contains(const char* str)
{
	return strstr(buffer,str)!=NULL;
}

CString CString::Substring(size_t startIndex,size_t size)
{
	CString result;
	if (size==0) size = length()- startIndex;
	char sbuffer[size+1];
	if (startIndex+size<=length())
	{
		strncpy(sbuffer,buffer+startIndex,size);
		sbuffer[size] = '\0';
		result.Add(sbuffer);
	}
	return result;
}

int CString::IndexOf(const char* str)
{
	char* found = strstr(buffer,str);
	if (found!=NULL)
	{
		return found - buffer;
	}
	return -1;
}

int16_t CString::find(char znak, uint16_t start )
{
	for(uint16_t i=start;i<length();i++)
	{
		if (buffer[i]==znak) return i;
	}
	return -1;
}

CString CString::operator+( double val )
{
	CString result(*this);
	result.Add(val);
	return result;
}

CString::CString( char * cCString, uint16_t len )
{
    if( cCString == NULL )
    {
		cCString = (char*)"";
		len = 0;
	}
    strLength = len + 1;
    bufferLength = strLength + 1;
    buffer = new char[ bufferLength ];
	if (buffer!=NULL)
    {
		strncpy( buffer, cCString,len );
		buffer[strLength] = '\0';
	}
}

CString::CString( const char * cCString )
{
    if( cCString == NULL )
        cCString = "";
    strLength = strlen( cCString );
    if (strLength>0)
    {
		bufferLength = strLength + 1;
		//buffer = new char[ bufferLength ];
		buffer = (char*)malloc(bufferLength);
		if (buffer!=NULL)
		{
			strncpy( buffer, cCString, strLength );
			buffer[strLength] = '\0';
		}
    }
    else
    {
    	buffer = NULL;
    	bufferLength = 0;
    }
}

//vytvori seznam stringu dle oddelovace delimiter
Vector<CString*>* CString::Split(const char delimiter)
{
	Vector<CString*>* result = new Vector<CString*>();
		char *pstart;
		size_t len = 0;
		pstart = buffer;
		char delim = delimiter;
		for(size_t i=0;i<length();i++)
		{
			len++;
			if (buffer[i]==delim || i == length()-1)
			{
				if (i == length()-1) result->Add(new CString(pstart,len));
				else result->Add(new CString(pstart,len-1));
				len = 0;
				if (i+1 < length())
				{
					pstart = &buffer[i + 1];
				}
			}
		}
	return result;
}

CString::CString( const CString & str )
{
	if (this == &str)
	{
		return;
	}
    strLength = str.length( );

    if (str.length()>0)
    {
    	bufferLength = strLength + 1;
		//buffer = new char[ bufferLength ];
    	buffer = (char*)malloc(bufferLength);
		if (buffer!=NULL)
		{
			strncpy( buffer,str.buffer,strLength );
			buffer[bufferLength-1] = '\0';
		}
    }
	else
	{
		buffer = NULL;
		strLength = 0;
		bufferLength = 0;
	}
}

#ifdef _AVR_

CString & CString::Add_P(PGM_P pgstr)
{
	unsigned char mlen = strlen_P(pgstr);
	char *mbuff = new char[mlen];
	if (mbuff!=NULL)
		strcpy_P(mbuff,pgstr);

	*this += mbuff;
	delete[] mbuff;
	return *this;
}

#endif

CString & CString::Add(const char *str)
{
	CString result(str);
	*this += result;
	return *this;
}

CString & CString::Add(char val)
{
	char str[] = {val,'\0'};
	Add(str);

//	uint16_t oldlength = length( );
//	uint16_t newLength = oldlength + 1;
//	if (newLength > (uint16_t)CString::MAX_LENGTH) return *this;
//
//	bufferLength = newLength + 1;
//	char *oldBuffer = buffer;
//	buffer = new char[ bufferLength ];
//	if (buffer!=NULL)
//	{
//		if (buffer!=NULL)
//		{
//			strncpy( buffer, oldBuffer, oldlength );
//			delete [ ] oldBuffer;
//			oldBuffer = NULL;
//		}
//
//		buffer[oldlength] = val;
//		strLength = newLength;
//		buffer[bufferLength-1] = '\0';
//	}


	return *this;
}

// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
int i=0, j=len-1, temp;
while (i<j)
{
    temp = str[i];
    str[i] = str[j];
    str[j] = temp;
    i++; j--;
}
}

// Converts a given integer x to string str[].  d is the number
// of digits required in output. If d is more than the number
// of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
int i = 0;
while (x)
{
    str[i++] = (x%10) + '0';
    x = x/10;
}

// If number of digits required is more, then
// add 0s at the beginning
while (i < d)
    str[i++] = '0';

reverse(str, i);
str[i] = '\0';
return i;
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
// Extract integer part
int ipart = (int)n;

// Extract floating part
float fpart = n - (float)ipart;

// convert integer part to string
int i = intToStr(ipart, res, 1);

// check for display option after point
if (afterpoint != 0)
{
    res[i] = '.';  // add dot

    // Get the value of fraction part upto given no.
    // of points after dot. The third parameter is needed
    // to handle cases like 233.007
    fpart = fpart * pow( 10, afterpoint);
    int ipart = floor( fpart + 0.5 );
    intToStr(ipart, res + i + 1, afterpoint);
}
}

CString & CString::Add(double val)
{
	char mtemp[6];
	//dtostrf(val,2,1,mtemp);
	ftoa(val,mtemp,1);
	//snprintf(mtemp,6,"%1.1f",val);
	*this += mtemp;
	return *this;
}

CString & CString::Add(float val,int afterpoint)
{
	char mtemp[20];
	char strafterpoint[10];
	ftoa(val,mtemp,afterpoint);

	//dtostrf(val,2,1,mtemp);
	//snprintf(mtemp,6,"%1.2",val);
	*this += mtemp;
	return *this;
}

CString & CString::Add(int8_t val)
{
	char mtemp[100];
	//itoa(val,mtemp,10);
	snprintf(mtemp,10,"%d",val);
	*this += mtemp;
	return *this;
}

CString & CString::Add(unsigned long int val)
{
	char mtemp[100];
	//itoa(val,mtemp,10);
	snprintf(mtemp,sizeof(mtemp),"%d",val);
	*this += mtemp;
	return *this;
}

CString & CString::Add(uint8_t val)
{
	char mtemp[6];
	//itoa(val,mtemp,10);
	snprintf(mtemp,6,"%d",val);
	*this += mtemp;
	return *this;
}



CString & CString::Add(const CString & val)
{
	*this += val;
	return *this;
}

void CString::RemoveAt(size_t pos,size_t pLength)
{
	if (pos<length())
	{
		size_t newlen = length()-pLength;
		size_t newbufferlen = newlen+1;
		char* newbuffer = new char[newbufferlen];
		strncpy(newbuffer,buffer,newlen);
		newbuffer[newbufferlen-1] = '\0';
		delete(buffer);
		buffer = newbuffer;
		newbuffer=NULL;
		strLength = newlen;
		bufferLength = newbufferlen;
	}
}

void CString::SetAt(size_t pos, char val)
{
	if (pos<length())
	{
		buffer[pos] = val;
	}
}

void CString::CopyTo(char *targetBuffer,uint16_t len)
{
	strncpy(targetBuffer,buffer,len);
}

void CString::PadRight(uint16_t totalLength)
{
	if (strLength > totalLength) return;
	int8_t padlen = totalLength - strLength;
	if (padlen > 0)
	{
		char *padbuff = new char[padlen+1];
		memset(padbuff,' ',padlen);
		padbuff[padlen] = 0; 

		*this += padbuff;

		delete [] padbuff;
	}
}



//CString & CString::Format(const char *format, ... )
//{
//	if (buffer!=NULL)
//	{
//		delete [] buffer;
//		buffer = NULL;
//	}
//
//	strLength = 0;
//	bufferLength = 0;
//
//	char mbuffer[1000];
//
//	va_list arg_list;
//	va_start(arg_list,format);
//
//	vsnprintf(mbuffer,sizeof(mbuffer), format, (va_list)arg_list);
//
//	va_end(arg_list);
//
//	strLength = strlen(mbuffer);
//	bufferLength = strlen(mbuffer)+sizeof(char);
//
//	buffer = new char[bufferLength];
//
//	strncpy(buffer,mbuffer,strLength);
//	buffer[bufferLength-1] = '\0';
//
//	return *this;
//}

CString CString::Format(const char *format, ... )
{
	char mbuffer[100];

	va_list arg_list;
	va_start(arg_list,format);

	vsnprintf(mbuffer,sizeof(mbuffer), format, (va_list)arg_list);

	va_end(arg_list);

	CString result(mbuffer);

	return result;
}


#ifdef _AVR_

CString & CString::Format_P(PGM_P reg, ... )
{
	delete [] buffer;
	buffer = NULL;
	
	strLength = 0;
	bufferLength = 0;

	uint8_t mstrLength = 3 * strlen_P( reg );
	if (mstrLength > (uint8_t)CString::MAX_LENGTH) mstrLength = (uint8_t)CString::MAX_LENGTH;
    uint8_t mbufferLength = mstrLength + 1;

	char *mbuffer = new char[mbufferLength];
	if (mbuffer!=NULL)	
	{
		va_list arg_list;
		va_start(arg_list,reg);      		      
		vsnprintf_P(mbuffer, mbufferLength, reg, arg_list);
		va_end(arg_list);

		*this = mbuffer;

		delete [] mbuffer;		
	}	

	return *this;
}

#endif

const CString & CString::operator=( const CString & rhs )
{
    if( this != &rhs )
    {
        //if( bufferLength < rhs.length( ))
        //{
       delete [ ] buffer;
       //free(buffer);
       bufferLength = rhs.length( )+1;
       buffer = new char[ bufferLength ];
       //buffer = (char*)malloc(bufferLength);
       //}
        strLength = rhs.length( );
		if (buffer!=NULL)
        {
			strncpy( buffer, rhs.buffer, rhs.length() );
			buffer[bufferLength-1] = 0;
        }

    }
    return *this;
}

CString CString::operator+( const CString & rhs )
{
	CString result(*this);
    result += rhs;
    return result;
}


CString CString::operator+( const char *cstr )
{
	CString result(*this);
	result.Add(cstr);
	return result;
}


const CString & CString::operator+=( const CString & rhs )
{
    if( this == &rhs )
    {
        CString copy( rhs );
        return *this += copy;
    }

    uint16_t oldlength = length( );
    uint16_t newLength = oldlength + rhs.length( );
	if (newLength > (uint16_t)CString::MAX_LENGTH) return *this;
    
	bufferLength = newLength + 1;
    char *oldBuffer = buffer;
    buffer = new char[ bufferLength ];
    //buffer = (char*)malloc(bufferLength);
    if (buffer!=NULL)
	{
    	if (buffer!=NULL)
    	{
    		strncpy( buffer, oldBuffer, oldlength );
    		//delete [ ] oldBuffer;
    		free(oldBuffer);
    		oldBuffer = NULL;
    	}

    	strncpy( buffer + length( ), rhs.buffer, rhs.length() );
    	strLength = newLength;
    	buffer[bufferLength-1] = 0;
	}
	
    return *this;
}

void CString::Clear()
{
	if (buffer!=NULL)
		delete(buffer);
	buffer = NULL;
	bufferLength = 0;
	strLength = 0;
}

char CString::operator[ ]( uint16_t k ) const
{
    if( k < 0 || k >= strLength )
    	return 0;//throw CStringIndexOutOfBounds( );
    return buffer[ k ];
}

bool operator==( const CString & lhs, const CString & rhs )
{
    return strcmp( lhs.buffer, rhs.buffer) == 0;
}

bool operator!=( const CString & lhs, const CString & rhs )
{
    return strcmp( lhs.buffer, rhs.buffer ) != 0;
}

bool operator<( const CString & lhs, const CString & rhs )
{
    return strcmp( lhs.buffer, rhs.buffer ) < 0;
}

bool operator<=( const CString & lhs, const CString & rhs )
{
    return strcmp( lhs.buffer, rhs.buffer ) <= 0;
}

bool operator>( const CString & lhs, const CString & rhs )
{
    return strcmp( lhs.buffer, rhs.buffer ) > 0;
}

bool operator>=( const CString & lhs, const CString & rhs )
{
    return strcmp( lhs.buffer, rhs.buffer ) >= 0;
}

