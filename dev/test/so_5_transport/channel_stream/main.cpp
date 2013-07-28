/*
	������������ ������� ������ ������.

	���� �����:
		��������� ������ � ������� �����������
		� ����������� �� �������������.
*/

#include <iostream>
#include <sstream>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cstdint>

#include <cpp_util_2/h/lexcast.hpp>
#include <cpp_util_2/h/defs.hpp>

#include <so_5/rt/h/rt.hpp>
#include <so_5_transport/h/channel_stream.hpp>

#include <utest_helper_1/h/helper.hpp>

so_5_transport::scattered_block_stream_ref_t
create_scatered_block_stream(
	size_t block_count,
	size_t max_block_count,
	size_t single_block_size )
{
	so_5_transport::scattered_block_stream_params_t params(
		block_count,
		max_block_count,
		single_block_size );

	return 	so_5_transport::scattered_block_stream_ref_t(
		new so_5_transport::scattered_block_stream_t( params ) );
}

void
quantity_test(
	so_5_transport::scattered_block_stream_ref_t & block_stream )
{
	// ����� ��� ������.
	so_5_transport::channel_input_stream_t
		incoming_stream( block_stream );

	// ����� ��� ������.
	so_5_transport::channel_output_stream_t
		outgoing_stream( block_stream, ACE_Time_Value::zero );

	// ���������� �� �����, ������ �� �����.
	for(oess_2::uint_t q = 1; q != 0; q <<= 1 )
	{
		oess_2::defs::quantity_t quantity( q );
		oess_2::defs::quantity_t res_quantity;

		outgoing_stream.begin_transaction();
		outgoing_stream.write( quantity );
		outgoing_stream.commit_transaction();

		// ������ � ������ ������ ���� ��������.
		UT_CHECK_CONDITION( true == block_stream->check_stream_has_blocks() );

		incoming_stream.begin_transaction();
		incoming_stream.read( res_quantity );
		incoming_stream.commit_transaction();

		UT_CHECK_EQ( quantity.value(), res_quantity.value() );

		// ������ � ������ �� ������ ���� ���������.
		UT_CHECK_CONDITION( false == block_stream->check_stream_has_blocks() );
	}

	// ���������� ��� ������� ���.
	outgoing_stream.begin_transaction();
	for(oess_2::uint_t q = 1; q != 0; q <<= 1 )
	{
		oess_2::defs::quantity_t quantity( q );

		outgoing_stream.write( quantity );
	}
	outgoing_stream.commit_transaction();

	// ������ � ������ ������ ���� ��������.
	UT_CHECK_CONDITION( true == block_stream->check_stream_has_blocks() );


	for(oess_2::uint_t q = 1; q != 0; q <<= 1 )
	{
		oess_2::defs::quantity_t quantity( q );
		oess_2::defs::quantity_t res_quantity;

		incoming_stream.begin_transaction();
		incoming_stream.read( res_quantity );
		incoming_stream.commit_transaction();

		UT_CHECK_EQ( quantity.value(), res_quantity.value() );

		// ���� ��� �� ��������� ��������,
		// �� � ������ ������ ���-�� ��������,
		// ���� �������� ���������, �� ��� ������ �� ������ ����.
		UT_CHECK_CONDITION( ( (q<<1) != 0 ) == block_stream->check_stream_has_blocks() );
	}

}

