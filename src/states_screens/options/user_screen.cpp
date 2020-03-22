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

#include "states_screens/options/user_screen.hpp"

#include "audio/sfx_manager.hpp"
#include "challenges/unlock_manager.hpp"
#include "config/player_manager.hpp"
#include "config/user_config.hpp"
#include "graphics/central_settings.hpp"
#include "guiengine/widgets/check_box_widget.hpp"
#include "guiengine/widgets/dynamic_ribbon_widget.hpp"
#include "guiengine/widgets/label_widget.hpp"
#include "guiengine/widgets/list_widget.hpp"
#include "guiengine/widgets/text_box_widget.hpp"
#include "states_screens/dialogs/message_dialog.hpp"
#include "states_screens/dialogs/kart_color_slider_dialog.hpp"
#include "states_screens/main_menu_screen.hpp"
#include "states_screens/options/options_screen_audio.hpp"
#include "states_screens/options/options_screen_general.hpp"
#include "states_screens/options/options_screen_input.hpp"
#include "states_screens/options/options_screen_ui.hpp"
#include "states_screens/options/options_screen_video.hpp"
#include "states_screens/register_screen.hpp"
#include "states_screens/state_manager.hpp"


using namespace GUIEngine;

// ----------------------------------------------------------------------------

BaseUserScreen::BaseUserScreen(const std::string &name) : Screen(name.c_str())
{
    m_new_registered_data = false;
}   // BaseUserScreen

// ----------------------------------------------------------------------------

void BaseUserScreen::loadedFromFile()
{
    m_username_tb = getWidget<TextBoxWidget >("username");
    assert(m_username_tb);
    m_players = getWidget<DynamicRibbonWidget>("players");
    assert(m_players);
    m_options_widget = getWidget<RibbonWidget>("options");
    assert(m_options_widget);
    m_info_widget = getWidget<LabelWidget>("message");
    assert(m_info_widget);

}   // loadedFromFile

// ----------------------------------------------------------------------------
/** Stores information from the register screen. It allows this screen to 
 *  use the entered user name and password to prefill fields so that the user
 *  does not have to enter them again.
 * \param online_name The online account name.
 */
void BaseUserScreen::setNewAccountData(const core::stringw &name)
{
    // Indicate for init that new user data is available.
    m_new_registered_data = true;
    m_username_tb->setText(name);
}   // setOnline

// ----------------------------------------------------------------------------
void BaseUserScreen::beforeAddingWidget()
{
}   // beforeAddingWidget

// ----------------------------------------------------------------------------
/** Initialises the user screen. Searches for all players to fill the 
 *  list of users with their icons, and initialises all widgets for the
 *  current user (e.g. the online flag etc).
 */
void BaseUserScreen::init()
{
    getWidget<IconButtonWidget>("default_kart_color")->setVisible(CVS->isGLSL());

    // The behaviour of the screen is slightly different at startup, i.e.
    // when it is the first screen: cancel will exit the game, and in
    // this case no 'back' error should be shown.
    bool is_first_screen = StateManager::get()->getMenuStackSize()==1;
    getWidget<IconButtonWidget>("back")->setVisible(!is_first_screen);
    getWidget<IconButtonWidget>("cancel")->setLabel(is_first_screen 
                                                    ? _("Exit game") 
                                                    : _("Cancel")      );

    // It should always be activated ... but just in case
    m_options_widget->setActive(true);
    // Clean any error message still shown
    m_info_widget->setText("", true);
    m_info_widget->setErrorColor();

    Screen::init();

    m_players->clearItems();
    int current_player_index = -1;

    for (unsigned int n=0; n<PlayerManager::get()->getNumPlayers(); n++)
    {
        const PlayerProfile *player = PlayerManager::get()->getPlayer(n);
        if (player->isGuestAccount()) continue;
        std::string s = StringUtils::toString(n);
        m_players->addItem(player->getName(), s, player->getIconFilename(), 0,
                           IconButtonWidget::ICON_PATH_TYPE_ABSOLUTE);
        if(player == PlayerManager::getCurrentPlayer())
            current_player_index = n;
    }

    m_players->updateItemDisplay();

    // Select the current player. That can only be done after
    // updateItemDisplay is called.
    if (current_player_index != -1)
        selectUser(current_player_index);
    // no current player found
    // The first player is the most frequently used, so select it
    else if (PlayerManager::get()->getNumPlayers() > 0)
        selectUser(0);

    // Disable changing the user while in game
    bool in_game = StateManager::get()->getGameState() == GUIEngine::INGAME_MENU;
    getWidget<IconButtonWidget>("ok")->setActive(!in_game);
    getWidget<IconButtonWidget>("new_user")->setActive(!in_game);
    getWidget<IconButtonWidget>("rename")->setActive(!in_game);
    getWidget<IconButtonWidget>("delete")->setActive(!in_game);
    if (getWidget<IconButtonWidget>("default_kart_color")->isVisible())
        getWidget<IconButtonWidget>("default_kart_color")->setActive(!in_game);

    m_new_registered_data = false;
}   // init

