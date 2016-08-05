/* Poziomy admina:
1 - próbny supporter
2 - supporter
3 - admin
4 - zarz¹d
*/

/*
Do zrobienia uprawnienia admina niezale¿ne od rangi(np. zarz¹dzanie pojazdami, itd.)
*/

#include <YSI\y_hooks>

enum PlayerDataEnum
{
	bool:pLogged = false,
	bool:pSpawned = false,
	pAdmin, // ipb_core_members, konieczna modyfikacja tabeli z forum(game_admin_level)
	pGID,
	pUID,
	pName[MAX_PLAYER_NAME],
	pSkin,
	pMoney,
	pHealth
};
new PlayerData[MAX_PLAYERS][PlayerDataEnum];
new PlayerCharacters[MAX_PLAYERS][MAX_PLAYER_CHARACTERS];

new Group:LoggedPlayers;
new Group:Admins[MAX_ADMIN_LEVELS + 1];

new Text3D:PlayerNameLabel[MAX_PLAYERS];

stock PlayerName(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	return name;
}

stock ClearPlayerChat(playerid)
{
	for(new i; i < 50; i++)
		SendClientMessage(playerid, 0xFFFFFFFF, "");
}

forward SavePlayerData(playerid);
public SavePlayerData(playerid) // TODO: dokoñczyæ
{
	return 1;
}

forward UpdatePlayerNameLabel(playerid);
public UpdatePlayerNameLabel(playerid) // TODO: dokoñczyæ
{
	if(PlayerData[playerid][pLogged] && PlayerData[playerid][pSpawned])
	{
		new buffer[128];
		format(buffer, sizeof(buffer), "%s (%d)", PlayerName(playerid), playerid);
		UpdateDynamic3DTextLabelText(PlayerNameLabel[playerid], 0xFFFFFFFF, buffer);
	}
	return 1;
}

forward SpawnPlayerAfterLogin(playerid);
public SpawnPlayerAfterLogin(playerid)
{
	SetSpawnInfo(playerid, 0, PlayerData[playerid][pSkin], 1481.8495, -1687.1045, 14.0469, 178.7321, 0, 0, 0, 0, 0, 0);
	PlayerData[playerid][pSpawned] = true;
	SetPlayerName(playerid, PlayerData[playerid][pName]);
	SpawnPlayer(playerid);
	GivePlayerMoney(playerid, PlayerData[playerid][pMoney]);
	SetPlayerScore(playerid, 0); // TODO: Gamescore?
	SetPlayerHealth(playerid, PlayerData[playerid][pHealth]);
	SetPlayerArmour(playerid, 0);
	return 1;
}

hook OnGameModeInit()
{
	LoggedPlayers = Group_Create("logged");
	Admins[1] = Group_Create("admin1");
	Admins[2] = Group_Create("admin2");
	Admins[3] = Group_Create("admin3");
	Admins[4] = Group_Create("admin4");
	return 1;
}

hook OnPlayerConnect(playerid)
{
	for(new PlayerDataEnum:e; e < PlayerDataEnum; e++)
		PlayerData[playerid][e] = 0;

	for(new i; i < MAX_PLAYER_CHARACTERS; i++)
		PlayerCharacters[playerid][i] = 0;

	ClearPlayerChat(playerid);

	PlayerNameLabel[playerid] = CreateDynamic3DTextLabel("", 0xFFFFFFFF, 0, 0, 0, 30, playerid);
		
	Dialog_Show(playerid, Login, DIALOG_STYLE_PASSWORD, "Logowanie", "Podaj swoje has³o by siê zalogowaæ.", "Zaloguj", "Anuluj");
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	DestroyDynamic3DTextLabel(PlayerNameLabel[playerid]);
	SavePlayerData(playerid);
	return 1;
}

hook OnPlayerUpdate(playerid)
{
	UpdatePlayerNameLabel(playerid); // Nie jestem pewny czy to pasuje do update, moze jakiœ timer co 5 sekund?
	return 1;
}