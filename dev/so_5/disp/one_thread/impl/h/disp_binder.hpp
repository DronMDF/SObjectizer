/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ��������� �������� ������ � ���������� one_thread.
*/

#if !defined( _SO_5__DISP__ONE_THREAD__IMPL__DISP_BINDER_HPP_ )
#define _SO_5__DISP__ONE_THREAD__IMPL__DISP_BINDER_HPP_

#include <so_5/rt/h/disp_binder.hpp>
#include <so_5/rt/h/agent_ref.hpp>
#include <so_5/rt/impl/h/so_environment_impl.hpp>

namespace so_5
{

namespace disp
{

namespace one_thread
{

namespace impl
{

//! ������������� ������ � ���������� ����� ����.
class disp_binder_t
	:
		public so_5::rt::disp_binder_t
{
	public:
		explicit disp_binder_t(
			const std::string & disp_name );
		virtual ~disp_binder_t();


		//! ������� �������� ������ � ����������.
		virtual void
		bind_agent(
			//! ����� so_5, ������� ����������� �����.
			so_5::rt::impl::so_environment_impl_t & env,
			//! ������ �� �����, ������� ������������� � ����������.
			so_5::rt::agent_ref_t & agent_ref );

		//! ������� �������� ������ � ����������.
		virtual void
		unbind_agent(
			//! ����� so � ���������� ������� ���������� ��������.
			so_5::rt::impl::so_environment_impl_t & env,
			//! ����� �������� ���� ��������� � ����������.
			so_5::rt::agent_ref_t & agent_ref );

	private:
		//! ��� ���������� � �������� ������ ���� �������� �����.
		const std::string m_disp_name;
};

} /* namespace impl */

} /* namespace one_thread */

} /* namespace disp */

} /* namespace so_5 */

#endif
