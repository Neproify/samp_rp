/* Poziomy admina:
1 - próbny supporter
2 - supporter
3 - admin
4 - zarz¹d
*/

/*
Do zrobienia uprawnienia admina niezale¿ne od rangi(np. zarz¹dzanie pojazdami, itd.)
*/

enum PlayerDataEnum
{
	bool:pLogged = false,
	bool:pSpawned = false,
	pAdmin, // ipb_members, konieczna modyfikacja tabeli z forum(game_admin_level)
	pGID,
	pName[32]
};

enum CharacterDataEnum
{
	UID,
	characterName[32]
};
new PlayerData[MAX_PLAYERS][PlayerDataEnum];
new PlayerCharacters[MAX_PLAYERS][MAX_PLAYER_CHARACTERS][CharacterDataEnum];

stock PlayerName(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	return name;
}