Dialog:Login(playerid, response, listitem, inputtext[])
{
	if(!response)
		Kick(playerid);

	new buffer[256];
	mysql_format(MySQL, buffer, sizeof(buffer), "SELECT * FROM `ipb_members` WHERE `name`='%s' LIMIT 1;", PlayerName(playerid));
	mysql_pquery(MySQL, buffer, "OnGlobalAccountDataGet", "is", playerid, inputtext);
	return 1;
}

Dialog:SelectCharacter(playerid, response, listitem, inputtext[])
{
	if(!response)
		Kick(playerid);

	new buffer[128];
	mysql_format(MySQL, buffer, sizeof(buffer), "SELECT * FROM `rp_characters` WHERE `UID`=%d AND `globalid`=%d LIMIT 1;", 
		PlayerCharacters[playerid][listitem][UID], PlayerData[playerid][pGID]);
	mysql_pquery(MySQL, buffer, "OnCharacterSelected", "i", playerid);
	return 1;
}

forward OnGlobalAccountDataGet(playerid, password[]);
public OnGlobalAccountDataGet(playerid, password[])
{
	new salt[6], hash[33], buffer[256], rows, fields;
	cache_get_data(rows, fields, MySQL);
	if(rows != 1) // Nie ma takiego konta
	{
		Dialog_Show(playerid, InfoAndKick, DIALOG_STYLE_MSGBOX, "Logowanie", "Nie posiadasz jeszcze konta na naszym serwerze! Za³ó¿ je na naszym forum.", "OK", "");
		return 0;
	}
	cache_get_field_content(0, "members_pass_salt", salt);
	cache_get_field_content(0, "members_pass_hash", hash);
	mysql_format(MySQL, buffer, sizeof(buffer), "SELECT * FROM `ipb_members` WHERE `name`='%s' AND `members_pass_hash`=md5(CONCAT(md5('%s'), md5('%s'))) LIMIT 1;", PlayerName(playerid), salt, password);
	mysql_pquery(MySQL, buffer, "OnGlobalAccountLogin", "i", playerid);
	return 1;
}

forward OnGlobalAccountLogin(playerid);
public OnGlobalAccountLogin(playerid)
{
	new rows, fields, buffer[256];
	cache_get_data(rows, fields, MySQL);
	if(rows != 1) // nie zalogowano
	{
		Dialog_Show(playerid, Login, DIALOG_STYLE_PASSWORD, "Logowanie", "Poda³eœ z³e has³o!", "Zaloguj", "Anuluj");
		return 0;
	}
	// zalogowano, pobieramy listê postaci.
	PlayerData[playerid][pLogged] = true;
	PlayerData[playerid][pGID] = cache_get_field_content_int(0, "member_id", MySQL);
	mysql_format(MySQL, buffer, sizeof(buffer), "SELECT * FROM `rp_characters` WHERE `globalid`=%d", PlayerData[playerid][pGID]);
	mysql_pquery(MySQL, buffer, "OnPlayerCharactersFetched", "i", playerid);
	return 1;
}

forward OnPlayerCharactersFetched(playerid);
public OnPlayerCharactersFetched(playerid)
{
	new rows, fields, buffer[1024] = "";
	cache_get_data(rows, fields, MySQL);
	if(rows < 1) // brak postaci
	{
		Dialog_Show(playerid, InfoAndKick, DIALOG_STYLE_MSGBOX, "Logowanie", "Nie posiadasz ¿adnej postaci. Za³ó¿ j¹ na naszym forum!", "OK", "");
		return 0;
	}
	for(new i = 0; i < rows; i++)
	{
		PlayerCharacters[playerid][i][UID] = cache_get_field_content_int(i, "UID", MySQL);
		cache_get_field_content(i, "name", PlayerCharacters[playerid][i][characterName], MySQL, 32);
		format(buffer, sizeof(buffer), "%s%s\n", buffer, PlayerCharacters[playerid][i][characterName]);
	}
	Dialog_Show(playerid, SelectCharacter, DIALOG_STYLE_LIST, "Wybierz postaæ", buffer, "Graj", "Anuluj");
	return 1;
}

forward OnCharacterSelected(playerid);
public OnCharacterSelected(playerid)
{
	new rows, fields;
	cache_get_data(rows, fields, MySQL);
	if(rows < 1) // wredny cheater, fuj
	{
		Kick(playerid);
		return 0;
	}
	cache_get_field_content(0, "name", PlayerData[playerid][pName], MySQL, 32);
	SetSpawnInfo(playerid, 0, 0, 1481.8495, -1687.1045, 14.0469, 178.7321, 0, 0, 0, 0, 0, 0);
	PlayerData[playerid][pSpawned] = true;
	SetPlayerName(playerid, PlayerData[playerid][pName]);
	SpawnPlayer(playerid);
	return 1;
}