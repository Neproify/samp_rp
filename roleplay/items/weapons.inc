#include <YSI\y_hooks>

hook OnPlayerUseItem(playerid, itemID)
{
	if(ItemData[itemID][iType] != 1)
	{
		return 0;
	}
	if(!PlayerData[playerid][pLogged])
	{
		// TODO: kick or something?
		return 0;
	}
	if(PlayerData[playerid][pUID] != ItemData[itemID][iOwnerID] || ItemData[itemID][iOwnerType] != 1)
	{
		return 0;
	}
	if(!ItemData[itemID][iUsed])
	{
		ItemData[itemID][iUsed] = true;
		GivePlayerWeapon(playerid, strval(GetItemPropertyBySlot(itemID, 0)), strval(GetItemPropertyBySlot(itemID, 1)));
		return 1;
	}
	else
	{
		ItemData[itemID][iUsed] = false;
		ResetPlayerWeapons(playerid);
		foreach(new i : PlayerItems<playerid>)
		{
			if(ItemData[i][iUsed] && ItemData[i][iType] == 1)
			{
				GivePlayerWeapon(playerid, strval(GetItemPropertyBySlot(i, 0)), strval(GetItemPropertyBySlot(i, 1)));
			}
		}
		return 1;
	}
}

hook OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if(!PlayerData[playerid][pLogged])
	{
		return 0;
	}
	foreach(new i : PlayerItems<playerid>)
	{
		if(ItemData[i][iUsed] && ItemData[i][iType] == 1)
		{
			if(strval(GetItemPropertyBySlot(i, 0)) == weaponid)
			{
				new value = strval(GetItemPropertyBySlot(i, 1));
				value = value - 1;
				new val2[MAX_ITEM_PROPERTIES_LENGTH];
				valstr(val2, value);
				SetItemPropertyBySlot(i, 1, val2);
				if(strval(GetItemPropertyBySlot(i, 1)) <= 0)
				{
					CallLocalFunction("OnPlayerUseItem", "dd", playerid, i);
				}
				return 1;
			}
		}
	}
	return 1;
}