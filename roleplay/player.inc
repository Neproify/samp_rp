enum PlayerDataEnum
{
	bool:pLogged = false,
	bool:pSpawned = false,
	pGID,
	pName[32]
};
enum CharacterDataEnum
{
	UID,
	characterName[32]
};
new PlayerData[MAX_PLAYERS][PlayerDataEnum];
new PlayerCharacters[MAX_PLAYERS][50][CharacterDataEnum]; // MAX_CHARACTERS?

stock PlayerName(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	return name;
}