/*
	�����, ������� ������������ ���������� ����������� �������,
	�������� ����� � �������� ���������� ������.
*/

#if !defined( _SO_5__TEST__BENCH__WORK_THREAD_SPEED__A_BENCH_ARBITER_HPP_ )
#define _SO_5__TEST__BENCH__WORK_THREAD_SPEED__A_BENCH_ARBITER_HPP_

#include <vector>
#include <set>

#include <ace/Time_Value.h>

#include <so_5/rt/h/rt.hpp>

// ��������� � ��� ��� ��������� �����
// �������� ������������ ���������.
struct msg_bench_finish
	:
		public so_5::rt::message_t
{
	msg_bench_finish( unsigned int message_count )
		:
			m_message_count( message_count )
	{}

	virtual ~msg_bench_finish()
	{}

	unsigned int m_message_count;
};

// ��������� � ��� ����������� ���� ������ �������� ���������.
struct msg_bench_start
	:
		public so_5::rt::message_t
{
	virtual ~msg_bench_start()
	{}
};



// ����� ������� ����������� ������� ���������.
class a_bench_arbiter_t
	:
		public so_5::rt::agent_t
{
		typedef so_5::rt::agent_t base_type_t;

	public:
		a_bench_arbiter_t(
			so_5::rt::so_environment_t & env,
			const std::string & cfg_file )
			:
				base_type_t( env ),
				m_self_mbox( so_environment().create_local_mbox() ),
				m_cfg_file( cfg_file ),
				m_total_message_count( 0 ),
				m_recivers_total( 0 ),
				m_recivers_in_work( 0 )
		{}

		virtual ~a_bench_arbiter_t()
		{}

		// ����������� ������.
		virtual void
		so_define_agent();

		// ��������� ������ ������ ������ � �������.
		// ����� �����. �������������� �������� ����������
		// � ���������� �����.
		void
		so_evt_start();

		// ��������� ��������� � ����������
		// ��������� ��������� ��������� �������.
		void
		evt_on_bench_finish(
			const so_5::rt::event_data_t< msg_bench_finish > & msg );

	private:
		// ����������� mbox ������.
		so_5::rt::mbox_ref_t	m_self_mbox;

		// ���������� ������� ������� ��� ��������.
		unsigned int m_recivers_total;

		// ���������� ������� ������� ��� ��������.
		unsigned int m_recivers_in_work;

		// ���������� ������������ ���������.
		unsigned int m_total_message_count;

		// ��� ������� �����.
		const std::string m_cfg_file;

		// ������� ������ ������.
		ACE_Time_Value m_start_time;

		std::vector< unsigned int > m_msg_bloks_size;

		std::set< std::string >  m_senders;
		std::set< std::string >  m_receivers;
};

#endif
