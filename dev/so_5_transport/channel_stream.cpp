/*
	SObjectizer 5 Transport.
*/

#include <cstring>
#include <algorithm>

#include <ace/OS.h>

#include <so_5/h/exception.hpp>

#include <so_5_transport/h/ret_code.hpp>
#include <so_5_transport/h/channel_stream.hpp>

namespace so_5_transport
{

channel_input_stream_t::channel_input_stream_t(
	const scattered_block_stream_ref_t & scattered_block_stream )
	:
		m_block_source( scattered_block_stream )
{
	reset_transaction_data();
}

channel_input_stream_t::~channel_input_stream_t()
{
	// ���� ���-�� �������� �� ������������,
	// �� ��� �������� � �����.
	rollback_transaction();
}

void
channel_input_stream_t::begin_transaction()
{
	// ���� �����-�� ���������� � ����� �����,
	// ��� ����������.
	rollback_transaction();

	// �������� ��� ����� ��������� � ������.
	m_bytes_initially_available = m_bytes_available =
		m_block_source->stream_query_all_blocks( m_untouched_list );

	if( !m_untouched_list.empty() )
	{
		m_current_block = m_untouched_list.front();
		m_untouched_list.pop_front();

		// ��� ������ ���������� ������� � ������ ����� ����������
		// ��������������, ������� ���������� �������� ������� ������� �����.
		m_first_block_initial_pos = m_current_block->read_pos();
	}
	else
	{
		// ������ ��������� ������, ������� ���� �����������.
		m_current_block = 0;
	}
}

void
channel_input_stream_t::checkpoint()
{
	// ���� ������� ���� ����.
	if( m_current_block )
	{
		// ���� �� �������� ����� �� ��� �������.
		if( m_current_block->read_size_available() > 0 )
		{
			// � �� �� �� ����� ���������,
			// �� ��������� ��� � ������ ������ ����������.
			m_untouched_list.push_front(
				m_current_block );
		}
		else
		{
			// ����� ��������� ��� � ��������.
			m_touched_list.push_back(
				m_current_block );
		}
		m_current_block = 0;
	}

	// ����������� �������� �����.
	// � ���������� �������� ��� ����������� ����������.
	m_block_source->release( m_touched_list );
	m_touched_list.clear();

	// ���������� ��������� ���������� ����
	// ��� ����� ��, ��� �� ������ ������ ��������.
	m_bytes_initially_available = m_bytes_available;

	if( !m_untouched_list.empty() )
	{
		m_current_block = m_untouched_list.front();
		m_untouched_list.pop_front();

		// ��� ������ ���������� ������� � ������ ����� ����������
		// ��������������, ������� ���������� �������� ������� ������� �����.
		m_first_block_initial_pos = m_current_block->read_pos();
	}
	else
	{
		// ������ ��������� ������, ������� ���� �����������.
		m_current_block = 0;
	}
}

void
channel_input_stream_t::commit_transaction()
{
	// ���� ������� ���� ����.
	if( m_current_block )
	{
		// ���� �� �������� ����� �� ��� �������.
		if( m_current_block->read_size_available() > 0 )
		{
			// � �� �� �� ����� ���������,
			// �� ��������� ��� � ������ ������ ����������.
			m_untouched_list.push_front(
				m_current_block );
		}
		else
		{
			// ����� ��������� ��� � ��������.
			m_touched_list.push_back(
				m_current_block );
		}
	}

	// ����������� �������� �����.
	m_block_source->release( m_touched_list );

	// ���������� ����� ��������� ������� � ������ ������.
	// ������ ������ ���� ����� ����� ������ ������ �� � ������ �����.
	m_block_source->stream_push_front( m_untouched_list, m_bytes_available );

	reset_transaction_data();
}

void
channel_input_stream_t::rollback_transaction()
{
	// ��������� ������� � ����������, ���� �� ����.
	if( m_current_block )
	{
		m_current_block->reset_read_pos();
		m_untouched_list.push_front(
			m_current_block );
	}

	// �������� ������: �������� ++ ����������.
	m_untouched_list.splice(
		m_untouched_list.begin(),
		m_touched_list );

	if( !m_untouched_list.empty() )
	{
		// ������� ����� ���� ��������� ��������� ������
		// ����� ��� �� ��� ��������.
		m_untouched_list.front()->shift_read_pos(
			m_first_block_initial_pos );
	}

	m_block_source->stream_push_front(
		m_untouched_list,
		m_bytes_initially_available );

	reset_transaction_data();
}

size_t
channel_input_stream_t::bytes_available() const
{
	return m_bytes_available;
}

bool
channel_input_stream_t::eof() const
{
	return m_bytes_available == 0;
}

const oess_2::char_t *
channel_input_stream_t::in_reserve(
	size_t item_count,
	size_t item_size,
	size_t & item_available )
{
	if( m_bytes_available < item_size )
	{
		// �������� ����������.
		rollback_transaction();

		throw so_5::exception_t(
			"not enough data in buffer to read",
			rc_not_enough_data_to_read_from_buffer );
	}

	// ���� ��� ����� ��� ��������� ������ ������ ��������.
	// ����� ������� ���� ������� �� ���������� �����,
	// � ������� ���.
	if( m_current_block->read_size_available() < item_size )
	{
		copy_devided_item_to_internal_buffer( item_size );
		item_available = 1;
		return m_internal_buf.data();
	}

	item_available = std::min< size_t >(
		m_current_block->read_size_available() / item_size,
		item_count );

	return m_current_block->current_read_ptr();
}

void
channel_input_stream_t::copy_devided_item_to_internal_buffer(
	size_t item_size )
{
	// ������� ���� �������� �����������.
	size_t bytes_to_copy = item_size;

	// ������� ���� ������ ���������� ������.
	raw_block_t * block = m_current_block;

	// �������� �� ��������� ����.
	raw_block_chain_t::iterator it_next = m_untouched_list.begin();

	// ���� ������ ������.
	oess_1::char_t * write_ptr = m_internal_buf.data();

	while( bytes_to_copy > 0 )
	{
		const size_t bytes_to_take_out_of_current_block =
			std::min< size_t >(
				block->read_size_available(),
				bytes_to_copy );

		std::memcpy(
			write_ptr,
			block->current_read_ptr(),
			bytes_to_take_out_of_current_block );

		bytes_to_copy -= bytes_to_take_out_of_current_block;
		write_ptr += bytes_to_take_out_of_current_block;

		// ���� ��� ���-�� �������� ��������.
		// ����� ��������� ����.
		if( bytes_to_copy > 0 )
			block = *(it_next++);
	}
}

void
channel_input_stream_t::in_shift(
	size_t item_count,
	size_t item_size )
{
	size_t total_size = item_count * item_size;
	m_bytes_available -= total_size;


	while( total_size &&
		total_size >= m_current_block->read_size_available() )
	{
		// �������� ������� ���� ��������
		// �� �������� �����.
		total_size -= m_current_block->read_size_available();

		// ���������� ���������� ������.
		// ��� ���� ����� ��� ������ ����������
		// ������� � ����� ����� � ������������� ���������.
		// ������� � ������ ����� ���������� ��� ������ ����������.
		m_current_block->reset_read_pos();

		m_touched_list.push_back( m_current_block );

		// ���� ���� ��� �����, �� ����� ���������.
		if( !m_untouched_list.empty() )
		{
			m_current_block = m_untouched_list.front();
			m_untouched_list.pop_front();
		}
		else
			m_current_block = 0;
	}

	// ���� ��� ��� �� �����, �� �������� ��������� ������ �������
	// �����.
	if( m_current_block )
		m_current_block->shift_read_pos( total_size );

}

void
channel_input_stream_t::reset_transaction_data()
{
	m_bytes_initially_available = 0;
	m_bytes_available = 0;
	m_first_block_initial_pos = 0;
	m_current_block = 0;
	m_touched_list.clear();
	m_untouched_list.clear();
}


//
// channel_output_stream_t
//

channel_output_stream_t::channel_output_stream_t(
	const scattered_block_stream_ref_t & scattered_block_stream,
	const ACE_Time_Value & free_blocks_awaiting_timeout )
	:
		m_trx_running( false ),
		m_block_destination( scattered_block_stream ),
		m_free_blocks_awaiting_timeout( free_blocks_awaiting_timeout ),
		m_current_block( 0 )
{
}

channel_output_stream_t::~channel_output_stream_t()
{
	rollback_transaction();
}

void
channel_output_stream_t::begin_transaction()
{
	// �������� ������� ����������, ���� ��� ����� �����.
	if( m_trx_running )
		rollback_transaction();

	m_trx_running = true;
	m_bytes_written = 0;
}

void
channel_output_stream_t::checkpoint()
{
	if( m_trx_running )
	{
		flush();
		m_bytes_written = 0;
	}
}

void
channel_output_stream_t::commit_transaction()
{
	if( m_trx_running )
	{
		flush();
		m_trx_running = false;
	}
}

void
channel_output_stream_t::rollback_transaction()
{
	if( m_trx_running )
	{
		release_blocks();
		m_trx_running = false;
	}
}

size_t
channel_output_stream_t::bytes_written() const
{
	return m_bytes_written;
}

void
channel_output_stream_t::flush()
{
	if( m_current_block != 0 )
	{
		// � ������� ������ ���������
		// �������.

		m_ready_blocks.push_back( m_current_block );
		m_current_block = 0;
	}

	if( !m_ready_blocks.empty() )
	{
		m_block_destination->stream_push_back( m_ready_blocks );
		m_block_destination->flush();

		m_ready_blocks.clear();
	}
}

void
handle_trx_is_not_running()
{
	throw so_5::exception_t(
		"transaction isn't started",
		rc_transactiorn_not_started );
}

void
handle_no_free_blocks_available()
{
	throw so_5::exception_t(
		"no free blocks available",
		rc_no_free_blocks_available_in_buffer );
}


void
channel_output_stream_t::occupy_next_block()
{
	if( !m_trx_running )
		handle_trx_is_not_running();

	if( m_current_block )
	{
		m_ready_blocks.push_back( m_current_block );
		m_current_block = 0;
	}

	const ACE_Time_Value before = ACE_OS::gettimeofday();

	bool was_waiting_for_free_blocks;
	m_current_block = m_block_destination->occupy(
		was_waiting_for_free_blocks,
		&m_free_blocks_awaiting_timeout );

	const ACE_Time_Value after = ACE_OS::gettimeofday();

	// ���� ����� ����� �������� ��������� ������,
	// �� ��������� ����� ����������� �� ��������.
	if( was_waiting_for_free_blocks )
	{
		const ACE_Time_Value duration = after - before;

		if( m_free_blocks_awaiting_timeout > duration )
			m_free_blocks_awaiting_timeout -= duration;
		else
			m_free_blocks_awaiting_timeout = ACE_Time_Value::zero;
	}

	if( !m_current_block )
		handle_no_free_blocks_available();
}

void
channel_output_stream_t::release_blocks()
{
	if( m_current_block )
	{
		m_ready_blocks.push_back( m_current_block );
		m_current_block = 0;
	}

	m_block_destination->release( m_ready_blocks );
	m_ready_blocks.clear();
}

oess_1::char_t *
channel_output_stream_t::out_reserve(
	size_t item_count,
	size_t item_size,
	size_t & item_available )
{
	// ���� ������ ������, �� ���� �������� ����.
	if( !m_current_block ||
		0 == m_current_block->write_size_available() )
	{
		occupy_next_block();
	}

	if( m_current_block->write_size_available() < item_size )
	{
		// ���� � ������� ����� ������������ ����� ��� ������
		// ������ ��������, �� ������� ��� � �����.
		// ������ ����� �������� ��������� � ������� ����,
		// � ������ ����� ��������� � ��������� ����
		item_available = 1;
		return m_internal_buf.data();
	}

	item_available = std::min< size_t >(
		m_current_block->write_size_available() / item_size,
		item_count );

	return m_current_block->current_write_ptr();
}

void
channel_output_stream_t::out_shift(
	size_t item_count,
	size_t item_size )
{
	m_bytes_written += item_count * item_size;

	if( m_current_block->write_size_available() < item_size )
	{
		// ������ ������ ��� ������ ����� � �����
		// � ��� ��� ������ 1 �������.
		size_t bytes_to_write = item_size;
		oess_2::char_t * src = m_internal_buf.data();

		while( bytes_to_write > 0 )
		{
			size_t how_many_can_i_write =
				std::min< size_t >(
					m_current_block->write_size_available(),
					bytes_to_write );

			std::memcpy(
				m_current_block->current_write_ptr(),
				src,
				how_many_can_i_write );

			m_current_block->shift_write_pos( how_many_can_i_write );

			// �������� ������� �� ��������.
			bytes_to_write -= how_many_can_i_write;
			// �������� ��������� �� �������� ������� �������.
			src += how_many_can_i_write;

			// ���� ���� ���������, ���� �� ��� �� ��� ��������,
			// �� ������� ��������� ����.
			if( bytes_to_write > 0 )
				occupy_next_block();
		}
	}
	else
	{
		// ������ ��� � ������� ����.
		m_current_block->shift_write_pos( item_count*item_size );
	}
}


} /* namespace so_5_transport */
