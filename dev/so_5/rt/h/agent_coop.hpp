/*
	SObjectizer 5.
*/

/*!
	\file
	\brief ����� ���������� so_5.
*/

#if !defined( _SO_5__RT__AGENT_COOP_HPP_ )
#define _SO_5__RT__AGENT_COOP_HPP_

#include <vector>
#include <memory>

#include <so_5/h/declspec.hpp>
#include <so_5/h/ret_code.hpp>
#include <so_5/h/types.hpp>
#include <so_5/h/throwing_strategy.hpp>

#include <so_5/rt/h/nonempty_name.hpp>
#include <so_5/rt/h/agent_ref.hpp>
#include <so_5/rt/h/disp_binder.hpp>

namespace so_5
{

namespace rt
{

namespace impl
{

class agent_core_t;
class so_environment_impl_t;

} /* namespace impl */


class so_environment_t;
class agent_coop_t;

//! ��������� ��� agent_coop_t.
typedef std::unique_ptr< agent_coop_t > agent_coop_unique_ptr_t;

//! ���������� �������.
/*!
	������ ��� ����������� ��������� �������, ��� ����� ������� �������
	SObjectizer. ��� ����������� ���������� ��� ������ ������ ����
	����������������, ����� ����������� ���������� �� ����������.
	������ ����������� � ���������� ����� ����� ���������� add_agent().
	��� ���������� ������ � ���������� �� ���������� �������� ���� �������.
*/
class SO_5_TYPE agent_coop_t
{
		friend class agent_t;
		friend class impl::agent_core_t;

		//! ����������� ����������.
		agent_coop_t(
			//! ��� ����������.
			const nonempty_name_t & name,
			//! �������� � ���������� ��� �������
			//! ���������� ��-���������.
			disp_binder_unique_ptr_t & coop_disp_binder,
			//! ���������� ����� so_5.
			impl::so_environment_impl_t & env_impl );

	public:
		//! �������� ����������.
		static agent_coop_unique_ptr_t
		create_coop(
			//! ��� ����������.
			const nonempty_name_t & name,
			//! �������� � ���������� ��� �������
			//! ���������� ��-���������.
			disp_binder_unique_ptr_t & coop_disp_binder,
			//! ���������� ����� so_5 � ������ �������
			//! ��������� ����������.
			impl::so_environment_impl_t & env_impl );

		virtual ~agent_coop_t();

		//! �������� ��� ����������.
		const std::string &
		query_coop_name() const;

		//! �������� ������ � ����������,
		//! �������� ������� ��������� � gent_coop_t.
		ret_code_t
		add_agent(
			//! �����.
			const agent_ref_t & agent_ref );

		//! �������� ������ � ���������� � ��������� ��������
		//! � ����������� ���������,
		//! �������� ������� ��������� � agent_coop_t.
		/*!
			���� �����, ����������� � ����������, ������ �����
			�������� � ���������� �������� �� ��������
			���� ����������, �� ������������ ���� �����,
			�������� \a disp_binder �������� ����������
			������������ ��� ������� ������ �������������.
		*/
		ret_code_t
		add_agent(
			//! �����.
			const agent_ref_t & agent_ref,
			//! ������ ��� �������� � ����������.
			disp_binder_unique_ptr_t disp_binder );

		static inline void
		call_agent_finished( agent_coop_t & coop )
		{
			coop.agent_finished();
		}

		//! ��������� ������������� ������������� ����������.
		static inline void
		call_final_deregister_coop( agent_coop_t * coop )
		{
			coop->final_deregister_coop();
		}

	private:
		//! ����� � ��� ��������.
		/*!
			������� ������ � ������� �������� ������ � �� �������������.
		*/
		struct agent_with_disp_binder_t
		{
			agent_with_disp_binder_t(
				//! �����.
				const agent_ref_t & agent_ref,
				//! ��������.
				const disp_binder_ref_t & binder )
				:
					m_agent_ref( agent_ref ),
					m_binder( binder )
			{}

			//! �����.
			agent_ref_t m_agent_ref;

			//! ��������.
			disp_binder_ref_t m_binder;
		};

		//! ��� ��� ������� ������� c �� ���������� � �����������.
		typedef std::vector< agent_with_disp_binder_t > agent_array_t;

		//! ��������� ������� � ����������.
		void
		bind_agents_to_coop();

		//! ������� � ���� ������� ����� so_define_agent().
		void
		define_all_agents();

		//! ������� � ���� ������� ����� undefine_agent().
		void
		undefine_all_agents();

		//! ������� � ������� undefine_agent().
		void
		undefine_some_agents(
			//! �������� �� ������ � ���� ����������������
			//! �������� ���� �������� undefine_agent().
			agent_array_t::iterator it );

		//! ��������� �������� ������� � �����������.
		void
		bind_agents_to_disp();

		//! �������� ������� �� �����������.
		void
		unbind_agents_to_disp(
			//! �������� �� ������ ���� ����������������
			//! �������� ���� �������� �� �����������.
			agent_array_t::iterator it );

		//! ��������, ��� ��������� ����� ����������, �������� ������.
		/*!
			����� ������������� ����������, �� ������,
			���������� ������������ �������, ������� ������
			������ � ������� �� ����� �������������, � �����
			����� �����, ��� ��� �������, ������� ��� ���� ����
			���������� - ����������, ����� ������� �����������
			����������, ��� �� ��������.
			����� ��� ������ ���������� ��������� ���� ������,
			���������� ����� ����������.
		*/
		void
		agent_finished();

		//! ��������� ������������� ������������� ����������.
		void
		final_deregister_coop();

		//! ��� ����������.
		const std::string m_coop_name;

		//! ������ ��� ������������� �������� ��� �����������.
		ACE_Thread_Mutex & m_lock;

		//! ���� ����, ��� ������ ���������� �������������.
		bool m_agents_are_undefined;

		//! �������� � ���������� ��� ������� ���������� ��-���������.
		disp_binder_ref_t m_coop_disp_binder;

		//! ������ ������� ����������.
		agent_array_t m_agent_array;

		//! ���������� ����� so_5.
		impl::so_environment_impl_t & m_so_environment_impl;

		//! ���������� ���������� �������.
		atomic_counter_t m_working_agents_count;
};

//! ��� ��� ������ ��������� �� agent_coop_t.
typedef std::shared_ptr< agent_coop_t > agent_coop_ref_t;

} /* namespace rt */

} /* namespace so_5 */

#endif
