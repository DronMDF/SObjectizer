/*
	SObjectizer 5.
*/

/*!
	\file
	\brief �����, ��� ����������� mbox-�� ����������
	� �������� ����������� mbox-�� � �������.
*/

#if !defined( _SO_5__RT__IMPL__MBOX_CORE_HPP_ )
#define _SO_5__RT__IMPL__MBOX_CORE_HPP_

#include <memory>
#include <string>
#include <map>
#include <vector>

#include <ace/Thread_Mutex.h>
#include <ace/RW_Thread_Mutex.h>

#include <so_5/rt/h/atomic_refcounted.hpp>

#include <so_5/util/h/mutex_pool.hpp>

#include <so_5/rt/h/mbox_ref.hpp>
#include <so_5/rt/h/nonempty_name.hpp>

#include <so_5/rt/h/mbox.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

class mbox_core_ref_t;

//
// mbox_core_t
//

//! �����, ��� ����������� mbox-�� ����������
//! � �������� ����������� mbox-�� � �������.
class mbox_core_t
	:
		private atomic_refcounted_t
{
		friend class mbox_core_ref_t;

		mbox_core_t( const mbox_core_t & );
		void
		operator = ( const mbox_core_t & );

	public:
		explicit mbox_core_t(
			unsigned int mutex_pool_size );
		virtual ~mbox_core_t();

		//! ������� ���������� ��������� mbox.
		mbox_ref_t
		create_local_mbox();

		//! ������� ����������� ��������� mbox.
		mbox_ref_t
		create_local_mbox(
			//! ��� mbox-�.
			const nonempty_name_t & nonempty_name );

		//! ������� ���������� ��������� mbox,
		//! ������� ���������� �������� ������.
		mbox_ref_t
		create_local_mbox(
			//! ����� ��������� �������������
			std::unique_ptr< ACE_RW_Thread_Mutex >
				lock_ptr );

		//! ������� ����������� ��������� mbox,
		//! ������� ���������� �������� ������.
		mbox_ref_t
		create_local_mbox(
			//! ��� mbox-�.
			const nonempty_name_t & nonempty_name,
			//! ����� ��������� �������������
			std::unique_ptr< ACE_RW_Thread_Mutex >
				lock_ptr );

		//! ���������� ������ �� ����������� mbox.
		/*!
			���� ��� ���� ��������� ������,
			�� ����������� mbox ���������.
		*/
		void
		destroy_mbox(
			//! ��� mbox-�.
			const std::string & name );

		//! ����� ������ � �����������.
		ACE_RW_Thread_Mutex &
		allocate_mutex();

		//! ���������� �� ������������� �������.
		/*!
			���� ������ \a m �� �������� �������� �� ����,
			�� ���������, ��� �� ���������� �������� ��� ������ \c new
			� ������� ����� ������ ����� \c delete.
		*/
		void
		deallocate_mutex( ACE_RW_Thread_Mutex & m );

	private:
		//! ��� ��������.
		util::mutex_pool_t< ACE_RW_Thread_Mutex > m_mbox_mutex_pool;

		//! ����� ��� ������ �� ��������.
		ACE_RW_Thread_Mutex m_dictionary_lock;

		//! ���������� � ����������� mbox-�.
		struct named_mbox_info_t
		{
			named_mbox_info_t()
				:
					m_external_ref_count( 0 )
			{}

			named_mbox_info_t(
				const mbox_ref_t mbox )
				:
					m_external_ref_count( 1 ),
					m_mbox( mbox )
			{}

			//! ���������� ������ ����������� ������� mbox_ref.
			unsigned int m_external_ref_count;
			//! �������� mbox � ������� ��������
			//! ���� ���������.
			mbox_ref_t m_mbox;
		};

		//! ��� ��� ������� ����������� mbox-��
		typedef std::map< std::string, named_mbox_info_t >
			named_mboxes_dictionary_t;

		//! ������� ����������� mbox-��.
		named_mboxes_dictionary_t m_named_mboxes_dictionary;
};

//! ����� ����� ������ �� mbox_core_t.
class mbox_core_ref_t
{
	public:
		mbox_core_ref_t();

		explicit mbox_core_ref_t(
			mbox_core_t * mbox_core );

		mbox_core_ref_t(
			const mbox_core_ref_t & mbox_core_ref );

		void
		operator = ( const mbox_core_ref_t & mbox_core_ref );

		~mbox_core_ref_t();

		inline const mbox_core_t *
		get() const
		{
			return m_mbox_core_ptr;
		}

		inline mbox_core_t *
		get()
		{
			return m_mbox_core_ptr;
		}

		inline const mbox_core_t *
		operator -> () const
		{
			return m_mbox_core_ptr;
		}

		inline mbox_core_t *
		operator -> ()
		{
			return m_mbox_core_ptr;
		}

		inline mbox_core_t &
		operator * ()
		{
			return *m_mbox_core_ptr;
		}


		inline const mbox_core_t &
		operator * () const
		{
			return *m_mbox_core_ptr;
		}

		inline bool
		operator == ( const mbox_core_ref_t & mbox_core_ref ) const
		{
			return m_mbox_core_ptr ==
				mbox_core_ref.m_mbox_core_ptr;
		}

		inline bool
		operator < ( const mbox_core_ref_t & mbox_core_ref ) const
		{
			return m_mbox_core_ptr <
				mbox_core_ref.m_mbox_core_ptr;
		}

	private:
		//! ��������� ���������� ������ �� mbox_core
		//! � � ������ ������������� ������� ���.
		void
		inc_mbox_core_ref_count();

		//! ��������� ���������� ������ �� mbox_core
		//! � � ������ ������������� ������� ���.
		void
		dec_mbox_core_ref_count();

		mbox_core_t * m_mbox_core_ptr;
};

} /* namespace impl */

} /* namespace rt */

} /* namespace so_5 */

#endif