// ----------------------------------------------------------------------------
PlayerProfile* BaseUserScreen::getSelectedPlayer()
{
    const std::string &s_id = m_players
                            ->getSelectionIDString(PLAYER_ID_GAME_MASTER);
    unsigned int n_id;
    StringUtils::fromString(s_id, n_id);
    return PlayerManager::get()->getPlayer(n_id);
}   // getSelectedPlayer

// ----------------------------------------------------------------------------

void BaseUserScreen::tearDown()
{
    Screen::tearDown();
}   // tearDown

// ----------------------------------------------------------------------------

EventPropagation BaseUserScreen::filterActions(PlayerAction action,
    int deviceID,
    const unsigned int value,
    Input::InputType type,
    int playerId)
{
    if (action == PA_MENU_SELECT)
    {
        if (m_username_tb != NULL && m_username_tb->isFocusedForPlayer(PLAYER_ID_GAME_MASTER))
        {
            PlayerProfile *player = getSelectedPlayer();
            PlayerManager::get()->setCurrentPlayer(player);
            closeScreen();
            return EVENT_BLOCK;
        }
    }

    return EVENT_LET;
}

// ----------------------------------------------------------------------------
/** Called when a user is selected. It updates the online checkbox and
 *  entry fields.
 */
void BaseUserScreen::selectUser(int index)
{
    PlayerProfile *profile = PlayerManager::get()->getPlayer(index);
    assert(profile);

    // Only set focus in case of non-tabbed version (so that keyboard
    // or gamepad navigation with tabs works as expected, i.e. you can
    // select the next tab without having to go up to the tab list first.
    bool focus_it = !getWidget<RibbonWidget>("options_choice");
    m_players->setSelection(StringUtils::toString(index), PLAYER_ID_GAME_MASTER,
                            focus_it);
    
    if (!m_new_registered_data)
        m_username_tb->setText("");
}   // selectUser

// ----------------------------------------------------------------------------
/** Called when the user selects anything on the screen.
 */
void BaseUserScreen::eventCallback(Widget* widget,
                               const std::string& name,
                               const int player_id)
{
    // Clean any error message still shown
    m_info_widget->setText("", true);
    m_info_widget->setErrorColor();

    if (name == "players")
    {
        // Clicked on a name --> Find the corresponding online data
        // and display them
        const std::string &s_index = getWidget<DynamicRibbonWidget>("players")
                                   ->getSelectionIDString(player_id);
        if (s_index == "") return;  // can happen if the list is empty

        unsigned int id;
        if (StringUtils::fromString(s_index, id))
            selectUser(id);
    }
    else if (name == "options")
    {
        const std::string &button =
                             m_options_widget->getSelectionIDString(player_id);
        if (button == "ok")
        {
            PlayerProfile *player = getSelectedPlayer();
            PlayerManager::get()->setCurrentPlayer(player);
            closeScreen();
        }   // button==ok
        else if (button == "new_user")
        {
            RegisterScreen::getInstance()->push();
            RegisterScreen::getInstance()->setParent(this);
            // Make sure the new user will have an empty online name field
            // that can also be edited.
            m_username_tb->setText("");
            m_username_tb->setActive(true);
        }
        else if (button == "cancel")
        {
            // EscapePressed will pop this screen.
            StateManager::get()->escapePressed();
        }
        else if (button == "rename")
        {
            PlayerProfile *cp = getSelectedPlayer();
            RegisterScreen::getInstance()->setRename(cp);
            RegisterScreen::getInstance()->push();
            RegisterScreen::getInstance()->setParent(this);
            m_new_registered_data = false;
            // Init will automatically be called, which
            // refreshes the player list
        }
        else if (button == "default_kart_color")
        {
            new KartColorSliderDialog(getSelectedPlayer());
        }
        else if (button == "delete")
        {
            deletePlayer();
        }
    }   // options
    else if (name == "back")
    {
        StateManager::get()->escapePressed();
    }

    return;

}   // eventCallback

