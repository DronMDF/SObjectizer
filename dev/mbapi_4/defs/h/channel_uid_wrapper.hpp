/*
	MBAPI 4.
*/

/*!
	\file
	\brief ������� ��� ����������� �������������� ������.
*/

#if !defined( _MBAPI_4__DEFS__CHANNEL_UID_WRAPPER_HPP_ )
#define _MBAPI_4__DEFS__CHANNEL_UID_WRAPPER_HPP_

#include <string>

#include <mbapi_4/defs/h/declspec.hpp>

namespace mbapi_4
{

//
// channel_uid_wrapper_t
//

//! ������� ��� uid-�� ������.
/*!
	Mbapi ���� ������������ ����� ����� �������������
	����� ����� mbapi-������. ���� ��� ���� ����� ����������
	����� ��� ���� ����������, �� ������ ���������� ��������� ��������� �������.
	�������� ���� ���� A ������ ��������� �����, � ���� B
	��������� � ��� ��� ���������� ����������, �� ������ ����������
	����� ���������� ��������, ��� �� ������� A, ��� � �� ������� B.
	������������� ������ ����� �������� ������ ������ ������� ����,
	�� ������� �� ����������, � ������������ ���������� ������
	� ������ ����������� mbapi-����.

	��� ������������� ��������� ����� ������� ����,
	���������� ������� "���������" �����, ������� ������, ���
	����� ������������� �� ������ ���� � ��� ��������
	��������� ����� ������ �� ����� ���������� �������� �� ����.
*/
class MBAPI_4_DEFS_TYPE channel_uid_wrapper_t
{
	public:
		channel_uid_wrapper_t();

		explicit channel_uid_wrapper_t(
			const std::string & uid );

		~channel_uid_wrapper_t();

		inline bool
		operator < ( const channel_uid_wrapper_t & channel_uid ) const
		{
			return m_uid < channel_uid.m_uid;
		}

		inline bool
		operator == ( const channel_uid_wrapper_t & channel_uid ) const
		{
			return m_uid == channel_uid.m_uid;
		}

		inline bool
		operator != ( const channel_uid_wrapper_t & channel_uid ) const
		{
			return m_uid != channel_uid.m_uid;
		}

		//! �������� �� ����� ���������.
		inline bool
		is_local() const
		{
			return m_uid.empty();
		}

		//! ��������� ������������� �������������� ����.
		inline const std::string &
		str() const
		{
			return m_uid;
		}

	private:
		//! ������������� ������.
		std::string m_uid;
};

} /* namespace mbapi_4 */

#endif
