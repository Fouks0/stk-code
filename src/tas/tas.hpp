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
#include <memory>

// Class for TAS tools
class Tas
{
public:
    enum GameStatus {PAUSED, TICK_ADVANCE, NORMAL};
private:
    GameStatus m_game_status;
    
    // Static pointer to the one instance of the Tas object.
    static Tas *m_tas;
    
    Tas();
    ~Tas();
public:
    void  init();
    void  reset();
    static void create() {
        assert(!m_tas);
        m_tas = new Tas();
    }
    static Tas *get() {return m_tas;}
    static void destroy() {delete m_tas; m_tas = NULL;}
    
    // Press P to pause/unpause, and O to advance one tick
    GameStatus getGameStatus() {return m_game_status;}
    void  pause() {m_game_status = PAUSED;}
    void  tickAdvance() {m_game_status = TICK_ADVANCE;}
    void  resume() {m_game_status = NORMAL;}
};

#endif
