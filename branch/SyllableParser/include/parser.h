#ifndef _SYLLABLE_PARSER_H_
#define _SYLLABLE_PARSER_H_

#include <string.h>
#include <list>

// Copyright (c) 2012-2014 Jakob Riedle (DuffsDevice)
// 
// Version 1.0
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

class SyllableParser
{
	private:
		
		typedef const char* Literal;
		
		// Alphabet
		static constexpr Literal vowels			= "aeiouyöäü";
		static constexpr Literal consonants		= "bcdfghjklmnpqrstvwxzß";
		
		// Vocals that are spoken as one syllable
		static constexpr Literal diphthongs		= "äu,ue,oe,io,ie,eu,ei,au,ai,ae";
		// consonants that don't exist alone
		static constexpr Literal indivisibles	= "th,rh,qu,ph,ngs,ck,ch";
		// means, that a vowel has to go first, and the syllable break is behind the combination
		static constexpr Literal syllableEnd	= "xt,tz,ts,st,sch,rzt,rz,rtz,rt,rst,rn,rm,rl,rkt,rk,pft,pf,nz,nt,ns,nkt,ngs,ng,nd,mpft,mpf,kt,ht,hrts,hrst,hr,hn,hl,fts,ftl,ft,fl,dt,ckt,ck,chts,cht,chs,ch";
		// Any vocal before is considered not to belong to the syllable
		static constexpr Literal syllableStart	= "zw,wr,tr,th,str,st,spr,sp,schw,schr,schn,schm,schl,sch,rh,qu,pt,ps,pr,pl,ph,pfl,pf,kr,kn,kl,gr,gn,gl,fr,fl,ch,br,bl";
		// Sorted List of chars at which words could be separated
		static constexpr Literal wordSeparators = " !#$&()*,-./:;<=>?@[\\]^`{|}~";
		
		// Private Constructor
		SyllableParser(){}
		
		// Functions to check is a character belongs to a certain group
		// Returns the number of chars ahead matching the mask
		static unsigned char inline isConsonant( const char* c ){
			return charIsOf( *c , consonants );
		}
		static unsigned char inline isVowel( const char* c ){
			return charIsOf( *c , vowels );
		}
		static unsigned char inline isDiphtong( const char* txt ){
			return checkMatchList( txt , diphthongs );
		}
		static unsigned char inline isSyllableStart( const char* txt ){
			return checkMatchList( txt , syllableStart );
		}
		static unsigned char inline isSyllableEnd( const char* txt ){
			return checkMatchList( txt , syllableEnd );
		}
		static unsigned char inline isIndivisible( const char* txt ){
			return checkMatchList( txt , indivisibles );
		}
		static constexpr inline char toLower( char c ){ return c|32; }
		
		// Returns the number of letters before the next syllable
		static unsigned char getNumCharsBeforeSyllableEnd( const char* start );
		
		// Parse all words
		static std::list<int> parseTextInternal( const char* text , const char* end );
		
		// Parse a specific word
		static std::list<int> parseWordInternal( const char* startPos , const char* endPos );
		
		// Returns a pointer to the first occourence of any of the by 'match' specified characters
		static const char* findFirstOf( const char* text , const char* match )
		{
			if( !*text )
				return nullptr;
			do
			{
				const char* curPattern = match;
				do
				{
					if( *text == *curPattern )
						return text;
					// Sorting the list helps to accelerate comparisons
					else if( *text < *curPattern )
						break;
				}while( *++curPattern );
			}while( *++text );
			
			return nullptr;
		}
	
	public:
		
		static bool charIsOf( char c , const char* txt )
		{
			do
			{
				if( *txt == c )
					return true;
				// As the lists are all sorted, we can predict we won't find it anymore!
				else if( *txt > c )
					return false;
			}while( *++txt );
			
			return false;
		}
		
		// Check the begin of txt for any phrases in the list 'curExpr'
		static unsigned char checkMatchList( const char* txt , const char* curExpr )
		{
			while( *curExpr )
			{
				const char* curChar = curExpr;
				
				// Determine the length of the current pattern
				while( *curChar != ',' && *curChar )
					curChar++;
				
				unsigned char exprLen = curChar - curExpr;
				
				// Skip this: "xx,,xxx"
				if( exprLen )
				{
					int result = strncmp( curExpr , txt , exprLen );
					if( result == 0 )
						return exprLen;
					// As the lists are reverse sorted: can what we're looking for still occour?
					else if( result < 0 )
						return 0; // Nope..
				}
				
				// If this is the last pattern in the list: break before we increase (!) -> could lead to enless looping
				if( !*curChar )
					break;
				
				// Step one pattern ahead!
				curExpr += exprLen+1; // +1 for the comma
			};
			return 0;
		}
		
		// Pass the text as chonst char* and get back a list with the indices of possible syllable-breaks
		static std::list<int> parseText( const char* src , const char* end = nullptr )
		{
			int len = std::max( (size_t)1 , strlen(src) );
			
			char* text = new char[len];
			char* dest = text;
			
			if( !end )
				end = text + len;
			else
				end = text + ( end - src );
			
			// Convert to lowercase
			do
				*dest++ = toLower( *src++ );
			while( --len );
			
			std::list<int> lst = parseTextInternal( text , end );
			
			delete[] text;
			
			return move(lst);
		}
};

#endif