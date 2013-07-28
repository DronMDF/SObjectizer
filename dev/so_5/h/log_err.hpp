/*
	SObjectizer 5
*/

/*!
	\file
	\brief ��������������� �������.
*/

#ifndef _SO_5__LOG_ERR_HPP_
#define _SO_5__LOG_ERR_HPP_

#include <iostream>
#include <sstream>

#include <ace/Log_Msg.h>

/*!
	\brief ������ ��� �������� ��������� ������ ��� ������
	��������� ����� ACE logging.

	������ �������������:
	\code
	ACE_ERROR(( LM_ERROR, SO_5_LOG_FMT( "invalid cmd: %s" ), cmd ));
	\endcode
*/
#define SO_5_LOG_FMT(s) "[%D PID:%P TID:%t] %M -- " s " @%N:%l\n"

/*!
 * \brief ������ ��� �������� ���� �������� ACE-������� �
 * ���������� abort � ������ �������������.
 *
 * ������������ ��� ���������� ��������� � ������, ����� ��������
 * ����������� ��������, ������� �� ������ ��� ������. � �����
 * �������������� ����� �������� �������� ���������� (��������,
 * �� ������� ���������� ������� ����).
 *
 * ������ �������������:
 * \code
 * SO_5_ABORT_ON_ACE_ERROR(
 * 	ACE_Thread_Manager::instance()->spawn( *���������* ) );
 * \endcode
 */
#define SO_5_ABORT_ON_ACE_ERROR(action) do { \
	if( -1 == (action) ) {\
		ACE_ERROR((LM_EMERGENCY, SO_5_LOG_FMT( #action ))); \
		ACE_OS::abort(); \
	} \
} while(false)

#endif
