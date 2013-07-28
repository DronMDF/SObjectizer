/*
	SObjectizer 5 Transport.
*/

/*!
	\file
	\brief ����� ��� ������������ ������� ��������.
*/

#if !defined( _SO_5_TRANSPORT__SCATTERED_BLOCK_STREAM_HPP_ )
#define _SO_5_TRANSPORT__SCATTERED_BLOCK_STREAM_HPP_

#include <list>
#include <memory>

#include <ace/Time_Value.h>
#include <ace/Thread_Mutex.h>
#include <ace/Condition_Thread_Mutex.h>
#include <ace/Guard_T.h>

#include <so_5_transport/h/declspec.hpp>
#include <so_5_transport/h/ret_code.hpp>
#include <so_5/h/throwing_strategy.hpp>

#include <so_5/rt/h/atomic_refcounted.hpp>

#include <so_5_transport/h/raw_block.hpp>

namespace so_5_transport
{

class scattered_block_stream_t;

namespace block_stream_notifiers
{

class SO_5_TRANSPORT_TYPE flush_notifier_t
{
	public:
		virtual ~flush_notifier_t();

		virtual void
		notify() = 0;

		virtual void
		reset() = 0;
};

class SO_5_TRANSPORT_TYPE free_blocks_availability_notifier_t
{
	public:
		virtual ~free_blocks_availability_notifier_t();

		virtual void
		notify_lacking_free_blocks() = 0;

		virtual void
		notify_free_blocks(
			size_t default_block_count,
			size_t available_block_count ) = 0;
};

class SO_5_TRANSPORT_TYPE stream_operation_notifier_t
{
	public:
		virtual ~stream_operation_notifier_t();

		virtual void
		notify_blocks_pushed_back() = 0;

		virtual void
		notify_blocks_pushed_front() = 0;

		virtual void
		notify_blocks_queried() = 0;
};

} /* namespace block_stream_notifiers */

//
// scattered_block_stream_params_t
//

//! ��������� �������� ������-������.
struct SO_5_TRANSPORT_TYPE scattered_block_stream_params_t
{
	//! ������� ���������� ������.
	/*!
		�������� �� ���������: 16.
	*/
	size_t m_block_count;

	//! ����� �� �������� ����� �����������
	//! ���������� ������.
	/*!
		�������� �� ���������: 64.
	*/
	size_t m_max_block_count;

	//! ������ ���������� �����.
	/*!
		�������� �� ���������: 32*1024.
	*/
	size_t m_single_block_size;

