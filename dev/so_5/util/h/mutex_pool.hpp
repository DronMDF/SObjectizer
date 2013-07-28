/*
	SObjectizer 5.
*/

/*!
	\file
	\brief �����, ��� ����������� ���� ���������.
*/

#if !defined( _SO_5__UTIL__MUTEX_POOL_HPP_ )
#define _SO_5__UTIL__MUTEX_POOL_HPP_

#include <vector>

#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>

namespace so_5
{

namespace util
{

//
// mutex_pool_t
//

//! ����� ��� ���������� ���� �������� ��������� ����.
template < class MUTEX >
class mutex_pool_t
{
	public:
		explicit mutex_pool_t(
			unsigned int pool_size )
			:
				m_mutex_pool_size( pool_size ),
				m_mutex_pool( new MUTEX[ m_mutex_pool_size ] ),
				m_mutex_pool_charge( m_mutex_pool_size, 0 )
		{}
		~mutex_pool_t()
		{
			delete [] m_mutex_pool;
		}

		//! ����� ������ � �����������.
		MUTEX &
		allocate_mutex()
		{
			ACE_Guard< ACE_Thread_Mutex > lock( m_mutex_lock );

			size_t min_index = std::min_element(
				m_mutex_pool_charge.begin(),
				m_mutex_pool_charge.end() ) - m_mutex_pool_charge.begin();

			++m_mutex_pool_charge[ min_index ];
			return m_mutex_pool[ min_index ];
		}

		//! ���������� �� ������������� �������.
		/*!
			\return ���� ������ ����������� ����,
			�� ��������� ���������� ������ �� ���� � ����������
			true, � ��������� ������ ������ false � ������
			�� �������.
		*/
		bool
		deallocate_mutex( MUTEX & m )
		{
			// ���� ������ �� ����, �� ����� ���������
			// ���������� ������ �� ����.
			if( &m >= m_mutex_pool &&
				&m < m_mutex_pool + m_mutex_pool_size )
			{
				ACE_Guard< ACE_Thread_Mutex > lock( m_mutex_lock );
				const size_t mutex_index =  &m - m_mutex_pool;

				--m_mutex_pool_charge[ mutex_index ];
				return true;
			}

			// ���� ������ �� ���, ��:
			return false;
		}

	private:
		//! ����� ��� ������ ��������.
		ACE_Thread_Mutex m_mutex_lock;

		//! ������ ���� ��������.
		const size_t m_mutex_pool_size;

		MUTEX * const m_mutex_pool;

		//! ������ ��� �������� ���������� ������ �� �������.
		std::vector< unsigned int > m_mutex_pool_charge;
};

} /* namespace util */

} /* namespace so_5 */

#endif