void
simple_string_test(
	so_5_transport::scattered_block_stream_ref_t & block_stream,
	UT_CONTEXT_DECL )
{
	UT_PUSH_CONTEXT( "testing strings" );

	// ����� ��� ������.
	so_5_transport::channel_input_stream_t
		incoming_stream( block_stream );

	// ����� ��� ������.
	so_5_transport::channel_output_stream_t
		outgoing_stream( block_stream, ACE_Time_Value::zero );

	{
		const std::string s1 = "123456789";
		outgoing_stream.begin_transaction();
		outgoing_stream.write( s1 );
		outgoing_stream.commit_transaction();

		std::string s2;
		incoming_stream.begin_transaction();
		incoming_stream.read( s2 );
		incoming_stream.commit_transaction();

		UT_CHECK_EQ( s2, s1 );
	}

	{
		const std::string s1 = "123456789123456789";
		outgoing_stream.begin_transaction();
		outgoing_stream.write( s1 );
		outgoing_stream.commit_transaction();

		std::string s2;
		incoming_stream.begin_transaction();
		incoming_stream.read( s2 );
		incoming_stream.commit_transaction();

		UT_CHECK_EQ( s2, s1 );
	}

	{
		const std::string s1 = "abcdefghijklmnopqrstuvwxyz"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		outgoing_stream.begin_transaction();
		outgoing_stream.write( s1 );
		outgoing_stream.commit_transaction();

		std::string s2;
		incoming_stream.begin_transaction();
		incoming_stream.read( s2 );
		incoming_stream.commit_transaction();

		UT_CHECK_EQ( s2, s1 );
	}

	{
		const std::string s1 = "abcdefghijklmnopqrstuvwxyz"
			"\n\r\t"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		outgoing_stream.begin_transaction();
		outgoing_stream.write( s1 );
		outgoing_stream.commit_transaction();

		std::string s2;
		incoming_stream.begin_transaction();
		incoming_stream.read( s2 );
		incoming_stream.commit_transaction();

		UT_CHECK_EQ( s2, s1 );
	}

	{
		const std::string s1 = "abcdefghijklmnopqrstuvwxyz"
			"\n\r\t"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		outgoing_stream.begin_transaction();
		outgoing_stream.write( s1 );
		outgoing_stream.commit_transaction();

		const std::string s2 =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"\n\r\t"
			"abcdefghijklmnopqrstuvwxyz";

		outgoing_stream.begin_transaction();
		outgoing_stream.write( s2 );
		outgoing_stream.commit_transaction();
		outgoing_stream.commit_transaction();

		std::string s3;
		std::string s4;
		incoming_stream.begin_transaction();
		incoming_stream.read( s3 );
		incoming_stream.read( s4 );
		incoming_stream.commit_transaction();

		UT_CHECK_EQ( s3, s1 );
		UT_CHECK_EQ( s4, s2 );
	}

	{
		const std::string s1 =
			"abcdefghijklmnopqrstuvwxyz"
			"\n\r\t"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		outgoing_stream.begin_transaction();
		outgoing_stream.write( s1 );
		outgoing_stream.commit_transaction();

		const std::string s2 =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"\n\r\t"
			"abcdefghijklmnopqrstuvwxyz";

		outgoing_stream.begin_transaction();
		outgoing_stream.write( s2 );
		outgoing_stream.commit_transaction();
		outgoing_stream.commit_transaction();

		std::string s3;
		std::string s4;

		incoming_stream.begin_transaction();
		incoming_stream.read( s3 );
		incoming_stream.begin_transaction();
		incoming_stream.read( s3 );
		incoming_stream.commit_transaction();
		incoming_stream.commit_transaction();

		incoming_stream.begin_transaction();
		incoming_stream.read( s4 );
		incoming_stream.commit_transaction();
		incoming_stream.commit_transaction();

		UT_CHECK_EQ( s1, s3 );
		UT_CHECK_EQ( s2, s4 );
	}

	UT_POP_CONTEXT();
}


class array_tester_base_t
{
	public:
		// ��������� ������ ���������� ����������.
		virtual void
		randomize() = 0;

		// ������ �������.
		virtual size_t
		array_size_in_bytes() const = 0;

		// �������� ������ � �����.
		virtual void
		write_to( oess_2::io::obinstream_t & o ) const = 0;

		// ������� ������ �� �����.
		virtual void
		read_from( oess_2::io::ibinstream_t & i ) const = 0;
};

typedef std::shared_ptr< array_tester_base_t >
	array_tester_ref_t;

typedef std::vector< array_tester_ref_t > array_tester_list_t;

