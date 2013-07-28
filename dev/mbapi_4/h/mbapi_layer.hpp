/*
	MBAPI 4.
*/

/*!
	\file
	\brief ���� ��� ������ � mbapi_4.
*/

#if !defined( _MBAPI_4__MBAPI_LAYER_HPP_ )
#define _MBAPI_4__MBAPI_LAYER_HPP_

#include <memory>

#include <oess_2/stdsn/h/serializable.hpp>

#include <so_5/rt/h/so_layer.hpp>

#include <mbapi_4/h/declspec.hpp>

#include <mbapi_4/defs/h/endpoint.hpp>
#include <mbapi_4/defs/h/stagepoint.hpp>
#include <mbapi_4/defs/h/oess_id_wrapper.hpp>

#include <mbapi_4/h/stagepoint_bind.hpp>
#include <mbapi_4/h/endpoint_bind.hpp>

namespace mbapi_4
{

namespace impl
{

// ���������� ���� ������ � mbapi_4.
class mbapi_layer_impl_t;

} /* namespace impl */

//
// mbapi_layer_t
//

//! ���� mbapi ��� ������ �������������� mbox-��.
class MBAPI_4_TYPE mbapi_layer_t
	:
		public so_5::rt::so_layer_t
{
	public:
		mbapi_layer_t();
		virtual ~mbapi_layer_t();

		//! �������� ������ � ���������� ����.
		//! \{
		impl::mbapi_layer_impl_t &
		impl();

		const impl::mbapi_layer_impl_t &
		impl() const;
		//! \}

		//! ���������� � �������� ������������.
		//! \{
		void
		add_repository(
			const oess_2::stdsn::types_repository_t & types_repository );

		void
		remove_repository(
			const oess_2::stdsn::types_repository_t & types_repository );
		//! \}

		//! �������� ������������ �������� ����� � ������.
		//! \{
		stagepoint_bind_unique_ptr_t
		create_stagepoint_bind(
			const stagepoint_t & stagepoint,
			so_5::rt::agent_t & agent );

		endpoint_bind_unique_ptr_t
		create_endpoint_bind(
			const endpoint_t & endpoint,
			so_5::rt::agent_t & agent );

		endpoint_bind_unique_ptr_t
		create_endpoint_bind(
			const endpoint_stage_chain_t & endpoint_stage_chain,
			so_5::rt::agent_t & agent );
		//! \}

		//! �������� ������ ��������� endpoint-��.
		endpoint_list_t
		query_endpoint_list() const;

	protected:
		//! \name ���������� �������������� �������.
		//! \{
		//! ��������� ������ ���������.
		so_5::ret_code_t
		start();

		//! ���������� ������ ���������.
		void
		shutdown();

		//! ����� ���������� ���� ���������.
		void
		wait();
		//! \}

	private:
		friend class message_base_t;

		//! ���������� ������ �������� ���������.
		//! \{

		//! ��������� ��������� ������ �� ������� ������.
		void
		send(
			const endpoint_t & from,
			const endpoint_t & to,
			const stagepoint_t & current_stage,
			const oess_id_wrapper_t & oess_id,
			std::unique_ptr< oess_2::stdsn::serializable_t > msg );

		//! ������ �������� ��������� �� ����� �������� ������
		//! �� ������.
		void
		send(
			const endpoint_t & from,
			const endpoint_t & to,
			const oess_id_wrapper_t & oess_id,
			std::unique_ptr< oess_2::stdsn::serializable_t > msg );
		//! \}

		std::unique_ptr< impl::mbapi_layer_impl_t > m_impl;
};

} /* namespace mbapi_4 */

#endif