	scattered_block_stream_params_t();
	scattered_block_stream_params_t(
		//! ���������� ������.
		size_t block_count,
		//! ����� �� �������� ����� �����������
		//! ���������� ������.
		size_t max_block_count,
		//! ������ ���������� �����.
		size_t single_block_size );
};

//
// scattered_block_stream_t
//

//! ����� ������-�������.
/*!
	�������� � ��������� ������ ���������� ����� �������������
	�������� SObjectizer � Ace_Reactor frasmework �����
	scattered_block_stream_t. �� ������� SObjectizer
	�� ������ �� �������� � scattered_block_stream_t, �
	�������� �� ���� ������� ���������� ���� �
	channel_input_stream_t � channel_input_stream_t
	��� ������ ��������� ����������.

	����� �������� ��� ������-�������. �� ����� ���� ����� ����� �����
	������ � ��� � ��������� � �����.
	�� ������ ������ ����� ����� ����� � ���������� �� ��� ������,
	����� ���� ���������� ����� � ���, � ������������� ����� � ������ ������.
*/
class SO_5_TRANSPORT_TYPE scattered_block_stream_t
	:
		private so_5::rt::atomic_refcounted_t
{
		friend class scattered_block_stream_ref_t;

	public:
		scattered_block_stream_t(
			//! ������� ���������� ������.
			size_t block_count,
			//! ����� �� �������� ����� �����������
			//! ���������� ������.
			size_t max_block_count,
			//! ������ ���������� �����.
			size_t single_block_size );

		explicit scattered_block_stream_t(
			//! ���������.
			const scattered_block_stream_params_t & scattered_block_stream_params );

		virtual ~scattered_block_stream_t();

		//! �������������� ������������.
		//! \{
		void
		reset_flush_notifier(
			block_stream_notifiers::flush_notifier_t * flush_notifier = nullptr );

		void
		reset_free_blocks_availability_notifier(
			block_stream_notifiers::free_blocks_availability_notifier_t *
				free_blocks_availability_notifier = nullptr );
		void
		reset_stream_operation_notifier(
			block_stream_notifiers::stream_operation_notifier_t *
				stream_operation_notifier = nullptr );
		//! \}

		//! \name ��������� ��� ��������� � �������� ������ ��/� ���.
		//! \{

		//! �������� ���� ��� ������/������.
		raw_block_t *
		occupy(
			//! �������� �����: ���� �� �������� ��������� ������.
			bool & was_waiting_for_free_blocks,
			//! ���������� ����� ��������.
			//! * \@ timeout = 0, ������� ��� �����������.
			//! * \@ timeout = &ACE_Time_Value::zero �� �����.
			//! * \@ timeout = &some_time - ������� �����-�� �����.
			const ACE_Time_Value * timeout = &ACE_Time_Value::zero );

		//! �������� ���� ��� ������/������.
		raw_block_t *
		occupy(
			//! ���������� ����� ��������.
			//! * \@ timeout = 0, ������� ��� �����������.
			//! * \@ timeout = &ACE_Time_Value::zero �� �����.
			//! * \@ timeout = &some_time - ������� �����-�� �����.
			const ACE_Time_Value * timeout = &ACE_Time_Value::zero );

		//! �������� ��������� ������ ��� ������/������.
		/*!
			\note ���� �� ���������� ����� �������� ��� ����� �� �������,
			�� �� ��������� ����� ��������, � �������� �� �����
			��������� �����������.
		*/
		void
		occupy(
			//! �������� ���������� ������.
			size_t n,
			//! �������� ������.
			raw_block_chain_t & block_chain,
			//! �������� �����: ���� �� �������� ��������� ������.
			bool & was_waiting_for_free_blocks,
			//! ���������� ����� ��������.
			//! * \@ timeout = 0, ������� ��� �����������.
			//! * \@ timeout = &ACE_Time_Value::zero �� �����.
			//! * \@ timeout = &some_time - ������� �����-�� �����.
			const ACE_Time_Value * timeout = &ACE_Time_Value::zero );

		//! �������� ��������� ������ ��� ������/������.
		/*!
			\note ���� �� ���������� ����� �������� ��� ����� �� �������,
			�� �� ��������� ����� ��������, � �������� �� �����
			��������� �����������.
		*/
		void
		occupy(
			//! �������� ���������� ������.
			size_t n,
			//! �������� ������.
			raw_block_chain_t & block_chain,
			//! ���������� ����� ��������.
			//! * \@ timeout = 0, ������� ��� �����������.
			//! * \@ timeout = &ACE_Time_Value::zero �� �����.
			//! * \@ timeout = &some_time - ������� �����-�� �����.
			const ACE_Time_Value * timeout = &ACE_Time_Value::zero );

		//! ������� ����.
		void
		release( raw_block_t * block );

		//! ������� ������� ������.
		void
		release(
			//! ������� ������.
			const raw_block_chain_t & chain );
		//! \}

		//! \name ��������� ���������� � ������� ������ � �����.
		//! \{

		//! ��������� ���� � ����� ������.
		void
		stream_push_back( raw_block_t * block );

		//! ��������� ����� � ����� ������.
		void
		stream_push_back( const raw_block_chain_t & chain );

		//! ��������� ���� � ������ ������.
		void
		stream_push_front( raw_block_t * block );

		//! ��������� ����� � ������ ������.
		void
		stream_push_front( const raw_block_chain_t & chain );

		//! ���������������� ������� � ������.
		/*!
			� channel_input_stream_t ��� ������
			���������� � ��� �� �������������, ����� �� ��� ����� ����
			�� ����� ���������, ���������� ������� ������ �� ������
			������ � ������ ������. ����� ������ ��� �������
			��� ������� ������������, ��� ������������
			����� ������ �� ���������� ���������� \@ chain �
			�� �������� ���������� �������� \@ size_in_bytes,
			������� �������� ������ ������ � �������.
		*/
		void
		stream_push_front(
			//! ������� ��� �������.
			raw_block_chain_t & chain,
			//! ������ ������ � �������.
			size_t size_in_bytes );

		//! ������� ��� ����� �� ������.
		/*!
			\return ���������� ���� � ���������� ������.
			\note
		*/
		size_t
		stream_query_all_blocks(
			//! �������� ����������.
			raw_block_chain_t & chain );

		//! ������� �� ����� n ������ �� ������.
		/*!
			\return ���������� ���� � ���������� ������.
		*/
		size_t
		stream_query_n_blocks(
			//! �������� ���������� ������.
			size_t n,
			//! �������� ����������.
			raw_block_chain_t & chain );

		//! \}

		//! �������� �����.
		/*!
			���� � ������ ���� �����,
			�� ���������� ����� ��������� ���������
			������������ �����������.
		*/
		void
		flush();

		//! ��������� �������� �� ����� ������?
		/*!
			���� ����� �������� ������, �� � �����������
			���������� notify_empty_stream � ��������� ����
			� ������������� ����������� � ������ ������.

			\return ���� � ������ ���� ��������, ��
			������ true, ����� false.
		*/
		bool
		check_stream_has_blocks();

		//! ����������� ������������� ������.
		void
		touch_stream();

		//! ��������� ������ ������.
		inline size_t
		default_block_count() const
		{
			return m_block_count;
		}

		//! ����������� ���������� ���������� ������-������� � ���������.
		inline size_t
		max_block_count() const
		{
			return m_max_block_count;
		}

		//! ���������� ������-�������, ������� � ������
		//! ������ ��������� � ���������.
		size_t
		current_block_count() const;

		//! ������ ������ ����� � ������.
		inline size_t
		single_block_size() const
		{
			return m_single_block_size;
		}

	private:
		//! ��������� ��� �������� ������������� ACE_Guard.
		typedef ACE_Guard< ACE_Thread_Mutex > scoped_lock_t;

		//! ��������� ����� ��� ��������� �����.
		raw_block_t *
		occupy_impl(
			//! ���������� ����� �� �������� ����� �����
			//! ��������� ������, ���� �� � ������ ������ �� ���������.
			//! * \@ timeout = 0, ������� ��� �����������.
			//! * \@ timeout != 0, ������� ������ �� ����������� �������.
			const ACE_Time_Value * abs_value,
			//! �������� �����: ���� �� �������� ��������� ������.
			bool & was_waiting_for_free_blocks );

		//! ��������� ����� ��� ����������� �����.
		void
		release_impl(
			raw_block_t * block );

		void
		release_impl(
			const raw_block_chain_t & chain );

		//! ����������, ���� �����, �������� �����.
		void
		allocate_blocks();

		//! ��������� ���������� ������.
		void
		deallocate_blocks();

		//! ���������� ������ �� ���������.
		/*!
			��������� ��������� �������������� ������ � �� ������������
			�������� �� �� ����� ������� ���������� ��������� ������ ������
			�����.
		*/
		const size_t m_block_count;

		//! ����������� ��������� ���������� ������.
		const size_t m_max_block_count;

		//! ����� ��� �������� ������ ������.
		/*!
			��� ������ ���������� ���������� ��������� ������
			������� ���� ������ ������, ������ ����� �������������.
		*/
		const size_t m_deallocate_block_count_threshold;

		//! ������ ���������� ������ � ���������.
		size_t m_current_block_count;

		//! ������ ������� ����� � ������.
		const size_t m_single_block_size;

		//! ������������� ������ � �������.
		//! \{
		mutable ACE_Thread_Mutex m_lock;
		ACE_Condition_Thread_Mutex m_has_free_blocks;
		//! \}

		//! ����� ������.
		raw_block_chain_t m_stream_chain;

		//! ��������� � ������� �����.
		raw_block_chain_t m_free_blocks;

		//! ���������� ����, ������� ������ ����� � ������� ������.
		size_t m_stream_size_in_bytes;

		//! ����������� ��������.
		//! \{
		block_stream_notifiers::flush_notifier_t * m_flush_notifier;
		block_stream_notifiers::free_blocks_availability_notifier_t *
			m_free_blocks_availability_notifier;
		block_stream_notifiers::stream_operation_notifier_t *
			m_stream_operation_notifier;
		//! \}
};

//
// scattered_block_stream_ref_t
//

//! ����� ������ �� scattered_block_stream_t.
class SO_5_TRANSPORT_TYPE scattered_block_stream_ref_t
{
	public:
		scattered_block_stream_ref_t();

