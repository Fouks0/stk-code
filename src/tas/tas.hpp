//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2019 Fouks
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

#ifndef HEADER_TAS_HPP
#define HEADER_TAS_HPP

#include <cassert>
#include <iomanip>
#include <memory>

#include "savestate.hpp"
#include "stats.hpp"

// Stores an input
class TasInput
{
private:
    uint8_t  m_action;
    int32_t  m_steer;
    uint16_t m_accel;

public:
    TasInput(uint8_t action = 0, int32_t steer = 0, uint16_t accel = 0) : m_action(action), m_steer(steer), m_accel(accel) {}

    std::string toString() const;
    std::string toStringConstSize() const;
    bool parse(std::string);

    bool isBraking() const {return (m_action & 1) == 1;}
    bool isUsingNitro() const {return (m_action & 2) == 2;}
    bool isFiring() const {return (m_action & 8) == 8;}
    bool isLookingBack() const {return (m_action & 16) == 16;}
    bool isSkiddingLeft() const {return (m_action & 96) == 64;}
    bool isSkiddingRight() const {return (m_action & 96) == 96;}

    bool isValid() const;
    bool canSucceed(TasInput) const;

    uint8_t  getAction() const {return m_action;}
    int32_t  getSteer () const {return m_steer;}
    uint16_t getAccel () const {return m_accel;}
};

class AbstractKart;
class StandardRace;

// Class for TAS tools
class Tas
{
public:
    enum GameStatus {PAUSED, TICK_ADVANCE, NORMAL};
private:
    GameStatus m_game_status;
    
    // Static pointer to the one instance of the Tas object.
    static Tas *m_tas;
    
    // Race infos
    bool m_inited_for_race;
    uint64_t m_current_tick;
    StandardRace *m_standard_race; // Current World
    AbstractKart *m_player_kart; // Current Player Kart. For now, only support Single Player TASes

    // Frame recording, in order to assemble them later to make a TAS video
    bool m_is_recording_frames;
    uint64_t m_frame_number;

    // Data for inputs recording.
    bool m_is_recording_inputs;
    std::vector<TasInput> m_recorded_inputs;

    // Data for inputs replay.
    bool m_has_started; // For StartUp Boost
    std::string m_inputs_to_play_filename;
    std::vector<TasInput> m_inputs_to_play;
    
    // Save State
    SaveState m_save_state;

    // Some Stats
    Stats m_stats;

    Tas();
    ~Tas();
public:
    void  init();
    void  initForRace(bool);
    void  reset();
    void  resetForRace();
    static void create() {
        assert(!m_tas);
        m_tas = new Tas();
    }
    static Tas *get() {return m_tas;}
    static void destroy() {delete m_tas; m_tas = NULL;}
    void  update();

    unsigned int getCurrentKartId() const;
    
    // Press P to pause/unpause, and O to advance one tick
    GameStatus getGameStatus() {return m_game_status;}
    void  pause() {m_game_status = PAUSED;}
    void  tickAdvance() {m_game_status = TICK_ADVANCE;}
    void  resume() {m_game_status = NORMAL;}
    
    // Frames recording. Press L to start/stop
    bool isRecordingFrames() const {return m_is_recording_frames;}
    uint64_t getFrameNumber() const {return m_frame_number;}
    void saveFrame();
    void startRecordingFrames();
    void stopRecordingFrames();

    // Inputs recording.
    void  saveInputs();
    bool  isInputsRecordingFinished() const {return m_current_tick >= m_recorded_inputs.size();}

    // Inputs replay.
    void  setInputsFilename(std::string filename) {m_inputs_to_play_filename = filename;}
    bool  loadInputs();
    bool  isInputReplayFinished() const;
    void  applyCurrentInput();
    TasInput getCurrentInput() const;

    // Save States
    void saveState();
    void restoreState();

    // Stats
    Stats getStats() const {return m_stats;}
    std::string getReadableInfos() const;
    std::string getReadableSurroundingInputsToPlay() const;
};

#endif
