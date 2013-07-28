/*
	MBAPI 4.
*/

/*!
	\file
	\brief �������� ����� ��� ������ � �������� ���������.
*/

#if !defined( _MBAPI_4__ENDPOINT_HPP_ )
#define _MBAPI_4__ENDPOINT_HPP_

#include <iosfwd>
#include <string>
#include <vector>

#include <mbapi_4/defs/h/declspec.hpp>

namespace mbapi_4
{

//
// endpoint_t
//

//! �������� �����.
/*!
	��� �������� ����� ����� �������� ������ ��:
	\li ���� ����������� �������� a..zA..Z;
	\li ���� 0..9;
	\li �������� "_-./|#$".
*/
class MBAPI_4_DEFS_TYPE endpoint_t
{
	public:
		endpoint_t();

		endpoint_t(
			//! ��� �������� �����
			const std::string & endpoint_name );

		//! �������� ��� �������� �����.
		inline const std::string &
		name() const
		{
			return m_endpoint_name;
		}

		//! ��������� ���������.
		//! \{
		inline bool
		operator < ( const endpoint_t & endpoint ) const
		{
			return m_endpoint_name < endpoint.m_endpoint_name;
		}

		inline bool
		operator == ( const endpoint_t & endpoint ) const
		{
			return m_endpoint_name == endpoint.m_endpoint_name;
		}
		//! \}

		//! �������� ��� �������� �����.
		void
		set_name(
			//! ��� �������� �����
			const std::string & endpoint_name );

	private:
		std::string m_endpoint_name;
};

//! ��� ��� ������ endpoint.
typedef std::vector< endpoint_t > endpoint_list_t;

//! ������ ������ �������� �����.
typedef std::vector< std::string > stage_list_t;

//
// endpoint_stage_chain_t
//

//! ���� �������� �����.
/*!
	��������� � mbapi ������������ �� ����� �������� ����� �� ������.
	� �������� ����� ����� ���� ��������� ������.
	������������ ��������� ������ ��������� ��� ������ ������ ���
	������� ���������� �������� ����� - ���������� ���������.
	���� ��������� ���������� �� �������� �������� �����, �� ��� �������
	�������� ����� ��� ������ ���� �������� ����� � ������ �������.
	���� ��������� ���������� � �������� �������� �����, �� ��� �������
	�������� ����� ������ ���� �������� ����� � �������� �������.

	����� ������ �� ������ ��������� � ������ �������� �����, � �����
	�� ������ �����������. � ��������� ������ ����� ������������� ����������.
*/
class MBAPI_4_DEFS_TYPE endpoint_stage_chain_t
{
	public:
		endpoint_stage_chain_t();

		endpoint_stage_chain_t(
			//! �������� �����.
			const endpoint_t & endpoint );

		endpoint_stage_chain_t(
			//! �������� �����.
			const endpoint_t & endpoint,
			//! ������ ������.
			const stage_list_t & stages );

		~endpoint_stage_chain_t();

		//! ������� endpoint_stage_chain_t �� ������.
		/*!
			������ � ��������� ����� � �������
			������ ����� ��������� ������:
			endpoint[stage1, stage2, ..., stageN].

			��������:
				\li ep[s1, s2, s3] - �������� ����� - "ep",
				�� ������ "s1", "s2", "s3";
				\li ep - �������� ����� - "ep", ������ �� �����.
				\li ep[] - �������� ����� - "ep", ������ �� �����.
		*/
		static endpoint_stage_chain_t
		create_from_string(
			const std::string & s );

		//! �������� �������� �����.
		inline const endpoint_t &
		endpoint() const
		{
			return m_endpoint;
		}

		//! �������� ��� �������� �����.
		inline void
		set_endpoint(
			const endpoint_t & endpoint )
		{
			m_endpoint = endpoint;
		}

		//! �������� ������ ������.
		//! \{
		inline const stage_list_t &
		stages() const
		{
			return m_stages;
		}
		//! \}

		//! ���������� ������ ������.
		void
		set_stages(
			const stage_list_t & stages );

	private:
		//! ��������� �� ��������� �� ��� �������� �����
		//! � ������ ����� �� ������, � ����� ������������
		//! ���� ����� ������ ����� �����.
		/*!
			� ������ ����������� ���������� ������������� ����������.
		*/
		void
		validate_stage_names() const;

		//! ��� �������� �����.
		endpoint_t m_endpoint;

		//! ������ ������ ���������� �������� ������.
		stage_list_t m_stages;
};

} /* namespace mbapi_4 */

#endif
