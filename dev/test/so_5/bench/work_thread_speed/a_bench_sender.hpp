/*
	�����, ������� ���������� ���������.
*/

#if !defined( _SO_5__TEST__BENCH__WORK_THREAD_SPEED__A_BENCH_SENDER_HPP_ )
#define _SO_5__TEST__BENCH__WORK_THREAD_SPEED__A_BENCH_SENDER_HPP_

#include <vector>

#include <so_5/rt/h/rt.hpp>

#include "a_bench_arbiter.hpp"

// ����� ������� ����������� ������� ���������.
class a_bench_sender_t
	:
		public so_5::rt::agent_t
{
		typedef so_5::rt::agent_t base_type_t;

	public:
		a_bench_sender_t(
			so_5::rt::so_environment_t & env,
			// mbox, ���� ���������� ���������.
			const so_5::rt::mbox_ref_t & target_mbox,
			// mbox �������.
			const so_5::rt::mbox_ref_t & arbiter_mbox,
			// ���������� ���������.
			unsigned int message_count,
			// ������� ������������ �� ��� ���������
			const std::vector< unsigned int > & packs_size )
			:
				base_type_t( env ),
				m_self_mbox( so_environment().create_local_mbox() ),
				m_target_mbox( target_mbox ),
				m_arbiter_mbox( arbiter_mbox ),
				m_message_count( message_count ),
				m_sent_message_count( 0 ),
				m_iteration_count( 0 ),
				m_packs_size( packs_size )
		{}

		virtual ~a_bench_sender_t()
		{}

		virtual void
		so_define_agent();

		// ������������� ����� ���������.
		void
		evt_genetrate_pack(
			const so_5::rt::event_data_t< msg_bench_start > & msg );

private:
	// ����������� mbox ������.
	so_5::rt::mbox_ref_t	m_self_mbox;

	// mbox, ���� ���������� ���������.
	so_5::rt::mbox_ref_t	m_target_mbox;

	// mbox �������.
	so_5::rt::mbox_ref_t	m_arbiter_mbox;

	// ���������� ���������� ���������.
	const unsigned int m_message_count;

	// ���������� ���������� �� ������ ������ ���������.
	unsigned int m_sent_message_count;

	unsigned int m_iteration_count;

	// ������� ����� ���������.
	const std::vector< unsigned int > & m_packs_size;
};

#endif
