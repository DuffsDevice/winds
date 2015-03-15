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

#ifndef _SYLLABLE_PARSER_H_
#define _SYLLABLE_PARSER_H_

#include <string.h>
#include <list>
#include <wchar.h>

struct LanguageDefinition
{
	typedef const char* Literal;
	
	// Alphabet
	Literal vowels;
	Literal consonants;
	
	// Vocals that are spoken as one syllable
	Literal diphthongs;
	
	// consonants that don't exist alone
	Literal indivisibles;
	
	// means, that a vowel has to go first, and the syllable break is behind the combination
	Literal syllableEnd;
	
	// Any vocal before is considered not to belong to the syllable
	Literal syllableStart;
	
	// Sorted List of chars at which words could be separated
	Literal wordSeparators;
	
	// Ctor
	constexpr LanguageDefinition( Literal vowels , Literal consonants , Literal diphthongs , Literal indivisibles , Literal syllableEnd , Literal syllableStart , Literal wordSeparators ) :
		vowels( vowels )
		, consonants( consonants )
		, diphthongs( diphthongs )
		, indivisibles( indivisibles )
		, syllableEnd( syllableEnd )
		, syllableStart( syllableStart )
		, wordSeparators( wordSeparators )
	{}
};

namespace TinyHyphenatorLanguages
{
	struct German : public LanguageDefinition
	{
		German() : LanguageDefinition(
			"aeiouyöäü"
			, "bcdfghjklmnpqrstvwxzß"
			, "äu,ue,oe,io,ie,eu,ei,au,ai,ae"
			, "th,rh,qu,ph,ngs,ck,ch"
			, "xt,tz,ts,st,sch,rzt,rz,rtz,rt,rst,rn,rm,rl,rkt,rk,pft,pf,nz,nt,ns,nkt,ngs,ng,nd,mpft,mpf,kt,ht,hrts,hrst,hr,hn,hl,fts,ftl,ft,fl,dt,ckt,ck,chts,cht,chs,ch"
			, "zw,wr,tr,th,str,st,spr,sp,schw,schr,schn,schm,schl,sch,rh,qu,pt,ps,pr,pl,ph,pfl,pf,kr,kn,kl,gr,gn,gl,fr,fl,ch,br,bl"
			, " !#$&()*,-./:;<=>?@[\\]^`{|}~"
		) {}
	};
}

class TinyHyphenator
{
	private:
		
		typedef const char* Literal;
		
		LanguageDefinition language;
		
		// Functions to check is a character belongs to a certain group
		// Returns the number of chars ahead matching the mask
		unsigned char inline isConsonant( Literal c ) const {
			return charIsOf( *c , language.consonants );
		}
		unsigned char inline isVowel( Literal c ) const {
			return charIsOf( *c , language.vowels );
		}
		unsigned char inline isDiphtong( Literal txt ) const {
			return checkMatchList( txt , language.diphthongs );
		}
		unsigned char inline isSyllableStart( Literal txt ) const {
			return checkMatchList( txt , language.syllableStart );
		}
		unsigned char inline isSyllableEnd( Literal txt ) const {
			return checkMatchList( txt , language.syllableEnd );
		}
		unsigned char inline isIndivisible( Literal txt ) const {
			return checkMatchList( txt , language.indivisibles );
		}
		constexpr static inline char toLower( char c ){ return c|32; }
		
		// Returns the number of letters before the next syllable
		unsigned char getNumCharsBeforeSyllableEnd( Literal start ) const ;
		
		// Parse all words
		std::list<int> parseTextInternal( Literal text , Literal end ) const ;
		
		// Parse a specific word
		std::list<int> parseWordInternal( Literal startPos , Literal endPos ) const ;
		
		// Returns a pointer to the first occourence of any of the by 'match' specified characters
		Literal findFirstOf( Literal text , Literal match ) const 
		{
			if( !*text )
				return nullptr;
			do
			{
				Literal curPattern = match;
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
		
		// Constructor
		TinyHyphenator( LanguageDefinition language ) : 
			language( language )
		{}
		
		bool charIsOf( char c , Literal txt ) const 
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
		unsigned char checkMatchList( Literal txt , Literal curExpr ) const 
		{
			while( *curExpr )
			{
				Literal curChar = curExpr;
				
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
		std::list<int> parseText( Literal src , Literal end = nullptr ) const 
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