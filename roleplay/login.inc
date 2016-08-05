Dialog:Login(playerid, response, listitem, inputtext[])
{
	if(!response)
		Kick(playerid);

	new buffer[256];
	mysql_format(dbHandler, buffer, sizeof(buffer), "SELECT * FROM `ipb_members` WHERE `name`='%s' LIMIT 1;", PlayerName(playerid));
	mysql_pquery(dbHandler, buffer, "OnGlobalAccountDataGet", "is", playerid, inputtext);
	return 1;
}

Dialog:SelectCharacter(playerid, response, listitem, inputtext[])
{
	if(!response)
		Kick(playerid);

	new buffer[128];
	mysql_format(dbHandler, buffer, sizeof(buffer), "SELECT * FROM `rp_characters` WHERE `UID`=%d AND `globalid`=%d LIMIT 1;", 
		PlayerCharacters[playerid][listitem], PlayerData[playerid][pGID]);
	mysql_pquery(dbHandler, buffer, "OnCharacterSelected", "i", playerid);
	return 1;
}

forward OnGlobalAccountDataGet(playerid, password[]);
public OnGlobalAccountDataGet(playerid, password[])
{
	new salt[6], hash[33], buffer[256], rows, fields;
	cache_get_data(rows, fields, dbHandler);
	if(rows != 1) // Nie ma takiego konta
	{
		Dialog_Show(playerid, InfoAndKick, DIALOG_STYLE_MSGBOX, "Logowanie", "Nie posiadasz jeszcze konta na naszym serwerze! Za³ó¿ je na naszym forum.", "OK", "");
		return 0;
	}
	cache_get_field_content(0, "members_pass_salt", salt);
	cache_get_field_content(0, "members_pass_hash", hash);
	mysql_format(dbHandler, buffer, sizeof(buffer), "SELECT * FROM `ipb_members` WHERE `name`='%s' AND `members_pass_hash`=md5(CONCAT(md5('%s'), md5('%s'))) LIMIT 1;", PlayerName(playerid), salt, password);
	mysql_pquery(dbHandler, buffer, "OnGlobalAccountLogin", "i", playerid);
	return 1;
}

forward OnGlobalAccountLogin(playerid);
public OnGlobalAccountLogin(playerid)
{
	new rows, fields, buffer[256];
	cache_get_data(rows, fields, dbHandler);
	if(rows != 1) // nie zalogowano
	{
		Dialog_Show(playerid, Login, DIALOG_STYLE_PASSWORD, "Logowanie", "Poda³eœ z³e has³o!", "Zaloguj", "Anuluj");
		return 0;
	}
	// zalogowano, pobieramy listê postaci.
	PlayerData[playerid][pLogged] = true;
	PlayerData[playerid][pGID] = cache_get_field_content_int(0, "member_id", dbHandler);
	PlayerData[playerid][pAdmin] = cache_get_field_content_int(0, "game_admin_level", dbHandler);
	mysql_format(dbHandler, buffer, sizeof(buffer), "SELECT * FROM `rp_characters` WHERE `globalid`=%d", PlayerData[playerid][pGID]);
	mysql_pquery(dbHandler, buffer, "OnPlayerCharactersFetched", "i", playerid);
	Group_SetPlayer(LoggedPlayers, playerid, true);
	if(PlayerData[playerid][pAdmin] > 0)
	{
		Group_SetPlayer(Admins[PlayerData[playerid][pAdmin]], playerid, true);
	}
	return 1;
}

forward OnPlayerCharactersFetched(playerid);
public OnPlayerCharactersFetched(playerid)
{
	new rows, fields, buffer[1024] = "";
	cache_get_data(rows, fields, dbHandler);
	if(rows < 1) // brak postaci
	{
		Dialog_Show(playerid, InfoAndKick, DIALOG_STYLE_MSGBOX, "Logowanie", "Nie posiadasz ¿adnej postaci. Za³ó¿ j¹ na naszym forum!", "OK", "");
		return 0;
	}
	for(new i = 0; i < rows; i++)
	{
		if(i > MAX_PLAYER_CHARACTERS)
		{
			printf("PRZEKROCZONO MAX_PLAYER_CHARACTERS(%d)", rows);
			break;
		}
		new name[MAX_PLAYER_NAME + 1];
		PlayerCharacters[playerid][i] = cache_get_field_content_int(i, "UID", dbHandler);
		cache_get_field_content(i, "name", name, dbHandler, sizeof(name));
		format(buffer, sizeof(buffer), "%s%s\n", buffer, name);
	}
	Dialog_Show(playerid, SelectCharacter, DIALOG_STYLE_LIST, "Wybierz postaæ", buffer, "Graj", "Anuluj");
	return 1;
}

forward OnCharacterSelected(playerid);
public OnCharacterSelected(playerid)
{
	new rows, fields;
	cache_get_data(rows, fields, dbHandler);
	if(rows < 1) // wredny cheater, fuj
	{
		Kick(playerid);
		return 0;
	}
	PlayerData[playerid][pUID] = cache_get_field_content_int(0, "UID", dbHandler);
	cache_get_field_content(0, "name", PlayerData[playerid][pName], dbHandler, MAX_PLAYER_NAME);
	PlayerData[playerid][pSkin] = cache_get_field_content_int(0, "skin", dbHandler);
	PlayerData[playerid][pMoney] = cache_get_field_content_int(0, "money", dbHandler);
	PlayerData[playerid][pHealth] = cache_get_field_content_int(0, "health", dbHandler);
	CallLocalFunction("SpawnPlayerAfterLogin", "d", playerid);
	return 1;
}