template< class OESS_TYPE >
class array_tester_t
	:
		public array_tester_base_t
{
	public:
		array_tester_t(
			size_t size )
			:
				m_size( size ),
				m_etalon( new OESS_TYPE[ m_size.value() ] )
		{}

		~array_tester_t(){}

		const oess_2::defs::quantity_t &
		size() const
		{
			return m_size;
		}

		OESS_TYPE *
		data()
		{
			return m_etalon.get();
		}

		const OESS_TYPE *
		data() const
		{
			return m_etalon.get();
		}

		virtual void
		randomize()
		{
			for( size_t i = 0; i < m_size.value(); ++i )
			{
				if( &typeid(OESS_TYPE) != &typeid( oess_2::single_t ) &&
					&typeid(OESS_TYPE) != &typeid( oess_2::double_t ) )
				{
					if( sizeof(OESS_TYPE) <= 2 )
						data()[i] = rand();
					else
						data()[i] = (rand() << 16) + rand();
				}
				else
				{
					data()[i] = 100.0 * ( 0.0 + rand() - (1<<15) ) / (1 << 15);
				}
			}
		}

		virtual size_t
		array_size_in_bytes() const
		{
			return m_size.value() * sizeof( OESS_TYPE );

		}

		virtual void
		write_to( oess_2::io::obinstream_t & o ) const
		{
			o.write( m_size );

			o.write( m_etalon.get(), m_size.value() );
		}

		virtual void
		read_from(
			oess_2::io::ibinstream_t & i ) const
		{
			oess_2::defs::quantity_t res_quantity;
			i.read( res_quantity );
			UT_CHECK_EQ( m_size.value(), res_quantity.value() );

			std::unique_ptr< OESS_TYPE[] > res(
				new OESS_TYPE[ m_size.value() ] );

			i.read( res.get(),  m_size.value() );

			UT_CHECK_EQ(
				0,
				std::memcmp(
					m_etalon.get(),
					res.get(),
					m_size.value() * sizeof( OESS_TYPE ) ) );
		}

	private:
		// ������ �������.
		const oess_2::defs::quantity_t m_size;
		// ��������� �����.
		std::unique_ptr< OESS_TYPE[] > m_etalon;
};

template< class OESS_TYPE >
void
simple_test_array(
	so_5_transport::scattered_block_stream_ref_t & block_stream,
	size_t bytes_available,
	UT_CONTEXT_DECL )
{
	UT_PUSH_CONTEXT( "testing arrays" )
		<< "type is " << typeid( OESS_TYPE ).name();

	// ����� ��� ������.
	so_5_transport::channel_input_stream_t
		incoming_stream( block_stream );

	// ����� ��� ������.
	so_5_transport::channel_output_stream_t
		outgoing_stream( block_stream, ACE_Time_Value::zero );

	size_t size = bytes_available / sizeof( OESS_TYPE );
	array_tester_t< OESS_TYPE > arr( size );

	arr.randomize();

	outgoing_stream.begin_transaction();
	arr.write_to( outgoing_stream );
	outgoing_stream.commit_transaction();

	UT_CHECK_CONDITION( true == block_stream->check_stream_has_blocks() );

	incoming_stream.begin_transaction();
	arr.read_from( incoming_stream );
	incoming_stream.commit_transaction();

	UT_CHECK_CONDITION( false == block_stream->check_stream_has_blocks() );

	UT_POP_CONTEXT();
}

