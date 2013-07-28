	/*
	SObjectizer 5.
*/

/*!
	\file
	\brief �������� ����� ���������� �������� �����.
*/

#if !defined( _SO_5__DISP__ACTIVE_GROUP__IMPL__DISP_HPP_ )
#define _SO_5__DISP__ACTIVE_GROUP__IMPL__DISP_HPP_

#include <map>

#include <ace/Thread_Mutex.h>

#include <so_5/rt/h/disp.hpp>

namespace so_5 {

namespace rt
{

class agent_t;

} /* namespace rt */


namespace disp {

namespace active_group {

namespace impl {

//
// dispatcher_t
//

/*!
	\brief ��������� �������� �����.
*/
class dispatcher_t
	:
		public so_5::rt::dispatcher_t
{
	public:
		dispatcher_t();
		virtual ~dispatcher_t();

		//! ���������� ������� ���������� so_5::rt::dispatcher_t.
		//! \{

		//! ��������� ���������.
		virtual ret_code_t
		start();

		//! ���� ������ ���������� ��������� ������.
		virtual void
		shutdown();

		//! ������� ������� ���������� ������ ����������
		virtual void
		wait();

		//! ��������� ������ �� ���������� ������� �������.
		//! �.�. ������������� ����� ������� ������.
		/*! \note �� ������ ���������� �������� � ����� ����������. */
		virtual void
		put_event_execution_request(
			const so_5::rt::agent_ref_t &,
			unsigned int);
		//! \}

		//! �������� ��������� �������� ������ \@ group_name .
		/*! ���� ���������� ��� ����� �������� ������ ���,
			�� �� ��������� �������� �������, ��� �� ��� ����� ���������
			1 �����. ���� ����������, ��� ��������� ����������, ��
			�������� �������, ��� �� ����� ������������ ��� 1 �����.
		*/
		so_5::rt::dispatcher_t &
		query_disp_for_group( const std::string & group_name );

		//! ��������� ��������� ��� �������� ������ \@ group_name .
		/*! ��������, ��� �� 1 ����� ������ ������ ����������
			���� �������� �������. �, ���� ��� ����������� ���������
			����� ���� �������� ������, �� ��������� ���
			���� �������� ������ ������������.
		*/
		void
		release_disp_for_group( const std::string & group_name );

	private:
		//! ��������������� ����� ��� �������� �������
		//! ���������� � �������� ������.
		struct disp_with_ref_t
		{
			disp_with_ref_t()
				:
					m_user_agent( 0 )
			{}

			disp_with_ref_t(
				const so_5::rt::dispatcher_ref_t & disp_ref,
				unsigned int user_agent )
				:
					m_disp_ref( disp_ref ),
					m_user_agent( user_agent)
			{}

			so_5::rt::dispatcher_ref_t m_disp_ref;
			unsigned int m_user_agent;
		};

		//! ��� ���� �������� ����� � ��
		//! ����������� � ����� ������� �����.
		typedef std::map<
				std::string,
				disp_with_ref_t >
			active_group_disp_map_t;

		//! ����� ����������� ��������� ��� �������� ������.
		active_group_disp_map_t m_group_disp;

		//! ����, ��������� � ���, ��� ������ ����������
		//! ������ ����������.
		bool m_shutdown_started;

		//! ����� ��� �������� �������� � �������� �����������.
		ACE_Thread_Mutex m_lock;
};

} /* namespace impl */

} /* namespace active_group */

} /* namespace disp */

} /* namespace so_5 */

#endif
