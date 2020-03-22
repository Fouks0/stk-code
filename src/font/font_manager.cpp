//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2016 SuperTuxKart-Team
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "font/font_manager.hpp"

#include "config/stk_config.hpp"
#include "font/bold_face.hpp"
#include "font/digit_face.hpp"
#include "font/face_ttf.hpp"
#include "font/regular_face.hpp"
#include "modes/profile_world.hpp"
#include "utils/string_utils.hpp"

FontManager *font_manager = NULL;
// ----------------------------------------------------------------------------
/** Constructor. It will initialize the \ref m_ft_library.
 */
FontManager::FontManager()
{
#ifndef SERVER_ONLY
    m_ft_library = NULL;
    if (ProfileWorld::isNoGraphics())
        return;

    checkFTError(FT_Init_FreeType(&m_ft_library), "loading freetype library");
#endif
}   // FontManager

// ----------------------------------------------------------------------------
/** Destructor. Clears all fonts and related stuff.
 */
FontManager::~FontManager()
{
    for (unsigned int i = 0; i < m_fonts.size(); i++)
        delete m_fonts[i];
    m_fonts.clear();

    delete m_normal_ttf;
    m_normal_ttf = NULL;
    delete m_digit_ttf;
    m_digit_ttf = NULL;

#ifndef SERVER_ONLY
    if (ProfileWorld::isNoGraphics())
        return;

    checkFTError(FT_Done_FreeType(m_ft_library), "removing freetype library");
#endif
}   // ~FontManager

// ----------------------------------------------------------------------------
/** Initialize all \ref FaceTTF and \ref FontWithFace members.
 */
void FontManager::loadFonts()
{
    // First load the TTF files required by each font
    m_normal_ttf = new FaceTTF(stk_config->m_normal_ttf);
    m_digit_ttf = new FaceTTF(stk_config->m_digit_ttf);

    // Now load fonts with settings of ttf file
    unsigned int font_loaded = 0;
    RegularFace* regular = new RegularFace(m_normal_ttf);
    regular->init();
    m_fonts.push_back(regular);
    m_font_type_map[std::type_index(typeid(RegularFace))] = font_loaded++;

    BoldFace* bold = new BoldFace(m_normal_ttf);
    bold->init();
    m_fonts.push_back(bold);
    m_font_type_map[std::type_index(typeid(BoldFace))] = font_loaded++;

    DigitFace* digit = new DigitFace(m_digit_ttf);
    digit->init();
    m_fonts.push_back(digit);
    m_font_type_map[std::type_index(typeid(DigitFace))] = font_loaded++;
}   // loadFonts
