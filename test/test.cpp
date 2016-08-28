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

// A little test program to see if the notification streams work as expected
//#include "utils.h"
#include "notify_stream.hpp"
#include "utils.h"
#include <iomanip>



__attribute__ ((init_priority (101))) Notify_stream<char, std::char_traits<char>, true> notify_active(NOTIFY_URGENCY_CRITICAL);
__attribute__ ((init_priority (101))) Notify_stream<char, std::char_traits<char>, false> notify_passive(NOTIFY_URGENCY_CRITICAL);


int main()
{
	// Check whether the inactivated streams are really optimised away:
	const uint64_t ITERATIONS = 100; // There seems to be a limit of 1000 active notifications
	const uint64_t LOOPS = 10;       // so when I increase these numbers, the program just hangs.
	uint64_t t0, t1;
	uint64_t t_offset = 0;
	uint64_t t_print = 0;
	uint64_t t_noprint = 0;
	uint64_t t_none = 0;

	for(uint64_t loops=0; loops<LOOPS; loops++)
	{
		rdtsc(t0);
		for(uint64_t i=0; i<ITERATIONS; i++)
		{
			notify_active << "foobar " << i << std::endl;
		}
		rdtsc(t1);
		t_print += t1-t0;

		rdtsc(t0);
		for(uint64_t i=0; i<ITERATIONS; i++)
		{
			notify_passive << "foobar " << i << std::endl;
		}
		rdtsc(t1);
		t_noprint += t1-t0;

		rdtsc(t0);
		for(uint64_t i=0; i<ITERATIONS; i++)
		{
			if(0) notify_active << "foobar " << i << std::endl;
		}
		rdtsc(t1);
		t_none += t1-t0;

		rdtsc(t0);
		rdtsc(t1);
		t_offset += t1-t0;
	}


	std::cout<<"Active (with real output):     "<<t_print<<'\n';
	// If optimisaton is enabled with the compiler, the three times without output should be identical.
	std::cout<<"Passive (output deactivated):  "<<t_noprint<<'\n';
	std::cout<<"None (with an if(0) prepended: "<<t_none<<std::endl;
	std::cout<<"Measurement offset:            "<<t_offset<<'\n';


	notify_low<<"LOW"<<"low"<<std::endl;
	notify_normal<<sett(5000)<<"NORMAL"<<"normal"<<99<<std::endl;
	notify_critical<<sett(10000)<<"CRITICAL"<<"critical"<<std::setw(10)<<99<<'.'<<std::endl;

	return 0;
}


