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


#pragma once


#ifndef NOTIFY_LEVEL
	#pragma message("warning:To use the notify stream, set the notification level.\n" \
	"	* Use -DNOTIFY_LEVEL=0 to disable all notification output\n"                  \
	"	* Use -DNOTIFY_LEVEL=1 to enable only critical output\n"                      \
	"	* Use -DNOTIFY_LEVEL=2 to enable normal and critical output\n"                \
	"	* Use -DNOTIFY_LEVEL=3 to enable all output.\n" )
	#define NOTIFY_LEVEL 0
#endif

#define USE_NOTIFY_LOW false
#define USE_NOTIFY_NORMAL false
#define USE_NOTIFY_CRITICAL false

#if NOTIFY_LEVEL >= 1
	#undef USE_NOTIFY_CRITICAL
	#define USE_NOTIFY_CRITICAL true
#endif

#if NOTIFY_LEVEL >= 2
	#undef USE_NOTIFY_NORMAL
	#define USE_NOTIFY_NORMAL true
#endif

#if NOTIFY_LEVEL >= 3
	#undef USE_NOTIFY_LOW
	#define USE_NOTIFY_LOW true
#endif



#ifndef NOTIFY_PROGRAM_NAME
	#pragma message("warning: You should probably define a proper name for your program." )
	#define NOTIFY_PROGRAM_NAME "My fancy pantsy program"
#endif

#ifndef NOTIFY_TIMEOUT
	#define NOTIFY_TIMEOUT 1000
#endif


#include <iostream>
#include <sstream>
#include <libnotify/notify.h>
/**
 * \brief A stream class to write desktop notifications
 *
 * This class provides an output stream that are translated to desktop notifications using notify-send.
 * There are three streams provided, notify_low, notify_normal and notify_critical, corresponding to the three notifications urgency levels.
 * To write a notification, use eg: notify_normal << "This is shown as the headline" << "This is shown as the notification body" << std::endl;
 * The streams up to the specified urgency level are enabled if USE_NOTIFY_LOW, USE_NOTIFY_NORMAL or USE_NOTIFY_CRITICAL.
 */
template<class CharT, class Traits = std::char_traits<CharT>, bool active = true>
class Notify_stream : public std::ostream
{
	private:
		static unsigned int instance_count;
		class stream_buf: public std::stringbuf
		{
			private:
				bool is_first;
				std::string first;
				std::string second;
				NotifyUrgency urgency;

			public:
				unsigned int timeout;
				stream_buf(NotifyUrgency urgency, unsigned int timeout) : is_first(true), first(), second(), urgency(urgency), timeout(timeout) {}

				virtual int sync()
				{
					NotifyNotification* notification = notify_notification_new(first.c_str(), second.c_str(), "dialog-information");

					notify_notification_set_timeout(notification, timeout);
					notify_notification_set_urgency(notification, urgency);
					notify_notification_show(notification, NULL);
					g_object_unref(G_OBJECT(notification));
					second.clear();
					is_first = true;
					return 0;
				}

				std::streamsize xsputn(const char_type* string, std::streamsize length)
				{
					if( is_first )	// The first string is the headline...
					{
						first.assign(string, length);
						is_first = false;
					}
					else// ...all following strings make up the body.
					{
						second.append(string, length);
					}
					return length;
				}
		};

		stream_buf buffer;
	public:
		Notify_stream(NotifyUrgency urgency = NOTIFY_URGENCY_NORMAL, unsigned int timeout = NOTIFY_TIMEOUT) : std::ostream(&buffer), buffer(urgency, timeout)
		{
			if( !instance_count )
			{
				notify_init(NOTIFY_PROGRAM_NAME);
			}
			instance_count++;
		}
		~Notify_stream(void)
		{
			instance_count--;
			if( !instance_count )
			{
				notify_uninit();
			}
		}
		void timeout(unsigned int timeout)
		{
			buffer.timeout = timeout;
		}
};






/**
 * Partial specialisation that disables the output
 */
template<class CharT, class Traits>
class Notify_stream<CharT, Traits, false> : public std::ostream
{
	public:
		Notify_stream(NotifyUrgency urgency = NOTIFY_URGENCY_NORMAL)
		{
			(void) urgency;
		}

		/**
		 * The function handling formatting functions like std::endl.
		 * This function is actually needed and has to be inlined to allow the compiler to completely optimise it away.
		 */
		__attribute__((always_inline)) inline __ostream_type& operator<<(__ostream_type&(*__pf)(__ostream_type&))
		{
			(void) __pf;
			return *this;
		}

		/**
		 * The function handling normal data insertion into the stream.
		 * These functions are actually needed and have to be inlined to allow the compiler to completely optimise them away.
		 */
		template<typename T>
		__attribute__((always_inline)) inline Notify_stream<CharT, Traits, false>& operator<<(T value)
		{
			(void) value;
			return *this;
		}
};






/**
 * The magic to allow setting the timeout as a stream modifier. It is analogous to the implementation of std::setw.
 * These functions are actually needed and have to be inlined to allow the compiler to completely optimise them away.
 */
struct _Sett
{
	unsigned int timeout;
};

inline _Sett sett(unsigned int timeout)
{
	return
	{
		timeout
	};
}

template<typename CharT, typename Traits>
inline Notify_stream<CharT, Traits, true>& operator<<(Notify_stream<CharT, Traits, true>& stream, _Sett timeout)
{
	stream.timeout(timeout.timeout);
	return stream;
}

/**
 * The deactivated specialisation is actually needed and has to be inlined to allow the compiler to completely optimise them away.
 */
template<typename CharT, typename Traits>
__attribute__((always_inline)) inline Notify_stream<CharT, Traits, false>& operator<<(Notify_stream<CharT, Traits, false>& stream, _Sett)
{
	return stream;
}




extern Notify_stream<char, std::char_traits<char>, USE_NOTIFY_LOW> notify_low;
extern Notify_stream<char, std::char_traits<char>, USE_NOTIFY_NORMAL> notify_normal;
extern Notify_stream<char, std::char_traits<char>, USE_NOTIFY_CRITICAL> notify_critical;
