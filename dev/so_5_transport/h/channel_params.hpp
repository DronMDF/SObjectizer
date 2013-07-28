/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ��������� ������������� ������.
*/

#if !defined( _SO_5_TRANSPORT__CHANNEL_PARAMS_HPP_ )
#define _SO_5_TRANSPORT__CHANNEL_PARAMS_HPP_

#include <so_5_transport/h/declspec.hpp>

#include <so_5_transport/h/scattered_block_stream.hpp>

namespace so_5_transport
{

//
// channel_params_t
//

//! ������������ ��������� ���������� ������������� ������.
class SO_5_TRANSPORT_TYPE channel_params_t
{
	public:
		channel_params_t();
		~channel_params_t();

		//! \name ��������� ��������� ������.
		//! \{
		const scattered_block_stream_params_t &
		channel_input_stream_params() const;

		channel_params_t &
		set_channel_input_stream_params(
			const scattered_block_stream_params_t & v );
		//! \}

		//! \name ��������� ���������� ������.
		//! \{
		const scattered_block_stream_params_t &
		channel_output_stream_params() const;

		channel_params_t &
		set_channel_output_stream_params(
			const scattered_block_stream_params_t & v );
		//! \}

		//! \name ������ ������� �������� ���������� ������ (� ��������).
		//! \{
		unsigned int
		time_checking_period() const;

		channel_params_t &
		set_time_checking_period(
			unsigned int v );
		//! \}

		//! \name ������������ ����� ���������� �������� ������ (� ��������).
		//! \{
		unsigned int
		max_input_block_timeout() const;

		channel_params_t &
		set_max_input_block_timeout(
			unsigned int v );
		//! \}

		//! \name ������������ ����� ���������� ��������� ������ (� ��������).
		//! \{
		unsigned int
		max_output_block_timeout() const;

		channel_params_t &
		set_max_output_block_timeout(
			unsigned int v );
		//! \}

	private:
		//! �������� scattered_block_stream_t ��������� ������.
		scattered_block_stream_params_t m_channel_input_stream_params;

		//! �������� scattered_block_stream_t ���������� ������.
		scattered_block_stream_params_t m_channel_output_stream_params;


		//! \name ������ ������� �������� ���������� ������ (� ��������).
		/*!
			�� ��������� - 1 s.
		*/
		unsigned int m_time_checking_period;

		//! \name ������������ ����� ���������� �������� ������ (� ��������).
		/*!
			�� ��������� - 30 s.
		*/
		unsigned int m_max_input_block_timeout;

		//! \name ������������ ����� ���������� ��������� ������ (� ��������).
		/*!
			�� ��������� - 30 s.
		*/
		unsigned int m_max_output_block_timeout;
};

} /* namespace so_5_transport */

#endif
