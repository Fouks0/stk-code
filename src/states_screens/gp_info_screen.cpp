//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2009-2015  Marianne Gagnon
//            (C) 2014-2015  Joerg Henrichs, konstin
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

#include "states_screens/gp_info_screen.hpp"

#include "audio/sfx_manager.hpp"
#include "challenges/unlock_manager.hpp"
#include "config/player_manager.hpp"
#include "config/saved_grand_prix.hpp"
#include "graphics/stk_tex_manager.hpp"
#include "guiengine/engine.hpp"
#include "guiengine/screen.hpp"
#include "guiengine/widgets/icon_button_widget.hpp"
#include "guiengine/widgets/label_widget.hpp"
#include "guiengine/widgets/list_widget.hpp"
#include "guiengine/widgets/ribbon_widget.hpp"
#include "guiengine/widgets/spinner_widget.hpp"
#include "io/file_manager.hpp"
#include "race/grand_prix_manager.hpp"
#include "race/grand_prix_data.hpp"
#include "race/race_manager.hpp"
#include "states_screens/state_manager.hpp"
#include "tracks/track.hpp"
#include "tracks/track_manager.hpp"

#include <IGUIEnvironment.h>
#include <IGUIStaticText.h>

#include <algorithm>

using irr::gui::IGUIStaticText;
using namespace GUIEngine;

/** Constructor, initialised some variables which might be used before
 *  loadedFromFile is called.
 */
GPInfoScreen::GPInfoScreen() : Screen("gp_info.stkgui")
{
    m_curr_time = 0.0f;
    // Necessary to test if loadedFroMFile() was executed (in setGP)
    m_reverse_spinner   = NULL;
    m_max_num_tracks = 0;
}   // GPInfoScreen

// ----------------------------------------------------------------------------
/** Called when the stkgui file is read. It stores the pointer to various
 *  widgets and adds the right names for reverse mode.
 */
void GPInfoScreen::loadedFromFile()
{
    // The group spinner is filled in init every time the screen is shown
    // (since the groups can change if addons are added/deleted).
    m_group_spinner      = getWidget<SpinnerWidget>("group-spinner");
    m_reverse_spinner    = getWidget<SpinnerWidget>("reverse-spinner");
    m_reverse_spinner->addLabel(_("Default"));
    m_reverse_spinner->addLabel(_("None"));
    m_reverse_spinner->addLabel(_("All"));
    m_reverse_spinner->addLabel(_("Random"));
    m_reverse_spinner->setValue(0);

    m_ai_kart_spinner = getWidget<SpinnerWidget>("ai-spinner");
    
    GUIEngine::IconButtonWidget* screenshot = getWidget<IconButtonWidget>("screenshot");
    screenshot->setFocusable(false);
    screenshot->m_tab_stop = false;
}   // loadedFromFile

// ----------------------------------------------------------------------------
/** Sets the GP to be displayed. */
void GPInfoScreen::setGP(const std::string &gp_ident)
{
    m_gp = *grand_prix_manager->getGrandPrix(gp_ident);
}   // setGP

// ----------------------------------------------------------------------------
/** Converts the currently selected reverse status into a value of type
*  GPReverseType .
*/
GrandPrixData::GPReverseType GPInfoScreen::getReverse() const
{
    switch (m_reverse_spinner->getValue())
    {
    case 0: return GrandPrixData::GP_DEFAULT_REVERSE; break;
    case 1: return GrandPrixData::GP_NO_REVERSE;      break;
    case 2: return GrandPrixData::GP_ALL_REVERSE;     break;
    case 3: return GrandPrixData::GP_RANDOM_REVERSE;  break;
    default: assert(false);
    }   // switch
    // Avoid compiler warning
    return GrandPrixData::GP_DEFAULT_REVERSE;
}   // getReverse
// ----------------------------------------------------------------------------
void GPInfoScreen::beforeAddingWidget()
{
    // Check if there is a saved GP:
    SavedGrandPrix* saved_gp = SavedGrandPrix::getSavedGP(
        StateManager::get()->getActivePlayerProfile(0)->getUniqueID(),
        m_gp.getId(),
        race_manager->getMinorMode(),
        race_manager->getNumLocalPlayers());
    
    int tracks = (int)m_gp.getTrackNames().size();
    bool continue_visible = saved_gp && saved_gp->getNextTrack() > 0 &&
                                        saved_gp->getNextTrack() < tracks;

    RibbonWidget* ribbonButtons = getWidget<RibbonWidget>("buttons");
    int id_continue_button = ribbonButtons->findItemNamed("continue");
    ribbonButtons->setItemVisible(id_continue_button, continue_visible);
    ribbonButtons->setLabel(id_continue_button, _("Continue saved GP"));
}

// ----------------------------------------------------------------------------
/** Called before the screen is shown. It adds the screenshot icon, and
 *  initialises all widgets depending on GP mode (random or not), if a saved
 *  GP is available etc.
 */
