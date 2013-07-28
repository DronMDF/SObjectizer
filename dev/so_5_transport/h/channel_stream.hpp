/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ����� ��� ������������ ������� ��������.
*/
#if !defined( _SO_5_TRANSPORT__CHANNEL_STREAM_HPP_ )
#define _SO_5_TRANSPORT__CHANNEL_STREAM_HPP_

#include <memory>
#include <array>

#include <oess_2/io/h/binstream.hpp>

#include <so_5_transport/h/declspec.hpp>

#include <so_5_transport/h/scattered_block_stream.hpp>

namespace so_5_transport
{

//
// channel_input_stream_t
//

//! �������� oess_2::io::ibinstream_t ����� ������� ������ �� scattered_block_stream_t.
/*!
	������ � ������� ���������� �� ������� ���������� ������ ������.
	� ������� ���������� ����� ��������� ��� ������ �� ������,
	� ���������� ������� ���� �������� ��� ������.
	����� ������ ���������� ���������� ��� �� ����������� ����
	�������� ����� ����� bytes_available().

	���� ������������ ���������� �������� ������ ���� ���
	��������, �� ����� ��������� ����������, � ����������
	������ ����� ��������.

	����� ���� ��� �� ������ �������� ��� ������, �������
	������� ��������� ��������, ���� ����������� ����������
	����� ����� commit_transaction().

	����� ������������� ��� ������ ���������� ����� �� ��������
	������� ������, � ������� ������ �������� � ����������.
*/
class SO_5_TRANSPORT_TYPE channel_input_stream_t
	:
		public oess_2::io::ibinstream_t
{
		channel_input_stream_t(
			const channel_input_stream_t & );
		void
		operator = (
			const channel_input_stream_t & );

	public:
		channel_input_stream_t(
			const scattered_block_stream_ref_t & scattered_block_stream );
		virtual ~channel_input_stream_t();

		//! �������� �� ������ ������
		//! ��� ����� � ���������� ����� ������ ��� ����.
		/*!
			��� ������ ���������� ���������� ��������� ����������� �����.
		*/
		void
		begin_transaction();

		//! ���������� ����������� ����� (������������� ������������� ����������).
		/*!
			������������ ���������� ����������, ��
			��������� � ������ �������� � �������.
			���� ����� ����� ����� ����������� ������ ����������,
			�� ��� ���������� � ��������� ���������������
			��������� ����������� ������.
		*/
		void
		checkpoint();

		//! ����������� ���������� ������ ������.
		void
		commit_transaction();

		//! �������� ���������� ������ ������.
		/*!
			�������� ��� �������� ������ � ������ ����������
			� ������ �� ������� � �����.
		*/
		void
		rollback_transaction();

		//! ���������� ��������� � ������ ����.
		size_t
		bytes_available() const;

		//! ��������� �� ����� ������.
		virtual bool
		eof() const;

	private:
		//! \name ���������� ������� �������� ������.
		//! \{

		//! ����� ��� ����������� ���������, �� ��������
		//! ����� ��������� ������.
		virtual const oess_2::char_t *
		in_reserve(
			//! ���������� ���������, ������� �����
			//! ���������.
			size_t item_count,
			//! ������ ������ �������� � ������.
			size_t item_size,
			//! ���������� ���������, ��������� ��
			//! ������������� ���������.
			size_t & item_available );

		//! ��������������� ����� ��� �����������
		//! �� ���������� ������ ��������, �������
		//! ��������� � 2-�� ������.
		void
		copy_devided_item_to_internal_buffer(
			size_t item_size );

		//! �������� ���������.
		virtual void
		in_shift(
			//! ���������� ������� ����������� ���������.
			size_t item_count,
			//! ������ ������ �������� � ������.
			size_t item_size );
		//! \}

		//! �������� ������.
		scattered_block_stream_ref_t m_block_source;

		//! \name �������� � ����������.
		//! \{

		// ��������� ������ � ����������.
		void
		reset_transaction_data();

		//! ���������� ���� ������� ���������� ���� �������� � ������.
		size_t m_bytes_initially_available;

		//! ���������� ��������� ��� ������ � ������.
		size_t m_bytes_available;

		//! ��������� ������� ������� �����.
		/*!
			���� ���������� ���� ����� ��������,
			�� ������ ���� ������ ����� �� �� ������� � ������.
		*/
		size_t m_first_block_initial_pos;

		//! ������� ����.
		raw_block_t * m_current_block;

		//! ����� ������� ��� ��������,
		//! �� ���������� ��� �� ������������.
		raw_block_chain_t m_touched_list;

		//! ����� ������� ��� �� �������� � ������ ������� ����������.
		raw_block_chain_t m_untouched_list;
		//! \}

		/*!
			���������� ����� ��� ������, ������� ������������� ��
			������� 2-�� ������.

			\note � ������ ������ oess_2::io::ibinstream_t
			���������� ������ ����� �����, ��� ������ ������ ��������
			���� ����� �� ��������� 8 ���� (�������� double ��� long int),
			������� ������ � 16 ���� ���������� ��� �������� ��������
			������ ����, ��� �������� ���������� ������ ����� oess_2::io::ibinstream_t.
			���� �� �������� � oess_2::io::ibinstream_t ��������� ������ ����
			������� �� ��������� � 16 ���� �� ������ ������ ���� ����� ������.
		*/
		std::array< oess_2::char_t, 16 > m_internal_buf;
};

//! ��������� unique_ptr ��� channel_input_stream_t.
typedef std::unique_ptr< channel_input_stream_t >
	channel_input_stream_unique_ptr_t;

//
// channel_output_stream_t
//

//! ��������� oess_2::io ����� ����������� ������ � scattered_block_stream_t.
/*!

*/
class SO_5_TRANSPORT_TYPE channel_output_stream_t
	:
		public oess_2::io::obinstream_t
{
	public:

		channel_output_stream_t(
			const scattered_block_stream_ref_t & scattered_block_stream,
			//! ������� ������� ����� ��������� �� ��������
			//! ��������� ������.
			const ACE_Time_Value & free_blocks_awaiting_timeout );

		virtual ~channel_output_stream_t();

		//! ������ ���������� ������ ������.
		/*!
			�������� ����� � ��������� ���������� � ������ ������.
			\note ���� � ������ ������ �� ������ ����������� ����������,
			�� ��� ���������� � ����������� �����.
		*/
		void
		begin_transaction();

		//! ���������� ����������� ����� (������������� ������������� ����������).
		/*!
			��������� ����������� ����� � ����������� ������� � �����,
			��� ���� ��������� ���������� ���������� � ����������
			����� ������ ������.
			���� ����� ����� ����� ����������� ������ ����������,
			�� ��� ���������� � ��������� ���������������
			��������� ����������� ������.
		*/
		void
		checkpoint();

		//! ����������� ���������� ������ ������.
		/*!
			��������� ����������� ����� � ����������� ������� � �����.
		*/
		void
		commit_transaction();

		//! �������� ���������� ������ ������.
		/*!
			�������� ��� �������� ������ � ������ ����������
			� ����������� �����.
		*/
		void
		rollback_transaction();

		//! ������� ���� �������� � �����.
		/*!
			\note �������� ���������� �������� ������
			���� ���������� �������.
		*/
		size_t
		bytes_written() const;

	private:
		//! �������� ������ ������� � ��������.
		void
		flush();

		//! �������� ��������� ���� ��� ������ � ���� ������.
		void
		occupy_next_block();

		//! ���������� ��������� �����.
		void
		release_blocks();

		//! \name ���������� ������� �������� ������.
		//! \{

		//! ����� ��� ����������� ���������, � �������
		//! ����� ���������� ������.
		/*!
			���� ��� ������������ ������ ������ ��� ������
			���� �� ������ ��������, �� ������ �����������
			����������.
		*/
		virtual oess_2::char_t *
		out_reserve(
			//! ���������� ���������, ������� �����
			//! ��������.
			size_t item_count,
			//! ������ ������ �������� � ������.
			size_t item_size,
			//! ���������� ���������, ��������� ��� ������ ��
			//! ������������� ���������.
			size_t & item_available );

		//! �������� ���������.
		/*!
			���������� ����� ��������
			������ � �����, ��������� �� ������� ���
			��������� ������� out_reserve.
		*/
		virtual void
		out_shift(
			//! ���������� ������� ���������� ���������.
			size_t item_count,
			//! ������ ������ �������� � ������.
			size_t item_size );
		//! \}

		//! ����-���������: �������� �� ����� � �������� ���������� ����������.
		bool m_trx_running;

		//! ���������� ���� ���������� � �����.
		size_t m_bytes_written;

		//! ������ ����� ��������� ����� � ����� ���� ��������� ������.
		scattered_block_stream_ref_t m_block_destination;

		//! ������� ������� ����� ������� ������� ����� ������.
		ACE_Time_Value m_free_blocks_awaiting_timeout;

		//! ������� ����.
		raw_block_t * m_current_block;

		//! ����� ������� ��������, �� �� ����������.
		raw_block_chain_t m_ready_blocks;

		/*!
			���������� ����� ��� ������ ������, �������
			�� ����� ���� �������� � ���� ����.

			\note � ������ ������ oess_2::io::obinstream_t
			���������� ������ ����� �����, ��� ������ ������ ��������
			���� ����� �� ��������� 8 ���� (�������� double ��� long int),
			������� ������ � 16 ���� ���������� ��� ������ ��������
			������ ����, ��� �������� ���������� ������ �����
			oess_2::io::obinstream_t. ���� �� �������� �
			oess_2::io::obinstream_t ��������� ������ ����
			������� �� ��������� � 16 ���� �� ������ ������
			���� ����� ���������.
		*/
		std::array< oess_2::char_t, 16 > m_internal_buf;
};

//! ��������� unique_ptr ��� channel_output_stream_t.
typedef std::unique_ptr< channel_output_stream_t >
	channel_output_stream_unique_ptr_t;

} /* namespace so_5_transport */

#endif
