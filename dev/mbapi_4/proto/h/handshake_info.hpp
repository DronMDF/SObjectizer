/*
	MBAPI 4.
*/

/*!
	\file
	\brief �������� mbapi-������ ��� ��������� handshake.
*/

#if !defined( _MBAPI_4__PROTO__HANDSHAKE_INFO_HPP_ )
#define _MBAPI_4__PROTO__HANDSHAKE_INFO_HPP_

#include <set>
#include <string>

#include <mbapi_4/proto/h/req_info.hpp>

namespace mbapi_4
{

namespace proto
{

namespace handshake
{

//
// version_t
//

//! �������� ������ MBAPI.
struct version_t
	:
		public oess_2::stdsn::serializable_t
{
		OESS_SERIALIZER( version_t )

	public:
		//! ����� ���������.
		oess_2::uchar_t m_generation;
		//! ����� �����.
		oess_2::uchar_t m_branch;
		//! ����� ������.
		oess_2::uchar_t m_release;
		//! ����� ���������� ������.
		oess_2::uchar_t m_publish;

		//! ����������� ��-���������.
		version_t();
		//! ��������� ���������������� �����������.
		version_t(
			oess_2::uchar_t generation,
			oess_2::uchar_t branch,
			oess_2::uchar_t release,
			oess_2::uchar_t publish );
		virtual ~version_t();

		//! ��������� ������� ������ SObjectizer-�.
		static version_t
		current();
};

//
// compression_t
//

//! �������� �������������� ���������� ���������� ������.
struct compression_t
	:
		public oess_2::stdsn::serializable_t
{
		OESS_SERIALIZER( compression_t )

	public:
		//! �������������� �������� ��������� ����������.
		/*! ������ ��������� ���� �� ���� ������� -- ���, �������
			������ � m_preferred. */
		std::set< std::string >	m_supported;

		//! �������������� �������� �������� ����������.
		/*! �������� ������ ��������, ���� ���������� ��������������
			�� ������. */
		std::string m_preferred;

	//! \name �������������� MBAPI ��������� ����������.
	//! \{
		//! �������� zlib.
		/*! ��. http://www.gzip.org */
		static const std::string
		zlib();
	//! \}

		//! ����������� ��-���������.
		/*!
			������������� m_supported � m_preferred � ������ �������� --
			���������� �� ������������.
		*/
		compression_t();

		//! ���������������� �����������.
		/*! � m_supported ���������� ��� �������� �� \a supported
			� ������������� ���������� �������� \a preferred. */
		compression_t(
			//! ��� ��������������� ��������� ����������.
			const std::string & preferred,
			//! ������ �������������� ��������� ����������.
			const std::set< std::string > & supported );

		virtual ~compression_t();

		//! �������� ����, ��� ���������� ������������.
		/*! \return true, ���� ������ �������� m_preferred � ���
			�������� ��������� � m_supported. */
		operator bool() const;

		//! �������� ����, ��� ��������� �������� ������ � �����
		//! �������������� ����������.
		/*!
			\return true, ���� �������� ���������� � m_supported.
		*/
		bool
		is_supported( const std::string & name ) const;
};

} /* namespace handshake */

//
// handshake_info_t
//

/*!
	\brief ������ �������� handshake.

	� ���� m_version ���������� ������ MBAPI-� �� �������
	������� (������ ���������� �������� handshake).
*/
class handshake_info_t
	:
		public req_info_t
{
		OESS_SERIALIZER( handshake_info_t )

	public:
		//! ����������� �� ���������.
		/*! � �������� ������ ������ ������������ ��������
			mbapi_4::proto::handshake::version_t::current(). */
		handshake_info_t();

		//! ���������������� �����������.
		handshake_info_t(
			//! �������������� ������.
			const handshake::version_t & version );

		virtual ~handshake_info_t();

		//! �������� ����� �������������� ������.
		const handshake::version_t
		version() const;

		//! �������� �������� ��������� ������ ����������.
		const handshake::compression_t
		compression() const;

		//! ���������� �������� ����� ����������.
		void
		set_compression(
			const handshake::compression_t & c );

		//! �������� ������������� ����.
		const std::string &
		node_uid() const;

		//! ���������� ������������� ����.
		void
		set_node_uid( const std::string & uid );

	private:
		//! ����� ������ MBAPI �� ������� �������.
		handshake::version_t	m_version;

		//! �������� ����� ����������.
		handshake::compression_t	m_compression;

		//! ������������� ���� � ������� ���������� �����.
		std::string m_node_uid;
};

//
// handshake_resp_info_t
//

/*!
	\brief ��������� �������� handshake.

	� ���� m_version ���������� ������ MBAPI-� �� �������
	������� (������ ������������ �������� handshake).
*/
class handshake_resp_info_t
	:
		public resp_info_t
{
		OESS_SERIALIZER( handshake_resp_info_t )

	public:
		//! ����������� �� ���������.
		/*! � �������� ������ ������ ������������ ��������
			mbapi_4::proto::handshake::version_t::current(). */
		handshake_resp_info_t();
		//! ���������������� �����������.
		handshake_resp_info_t(
			//! ������ MBAPI �� ������� �������.
			const handshake::version_t & version );
		virtual ~handshake_resp_info_t();

		//! �������� ����� ������ �������.
		const handshake::version_t
		version() const;

		//! ������ �� �������������� ����������?
		/*! \return true, ���� ����� ���������� ����� � �����
			�������� ��� �������� ������� compression_algorithm(). */
		bool
		is_compression_defined() const;

		//! ��������� �������� �������� ����������.
		/*!
			\note ���������� ���������� �������� ������, ����
				true == is_compression_defined().
		*/
		const std::string &
		compression_algorithm() const;

		//! ���������� �������� ���������� �������� ��������� ����������.
		/*!
			������ �������� ���������, ��� ���������� �� ������������.
		*/

		void
		set_compression_algorithm(
			const std::string & name );

		//! �������� ������������� ����.
		const std::string &
		node_uid() const;

		//! ���������� ������������� ����.
		void
		set_node_uid( const std::string & uid );

	private:
		//! ����� ������ MBAPI �� ������� �������.
		handshake::version_t m_version;

		//! ��� ���������� �������� ��������� ����������.
		/*!
			������ �������� ���������, ��� ���������� �� ������������.
		*/
		std::string m_compression_algorithm;

		//! ������������� ���� � ������� ���������� �����.
		std::string m_node_uid;
};

} /* namespace proto */

} /* namespace mbapi_4 */

#endif
