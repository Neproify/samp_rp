#include <YSI\y_hooks>

hook OnPlayerText(playerid, text[])
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
				SendClientMessage(i, 0xFFFFFFFF, buffer);
		}
	}
	return 0;
}