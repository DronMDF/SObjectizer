/*
	MBAPI 4.
*/

/*!
	\file
	\brief ���������-������ ��� ������������� ������ �������� ����� � ����� ������
	� ����� ������ ������.
*/

#if !defined( _MBAPI_4__COMM__TABLES_SYNC_MESSAGE_HPP_ )
#define _MBAPI_4__COMM__TABLES_SYNC_MESSAGE_HPP_

#include <so_5/rt/h/rt.hpp>

#include <mbapi_4/h/declspec.hpp>

namespace mbapi_4
{

namespace comm
{

//! ���������-������ ��� �������������.
struct MBAPI_4_TYPE tables_sync_message_t
	:
		public so_5::rt::message_t
{
};

} /* namespace comm */

} /* namespace mbapi_4 */


#endif
