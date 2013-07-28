/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ����� event_exception_logger_t.
*/

#if !defined( _SO_5__RT__EVENT_EXCEPTION_LOGGER_HPP_ )
#define _SO_5__RT__EVENT_EXCEPTION_LOGGER_HPP_

#include <exception>
#include <string>
#include <memory>

#include <so_5/h/declspec.hpp>

namespace so_5
{

namespace rt
{

//
// event_exception_logger_t
//

class event_exception_logger_t;

//! ��������� �� ����� ��������� � event_exception_logger_t.
typedef std::unique_ptr< event_exception_logger_t >
	event_exception_logger_unique_ptr_t;

//! ��������� �������, ������� �������� ����������.
class SO_5_TYPE event_exception_logger_t
{
	public:
		virtual ~event_exception_logger_t();

		/*!
			\brief ����� ����������� ����������.
			����������� ������������ ������������ ������.
		*/
		virtual void
		log_exception(
			//! ������ �� ��������� ���������� ����������.
			const std::exception & event_exception,
			//! ��� ����������, ������� ����������� �����.
			const std::string & coop_name ) = 0;

		//! ��������� �����������.
		/*! ���� ������� ������������ ��������� � �� �����, �����
			��� ���� ������������, �� ����� ������ ������,
			��� ������ �� ������.
			���������� �� ��������� ������� ������ ������������.
		*/
		virtual void
		on_install(
			//! ���������� ������������.
			event_exception_logger_unique_ptr_t && previous_logger );
};

//
// create_std_event_exception_logger
//

//! ������� ����� ���������� �� ���������.
CPP_UTIL_2_EXPORT_FUNC_SPEC( event_exception_logger_unique_ptr_t )
create_std_event_exception_logger();

} /* namespace rt */

} /* namespace so_5 */

#endif
