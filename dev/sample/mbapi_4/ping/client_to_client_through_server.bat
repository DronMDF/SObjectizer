cd ../../../
REM ������ ����� ���� ��� ���� ����� ��������� ��������,
REM ������� �� ����� ���������� ��������� ���� �����.
start sample.mbapi_4.ping.srv.exe :11411 server_ep
start sample.mbapi_4.ping.cln.exe :11411 Alice Bob
start sample.mbapi_4.ping.cln.exe :11411 Bob Alice