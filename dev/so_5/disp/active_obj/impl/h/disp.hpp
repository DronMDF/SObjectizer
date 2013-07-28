/*
	SObjectizer 5.
*/

/*!
	\file
	\brief �������� ����� ���������� �������� ��������.
*/

#if !defined( _SO_5__DISP__ACTIVE_OBJ__IMPL__DISP_HPP_ )
#define _SO_5__DISP__ACTIVE_OBJ__IMPL__DISP_HPP_

#include <map>

#include <ace/Thread_Mutex.h>

#include <so_5/rt/h/disp.hpp>

namespace so_5 {

namespace rt
{
	class agent_t;

} /* namespace rt */


namespace disp
{

namespace active_obj
{

namespace impl
{

//
// dispatcher_t
//

/*!
	\brief ��������� �������� ��������.
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
			//! ����� ������� �������� ���� �������������.
			const so_5::rt::agent_ref_t & agent_ref,
			//! ���������� �������,
			//! ������� ������ ��������� � ����� ������.
			unsigned int event_count );
		//! \}

		so_5::rt::dispatcher_t &
		create_disp_for_agent( const so_5::rt::agent_t & agent );

		void
		destroy_disp_for_agent( const so_5::rt::agent_t & agent );

	private:
		typedef std::map<
				const so_5::rt::agent_t *,
				so_5::rt::dispatcher_ref_t >
			agent_disp_map_t;

		//! ����� ����������� ��������� ��� �������.
		agent_disp_map_t m_agent_disp;

		//! ����, ��������� � ���, ��� ������ ����������
		//! ������ ����������.
		bool m_shutdown_started;

		//! ����� ��� �������� �������� � �������� �����������.
		ACE_Thread_Mutex m_lock;
};

} /* namespace impl */

} /* namespace active_obj */

} /* namespace disp */

} /* namespace so_5 */

#endif
