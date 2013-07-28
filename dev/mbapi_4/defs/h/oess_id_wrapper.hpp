/*
	MBAPI 4.
*/

/*!
	\file
	\brief ������� ��� ��������������� �ess.
*/

#if !defined( _MBAPI_4__DEFS__OESS_ID_WRAPPER_HPP_ )
#define _MBAPI_4__DEFS__OESS_ID_WRAPPER_HPP_

#include <string>
#include <memory>

#include <mbapi_4/defs/h/declspec.hpp>

namespace mbapi_4
{

//
// oess_id_wrapper_t
//

//! ������� ��� ��������������� oess ����.
/*!
	������ ��� ����������� �������� ������� �� ��������� ���������,
	� ����� ��� �������� ���� ���������, � ������ �������� �� �� ����
	� �������� ����.
*/
class MBAPI_4_DEFS_TYPE oess_id_wrapper_t
{
	public:
		oess_id_wrapper_t();

		//! ����������� ��� ������ ����� ������ ���� �������
		//! ��������, �.�. ��� ������� ��������
		//! ���������� ���������� std::string.
		oess_id_wrapper_t(
			const std::string & type_name );

		//! �����������, ��� ������, ����� ��� ���� �����
		//! �������� ����� oess_serializer_t::type_name().
		oess_id_wrapper_t(
			const std::string * type_name );

		oess_id_wrapper_t(
			const oess_id_wrapper_t & oess_id );

		void
		operator = (
			const oess_id_wrapper_t & oess_id );

		~oess_id_wrapper_t();

		inline bool
		operator < ( const oess_id_wrapper_t & oess_id_wrapper ) const
		{
			return
				m_type_name !=  oess_id_wrapper.m_type_name &&
				*m_type_name < *oess_id_wrapper.m_type_name;
		}

		inline bool
		operator == ( const oess_id_wrapper_t & oess_id_wrapper ) const
		{
			return m_type_name ==  oess_id_wrapper.m_type_name ||
				*m_type_name == *oess_id_wrapper.m_type_name;
		}

		//! ��������� ������������� ����.
		inline const std::string &
		str() const
		{
			return *m_type_name;
		}

	private:
		std::unique_ptr< std::string > m_local_storrage;

		//! ��� oess �������������� ����.
		const std::string * m_type_name;
};

} /* namespace mbapi_4 */

#endif
