/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ��������� ����� event_data_t.

	������, ���������� ���������� �������� ������
	����������� ��� ������� ��������� �������.
*/


#if !defined( _SO_5__RT__EVENT_DATA_HPP_ )
#define _SO_5__RT__EVENT_DATA_HPP_

#include <so_5/rt/h/message.hpp>

#include <so_5/rt/h/mbox_ref.hpp>

namespace so_5
{

namespace rt
{

class mbox_t;

//! ��������� ����� ��� ������������ ���������,
//! ������� �������� ���������� ����������� ������� ������.
/*!
	\code
		void
		a_sample_t::evt_smth(
			const so_5::rt::event_data_t< sample_message_t > & msg )
		{
			// ...
		}
	\endcode
*/
template< class MESSAGE >
class event_data_t
{
	public:
		//! ����������� �� ���������, �������
		//! ������� ������ ��������� - ������.
		event_data_t()
			:
				m_message_instance( 0 )
		{}

		event_data_t( const MESSAGE * message_instance )
			:
				m_message_instance( message_instance )
		{}

		~event_data_t()
		{}

		//! �������� ������ �� ���������.
		const MESSAGE&
		operator * () const
		{
			return *m_message_instance;
		}

		//! �������� ��������� �� ���������.
		const MESSAGE *
		get() const
		{
			return m_message_instance;
		}

		//! ������� ����� � ���������.
		const MESSAGE *
		operator -> () const
		{
			return get();
		}

	private:
		//! ��������� �� ��������� ���������.
		const MESSAGE * const m_message_instance;
};

//! ��������� ����� ��� ������������ ���������,
//! ������� �������� ���������� ����������� ������� ������, � �������
//! ������������� ��������� �� ������� ������������ ������ ���������.
/*!
	\code
		void
		a_sample_t::evt_smth(
			const so_5::rt::not_null_event_data_t< sample_message_t > & msg )
		{
			// ...
		}
	\endcode
*/
template< class MESSAGE >
class not_null_event_data_t
{
	public:
		//! ����������� �� ���������, �������
		//! ������� ������ ��������� - ������.
		not_null_event_data_t( const MESSAGE & message_instance )
			:
				m_message_instance( message_instance )
		{}

		~not_null_event_data_t()
		{}

		//! �������� ������ �� ���������.
		const MESSAGE&
		operator * () const
		{
			return *m_message_instance;
		}

		//! �������� ��������� �� ���������.
		const MESSAGE *
		get() const
		{
			return &m_message_instance;
		}

		//! ������� ����� � ���������.
		const MESSAGE *
		operator -> () const
		{
			return get();
		}

	private:
		//! ������ �� ��������� ���������.
		const MESSAGE & m_message_instance;
};

} /* namespace rt */

} /* namespace so_5 */

#endif
