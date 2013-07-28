/*
	SObjectizer 5.
*/

/*!
	\file
	\brief �������� ����� ���������� � ����� ������� �����.
*/

#if !defined( _SO_5__DISP__ONE_THREAD__IMPL__DISP_HPP_ )
#define _SO_5__DISP__ONE_THREAD__IMPL__DISP_HPP_

#include <so_5/rt/h/disp.hpp>
#include <so_5/disp/reuse/work_thread/h/work_thread.hpp>

namespace so_5
{

namespace disp
{

namespace one_thread
{

namespace impl
{

//
// dispatcher_t
//

/*!
	\brief ��������� � ����� ������� ����� � �������� ������.
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
		virtual void
		put_event_execution_request(
			//! ����� ������� �������� ���� �������������.
			const so_5::rt::agent_ref_t & agent_ref,
			//! ���������� �������,
			//! ������� ������ ��������� � ����� ������.
			unsigned int event_count );
		//! \}

	private:
		//! ������� ���� ��� ����������.
		so_5::disp::reuse::work_thread::work_thread_t m_work_thread;
};


} /* namespace impl */

} /* namespace one_thread */

} /* namespace disp */

} /* namespace so_5 */

#endif
