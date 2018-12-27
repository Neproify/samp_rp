Dialog:Login(playerid, response, listitem, inputtext[])
{
	if(!response)
		Kick(playerid);

	new buffer[256];
	mysql_format(dbHandler, buffer, sizeof(buffer), "SELECT * FROM `ipb_core_members` WHERE `name`='%s' LIMIT 1;", PlayerName(playerid));
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
	new hash[128], rows;
	cache_get_row_count(rows);
	if(rows != 1) // Nie ma takiego konta
	{
		Dialog_Show(playerid, InfoAndKick, DIALOG_STYLE_MSGBOX, "Logowanie", "Nie posiadasz jeszcze konta na naszym serwerze! Za³ó¿ je na naszym forum.", "OK", "");
		return 0;
	}
	cache_get_value_name(0, "members_pass_hash", hash);
	cache_get_value_name_int(0, "member_id", PlayerData[playerid][pGID]);
	cache_get_value_name_int(0, "game_admin_level", PlayerData[playerid][pAdmin]);
	bcrypt_check(password, hash, "OnGlobalAccountLogin", "d", playerid);
	return 1;
}

forward OnGlobalAccountLogin(playerid);
public OnGlobalAccountLogin(playerid)
{
	new buffer[256];
	if(!bcrypt_is_equal()) // nie zalogowano
	{
		Dialog_Show(playerid, Login, DIALOG_STYLE_PASSWORD, "Logowanie", "Poda³eœ z³e has³o!", "Zaloguj", "Anuluj");
		return 0;
	}
	// zalogowano, pobieramy listê postaci.
	PlayerData[playerid][pLogged] = true;
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
	new rows, buffer[1024] = "";
	cache_get_row_count(rows);
	if(rows < 1) // brak postaci
	{
		for(new PlayerDataEnum:e; e < PlayerDataEnum; e++)
		PlayerData[playerid][e] = 0;
		
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
		cache_get_value_name_int(i, "UID", PlayerCharacters[playerid][i]);
		cache_get_value_name(i, "name", name, sizeof(name));
		format(buffer, sizeof(buffer), "%s%s\n", buffer, name);
	}
	Dialog_Show(playerid, SelectCharacter, DIALOG_STYLE_LIST, "Wybierz postaæ", buffer, "Graj", "Anuluj");
	return 1;
}

forward OnCharacterSelected(playerid);
public OnCharacterSelected(playerid)
{
	new rows;
	cache_get_row_count(rows);
	if(rows < 1) // wredny cheater, fuj
	{
		Kick(playerid);
		return 0;
	}
	cache_get_value_name_int(0, "UID", PlayerData[playerid][pUID]);
	cache_get_value_name(0, "name", PlayerData[playerid][pName], MAX_PLAYER_NAME);
	cache_get_value_name_int(0, "skin", PlayerData[playerid][pSkin]);
	cache_get_value_name_int(0, "money", PlayerData[playerid][pMoney]);
	cache_get_value_name_int(0, "health", PlayerData[playerid][pHealth]);
	CallLocalFunction("SpawnPlayerAfterLogin", "d", playerid);
	return 1;
}