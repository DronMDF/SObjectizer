/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ��������� ������� ������.
*/

#if !defined( _SO_5_TRANSPORT__CHANNEL_IO_HPP_ )
#define _SO_5_TRANSPORT__CHANNEL_IO_HPP_

#include <so_5_transport/h/declspec.hpp>

#include <so_5_transport/h/scattered_block_stream.hpp>
#include <so_5_transport/h/channel_stream.hpp>
#include <so_5_transport/h/io_trx.hpp>

namespace so_5_transport
{

//
// channel_io_t
//

//! ����� ��� ��������� ������� ������.
/*!
	����� � ����������� ���������� ���������� ������ ������ ������.
*/
class SO_5_TRANSPORT_TYPE channel_io_t
{
	public:
		channel_io_t();
		channel_io_t(
			const scattered_block_stream_ref_t & incoming,
			const scattered_block_stream_ref_t & outgoing );
		~channel_io_t();


		//! \name ��������� ��� �������� ������� ������/������.
		//! \{

		//! ������� ����� ��� ������.
		input_trx_unique_ptr_t
		begin_input_trx() const;

		//! ������� ����� ��� ������.
		output_trx_unique_ptr_t
		begin_output_trx(
			//! ����� ������� ����� ��������� �� ��������
			//! ��������� ������ � ������ �� ���������� ��� ����������
			//! � ���� ������.
			const ACE_Time_Value &
				free_blocks_awaiting_timeout = ACE_Time_Value::zero ) const;
		//! \}

		// //! ���������� ��������� ������ ������.
		// /*!
			// ���������� � ������ �������.
			// ����� ��� ���� ����� ����� ���������� ���������
			// msg_channel_created �������� �� ��������� ���� ������.
		// */
		// static void
		// set_streams(
			// channel_io_t & channel_io,
			// const scattered_block_stream_ref_t & in,
			// const scattered_block_stream_ref_t & out );

		// //! ���������� ��������� �������� ����� ������.
		// static void
		// set_incoming_stream(
			// channel_io_t & channel_io,
			// const scattered_block_stream_ref_t & in );

		// //! ���������� ��������� ��������� ����� ������.
		// static void
		// set_outgoing_stream(
			// channel_io_t & channel_io,
			// const scattered_block_stream_ref_t & out );


		//! \name ����������� ������ ��� ������ ������� � ������� �������.
		//! \{
		static inline scattered_block_stream_ref_t &
		access_input(
			channel_io_t & io )
		{
			return io.m_input;
		}

		static inline scattered_block_stream_ref_t &
		access_output(
			channel_io_t & io )
		{
			return io.m_output;
		}

		//! \}

	private:
		// //! \name ���������� ��������� ������ ������.
		// //! \{
		// void
		// set_incoming_stream(
			// const scattered_block_stream_ref_t & in );

		// void
		// set_outgoing_stream(
			// const scattered_block_stream_ref_t & out );
		// //! \}

		//! ������� ����� ��� ������.
		scattered_block_stream_ref_t m_input;

		//! ������� ����� ��� ������.
		scattered_block_stream_ref_t m_output;
};


} /* namespace so_5_transport */

#endif
