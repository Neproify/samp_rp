#include <a_samp>
#include <a_mysql>
#include <easydialog>
#include <YSF>
#include <YSI\y_commands>

// nasze includy
#include "roleplay\dbsettings.inc" // dane do bazy danych
#include "roleplay\settings.inc" // ustawienia, wszelakie definicje, itd.
#include "roleplay\dialogs.inc" // wszelakie uniwersalne dialogi
#include "roleplay\player.inc" // rzeczy zwi¹zane z graczem
#include "roleplay\login.inc" // logowanie
#include "roleplay\admin_cmds.inc" // komendy dla administracji

main() {}

public OnGameModeInit()
{
	print("===================");
	print("Gamemode Roleplay "GAMEMODE_VERSION_STRING);
	print("Autor: "GAMEMODE_AUTHOR"");
	print("===================");

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

	SetGameModeText(GAMEMODE_NAME " " GAMEMODE_VERSION_STRING);

	return 1;
}

public OnGameModeExit()
{
	mysql_close(dbHandler);
	return 1;
}

public OnPlayerConnect(playerid)
{
	for(new PlayerDataEnum:e; e < PlayerDataEnum; e++)
		PlayerData[playerid][e] = 0;

	for(new CharacterDataEnum:e; e < CharacterDataEnum; e++)
		for(new i; i < MAX_PLAYER_CHARACTERS; i++)
			PlayerCharacters[playerid][i][e] = 0;
		
	Dialog_Show(playerid, Login, DIALOG_STYLE_PASSWORD, "Logowanie", "Podaj swoje has³o by siê zalogowaæ.", "Zaloguj", "Anuluj");
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	if(GlobalChatEnabled)
		return 1;
	new buffer[256], Float:posX, Float:posY, Float:posZ;
	GetPlayerPos(playerid, posX, posY, posZ);
	format(buffer, sizeof(buffer), "%s mówi: %s", PlayerName(playerid), text);
	for(new i = 0; i <= GetPlayerPoolSize(); i++)
	{
		if(IsPlayerConnected(i))
		{
			if(GetPlayerDistanceFromPoint(i, posX, posY, posZ) < 15)
				SendClientMessage(i, 0xFFFFFF, buffer);
		}
	}
	return 0;
}