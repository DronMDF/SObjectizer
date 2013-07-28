/*
	MBAPI 4.
*/


/*!
	\file
	\brief ��������������� �������.
*/

#if !defined( _MBAPI_4__COMM__IMPL__TRAFFIC_TRANSFORMATOR_HPP_ )
#define _MBAPI_4__COMM__IMPL__TRAFFIC_TRANSFORMATOR_HPP_

#include <memory>

#include <oess_2/io/h/binstream.hpp>

#include <mbapi_4/proto/h/package_header.hpp>

namespace mbapi_4
{

namespace comm
{

namespace impl
{

//
// traffic_transformator_iface_t
//

//! ��������� ������������� �������.
class traffic_transformator_iface_t
{
	public:
		virtual ~traffic_transformator_iface_t();

		//! ���������������� ������ ���������� ������ � �������� �����.
		/*!
			\note ����� ����� ������, ������� ������������,
			������ ���� �� ������ ��� ����� ����� �������� ���������.
		*/
		virtual oess_2::io::ibinstream_t &
		transform_input(
			oess_2::io::ibinstream_t & source, size_t stream_size ) = 0;

		//! �������� ����� ���� ���������� ������ ����� ��������������.
		virtual oess_2::io::obinstream_t &
		start_output(
			//! �������� ����� ������� ������ ������� ������ �����
			//! �������������.
			oess_2::io::obinstream_t & target ) = 0;

		//! ������������� ������ � �������.
		virtual void
		finish_output() = 0;

		//! �������� magic_number ��� ���������� �������.
		virtual const mbapi_4::proto::package_header_t::magic_number_t &
		quiery_magic_value() const = 0;
};

//! ��������� unique_ptr ��� traffic_transformator_iface_t.
typedef std::unique_ptr< traffic_transformator_iface_t > traffic_transformator_unique_ptr_t;

//! ������� ���������� ������������� �������, ������� ������ �� ������.
traffic_transformator_unique_ptr_t
create_transparent_traffic_transformator();

//! ������� ���������� ������������� �������, ������� ������ �� ������.
traffic_transformator_unique_ptr_t
create_zlib_traffic_transformator();

} /* namespace impl */

} /* namespace comm */

} /* namespace mbapi_4 */

#endif
