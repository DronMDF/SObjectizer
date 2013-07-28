/*
	MBAPI 4.
*/

/*!
	\file
	\brief ���� ������ MBAPI 4.
*/

#if !defined( _MBAPI_4__DEFS__RET_CODE_HPP_ )
#define _MBAPI_4__DEFS__RET_CODE_HPP_

#include <so_5/h/ret_code.hpp>


namespace mbapi_4
{

//! ��������������� ������ ��� ����������� ����� ������ ������������� ����.
#define SO_5_MBAPI_4_RET_CODE( rc ) 3000 + rc

//! ����������� �������� ����� �� ������� ����.
const int rc_duplicating_endpoint_on_local_node = SO_5_MBAPI_4_RET_CODE( 1 );

//! ����������� �����-������ �� ������� ����.
const int rc_duplicating_stagepoint_on_local_node = SO_5_MBAPI_4_RET_CODE( 2 );

//! ���� mbapi �� ����������.
const int rc_mbapi_layer_not_set = SO_5_MBAPI_4_RET_CODE( 10 );

//! ����� ��� �������� ���������� �� ������ ��� ��������� � ������ ���������.
const int rc_duplicating_event_subscription = SO_5_MBAPI_4_RET_CODE( 20 );
//! ����� �� ���������� ���������� �� ������ ��� ��������� � ������ ���������.
const int rc_event_not_subscribed = SO_5_MBAPI_4_RET_CODE( 21 );

//! �� ���������� ����������� �����.
const int rc_zlib_crc_mismatch =  SO_5_MBAPI_4_RET_CODE( 30 );
//! ���������� ������ ������ ��������.
const int rc_zlib_uncompress_failure = SO_5_MBAPI_4_RET_CODE( 31 );
//! ���������� ������ ������ ��������.
const int rc_zlib_compress_failure = SO_5_MBAPI_4_RET_CODE( 32 );
//! � ������������� ������ ��� ���������� ����� ��� ���������.
const int rc_zlib_uncompressed_pkg_has_no_header =  SO_5_MBAPI_4_RET_CODE( 33 );
//! ����������� �������� ��������.
const int rc_unknown_compression_algorithm =  SO_5_MBAPI_4_RET_CODE( 34 );

//! �� �������� ���������.
const int rc_proto_invalid_header = SO_5_MBAPI_4_RET_CODE( 40 );
//! ������������ ���������.
const int rc_proto_syntax_error = SO_5_MBAPI_4_RET_CODE( 41 );

//! ��� ����� �������� ������������.
const int rc_invalid_point_name = SO_5_MBAPI_4_RET_CODE( 50 );

//! ��� ����� �������� ������������.
const int rc_invalid_stage_desc_format = SO_5_MBAPI_4_RET_CODE( 51 );

//! ��� ����� �� ������ �������� ����� ��������� � �� ������.
const int rc_sp_name_matches_ep_name = SO_5_MBAPI_4_RET_CODE( 52 );

//! ��� ������ � ������� �����������.
const int rc_duplicating_stage_name = SO_5_MBAPI_4_RET_CODE( 53 );

//! ������ ����������� �� �������������� ���������,
//! �.�. ��� ���� �������� �� ��������.
const int rc_unable_to_subscribe_on_typed_message = SO_5_MBAPI_4_RET_CODE( 54 );

//! ������ ����������� �� �������� ���������, �.�. ��� ���� �������� ��
//! �������������� ���������.
const int rc_unable_to_subscribe_on_binary_message = SO_5_MBAPI_4_RET_CODE( 55 );


} /* namespace mbapi_4 */

#endif
