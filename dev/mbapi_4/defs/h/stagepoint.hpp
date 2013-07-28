/*
	MBAPI 4.
*/

/*!
	\file
	\brief ������������� ������.
*/

#if !defined( _MBAPI_4__STAGEPOINT_HPP_ )
#define _MBAPI_4__STAGEPOINT_HPP_

#include <iosfwd>

#include <string>
#include <vector>

#include <mbapi_4/defs/h/declspec.hpp>

namespace mbapi_4
{

class endpoint_t;

//
// stagepoint_t
//

//! ������������� ������ �������� �����.
/*!
	����� ������ ������, ��� ��� ������������� �����-������,
	��� � ��� ������������� ����� �������� �����, � ��������� ������
	��� ������ ��������� � ������ �������� �����.

	��� ������ ����� �������� ������ ��:
	\li ���� ����������� �������� a..zA..Z;
	\li ���� 0..9;
	\li �������� "_-./|#$".
*/
class MBAPI_4_DEFS_TYPE stagepoint_t
{
	public:
		stagepoint_t();

		stagepoint_t(
			//! ��� ������.
			const std::string & stagepoint_name,
			//! ��� �������� �����.
			const std::string & endpoint_name );

		//! ������� ������, ������� ������������� ����� �������� �����.
		stagepoint_t(
			//! ��� ������.
			const endpoint_t & endpoint );

		~stagepoint_t();

		//! ������� ������ �� ������.
		/*!
			������ ������ ������ ���� ���������:
				\li stagename@endpointname;
				\li endpointname.

			� ������ ������ ����������� ������������� ������,
			�� ������ ������, ������� ������������ �����
			���� �������� �����.
		*/
		static stagepoint_t
		create_from_string(
			const std::string & s );

		//! ��������� ���������.
		//! \{
		inline bool
		operator < ( const stagepoint_t & stagepoint ) const
		{
			if( m_endpoint_name != stagepoint.m_endpoint_name )
				return m_endpoint_name < stagepoint.m_endpoint_name;

			return m_stagepoint_name < stagepoint.m_stagepoint_name;
		}

		inline bool
		operator == ( const stagepoint_t & stagepoint ) const
		{
			return
				m_endpoint_name == stagepoint.m_endpoint_name &&
				m_stagepoint_name == stagepoint.m_stagepoint_name;
		}
		//! \}

		//! ��������� �������� �� ������ ��������, �.�.
		//! �������� ������.
		inline bool
		is_endpoint() const
		{
			return m_stagepoint_name == m_endpoint_name;
		}

		//! ��������� � ��������� ���� ������ � �������� �����.
		//! \{
		inline const std::string &
		name() const
		{
			return m_stagepoint_name;
		}

		inline void
		set_name(
			const std::string & name )
		{
			m_stagepoint_name = name;
		}

		inline const std::string &
		endpoint_name() const
		{
			return m_endpoint_name;
		}

		//! �������� ��� �������� �����.
		inline void
		set_endpoint_name(
			const std::string & endpoint_name )
		{
			m_endpoint_name = endpoint_name;
		}
		//! \}


	private:
		//! ��� ������.
		std::string m_stagepoint_name;

		//! ��� �������� �����.
		std::string m_endpoint_name;
};

} /* namespace mbapi_4 */

#endif
