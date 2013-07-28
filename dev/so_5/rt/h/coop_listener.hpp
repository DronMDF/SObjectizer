/*
	SObjectizer 5.
*/

/*!
	\file
	\brief �������� ��������� ����������.
*/

#if !defined( _SO_5__RT__COOP_LISTENER_HPP_ )
#define _SO_5__RT__COOP_LISTENER_HPP_

#include <string>
#include <memory>

#include <so_5/h/declspec.hpp>

namespace so_5
{

namespace rt
{

class so_environment_t;

//
// coop_listener_t
//

//! ��������� �������� ������������ � ������������.
/*!
	������ ��� ��������� ������� �����������
	� ������������� ����������.

	������ ������������ �� ����������������,
	� ����� ����������� � ������ �����. ������� � ������
	������������� ������������� � ��� ������ ������������
	���������� �����������.
*/
class SO_5_TYPE coop_listener_t
{
	public:
		virtual ~coop_listener_t();

		//! ����� ��������� ����������� ����������.
		/*!
			���������� ����� ����, ��� ���������� ������� ����������������.
		*/
		virtual void
		on_registered(
			//! ����� Sobjectizer.
			so_environment_t & so_env,
			//! ��� ������������������ ����������.
			const std::string & coop_name ) throw() = 0;

		//! ����� ��������� ������������� ����������.
		/*!
			���������� ����� ����, ��� ���������� ������������ ����������������.
		*/
		virtual void
		on_deregistered(
			//! ����� Sobjectizer.
			so_environment_t & so_env,
			//! ��� ������������������ ����������.
			const std::string & coop_name ) throw() = 0;
};

//! ��������� unique_ptr ��� coop_listener_t.
typedef std::unique_ptr< coop_listener_t > coop_listener_unique_ptr_t;

} /* namespace rt */

} /* namespace so_5 */

#endif
