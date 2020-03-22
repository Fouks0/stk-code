//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2009-2015 Marianne Gagnon
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

#include "states_screens/options/options_screen_general.hpp"

#include "audio/sfx_manager.hpp"
#include "audio/sfx_base.hpp"
#include "config/player_manager.hpp"
#include "config/user_config.hpp"
#include "font/bold_face.hpp"
#include "font/font_manager.hpp"
#include "font/regular_face.hpp"
#include "graphics/irr_driver.hpp"
#include "guiengine/scalable_font.hpp"
#include "guiengine/screen.hpp"
#include "guiengine/widgets/button_widget.hpp"
#include "guiengine/widgets/check_box_widget.hpp"
#include "guiengine/widgets/dynamic_ribbon_widget.hpp"
#include "guiengine/widgets/label_widget.hpp"
#include "guiengine/widgets/list_widget.hpp"
#include "guiengine/widgets/spinner_widget.hpp"
#include "guiengine/widget.hpp"
#include "io/file_manager.hpp"
#include "states_screens/main_menu_screen.hpp"
#include "states_screens/dialogs/message_dialog.hpp"
#include "states_screens/options/options_screen_audio.hpp"
#include "states_screens/options/options_screen_input.hpp"
#include "states_screens/options/options_screen_ui.hpp"
#include "states_screens/options/options_screen_video.hpp"
#include "states_screens/state_manager.hpp"
#include "states_screens/options/user_screen.hpp"
#include "utils/log.hpp"
#include "utils/string_utils.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace GUIEngine;
using namespace Online;

// -----------------------------------------------------------------------------

OptionsScreenGeneral::OptionsScreenGeneral() : Screen("options_general.stkgui")
{
    m_inited = false;
}   // OptionsScreenVideo

// -----------------------------------------------------------------------------

void OptionsScreenGeneral::loadedFromFile()
{
    m_inited = false;
}   // loadedFromFile

// -----------------------------------------------------------------------------

void OptionsScreenGeneral::init()
{
    Screen::init();
    RibbonWidget* ribbon = getWidget<RibbonWidget>("options_choice");
    assert(ribbon != NULL);
    ribbon->setFocusForPlayer(PLAYER_ID_GAME_MASTER);
    ribbon->select( "tab_general", PLAYER_ID_GAME_MASTER );
    CheckBoxWidget* difficulty = getWidget<CheckBoxWidget>("perPlayerDifficulty");
    assert( difficulty != NULL );
    difficulty->setState( UserConfigParams::m_per_player_difficulty );
    // I18N: Tooltip in the UI menu. Use enough linebreaks to make sure the text fits the screen in low resolutions.
    difficulty->setTooltip(_("In multiplayer mode, players can select handicapped\n(more difficult) profiles on the kart selection screen"));

}   // init

// -----------------------------------------------------------------------------

void OptionsScreenGeneral::eventCallback(Widget* widget, const std::string& name, const int playerID)
{
#ifndef SERVER_ONLY
    if (name == "options_choice")
    {
        std::string selection = ((RibbonWidget*)widget)->getSelectionIDString(PLAYER_ID_GAME_MASTER);

        Screen *screen = NULL;
        if (selection == "tab_audio")
            screen = OptionsScreenAudio::getInstance();
        else if (selection == "tab_video")
            screen = OptionsScreenVideo::getInstance();
        else if (selection == "tab_players")
            screen = TabbedUserScreen::getInstance();
        else if (selection == "tab_controls")
            screen = OptionsScreenInput::getInstance();
        else if (selection == "tab_ui")
            screen = OptionsScreenUI::getInstance();
        //else if (selection == "tab_general")
        //    screen = OptionsScreenGeneral::getInstance();
        else if (selection == "tab_language")
            new MessageDialog("Deleted feature! There is only one surviving language since 20XX.");
        if(screen)
            StateManager::get()->replaceTopMostScreen(screen);
    }
    else if(name == "back")
    {
        StateManager::get()->escapePressed();
    }
    else if (name=="perPlayerDifficulty")
    {
        CheckBoxWidget* difficulty = getWidget<CheckBoxWidget>("perPlayerDifficulty");
        assert( difficulty != NULL );
        UserConfigParams::m_per_player_difficulty = difficulty->getState();
    }
#endif
}   // eventCallback

// -----------------------------------------------------------------------------

void OptionsScreenGeneral::tearDown()
{
    Screen::tearDown();
    // save changes when leaving screen
    user_config->saveConfig();
}   // tearDown

// -----------------------------------------------------------------------------

void OptionsScreenGeneral::unloaded()
{
    m_inited = false;
}   // unloaded

// -----------------------------------------------------------------------------
