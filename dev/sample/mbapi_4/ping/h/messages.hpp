/*
	MBAPI 4 Sample
*/

/*!
	���� ������������ ����� �������� � �������� ���������.
*/

#if !defined( _SAMPLE__MBAPI_4__PING__MESSAGES_HPP_ )
#define _SAMPLE__MBAPI_4__PING__MESSAGES_HPP_

#include <string>

#include <oess_2/defs/h/types.hpp>
#include <oess_2/stdsn/h/serializable.hpp>

namespace sample_mbapi_4
{

struct ping_t
	:
		public oess_2::stdsn::serializable_t
{
		OESS_SERIALIZER( ping_t )

	public:
		//! ����� ����� ��������� ping.
		std::string m_timestamp;
		//! �����.
		oess_2::uint_t m_num;
};

struct pong_t
	:
		public oess_2::stdsn::serializable_t
{
		OESS_SERIALIZER( pong_t )

	public:
		//! ����� ����� ��������� pong.
		std::string m_timestamp;

		//! �����.
		oess_2::uint_t m_num;
};


} /* namespace sample_mbapi_4 */

#endif