void
single_array_test(
	so_5_transport::scattered_block_stream_ref_t & block_stream,
	size_t bytes_avail,
	UT_CONTEXT_DECL )
{
	UT_PUSH_CONTEXT( "Single array" );

	simple_test_array< oess_2::char_t >(
		block_stream,
		std::max< size_t >( bytes_avail, sizeof( oess_2::char_t ) ),
		UT_CONTEXT );
	simple_test_array< oess_2::schar_t >(
		block_stream,
		std::max< size_t >( bytes_avail, sizeof( oess_2::schar_t ) ),
		UT_CONTEXT);
	simple_test_array< oess_2::uchar_t >(
		block_stream,
		std::max< size_t >( bytes_avail, sizeof( oess_2::uchar_t ) ),
		UT_CONTEXT );

	simple_test_array< oess_2::short_t >(
		block_stream,
		std::max< size_t >( bytes_avail, sizeof( oess_2::short_t ) ),
		UT_CONTEXT );
	simple_test_array< oess_2::ushort_t >(
		block_stream,
		std::max< size_t >( bytes_avail, sizeof( oess_2::ushort_t ) ),
		UT_CONTEXT );

	simple_test_array< oess_2::int_t >(
		block_stream,
		std::max< size_t >( bytes_avail, sizeof( oess_2::int_t ) ),
		UT_CONTEXT );
	simple_test_array< oess_2::uint_t >(
		block_stream,
		std::max< size_t >( bytes_avail, sizeof( oess_2::uint_t ) ),
		UT_CONTEXT );

	simple_test_array< oess_2::single_t >(
		block_stream,
		std::max< size_t >( bytes_avail, sizeof( oess_2::single_t ) ),
		UT_CONTEXT );
	simple_test_array< oess_2::double_t >(
		block_stream,
		std::max< size_t >( bytes_avail, sizeof( oess_2::double_t ) ),
		UT_CONTEXT );

	UT_POP_CONTEXT();
}


void
heterogeneous_test_array_impl(
	array_tester_list_t & array_tester_list,
	so_5_transport::scattered_block_stream_ref_t & block_stream,
	UT_CONTEXT_DECL )
{
	UT_PUSH_CONTEXT( "heterogeneous_test_array_impl" );

	// ����� ��� ������.
	so_5_transport::channel_input_stream_t
		incoming_stream( block_stream );

	// ����� ��� ������.
	so_5_transport::channel_output_stream_t
		outgoing_stream( block_stream, ACE_Time_Value::zero );

	outgoing_stream.begin_transaction();
	for( size_t i = 0; i < array_tester_list.size(); ++i )
	{
		array_tester_list[ i ]->randomize();
		array_tester_list[ i ]->write_to( outgoing_stream );
	}
	// ���� �� ������ flush.
	UT_CHECK_CONDITION( false == block_stream->check_stream_has_blocks() );
	outgoing_stream.commit_transaction();
	UT_CHECK_CONDITION( true == block_stream->check_stream_has_blocks() );

	// ������� ��� �������� ��� ����� �����������.
	incoming_stream.begin_transaction();
	for( size_t i = 0; i < array_tester_list.size(); ++i )
	{
		array_tester_list[ i ]->read_from( incoming_stream );
	}
	incoming_stream.commit_transaction();

	// ������ �� ������ ����, ������ ��� ��� ��������.
	UT_CHECK_CONDITION( false == block_stream->check_stream_has_blocks() );

	outgoing_stream.begin_transaction();
	for( size_t i = 0; i < array_tester_list.size(); ++i )
	{
		array_tester_list[ i ]->randomize();
		array_tester_list[ i ]->write_to( outgoing_stream );
	}

	UT_CHECK_CONDITION( false == block_stream->check_stream_has_blocks() );
	outgoing_stream.commit_transaction();
	UT_CHECK_CONDITION( true == block_stream->check_stream_has_blocks() );

	// �������� ������ ������ ��� ����� �����������.
	for( size_t i = 0; i < array_tester_list.size(); ++i )
	{
		incoming_stream.begin_transaction();
		array_tester_list[ i ]->read_from( incoming_stream );
		incoming_stream.commit_transaction();

		UT_CHECK_EQ(
			i != array_tester_list.size()-1,
			block_stream->check_stream_has_blocks() );
	}

	outgoing_stream.begin_transaction();
	for( size_t i = 0; i < array_tester_list.size(); ++i )
	{
		array_tester_list[ i ]->randomize();
		array_tester_list[ i ]->write_to( outgoing_stream );
	}

	UT_CHECK_CONDITION( false == block_stream->check_stream_has_blocks() );
	outgoing_stream.commit_transaction();
	UT_CHECK_CONDITION( true == block_stream->check_stream_has_blocks() );

	incoming_stream.begin_transaction();
	// ��������.
	for( size_t i = 0; i < array_tester_list.size(); ++i )
	{
		array_tester_list[ i ]->read_from( incoming_stream );
	}
	incoming_stream.rollback_transaction();
	UT_CHECK_CONDITION( true == block_stream->check_stream_has_blocks() );

	incoming_stream.begin_transaction();
	for( size_t i = 0; i < array_tester_list.size(); ++i )
	{
		array_tester_list[ i ]->read_from( incoming_stream );
	}
	incoming_stream.commit_transaction();

	UT_CHECK_CONDITION( false == block_stream->check_stream_has_blocks() );

	UT_POP_CONTEXT();
}
// ������ ���� ����� ��� ������ 100 ��������� ������� ����.
// �.�. ������� 27*100 + 7 < 1024*1024
void
heterogeneous_test_array(
	so_5_transport::scattered_block_stream_ref_t & block_stream,
	UT_CONTEXT_DECL )
{
	UT_PUSH_CONTEXT( "testing heterogeneous arrays" );
	array_tester_list_t array_tester_list;

	array_tester_list.push_back(
		array_tester_ref_t( new array_tester_t< oess_2::char_t >( 100 ) ) );
	array_tester_list.push_back(
		array_tester_ref_t( new array_tester_t< oess_2::schar_t >( 100 ) ) );
	array_tester_list.push_back(
		array_tester_ref_t( new array_tester_t< oess_2::uchar_t >( 100 ) ) );

	array_tester_list.push_back(
		array_tester_ref_t( new array_tester_t< oess_2::short_t >( 100 ) ) );
	array_tester_list.push_back(
		array_tester_ref_t( new array_tester_t< oess_2::ushort_t >( 100 ) ) );

	array_tester_list.push_back(
		array_tester_ref_t( new array_tester_t< oess_2::int_t >( 100 ) ) );
	array_tester_list.push_back(
		array_tester_ref_t( new array_tester_t< oess_2::uint_t >( 100 ) ) );

	array_tester_list.push_back(
		array_tester_ref_t( new array_tester_t< oess_2::single_t >( 100 ) ) );
	array_tester_list.push_back(
		array_tester_ref_t( new array_tester_t< oess_2::double_t >( 100 ) ) );

	heterogeneous_test_array_impl(
		array_tester_list,
		block_stream,
		UT_CONTEXT );

	UT_POP_CONTEXT();
}


