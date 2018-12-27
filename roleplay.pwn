#include <a_samp>
#include <a_mysql>
#include <easydialog>
#include <streamer>
#include <bcrypt>
#include <YSF>
#include <YSI\y_commands>
#include <YSI\y_hooks>
#include <YSI\y_groups>
#include <YSI\y_iterate>
#include <sscanf2>
#include <strlib>

// nasze includy
#include "roleplay\dbsettings.inc" // dane do bazy danych - MUSI BYĆ PIERWSZE
#include "roleplay\settings.inc" // ustawienia, wszelakie definicje, itd.
#include "roleplay\dialogs.inc" // wszelakie uniwersalne dialogi
#include "roleplay\player.inc" // rzeczy związane z graczem
#include "roleplay\chat.inc" // chat
#include "roleplay\items.inc" // przedmioty
#include "roleplay\login.inc" // logowanie
#include "roleplay\admin_cmds.inc" // komendy dla administracji
#include "roleplay\items\weapons.inc" // Bronie

main()
{
	print("===================");
	print("Gamemode Roleplay "GAMEMODE_VERSION_STRING);
	print("Autor: "GAMEMODE_AUTHOR"");
	print("===================");
}

forward OnGroupPermissions();
public OnGroupPermissions() {}

public OnGameModeInit()
{
	SetGameModeText(GAMEMODE_NAME " " GAMEMODE_VERSION_STRING);
	UsePlayerPedAnims();
	DisableInteriorEnterExits();
	EnableStuntBonusForAll(false);
	ShowNameTags(false);
	ShowPlayerMarkers(false);
	Group_SetGlobalCommandDefault(false); // Wyłączamy wszystkie komendy.

	CallLocalFunction("OnGroupPermissions", ""); // Możemy podstawiać hooki w "modułach"
	return 1;
}

public OnGameModeExit()
{
	mysql_close(dbHandler);
	return 1;
}

public OnPlayerConnect(playerid)
{
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	return 0;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	return 1;
}