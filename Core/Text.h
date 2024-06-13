#pragma once

#include<map>
#include"./Types.h"

namespace Interface
{
	enum TextAlignment
	{
		LEFT = 0,
		CENTER = 1,
	};

	struct Character {
		uint TextureID; // ID handle of the glyph texture
		iVec2   Size;      // Size of glyph
		iVec2   Bearing;   // Offset from baseline to left/top of glyph
		uint Advance;   // Horizontal offset to advance to next glyph
	};

	// Map from stl std::map<char, Character>
	// - Char as key and Character (struct containg text info) as value
	using CharMap = std::map<char, Character>;	

	class Text
	{
		public:
		Text(String &text, String fontFamily) : m_text(text), m_fontFamily(fontFamily), m_textAlign(LEFT) {}

		int getHorizontalGlyphSize()
		{
			int size=0;
			char letter;
			for(uint i=0;i<(uint)m_text.size();++i)
			{
				letter = m_text[i];
				size += (((*m_activeCharacters)[letter].Size.x + (*m_activeCharacters)[letter].Advance)>>6);
			}

			return size/2.0;
		}

		int getVerticalGlyphSize()
		{
			// Here we are asuming a random character offset of y and taking it for granted..
			return (*m_activeCharacters)['A'].Bearing.y/2.0;
		}

		void SetAligment(TextAlignment align)	{	m_textAlign = align;	}

		fVec2 GetAlignmentOffset()
		{
			return (m_textAlign == CENTER) ? fVec2(getHorizontalGlyphSize(), getVerticalGlyphSize()) : fVec2(0);
		}

		String &m_text;
		String m_fontFamily;
		TextAlignment m_textAlign;
		CharMap *m_activeCharacters;
	};
}