/*
	MBAPI 4.
*/

/*!
	\file
	\brief ����� ��� ������ � ����������� ������� ��������� MBAPI.
*/

#if !defined( _MBAPI_4__PROTO__PACKAGE_HEADER_HPP_ )
#define _MBAPI_4__PROTO__PACKAGE_HEADER_HPP_

#include <oess_2/io/h/binstream.hpp>

namespace mbapi_4
{

namespace proto
{

//
// package_header_t
//

/*!
	\brief ����� ��� ��������� ��������� mbapi-������.

	������ ������ ������ � SOP ����������� � ���� ������ ��
	����������� ���������� � ������. ������ ��������� ��������
	������ �������������� ����� ������ � ������ � �����������
	����������� ����.

	������ �������������� ����� ������������ ��� ����������
	������� ������ �� ����������������� ������. �.�. �������
	����������� ��������� ������, ������� ����� �������
	������������ ������. ����� ����������� ������� ����, �������
	������� � ��������� ������. ����� ���� ����� ���������
	��������� �����������.

	����������� ���� ������������ ��� �������� ����, ���
	�������� ������������� mbapi ����� ���������� ������.
*/
class package_header_t
{
	public:
		//! 32-������� ����������� �����.
		typedef unsigned int	uint32_t;

		enum
		{
			//! ������ ��������� ������������� �������
			//! ������ � ������.
			IMAGE_SIZE = 8
		};

	public:
		//
		// magic_number
		//

		//! ��������� �������� ��� �������������� magic_number.
		/*!
			������������ ��� ���������� ���������������� ������
			������������ � ����� ����������.
		*/
		struct magic_number_t
		{
			//! ������������� �������� ��� magic_number.
			uint32_t m_value;

			//! ���������������� �����������.
			explicit inline
			magic_number_t( uint32_t value )
				:
					m_value( value )
			{}
		};

		//! �����������, ������� ��������� ���������� ������ ��������
		//! magic_number.
		package_header_t( const magic_number_t & value );

		//! �����������, ������� ��������� ���������� ��������
		//! magic_number � ����� �������������� ����� ������.
		package_header_t(
			//! �������� magic_number.
			const magic_number_t & value,
			//! ������ �������������� ����� ������ � ������.
			uint32_t package_size );

		//! �������� ������������ ��������� ������.
		/*!
			\return true, ���� ��������� �������� ����������.
		*/
		bool
		is_valid() const;

		//! ����� �������������� ����� ������ � ������.
		uint32_t
		size() const;

		//! ���������� ����� �������������� ����� ������.
		void
		set_size(
			//! ������ � ������.
			uint32_t package_size );

		//! ��������� ��������� ������ �� ���������� ������.
		void
		read(
			//! ������ �������������� � ������� �������.
			oess_2::io::ibinstream_t & from );

		//! �������� ��������� ������ � ��������� �����.
		void
		write(
			//! ������ �������������� � ������� �������.
			oess_2::io::obinstream_t & to ) const;

		/*!
		 * \brief �������� ����������� �������������.
		 *
		 * ������������ ��� ������ ���������� ����������.
		 */
		std::string
		debug_string_representation() const;

	private:
		//! �������� magic_number.
		uint32_t	m_magic_number;
		//! ������ �������������� ����� ������ � ������.
		uint32_t	m_size;
		//! ����������� ����.
		uint32_t	m_check_field;

		//! ����������� �������� ������������ ����.
		void
		calc_check_field();
};

//! ����� ������� ������ ������ ��������� ������ � �����.
inline oess_2::io::obinstream_t &
operator << (
	oess_2::io::obinstream_t & o,
	const package_header_t & h )
{
	h.write( o );
	return o;
}

//! ����� ������� ������ ������ ��������� ������ �� ������.
inline oess_2::io::ibinstream_t &
operator >> (
	oess_2::io::ibinstream_t & o,
	package_header_t & h )
{
	h.read( o );
	return o;
}

} /* namespace proto */

} /* namespace mbapi_4 */

#endif
