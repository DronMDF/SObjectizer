/*
	MBAPI 4.
*/

/*!
	\file
	\brief ������ ������ ������.
*/

#if !defined( _MBAPI_4__IMPL__IOSESSION_HPP_ )
#define _MBAPI_4__IMPL__IOSESSION_HPP_

#include <ace/RW_Thread_Mutex.h>

#include <oess_2/io/h/binstream.hpp>

#include <oess_2/stdsn/h/ent_std.hpp>

#include <mbapi_4/impl/h/types.hpp>

namespace mbapi_4
{

namespace impl
{

class mbapi_layer_impl_t;

//
// isession_t
//

//! ������ ������.
class isession_t
{
	friend class mbapi_layer_impl_t;

	isession_t( const isession_t & );
	void
	operator = ( const isession_t & );

	isession_t(
		//! ����� �� �������� ����� ������������ ������.
		oess_2::io::ibinstream_t & istream,
		//! ����� ������������� �� ����� ������.
		ACE_RW_Thread_Mutex & lock,
		//! ������ ������������ ������������ � �������� oess_2::stdsn::ient_std_t.
		const types_repositories_list_t & types_repositories );

	public:
		~isession_t();

		inline oess_2::stdsn::ient_std_t &
		ient()
		{
			return *m_ient;
		}

	private:
		//! ����� ������� ������� � ������ ������ �� ����� ������.
		ACE_RW_Thread_Mutex & m_lock;

		//! �������� ��������.
		std::unique_ptr< oess_2::stdsn::ient_std_t > m_ient;
};

//! ��������� unique_ptr ��� isession_t.
typedef std::unique_ptr< isession_t > isession_unique_ptr_t;

//
// osession_t
//

//! ������ ������.
class osession_t
{
	friend class mbapi_layer_impl_t;
	friend class stagepoint_bind_impl_t;

	osession_t( const osession_t & );
	void
	operator = ( const osession_t & );

	osession_t(
		//! ����� � ������� ����� ������������ ������.
		oess_2::io::obinstream_t & ostream );

	public:
		~osession_t();

		inline oess_2::stdsn::oent_std_t &
		oent()
		{
			return *m_oent;
		}

	private:
		//! �������� ��������.
		std::unique_ptr< oess_2::stdsn::oent_std_t > m_oent;
};

//! ��������� unique_ptr ��� osession_t.
typedef std::unique_ptr< osession_t > osession_unique_ptr_t;

} /* namespace impl */

} /* namespace mbapi_4 */

#endif