void GPInfoScreen::init()
{
    Screen::init();
    m_curr_time = 0.0f;
    getWidget<LabelWidget  >("group-text"   )->setVisible(false);
    m_group_spinner->setVisible(false);

    getWidget<LabelWidget>("name")->setText(m_gp.getName(), false);
    m_gp.checkConsistency();

    // Number of AIs
    // -------------
    const bool has_AI = race_manager->hasAI();
    m_ai_kart_spinner->setVisible(has_AI);
    getWidget<LabelWidget>("ai-text")->setVisible(has_AI);

    if (has_AI)
    {
        const int local_players = race_manager->getNumLocalPlayers();
        int min_ai = 0;
        int num_ai = int(UserConfigParams::m_num_karts_per_gamemode
            [RaceManager::MAJOR_MODE_GRAND_PRIX]) - local_players;

        // A ftl reace needs at least three karts to make any sense
        if (race_manager->getMinorMode()==RaceManager::MINOR_MODE_FOLLOW_LEADER)
        {
            min_ai = std::max(0, 3 - local_players);
        }
        
        num_ai = std::max(min_ai, num_ai);
        
        m_ai_kart_spinner->setActive(true);
        m_ai_kart_spinner->setValue(num_ai);
        m_ai_kart_spinner->setMax(stk_config->m_max_karts - local_players);
        m_ai_kart_spinner->setMin(min_ai);
    }   // has_AI

    addTracks();
    addScreenshot();

    RibbonWidget* bt_start = getWidget<GUIEngine::RibbonWidget>("buttons");
    bt_start->setFocusForPlayer(PLAYER_ID_GAME_MASTER);
}   // init

// ----------------------------------------------------------------------------
/** Updates the list of tracks shown.
 */
void GPInfoScreen::addTracks()
{
    const std::vector<std::string> tracks = m_gp.getTrackNames();

    ListWidget *list = getWidget<ListWidget>("tracks");
    list->clear();
    for (unsigned int i = 0; i < (unsigned int)tracks.size(); i++)
    {
        const Track *track = track_manager->getTrack(tracks[i]);
        std::string s = StringUtils::toString(i);
        list->addItem(s, track->getName());
    }
}   // addTracks

// ----------------------------------------------------------------------------
/** Creates a screenshot widget in the placeholder of the GUI.
 */
void GPInfoScreen::addScreenshot()
{
    GUIEngine::IconButtonWidget* screenshot = getWidget<IconButtonWidget>("screenshot");

    // Temporary icon, will replace it just after
    // (but it will be shown if the given icon is not found)
    screenshot->m_properties[PROP_ICON] = "gui/icons/main_help.png";

    const Track *track = track_manager->getTrack(m_gp.getTrackId(0));
    video::ITexture* image = STKTexManager::getInstance()
        ->getTexture(track->getScreenshotFile(),
        "While loading screenshot for track '%s':", track->getFilename());
    if (image != NULL)
        screenshot->setImage(image);
}   // addScreenShot

// ----------------------------------------------------------------------------
/** Handle user input.
 */
void GPInfoScreen::eventCallback(Widget *, const std::string &name,
                                 const int player_id)
{
    if(name=="buttons")
    {
        const std::string &button = getWidget<RibbonWidget>("buttons")
                                  ->getSelectionIDString(PLAYER_ID_GAME_MASTER);

        if (button == "start")
        {
            // Normal GP: start GP
            const int local_players = race_manager->getNumLocalPlayers();
            const bool has_AI = race_manager->hasAI();
            const int num_ai = has_AI ? m_ai_kart_spinner->getValue() : 0;
            
            race_manager->setNumKarts(local_players + num_ai);
            UserConfigParams::m_num_karts_per_gamemode[RaceManager::MAJOR_MODE_GRAND_PRIX] = local_players + num_ai;
            
            m_gp.changeReverse(getReverse());
            race_manager->startGP(m_gp, false, false);
        }
        else if (button == "continue")
        {
            // Normal GP: continue a saved GP
            m_gp.changeReverse(getReverse());
            race_manager->startGP(m_gp, false, true);
        }
    }   // name=="buttons"
    else if(name=="back")
    {
        StateManager::get()->escapePressed();
    }

}   // eventCallback

// ----------------------------------------------------------------------------
/** Called every update. Used to cycle the screenshots.
 *  \param dt Time step size.
 */
void GPInfoScreen::onUpdate(float dt)
{
    if (dt == 0)
        return; // if nothing changed, return right now

    m_curr_time += dt;
    int frame_after = (int)(m_curr_time / 1.5f);

    const std::vector<std::string> tracks = m_gp.getTrackNames();
    if (frame_after >= (int)tracks.size())
    {
        frame_after = 0;
        m_curr_time = 0;
    }

    Track* track = track_manager->getTrack(tracks[frame_after]);
    std::string file = track->getScreenshotFile();
    GUIEngine::IconButtonWidget* screenshot = getWidget<IconButtonWidget>("screenshot");
    screenshot->setImage(file, IconButtonWidget::ICON_PATH_TYPE_ABSOLUTE);
    screenshot->m_properties[PROP_ICON] = file;
}   // onUpdate