		explicit scattered_block_stream_ref_t(
			scattered_block_stream_t * scattered_block_stream );

		scattered_block_stream_ref_t(
			const scattered_block_stream_ref_t & scattered_block_stream_ref );

		void
		operator = ( const scattered_block_stream_ref_t & scattered_block_stream_ref );

		~scattered_block_stream_ref_t();

		inline const scattered_block_stream_t *
		get() const
		{
			return m_scattered_block_stream_ptr;
		}

		inline scattered_block_stream_t *
		get()
		{
			return m_scattered_block_stream_ptr;
		}

		inline const scattered_block_stream_t *
		operator -> () const
		{
			return m_scattered_block_stream_ptr;
		}

		inline scattered_block_stream_t *
		operator -> ()
		{
			return m_scattered_block_stream_ptr;
		}

		inline scattered_block_stream_t &
		operator * ()
		{
			return *m_scattered_block_stream_ptr;
		}


		inline const scattered_block_stream_t &
		operator * () const
		{
			return *m_scattered_block_stream_ptr;
		}

	private:
		//! ��������� ���������� ������ �� scattered_block_stream
		//! � � ������ ������������� ������� ���.
		void
		inc_scattered_block_stream_ref_count();

		//! ��������� ���������� ������ �� scattered_block_stream
		//! � � ������ ������������� ������� ���.
		void
		dec_scattered_block_stream_ref_count();

		//! �������� ������.
		scattered_block_stream_t * m_scattered_block_stream_ptr;
};

} /* namespace so_5_transport */

#endif
