/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ��������� ������� ������/������.
*/

#if !defined( _SO_5_TRANSPORT__SOCKET__IMPL__IO_TIMESTAMPS_HPP_ )
#define _SO_5_TRANSPORT__SOCKET__IMPL__IO_TIMESTAMPS_HPP_

#include <ace/Time_Value.h>

namespace so_5_transport
{

namespace socket
{

namespace impl
{

//
// io_timestamps_t
//

//! ��������� ������� �������� ������/������.
class io_timestamps_t
{
	public:
		io_timestamps_t();

		//! �������� ����� ���������� ������.
		void
		mark_last_input();

		//! �������� ����� ��������� ������.
		void
		mark_last_output();

		//! ������� ������� ������ � ������� ���������� ������.
		ACE_Time_Value
		since_last_input() const;

		//! ������� ������� ������ � ������� ��������� ������.
		ACE_Time_Value
		since_last_output() const;

	private:
		//! ��������� ����� ������.
		ACE_Time_Value m_last_input;

		//! ��������� ����� ������.
		ACE_Time_Value m_last_output;
};

} /* namespace impl */

} /* namespace socket */

} /* namespace so_5_transport */

#endif
