/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ����� nonempty_name_t.
*/

#if !defined( _SO_5__RT__NONEMPTY_NAME_HPP_ )
#define _SO_5__RT__NONEMPTY_NAME_HPP_

#include <string>

#include <so_5/h/declspec.hpp>

namespace so_5
{

namespace rt
{

//
// nonempty_name_t
//

//! ����� ��� ������������ �����, ������� �� ����� ���� ������.
/*!
	���� � ����������� ������� ���������� ������ ������,
	�� ����� ��������� ����������.
*/
class SO_5_TYPE nonempty_name_t
{
	public:
		//! ��� �� ������ ���� ������.
		nonempty_name_t(
			const char * name );

		//! ��� �� ������ ���� ������.
		nonempty_name_t(
			const std::string & name );

		~nonempty_name_t();

		//! �������� ���.
		inline const std::string &
		query_name() const
		{
			return m_nonempty_name;
		}

	private:
		//! ���.
		std::string m_nonempty_name;
};

} /* namespace rt */

} /* namespace so_5 */

#endif