// �������������� ��� ����� ���������� ������ �����
// �������� 10��.
void
random_heterogeneous_test_array(
	so_5_transport::scattered_block_stream_ref_t & block_stream,
	UT_CONTEXT_DECL )
{
	UT_PUSH_CONTEXT( "testing random heterogeneous arrays" );
	array_tester_list_t array_tester_list;

	// ��� ������ ���������� ������ � 5��
	// ���������� ��������� ������.
	const size_t max_size_in_bytes = 5*1024*1024;

	size_t approximate_used_size = 0;
	while( approximate_used_size < max_size_in_bytes )
	{
	
		array_tester_ref_t array_tester_ref;
		uint32_t x = rand() << 16;
		x += rand();
		const size_t size = x % (64 * 1024);

		switch( rand() % 9 )
		{
			case 0:
				array_tester_ref.reset(
					new array_tester_t< oess_2::char_t >( size ) );
				break;
			case 1:
				array_tester_ref.reset(
					new array_tester_t< oess_2::schar_t >( size ) );
				break;
			case 2:
				array_tester_ref.reset(
					new array_tester_t< oess_2::uchar_t >( size ) );
				break;

			case 3:
				array_tester_ref.reset(
					new array_tester_t< oess_2::short_t >( size ) );
				break;
			case 4:
				array_tester_ref.reset(
					new array_tester_t< oess_2::ushort_t >( size ) );
				break;

			case 5:
				array_tester_ref.reset(
					new array_tester_t< oess_2::int_t >( size ) );
				break;
			case 6:
				array_tester_ref.reset(
					new array_tester_t< oess_2::uint_t >( size ) );
				break;

			case 7:
				array_tester_ref.reset(
					new array_tester_t< oess_2::single_t >( size ) );
				break;
			case 8:
				array_tester_ref.reset(
					new array_tester_t< oess_2::double_t >( size ) );
				break;
		}

		// ������� � ������� �� ������.
		approximate_used_size +=
			array_tester_ref->array_size_in_bytes() + 5;
		array_tester_list.push_back( array_tester_ref );
	}

	heterogeneous_test_array_impl(
		array_tester_list,
		block_stream,
		UT_CONTEXT );

	UT_POP_CONTEXT();
}


