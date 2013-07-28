system( "cd ../../../" )

def start_client( addr, self_ep, dest_ep )
	system(
		"start ../../../sample.mbapi_4.ping.cln.exe #{addr} #{self_ep} #{dest_ep}" )
	sleep( 2 )
end

def start_server( addr, self_ep )
	system( "start ../../../sample.mbapi_4.ping.srv.exe #{addr} #{self_ep}" )
	sleep( 2 )
end

def start_proxy( saddrs, caddrs )
	servers = saddrs.map{|v| "-s #{v}" }.join(" ")
	clients = caddrs.map{|v| "-c #{v}" }.join(" ")
	system( "start ../../../sample.mbapi_4.ping.proxy.exe #{servers} #{clients}" )

	sleep( 2 )
end

# ������.
start_proxy( [":10100"], [] )

# ������ �������.
start_proxy( [":10110"], [":10100"] )
start_proxy( [":10120"], [":10100"] )

# ����������� ����� ������ �������
start_client( ":10110", "Alice", "Bob" )
start_client( ":10120", "Bob", "Alice" )


# ������ �������.
start_proxy( [":10111"], [":10110"] )
start_proxy( [":10112"], [":10110"] )
start_proxy( [":10121"], [":10120"] )
start_proxy( [":10122"], [":10120"] )

# ����������� ����� ������ �������, ��� ����� ��������� ������
# ���� ������ ����� ������
start_client( ":10112", "Romeo", "Juliet" )
start_client( ":10121", "Juliet", "Romeo" )

start_client( ":10111", "Tristan", "Iseult" )
start_client( ":10122", "Iseult", "Tristan" )

# ������� �������, � ������� � �����
start_proxy( [":10200"], [":10100"] )
start_proxy( [":10300"], [":10200"] )
start_proxy( [":10400"], [":10300"] )
start_proxy( [":10500"], [":10400"] )
start_proxy( [":10600"], [":10500"] )
start_proxy( [":10700"], [":10600"] )
start_proxy( [":10800"], [":10700"] )

# ������ "����������"
start_server( ":11111", "Snow_White" )
# "����������" ����������� � ������
start_proxy( [], [":11111", ":10100"] )

# ����� ������������ � ������ ������� �������.
start_client( ":10100", "Doc", "Snow_White" )
start_client( ":10200", "Grumpy", "Snow_White" )
start_client( ":10300", "Happy", "Snow_White" )
start_client( ":10400", "Bashful", "Snow_White" )
start_client( ":10500", "Sneezy", "Snow_White" )
start_client( ":10600", "Sleepy", "Snow_White" )
start_client( ":10700", "Dopey", "Snow_White" )

# �������� ������� � ������ ������� � ����������.
start_proxy( [], [ ":10800", ":11111"] )
