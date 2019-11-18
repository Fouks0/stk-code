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

#ifndef HEADER_STATS_HPP
#define HEADER_STATS_HPP

// Stores some stats
class Stats
{
private:
    // Speeds are in m/s here
    uint64_t m_speed_samples;
    float m_speed_sum, m_final_speed_avg, m_final_distance;
    float m_prev_speed, m_cur_speed;

public:
    Stats() {reset();}

    void reset()
    {
        m_speed_samples = 0;
        m_speed_sum = 0.;
        m_final_speed_avg = 0.;
        m_final_distance = 0.;
        m_prev_speed = 0.;
        m_cur_speed = 0.;
    }

    void addSpeedSample(float speed_sample)
    {
        m_speed_samples++;
        speed_sample >= 0. ? m_speed_sum += speed_sample : m_speed_sum -= speed_sample;
        m_prev_speed = m_cur_speed;
        m_cur_speed = speed_sample;
    }

    float v() const {return m_cur_speed;}
    float dv() const {return m_cur_speed - m_prev_speed;}
    float averageSpeed() const
    {
        if (m_speed_samples != 0) return m_speed_sum/(float) m_speed_samples;
        else return 0.;
    }
    float distance() const
    {
        return ((float) m_speed_samples)*averageSpeed()/120.;
    }
};

#endif