UT_UNIT_TEST( check_small_blocks )
{
	for( size_t i = 1; i <= 256; i<<=1 )
		for( size_t n = 1; n <= 16; ++n )
		{
			UT_PUSH_CONTEXT( "Scattered block stream params" )
				<< "blocks_count = " << i
				<< ", max_block_count " << 260
				<< ", block_size = " << n ;

			so_5_transport::scattered_block_stream_ref_t
				block_stream = create_scatered_block_stream(i, 260, n );

			quantity_test( block_stream );

			single_array_test(
				block_stream,
				i * n,
				UT_CONTEXT );

			simple_string_test(
				block_stream,
				UT_CONTEXT );

			UT_POP_CONTEXT();
		}

	for( size_t n = 1; n <= 16; ++n )
	{
		UT_PUSH_CONTEXT( "Heterogeneous arrays" )
			<< "block_size = " << n ;

		so_5_transport::scattered_block_stream_ref_t
			block_stream = create_scatered_block_stream(
				32 * 1024 / n, 2*1024*1024 / n, n );

		heterogeneous_test_array(
			block_stream,
			UT_CONTEXT );

		UT_POP_CONTEXT();
	}
}

UT_UNIT_TEST( check_normal_blocks )
{
	size_t sizes[] = {
		1024,
		8*1024,
		32*1024,
		64*1024 };

	for( size_t i = 0; i < CPP_UTIL_2_ASIZE( sizes ) ; ++i )
		for( size_t n = sizes[ i ] - 1; n < sizes[ i ] + 2; ++n )
		{
			UT_PUSH_CONTEXT( "Scattered block stream params" )
				<< "blocks_count = " << 100
				<< ", max_block_count " << 1000
				<< ", block_size = " << n ;

			so_5_transport::scattered_block_stream_ref_t
				block_stream = create_scatered_block_stream(
					50, 10*1024*1024 / sizes[ i ], n );

			quantity_test( block_stream );

			single_array_test(
				block_stream,
				i * n,
				UT_CONTEXT );

			heterogeneous_test_array(
				block_stream,
				UT_CONTEXT );

			random_heterogeneous_test_array(
				block_stream,
				UT_CONTEXT );

			simple_string_test(
				block_stream,
				UT_CONTEXT );

			UT_POP_CONTEXT();
		}
}


