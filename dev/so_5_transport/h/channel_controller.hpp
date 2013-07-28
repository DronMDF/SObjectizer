/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ��������� ����������� ������������� ������.
*/

#if !defined( _SO_5_TRANSPORT__CHANNEL_CONTROLLER_HPP_ )
#define _SO_5_TRANSPORT__CHANNEL_CONTROLLER_HPP_

#include <ace/Addr.h>

#include <cpp_util_2/h/nocopy.hpp>

#include <so_5_transport/h/declspec.hpp>
#include <so_5_transport/h/ret_code.hpp>

#include <so_5/rt/h/atomic_refcounted.hpp>
#include <so_5/rt/h/mbox_ref.hpp>

namespace so_5_transport
{

//
// channel_controller_t
//

/*!
	\brief ��������� ��� ����������� ������.
*/
class SO_5_TRANSPORT_TYPE channel_controller_t
	:
		private so_5::rt::atomic_refcounted_t
{
		friend class channel_controller_ref_t;
	public:
		channel_controller_t();
		virtual ~channel_controller_t();

		//! ��������� ������ �� ��������� ������� � ���� ������.
		virtual const std::string &
		local_address_as_string() const = 0;

		//! ��������� ������ �� ��������� ������ � ���� ������.
		virtual const std::string &
		remote_address_as_string() const = 0;

		//! ������������� �������� �� ����������� ������ � ������.
		/*!
			���������� �������� �� �������� ������ �� ������� ��������
			������. ���� �������� ������ �������, �� ��� �����������
			�������� ������ � ��������� mbox ����� ������������
			��������� msg_incoming_package.
		 */
		virtual so_5::ret_code_t
		enforce_input_detection() = 0;

		//! ������������ �������� ������,
		//! ����� ������ ����� ������, ��� ��������
		//! � ������� ������ ��������������.
		virtual so_5::ret_code_t
		close() = 0;
};

//
// channel_controller_ref_t
//


//! ����� ������ �� channel_controller_t.
class SO_5_TRANSPORT_TYPE channel_controller_ref_t
{
	public:
		channel_controller_ref_t();

		explicit channel_controller_ref_t(
			channel_controller_t * channel_controller );

		channel_controller_ref_t(
			const channel_controller_ref_t & channel_controller_ref );

		void
		operator = ( const channel_controller_ref_t & channel_controller_ref );

		~channel_controller_ref_t();

		inline const channel_controller_t *
		get() const
		{
			return m_channel_controller_ptr;
		}

		inline channel_controller_t *
		get()
		{
			return m_channel_controller_ptr;
		}

		inline const channel_controller_t *
		operator -> () const
		{
			return m_channel_controller_ptr;
		}

		inline channel_controller_t *
		operator -> ()
		{
			return m_channel_controller_ptr;
		}

		inline channel_controller_t &
		operator * ()
		{
			return *m_channel_controller_ptr;
		}


		inline const channel_controller_t &
		operator * () const
		{
			return *m_channel_controller_ptr;
		}

		inline bool
		operator == ( const channel_controller_ref_t & channel_controller_ref ) const
		{
			return m_channel_controller_ptr ==
				channel_controller_ref.m_channel_controller_ptr;
		}

		inline bool
		operator < ( const channel_controller_ref_t & channel_controller_ref ) const
		{
			return m_channel_controller_ptr <
				channel_controller_ref.m_channel_controller_ptr;
		}

	private:
		//! ��������� ���������� ������ �� channel_controller
		//! � � ������ ������������� ������� ���.
		void
		inc_channel_controller_ref_count();

		//! ��������� ���������� ������ �� channel_controller
		//! � � ������ ������������� ������� ���.
		void
		dec_channel_controller_ref_count();

		//! �������������� channel_controller_t.
		channel_controller_t * m_channel_controller_ptr;

};

} /* namespace so_5_transport */

#endif