// ----------------------------------------------------------------------------
/** Closes the BaseUserScreen, and makes sure that the right screen is displayed
 *  next.
 */
void BaseUserScreen::closeScreen()
{
    if (StateManager::get()->getMenuStackSize() > 1)
    {
        StateManager::get()->popMenu();
    }
    else
    {
        StateManager::get()->resetAndGoToScreen(MainMenuScreen::getInstance());
    }
}   // closeScreen

// ----------------------------------------------------------------------------
/** Called when a player will be deleted.
 */
void BaseUserScreen::deletePlayer()
{
    // Check that there is at least one player left: we need to have a
    // valid current player, so the last player can not be deleted.
    if(PlayerManager::get()->getNumNonGuestPlayers()==1)
    {
        m_info_widget->setText(_("You can't delete the only player."), true);
        m_info_widget->setErrorColor();
        return;
    }

    PlayerProfile *player = getSelectedPlayer();
    irr::core::stringw message =
        //I18N: In the player info dialog (when deleting)
        _("Do you really want to delete player '%s'?", player->getName());

    class ConfirmServer : public MessageDialog::IConfirmDialogListener
    {
        BaseUserScreen *m_parent_screen;
    public:
        virtual void onConfirm()
        {
            m_parent_screen->doDeletePlayer();
        }   // onConfirm
        // ------------------------------------------------------------
        ConfirmServer(BaseUserScreen *parent)
        {
            m_parent_screen = parent;
        }
    };   // ConfirmServer

    new MessageDialog(message, MessageDialog::MESSAGE_DIALOG_CONFIRM,
                      new ConfirmServer(this), true);
}   // deletePlayer

// ----------------------------------------------------------------------------
/** Callback when the user confirms to delete a player. This function actually
 *  deletes the player, discards the dialog, and re-initialised the BaseUserScreen
 *  to display only the available players.
 */
void BaseUserScreen::doDeletePlayer()
{
    PlayerProfile *player = getSelectedPlayer();
    PlayerManager::get()->deletePlayer(player);
    GUIEngine::ModalDialog::dismiss();

    // Special case: the current player was deleted. We have to make sure
    // that there is still a current player (all of STK depends on that).
    if(!PlayerManager::getCurrentPlayer())
    {
        for(unsigned int i=0; i<PlayerManager::get()->getNumPlayers(); i++)
        {
            PlayerProfile *player = PlayerManager::get()->getPlayer(i);
            if(!player->isGuestAccount())
            {
                PlayerManager::get()->setCurrentPlayer(player);
                break;
            }
        }
    }
    init();
}   // doDeletePlayer

// ============================================================================
/** In the tab version, make sure the right tab is selected.
 */
void TabbedUserScreen::init()
{
    RibbonWidget* tab_bar = getWidget<RibbonWidget>("options_choice");
    assert(tab_bar != NULL);
    tab_bar->setFocusForPlayer(PLAYER_ID_GAME_MASTER);
    tab_bar->select("tab_players", PLAYER_ID_GAME_MASTER);
    BaseUserScreen::init();
}   // init

// ----------------------------------------------------------------------------
/** Switch to the correct tab.
 */
void TabbedUserScreen::eventCallback(GUIEngine::Widget* widget,
                                     const std::string& name,
                                     const int player_id)
{
    if (name == "options_choice")
    {
        std::string selection = ((RibbonWidget*)widget)->getSelectionIDString(PLAYER_ID_GAME_MASTER);

        Screen *screen = NULL;
        if (selection == "tab_audio")
            screen = OptionsScreenAudio::getInstance();
        else if (selection == "tab_video")
            screen = OptionsScreenVideo::getInstance();
        //else if (selection == "tab_players")
        //    screen = TabbedUserScreen::getInstance();
        else if (selection == "tab_controls")
            screen = OptionsScreenInput::getInstance();
        else if (selection == "tab_ui")
            screen = OptionsScreenUI::getInstance();
        else if (selection == "tab_general")
            screen = OptionsScreenGeneral::getInstance();
        else if (selection == "tab_language")
            new MessageDialog("Deleted feature! There is only one surviving language since 20XX.");
        if(screen)
            StateManager::get()->replaceTopMostScreen(screen);
    }
    else
        BaseUserScreen::eventCallback(widget, name, player_id);

}   // eventCallback