UT_UNIT_TEST( check_transactions )
{
	so_5_transport::scattered_block_stream_ref_t
		block_stream = create_scatered_block_stream(
			10, 20, 1024 );

		// ����� ��� ������.
	so_5_transport::channel_input_stream_t
		incoming_stream( block_stream );

	// ����� ��� ������.
	so_5_transport::channel_output_stream_t
		outgoing_stream( block_stream, ACE_Time_Value::zero );

	{
		const std::string s1 = "123456789";

		// ������ 2-� ����.
		outgoing_stream.begin_transaction();
		outgoing_stream.begin_transaction();

		outgoing_stream.write( s1 );
		outgoing_stream.commit_transaction();

		std::string s2;

		incoming_stream.begin_transaction();
		incoming_stream.read( s2 );
		incoming_stream.commit_transaction();

		UT_CHECK_EQ( s2, s1 );
	}

	{
		const std::string s1 = "123456789";
		outgoing_stream.begin_transaction();
		outgoing_stream.write( s1 );

		// ������ 2-� ����.
		outgoing_stream.commit_transaction();
		outgoing_stream.commit_transaction();

		std::string s2;

		incoming_stream.begin_transaction();
		incoming_stream.read( s2 );
		incoming_stream.commit_transaction();

		UT_CHECK_EQ( s2, s1 );
	}

	{
		const std::string s1 = "123456789";
		outgoing_stream.begin_transaction();
		outgoing_stream.write( s1 );
		outgoing_stream.commit_transaction();

		std::string s2;

		// ������ 2-� ����.
		incoming_stream.begin_transaction();
		incoming_stream.begin_transaction();

		incoming_stream.read( s2 );

		incoming_stream.commit_transaction();

		UT_CHECK_EQ( s2, s1 );
	}

	{
		const std::string s1 = "123456789";
		outgoing_stream.begin_transaction();
		outgoing_stream.write( s1 );
		outgoing_stream.commit_transaction();

		std::string s2;

		incoming_stream.begin_transaction();

		incoming_stream.read( s2 );

		// ������ 2-� ����.
		incoming_stream.commit_transaction();
		incoming_stream.commit_transaction();

		UT_CHECK_EQ( s2, s1 );
	}

	{
		const std::string s1 = "123456789";
		outgoing_stream.begin_transaction();
		outgoing_stream.write( s1 );
		outgoing_stream.commit_transaction();

		std::string s2;
		incoming_stream.begin_transaction();
		incoming_stream.read( s2 );

		UT_CHECK_EQ( s2, s1 );

		// ������� � ������� ��� ���.
		incoming_stream.rollback_transaction();

		std::string s3;
		incoming_stream.begin_transaction();
		incoming_stream.read( s3 );

		UT_CHECK_EQ( s3, s1 );

		incoming_stream.rollback_transaction();

		std::string s4;
		incoming_stream.begin_transaction();
		incoming_stream.read( s4 );

		UT_CHECK_EQ( s3, s4 );

		incoming_stream.commit_transaction();

		UT_CHECK_CONDITION(
			false == block_stream->check_stream_has_blocks() );
	}
}


void
partial_trx_check(
	size_t min,
	size_t max,
	size_t size,
	UT_CONTEXT_DECL )
{
	UT_PUSH_CONTEXT( "testing partial commit" )
		<< "min: " << min << "; max: " << max << "; size: " << size;

	so_5_transport::scattered_block_stream_ref_t
		block_stream = create_scatered_block_stream(
			min, max, size );

		// ����� ��� ������.
	so_5_transport::channel_input_stream_t
		incoming_stream( block_stream );

	// ����� ��� ������.
	so_5_transport::channel_output_stream_t
		outgoing_stream( block_stream, ACE_Time_Value::zero );

	{
		const std::string s1 = "0123456789";
		const std::string s2 = "abcdefghijabcdefghij";

		outgoing_stream.begin_transaction();
		outgoing_stream.write( s1.c_str(), s1.size() );
		outgoing_stream.write( s1.c_str(), s1.size() );
		outgoing_stream.checkpoint();
		outgoing_stream.write( s2.c_str(), s2.size() );
		outgoing_stream.rollback_transaction();

		incoming_stream.begin_transaction();
		UT_CHECK_EQ( incoming_stream.bytes_available(), 2 * s1.size() ) ;

		char buffer[ 11 ];
		incoming_stream.read( buffer, 10 );
		incoming_stream.checkpoint();
		buffer[ 10 ] = 0;
		const std::string s3( buffer );
		UT_CHECK_EQ( s1, s3 );

		incoming_stream.read( buffer, 10 );
		buffer[ 10 ] = 0;
		const std::string s4( buffer );
		UT_CHECK_EQ( s1, s4 );
		incoming_stream.commit_transaction();
	}

	{
		const std::string s0 = "0123456789";
		const std::string s1 = "012345678901234567890123456789";

		outgoing_stream.begin_transaction();
		outgoing_stream.write( s1.c_str(), s1.size() );
		outgoing_stream.commit_transaction();

		incoming_stream.begin_transaction();
		UT_CHECK_EQ( incoming_stream.bytes_available(), s1.size() ) ;

		char buffer[ 11 ];
		incoming_stream.read( buffer, 10 );
		incoming_stream.checkpoint();
		buffer[ 10 ] = 0;
		const std::string s2( buffer );
		UT_CHECK_EQ( s0, s2 );

		incoming_stream.read( buffer, 10 );
		incoming_stream.checkpoint();
		buffer[ 10 ] = 0;
		const std::string s3( buffer );
		UT_CHECK_EQ( s0, s3 );

		incoming_stream.read( buffer, 10 );
		incoming_stream.checkpoint();
		buffer[ 10 ] = 0;
		const std::string s4( buffer );
		UT_CHECK_EQ( s0, s4 );
	}
}

