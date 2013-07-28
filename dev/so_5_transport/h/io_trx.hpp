/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ����� ��� ���������� IO ���������� c �������.
*/

#if !defined( _SO_5_TRANSPORT__IO_TRX_HPP_ )
#define _SO_5_TRANSPORT__IO_TRX_HPP_

#include <memory>

#include <so_5_transport/h/declspec.hpp>

#include <so_5_transport/h/channel_stream.hpp>

namespace so_5_transport
{

//
// input_trx_t
//

//! ����� ���������� ������ ������.
/*!
	��� �������� ����� �������� ���������� � �������������
	����������� ����� �� ���������� �����-���� ��������.
	� ���� ������ ������ ����� ������������� ����������� �����,
	������� ��������� ��� ���������� ������.
	��� �����������, ���� ���������� �� ���� ������������,
	���������� ����� ���������� � ��������� ����������� �����.
*/
class SO_5_TRANSPORT_TYPE input_trx_t
{
	public:
		input_trx_t(
			channel_input_stream_unique_ptr_t istream );

		//! �������� ����� ��� ������ ������.
		//! \note ���������� �� ������ ���� ���������.
		inline oess_2::io::ibinstream_t &
		istream()
		{
			return *m_istream;
		}

		//! ������������� ����������� ����� � ���������� ����������.
		void
		checkpoint();

		//! ����������� ����������.
		void
		commit();

		//! �������� ����������.
		/*!
			����� ���������� � ��������� ����������� �����.
		*/
		void
		rollback();

		//! ������ ������ � ������ ��������� ��� ������.
		inline size_t
		stream_size() const
		{
			return m_istream->bytes_available();
		}

	private:
		//! ����� ��� ������ ������.
		channel_input_stream_unique_ptr_t m_istream;
};

//! ��������� unique_ptr ��� input_trx_t.
typedef std::unique_ptr< input_trx_t > input_trx_unique_ptr_t;

//
// input_trx_t
//

//! ����� ���������� ������ ������.
/*!
	��� �������� ����� �������� ���������� � �������������
	����������� ����� �� ���������� �����-���� ��������.
	� ���� ������ ������ ����� ������������� ����������� �����,
	������� ��������� ��� ���������� ������.
	��� �����������, ���� ���������� �� ���� ������������,
	���������� ����� ���������� � ��������� ����������� �����.
*/
class SO_5_TRANSPORT_TYPE output_trx_t
{
	public:
		output_trx_t(
			channel_output_stream_unique_ptr_t ostream );

		//! �������� ����� ��� ������ ������.
		//! \note ���������� �� ������ ���� ���������.
		inline oess_2::io::obinstream_t &
		ostream()
		{
			return *m_ostream;
		}

		//! ������������� ��������� � ���������� ����������.
		void
		checkpoint();

		//! ������������� ����������� ����� � ���������� ����������.
		void
		commit();

		//! �������� ����������.
		void
		rollback();

		//! ������ ������ � ������, ������� �������� � ������� �������.
		inline size_t
		stream_size() const
		{
			return m_ostream->bytes_written();
		}

	private:
		//! ����� ��� ������ ������.
		channel_output_stream_unique_ptr_t m_ostream;
};

//! ��������� unique_ptr ��� output_trx_t.
typedef std::unique_ptr< output_trx_t > output_trx_unique_ptr_t;

} /* namespace so_5_transport */

#endif
