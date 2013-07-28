/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ��������� ����������� ����������, ������� ����� �������������
	� ���� ���������� �������.
*/

#if !defined( _SO_5__RT__EVENT_EXCEPTION_HANDLER_HPP_ )
#define _SO_5__RT__EVENT_EXCEPTION_HANDLER_HPP_

#include <memory>
#include <map>
#include <string>

#include <so_5/h/declspec.hpp>
#include <so_5/h/ret_code.hpp>

#include <so_5/rt/h/agent_ref.hpp>

namespace so_5
{

namespace rt
{

//
// event_exception_response_action_t
//

/*!
	\brief ������� ����� ��� ������� �� �������������
	���������� � ������� ������.

	������� ���������� ������� ������ ������������
	event_exception_handler_t ��� ��������� ����������.
*/
class SO_5_TYPE event_exception_response_action_t
{
	public:
		virtual ~event_exception_response_action_t();

		//! ��������� ��������-������� �� ��������� ����������.
		virtual void
		respond_to_exception() = 0;
};

//! ��������� �� ����� ��������� %event_exception_response_action_t.
typedef std::unique_ptr< event_exception_response_action_t >
	event_exception_response_action_unique_ptr_t;

//
// event_exception_handler_t
//

class event_exception_handler_t;

//! ��������� �� ����� ��������� � event_exception_handler_t.
typedef std::unique_ptr< event_exception_handler_t >
	event_exception_handler_unique_ptr_t;

class so_environment_t;

//
// dispatcher_t
//

//! ��������� �������, ������� ������������ ����������.
class SO_5_TYPE event_exception_handler_t
{
	public:
		virtual ~event_exception_handler_t();

		/*!
			\brief ����� ��������� ����������.
			������������ ���������� � ���������� ������,
			������� ��������� �������� ������� �� ����������.
		*/
		virtual event_exception_response_action_unique_ptr_t
		handle_exception(
			//! ����� SObjectizer.
			so_environment_t & so_environment,
			//! ������ �� ��������� ���������� ���������.
			const std::exception & event_exception,
			//! ��� ���������� ������� ����������� �����.
			const std::string & coop_name ) = 0;

		//! ��������� �����������.
		/*!
			���� ������� ���������� ��������� � �� �����, �����
			��� ���� ����������, �� ����� ���������� ������ ������,
			��� ������ �� ������.
			���������� �� ��������� ������� ������ ����������.
		*/
		virtual void
		on_install(
			//! ���������� ���������� ����������.
			event_exception_handler_unique_ptr_t && previous_handler );
};

//
// create_std_event_exception_handler
//

//! ������� ���������� ���������� �� ���������.
CPP_UTIL_2_EXPORT_FUNC_SPEC( event_exception_handler_unique_ptr_t )
create_std_event_exception_handler();

} /* namespace rt */

} /* namespace so_5 */

#endif
