#ifndef _SYLLABLE_PARSER_H_
#define _SYLLABLE_PARSER_H_

#include <string.h>
#include <list>

class SyllableParser
{
	private:
		
		typedef const char* Literal;
		
		// Alphabet
		static constexpr Literal vowels			= "aeiouyöäü";
		static constexpr Literal consonants		= "bcdfghjklmnpqrstvwxzß";
		
		// Vocals that are spoken as one syllable
		static constexpr Literal diphthongs		= "eu,ei,ie,au,ai,äu,ue,ae,oe,io";
		// consonants that don't exist alone
		static constexpr Literal indivisibles	= "ngs,ch,ck,qu,rh,ph,th";
		// means, that a vocal has to go first, and the syllable break is behind the combination
		static constexpr Literal syllableEnd	= "chts,mpft,hrst,chs,cht,rst,rtz,ngs,fts,pft,sch,mpf,rkt,nkt,ckt,kt,ftl,ft,ns,xt,tz,hr,ng,ts,nt,rm,st,nz,rzt,rz,rk,pf,rt,ch,nd,rn,hn,ht,hl,rl,dt,ck,fl";
		// Any vocal before is considered not to belong to the syllable
		static constexpr Literal syllableStart	= "schw,schm,schn,schl,schr,str,sch,spr,pfl,ch,qu,kr,kn,bl,br,pf,pt,pr,ps,ph,pl,st,sp,gl,gr,gn,kl,tr,wr,fr,fl,zw,th,rh";
		
		// Private Constructor
		SyllableParser(){}
		
		// Functions to check is a character belongs to a certain group
		// Returns the number of chars ahead matching the mask
		static unsigned char isConsonant( const char* c ){
			return charIsOf( *c , consonants );
		}
		static unsigned char isVowel( const char* c ){
			return charIsOf( *c , vowels );
		}
		static unsigned char isDiphtong( const char* txt ){
			return checkMatchList( txt , diphthongs );
		}
		static unsigned char isSyllableStart( const char* txt ){
			return checkMatchList( txt , syllableStart );
		}
		static unsigned char isSyllableEnd( const char* txt ){
			return checkMatchList( txt , syllableEnd );
		}
		static unsigned char isIndivisible( const char* txt ){
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
		static const char* findFirstOf( const char* text , const char* match );
	
	public:
		
		static bool charIsOf( char c , const char* txt )
		{
			do
			{
				if( *txt == c )
					return true;		
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
				
				if( exprLen && strncmp( curExpr , txt , exprLen ) == 0 )
					return exprLen;
				
				if( !*curChar ) // If it ends before ->break
					break;
				curExpr += exprLen+1; // +1 for the comma
			};
			return 0;
		}
		
		static std::list<int> parseText( const char* txt , const char* end = nullptr )
		{
			int len = std::max( (size_t)1 , strlen(txt) );
			
			char* text = new char[len];
			char* tmp = text;
			
			if( !end )
				end = text + len;
			else
				end = text + ( end - txt );
			
			// Make lowercase
			do
				*tmp++ = toLower( *txt++ );
			while( --len );
			
			std::list<int> lst = parseTextInternal( text , end );
			
			delete[] text;
			
			return move(lst);
		}
};

#endif