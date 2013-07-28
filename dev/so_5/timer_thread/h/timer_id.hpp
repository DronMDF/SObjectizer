/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ������ �� ���������.
*/

#if !defined( _SO_5__TIMER_THREAD__TIMER_ID_REF_HPP_ )
#define _SO_5__TIMER_THREAD__TIMER_ID_REF_HPP_

#include <ace/Basic_Types.h>

#include <so_5/h/declspec.hpp>

#include <so_5/rt/h/atomic_refcounted.hpp>

namespace so_5
{

namespace timer_thread
{

//! ������������� ���������� ������� SObjectizer.
typedef ACE_UINT64 timer_id_t;

class timer_thread_t;
class timer_id_ref_t;

//
// timer_id_internals_t
//

//! ������� ��� timer_id_t, ������� ������� ������� � ������
//! ������������ �����������.
class timer_id_internals_t
	:
		private so_5::rt::atomic_refcounted_t
{
		friend class timer_id_ref_t;

		timer_id_internals_t( const timer_id_internals_t & );
		void
		operator = ( const timer_id_internals_t & );

	public:

		timer_id_internals_t(
			//! ��������� ����.
			timer_thread_t & timer_thread,
			//! ������������� ���������� �������.
			timer_id_t timer_id );

		virtual ~timer_id_internals_t();

	private:
		//! ������ �� ��������� ����.
		timer_thread_t & m_timer_thread;

		//! ������������� ���������� �������.
		timer_id_t m_timer_id;
};

//
// timer_id_ref_t
//

//! Ref - ������� ��� timer_id_t.
class SO_5_TYPE timer_id_ref_t
{
		explicit timer_id_ref_t(
			timer_id_internals_t * timer_id_internals );
	public:

		static timer_id_ref_t
		create(
			//! ��������� ����.
			timer_thread_t & timer_thread,
			//! ������������� ���������� �������.
			timer_id_t timer_id );

		timer_id_ref_t();

		timer_id_ref_t( const timer_id_ref_t & timer_id );

		~timer_id_ref_t();

		timer_id_ref_t &
		operator = ( const timer_id_ref_t & timer_id );

		//! ��������� ���������� �� � timer_id_ref_t �������� �������.
		bool
		is_active() const;

		//! ��������� ��������� �������.
		void
		release();

	private:
		//! ��������� ���������� ������ �� ������.
		void
		inc_timer_id_ref_count();

		//! ��������� ���������� ������ �� ������.
		void
		dec_timer_id_ref_count();

		//! ��������� ���������� �������.
		timer_id_internals_t * m_timer_id_internals;
};

} /* namespace timer_thread */

} /* namespace so_5 */

#endif
