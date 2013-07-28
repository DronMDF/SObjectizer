/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ��������� �������.
*/

#if !defined( _SO_5__TIMER_THREAD__TIMER_ACT_HPP_ )
#define _SO_5__TIMER_THREAD__TIMER_ACT_HPP_

#include <memory>

#include <so_5/h/declspec.hpp>

#include <so_5/rt/h/type_wrapper.hpp>
#include <so_5/rt/h/mbox_ref.hpp>
#include <so_5/rt/h/message_ref.hpp>

namespace so_5
{

namespace timer_thread
{

//
// timer_act_t
//

//! ��������� �������.
class SO_5_TYPE timer_act_t
{
		timer_act_t( const timer_act_t & );
		void
		operator = ( const timer_act_t & );

	public:
		timer_act_t(
			//! ��� ���������.
			const rt::type_wrapper_t & type_wrapper,
			//! mbox, �� ������� ���� ���������� ������������� ���������.
			const rt::mbox_ref_t & mbox,
			//! ��������� ���������.
			const rt::message_ref_t & msg,
			/*! ��� �������������� ��������� ���������
				�����, ����� ������� ��������� ������
				���������� � ������ ���. */
			unsigned int delay,
			/*! ������, ������� �� 0 ��� ������������� ���������. */
			unsigned int period );

		~timer_act_t();

		//! ��������� ��������.
		void
		exec();

		//! �������� �� ��������� �������� �������������?
		bool
		is_periodic() const;

		//! �������� ��� ������� ����.
		unsigned int
		query_delay() const;

		/*! ������, ������� �� 0 ��� ������������� ���������. */
		unsigned int
		query_period() const;

	private:
		//! ��� ���������.
		const rt::type_wrapper_t m_type_wrapper;

		//! mbox, �� ������� ���� ���������� ������������� ���������.
		rt::mbox_ref_t m_mbox;

		//! ��������� ���������.
		const rt::message_ref_t m_msg;

		/*! ��� �������������� ��������� ���������
			�����, ����� ������� ��������� ������
			���������� � ������ ���. */
		const unsigned int m_delay;

		/*! ������, ������� �� 0 ��� ������������� ���������. */
		const unsigned int m_period;
};

//! ��������� unique_ptr ��� timer_act_t.
typedef std::unique_ptr< timer_act_t > timer_act_unique_ptr_t;

} /* namespace timer_thread */

} /* namespace so_5 */

#endif
