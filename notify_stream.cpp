/***********************************************************************
*                                                                      *
* (C) 2016, Moritz Nöltner-Augustin                                    *
*                                                                      *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 3 of the License, or    *
* (at your option) any later version.                                  *
*                                                                      *
* This program is distributed in the hope that it will be useful,      *
* but WITHOUT ANY WARRANTY; without even the implied warranty of       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU General Public License for more details.                         *
*                                                                      *
* You should have received a copy of the GNU General Public License    *
* along with this program; if not, write to the Free Software          *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 *
* USA                                                                  *
*                                                                      *
***********************************************************************/

#include "notify_stream.hpp"

// Initialise the actual streams.
// By setting init_priority to the lowest possible level, the notifications can be safely used in constructors.


template<class CharT, class Traits, bool active>
unsigned int Notify_stream<CharT, Traits, active>::instance_count = 0;

__attribute__ ((init_priority (101))) Notify_stream<char, std::char_traits<char>, USE_NOTIFY_LOW> notify_low(NOTIFY_URGENCY_LOW);

__attribute__ ((init_priority (101))) Notify_stream<char, std::char_traits<char>, USE_NOTIFY_NORMAL> notify_normal(NOTIFY_URGENCY_NORMAL);

__attribute__ ((init_priority (101))) Notify_stream<char, std::char_traits<char>, USE_NOTIFY_CRITICAL> notify_critical(NOTIFY_URGENCY_CRITICAL);
