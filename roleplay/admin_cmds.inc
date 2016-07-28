CMD:gmx(playerid, params[])
{
	if(PlayerData[playerid][pAdmin] == 4)
	{
		SendRconCommand("gmx");
	}
	return 1;
}

CMD:globalchat(playerid, params[])
{
	if(PlayerData[playerid][pAdmin] == 4)
	{
		if(GlobalChatEnabled == true)
		{
			GlobalChatEnabled = false;
		}
		else
		{
			GlobalChatEnabled = true;
		}
	}
	return 1;
}