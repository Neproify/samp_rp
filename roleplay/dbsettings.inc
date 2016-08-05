#include <YSI\y_hooks>

#define DB_HOSTNAME "127.0.0.1"
#define DB_USERNAME "root"
#define DB_PASSWORD ""
#define DB_DATABASE "samp"

new dbHandler;

hook OnGameModeInit()
{
	print("£¹czenie z baz¹ danych...");
	mysql_log(LOG_ERROR | LOG_WARNING | LOG_DEBUG);
	dbHandler = mysql_connect(DB_HOSTNAME, DB_USERNAME, DB_DATABASE, DB_PASSWORD);
	if(mysql_errno() != 0)
	{
		print("B³¹d podczas ³¹czenia z baz¹ danych.");
		mysql_close();
		SendRconCommand("exit");
	}
	else
	{
		print("Po³¹czono z baz¹ danych.");
	}
	return 1;
}