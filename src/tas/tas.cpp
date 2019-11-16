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

#include "graphics/irr_driver.hpp"
#include "tas.hpp"
#include "utils/log.hpp"

Tas *Tas::m_tas = NULL;

Tas::Tas()
{
    init();
}

Tas::~Tas()
{
    
}

void Tas::reset()
{
    m_game_status = GameStatus::NORMAL;
    m_frame_number = 0;
    m_is_recording_frames = false;
}

void Tas::init()
{
    reset();
    Log::info("TAS", "Inited.");
}

void Tas::saveFrame() {
    irr_driver->doFrameShot(m_frame_number);
    m_frame_number++;
}
void Tas::startRecordingFrames() {
    m_frame_number = 0;
    m_is_recording_frames = true;
}
void Tas::stopRecordingFrames() {
    m_is_recording_frames = false;
    m_frame_number = 0;
}
