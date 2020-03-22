//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2014-2015 Joerg Henrichs
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

#include "states_screens/register_screen.hpp"

#include "config/player_manager.hpp"
#include "config/user_config.hpp"
#include "audio/sfx_manager.hpp"
#include "guiengine/widgets/button_widget.hpp"
#include "guiengine/widgets/check_box_widget.hpp"
#include "guiengine/widgets/label_widget.hpp"
#include "guiengine/widgets/ribbon_widget.hpp"
#include "guiengine/widgets/text_box_widget.hpp"
#include "states_screens/dialogs/message_dialog.hpp"
#include "states_screens/main_menu_screen.hpp"
#include "states_screens/state_manager.hpp"
#include "states_screens/options/user_screen.hpp"
#include "utils/log.hpp"

using namespace GUIEngine;
using namespace Online;
using namespace irr;
using namespace core;

// -----------------------------------------------------------------------------

RegisterScreen::RegisterScreen() : Screen("online/register.stkgui")
{
    m_existing_player = NULL;
    m_parent_screen   = NULL;
}   // RegisterScreen

// -----------------------------------------------------------------------------
void RegisterScreen::init()
{
    m_info_widget = getWidget<LabelWidget>("info");
    assert(m_info_widget);
    m_info_widget->setDefaultColor();
    m_info_widget->setText(L"", false);
    m_options_widget = getWidget<RibbonWidget>("options");
    assert(m_options_widget);

    RibbonWidget* ribbon = getWidget<RibbonWidget>("mode_tabs");
    assert(ribbon);
    ribbon->select("tab_offline", PLAYER_ID_GAME_MASTER);

    // Hide the tabs in case of a rename
    ribbon->setVisible(m_existing_player == NULL);
    Screen::init();

    // If there is no player (i.e. first start of STK), pick default name
    stringw username = "";
    if (m_existing_player)
        username = m_existing_player->getName();
    else if (PlayerManager::get()->getNumPlayers() == 0)
        username = "TASer";

    TextBoxWidget* local_username = getWidget<TextBoxWidget>("local_username");
    local_username->setText(username);

    m_info_message_shown = false;

    local_username->setFocusForPlayer(PLAYER_ID_GAME_MASTER);
    
    // The behaviour of the screen is slightly different at startup, i.e.
    // when it is the first screen: cancel will exit the game, and in
    // this case no 'back' error should be shown.
    bool has_player_profile = (PlayerManager::get()->getNumPlayers() > 0);
    getWidget<IconButtonWidget>("back")->setVisible(has_player_profile);
    getWidget<IconButtonWidget>("cancel")->setLabel(has_player_profile ? _("Cancel") : _("Exit game"));
}   // init

// -----------------------------------------------------------------------------
/** If necessary creates the local user.
 *  \param local_name Name of the local user.
 */
void RegisterScreen::handleLocalName(const stringw &local_name)
{
    if (local_name.size() == 0)
        return;

    // If a local player with that name does not exist, create one
    if (!PlayerManager::get()->getPlayer(local_name))
    {
        PlayerProfile *player;
        // If it's a rename, change the name of the player
        if (m_existing_player && local_name.size() > 0)
        {
            m_existing_player->setName(local_name);
            player = m_existing_player;
        }
        else
            player = PlayerManager::get()->addNewPlayer(local_name);
        PlayerManager::get()->save();

        if (player)
            PlayerManager::get()->setCurrentPlayer(player);
        else
        {
            m_info_widget->setErrorColor();
            m_info_widget->setText(_("Could not create player '%s'.", local_name), false);
        }
    }
    else
    {
        m_info_widget->setErrorColor();
        m_info_widget->setText(_("Could not create player '%s'.", local_name), false);
    }
}   // handleLocalName

// -----------------------------------------------------------------------------
void RegisterScreen::eventCallback(Widget* widget, const std::string& name, const int playerID)
{
    if (name == "mode_tabs")
    {
        RibbonWidget *ribbon = static_cast<RibbonWidget*>(widget);
        std::string selection = ribbon->getSelectionIDString(PLAYER_ID_GAME_MASTER);
        if (selection == "tab_new_online" || selection == "tab_existing_online")
        {
            m_info_widget->setErrorColor();
            m_info_widget->setText(_("Internet is disabled in this Mod, cannot create Online Accounts."), false);
            return;
        }
    }
    else if (name=="options")
    {
        const std::string button = m_options_widget->getSelectionIDString(PLAYER_ID_GAME_MASTER);
        if (button=="next")
        {
            stringw local_name = getWidget<TextBoxWidget>("local_username")->getText().trim();
            if (local_name.empty())
            {
                m_info_widget->setErrorColor();
                m_info_widget->setText(_("User name cannot be empty."), false);
            }
            else
            {
                handleLocalName(local_name);
                StateManager::get()->popMenu();
                StateManager::get()->resetAndGoToScreen(MainMenuScreen::getInstance());
            }
        }
        else if (button=="cancel")
        {
            // We poop this menu, onEscapePress will handle the special case
            // of e.g. a fresh start of stk that is aborted.
            StateManager::get()->popMenu();
            m_existing_player = NULL;
            // Must be first time start, and player cancelled player creation
            // so quit stk. At this stage there are two menus on the stack:
            // 1) The UserScreen,  2) RegisterStreen
            // Popping them both will trigger STK to close.
            if (PlayerManager::get()->getNumPlayers() == 0)
                StateManager::get()->popMenu();
        }
    }
    else if (name == "back")
    {
        m_existing_player = NULL;
        StateManager::get()->escapePressed();
    }

}   // eventCallback