UT_UNIT_TEST( check_transactions_checkpoint )
{
	partial_trx_check( 10, 100, 1, UT_CONTEXT );
	partial_trx_check( 10, 50, 2, UT_CONTEXT );
	partial_trx_check( 10, 33, 3, UT_CONTEXT );
	partial_trx_check( 10, 25, 4, UT_CONTEXT );
	partial_trx_check( 10, 20, 5, UT_CONTEXT );
	partial_trx_check( 10, 20, 6, UT_CONTEXT );
	partial_trx_check( 10, 20, 7, UT_CONTEXT );
	partial_trx_check( 10, 20, 8, UT_CONTEXT );
	partial_trx_check( 10, 20, 9, UT_CONTEXT );
	partial_trx_check( 10, 20, 10, UT_CONTEXT );
	partial_trx_check( 10, 20, 11, UT_CONTEXT );
	partial_trx_check( 10, 20, 12, UT_CONTEXT );
	partial_trx_check( 10, 20, 13, UT_CONTEXT );
	partial_trx_check( 10, 20, 14, UT_CONTEXT );
	partial_trx_check( 10, 20, 15, UT_CONTEXT );
	partial_trx_check( 10, 20, 16, UT_CONTEXT );
	partial_trx_check( 10, 20, 17, UT_CONTEXT );
	partial_trx_check( 10, 20, 18, UT_CONTEXT );
	partial_trx_check( 10, 20, 19, UT_CONTEXT );
	partial_trx_check( 10, 20, 20, UT_CONTEXT );
	partial_trx_check( 10, 20, 21, UT_CONTEXT );
	partial_trx_check( 10, 20, 22, UT_CONTEXT );
	partial_trx_check( 10, 20, 23, UT_CONTEXT );
	partial_trx_check( 10, 20, 24, UT_CONTEXT );
	partial_trx_check( 10, 20, 25, UT_CONTEXT );

	partial_trx_check( 10, 20, 1024, UT_CONTEXT );

}


void
over_write(
	so_5_transport::channel_output_stream_t & outgoing_stream )
{
	while( true )
	{
		const std::string s = "12345";
		outgoing_stream.begin_transaction();
		outgoing_stream.write( s );
		outgoing_stream.commit_transaction();
	}
}

void
over_read(
	so_5_transport::channel_input_stream_t & incoming_stream )
{
	incoming_stream.begin_transaction();
	while( true )
	{
		std::string s;
		incoming_stream.read( s );
	}
	incoming_stream.commit_transaction();
}

UT_UNIT_TEST( check_throws )
{
	so_5_transport::scattered_block_stream_ref_t
		block_stream = create_scatered_block_stream(
			1, 2, 32 );

		// ����� ��� ������.
	so_5_transport::channel_input_stream_t
		incoming_stream( block_stream );

	// ����� ��� ������.
	so_5_transport::channel_output_stream_t
		outgoing_stream( block_stream, ACE_Time_Value::zero );

	UT_CHECK_THROW(
		so_5::exception_t,
		over_write( outgoing_stream ) );

	UT_CHECK_THROW(
		so_5::exception_t,
		over_read( incoming_stream ) );
}

int
main( int argc, char ** argv)
{
	srand( time(0) );
	UT_RUN_UNIT_TEST( check_small_blocks );
	UT_RUN_UNIT_TEST( check_normal_blocks );
	UT_RUN_UNIT_TEST( check_transactions );
	UT_RUN_UNIT_TEST( check_transactions_checkpoint );
	UT_RUN_UNIT_TEST( check_throws );

	return 0;
}
