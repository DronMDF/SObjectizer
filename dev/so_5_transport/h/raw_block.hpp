/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ����� ����� ������.
*/

#if !defined( _SO_5_TRANSPORT__RAW_BLOCK_HPP_ )
#define _SO_5_TRANSPORT__RAW_BLOCK_HPP_

#include <vector>
#include <list>

#include <oess_2/defs/h/types.hpp>

#include <so_5_transport/h/declspec.hpp>

namespace so_5_transport
{

//
// raw_block_t
//

//! ���������������� ����-�����.
/*!
	������������� ���������������� ������ � ������ ������
	������������� �����.
*/
class SO_5_TRANSPORT_TYPE raw_block_t
{
		raw_block_t( const raw_block_t & o );
		void
		operator = ( const raw_block_t & o );

	public:
		//! ����������� � ������������ ������ �������.
		/*!
			��������� ��� ��������� ������� 0 ���� �� ����������.
		*/
		raw_block_t(
			/*! ������� ������ � ������.*/
			size_t capacity = 32 *1024 );

		~raw_block_t();

		//! ������� �����.
		/*!
			������� = ����� ���������� ������ + ����� ��������� ��� ������ ������.
		*/
		inline size_t
		capacity() const
		{
			return m_block.capacity();
		}

		//! ��������� �� ������ ����� ������.
		inline const oess_2::char_t *
		ptr() const
		{
			return m_buff_ptr;
		}

		//! ��������� �� ������ ����� ������, ����������� �����.
		inline oess_2::char_t *
		ptr()
		{
			return m_buff_ptr;
		}

		//! \name �������� ������� ������/������ ������ ������� �����.
		//! \{

		//! ����������� ���� � ������.
		inline void
		reset_write_pos()
		{
			// ��� ���� ����������� ���� ����������
			// ������� ������.
			reset_read_pos();

			m_write_pos = 0;
		}

		//! ����������� ���� � ������.
		inline void
		reset_read_pos()
		{
			m_read_pos = 0;
		}

		//! \}

		//! \name ��������� ��� ������ ������.
		//! \{

		//! ������� ������� ��� ������.
		inline size_t
		write_pos() const
		{
			return m_write_pos;
		}

		//! ���������� ���� ��������� ��� ������.
		inline size_t
		write_size_available() const
		{
			return capacity() - write_pos();
		}

		//! �������� ������� ������� ��� ������.
		/*!
			�������� ����������� ������ ���� n <= write_size_available().
		*/
		inline void
		shift_write_pos( size_t n )
		{
			if( n <= write_size_available() )
				m_write_pos += n;
		}

		//! ���������� ������� ������� ��� ������ � �����.
		inline void
		total_shift_write_pos()
		{
			m_write_pos = capacity();
		}

		//! ��������� �� ������� ������� ��� ������.
		inline oess_2::char_t *
		current_write_ptr()
		{
			return m_buff_ptr + write_pos();
		}

		//! \}

		//! \name ��������� ��� ������ ������.
		//! \{

		//! ������� ������� ��� ������.
		inline size_t
		read_pos() const
		{
			return m_read_pos;
		}

		//! ������� ������ ��������� ��� ������ ������.
		inline size_t
		read_size_available() const
		{
			return write_pos() - read_pos();
		}

		//! ��������  ������� ������� ��� ������.
		/*!
			�������� ����������� ������ ���� n <= read_size_available().
		*/
		inline void
		shift_read_pos( size_t n )
		{
			if( n <= read_size_available() )
				m_read_pos += n;
		}

		//! ���������� ������� ������� ��� ������ � �����.
		inline void
		total_shift_read_pos()
		{
			m_read_pos = write_pos();
		}

		//! ��������� �� ������� ������� ��� ������.
		inline const oess_2::char_t *
		current_read_ptr() const
		{
			return m_buff_ptr + read_pos();
		}
		//! \}

	private:
		//! ��������� ������, ��������� �� �������� ������� ����.
		std::vector< oess_2::char_t > m_block;

		oess_2::char_t * m_buff_ptr;

		//! ������� ��� ������ ������.
		size_t m_write_pos;

		//! ������� ��� ������ ������.
		size_t m_read_pos;
};

//! ��������� ��� ������� ������.
/*!
	������������ � scattered_block_stream_t.
	\see scattered_block_stream_t::occupy(), scattered_block_stream_t::release().
*/
typedef std::list< raw_block_t * > raw_block_chain_t;

} /* namespace so_5_transport */

#endif
