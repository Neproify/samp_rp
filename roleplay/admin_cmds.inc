#include <YSI\y_hooks>

YCMD:gmx(playerid, params[], help)
{
	SendRconCommand("gmx");
	return 1;
}

YCMD:globalchat(playerid, params[], help)
{
	if(GlobalChatEnabled == true)
	{
		GlobalChatEnabled = false;
	}
	else
	{
		GlobalChatEnabled = true;
	}
	return 1;
}

// tutaj nadajemy uprawnienia do komend

hook OnGroupPermissions()
{
	// Dziêki pêtlom mo¿emy przekazaæ uprawnienia ni¿szych leveli wy¿szym
	// Admin level 1
	for(new i = 1; i <= 4; i++)
	{
		GROUP_ADD<Admins[i]>
		{

		}
	}
	// Admin level 2
	for(new i = 2; i <= 4; i++)
	{
		GROUP_ADD<Admins[i]>
		{

		}
	}
	// Admin level 3
	for(new i = 3; i <= 4; i++)
	{
		GROUP_ADD<Admins[i]>
		{

		}
	}
	// Admin level 4
	GROUP_ADD<Admins[4]>
	{
		@YCMD:gmx;
		@YCMD:globalchat;
	}
}