#include <YSI\y_hooks>

#define DB_HOSTNAME "127.0.0.1"
#define DB_USERNAME "root"
#define DB_PASSWORD ""
#define DB_DATABASE "samprp"

new MySQL:dbHandler;

hook OnGameModeInit()
{
	print("��czenie z baz� danych...");
	mysql_log(ALL);
	dbHandler = mysql_connect(DB_HOSTNAME, DB_USERNAME, DB_PASSWORD, DB_DATABASE);
	if(mysql_errno() != 0)
	{
		print("B��d podczas ��czenia z baz� danych.");
		mysql_close();
		SendRconCommand("exit");
	}
	else
	{
		print("Po��czono z baz� danych.");
	}
	return 1;
}