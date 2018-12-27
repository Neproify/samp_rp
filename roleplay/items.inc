#include <YSI\y_hooks>

/*
	W³aœciciele:
	1 - postaæ(ekwipunek)

	Typ: 1 - Broñ
		[0] - model broni
		[1] - amunicja
*/

enum ItemDataEnum
{
	iUID,
	iName[MAX_ITEM_NAME],
	iType,
	iProperties[MAX_ITEM_PROPERTIES_LENGTH], // Rozdzielane znakiem "|"
	iOwnerID,
	iOwnerType,// 1 - postaæ(ekwipunek)
	bool:iUsed
}
new ItemData[MAX_ITEMS][ItemDataEnum];
new Iterator:PlayerItems<MAX_PLAYERS, MAX_ITEMS>;
new Iterator:Items<MAX_ITEMS>;

forward OnPlayerUseItem(playerid, itemID);
public OnPlayerUseItem(playerid, itemID) {}

hook OnGameModeInit()
{
	mysql_pquery(dbHandler, "SELECT * FROM `rp_items`", "OnItemsFetched");
	return 1;
}

hook OnPlayerConnect(playerid)
{
	Iter_Clear(PlayerItems<playerid>);
	return 1;
}

forward OnItemsFetched();
public OnItemsFetched()
{
	new rows;
	cache_get_row_count(rows);
	for(new i = 0; i < rows; i++)
	{
		if(i > MAX_ITEMS)
		{
			printf("PRZEKROCZONO MAX_ITEMS(%d)!", rows);
			break;
		}
		cache_get_value_name_int(i, "UID", ItemData[i][iUID]);
		cache_get_value_name(i, "name", ItemData[i][iName], MAX_ITEM_NAME);
		cache_get_value_name_int(i, "type", ItemData[i][iType]);
		cache_get_value_name(i, "properties", ItemData[i][iProperties], MAX_ITEM_PROPERTIES_LENGTH);
		cache_get_value_name_int(i, "ownerID", ItemData[i][iOwnerID]);
		cache_get_value_name_int(i, "ownerType", ItemData[i][iOwnerType]);
		Iter_Add(Items, i);
	}
	if(rows > MAX_ITEMS)
	{
		printf("Za³adowano %d przedmiotów.", MAX_ITEMS);
	}
	else
	{
		printf("Za³adowano %d przedmiotów.", rows);
	}
	return 1;
}

hook SpawnPlayerAfterLogin(playerid)
{
	foreach(new i : Items)
	{
		if(ItemData[i][iOwnerType] == 1 && ItemData[i][iOwnerID] == PlayerData[playerid][pUID])
		{
			Iter_Add(PlayerItems<playerid>, i);
		}
	}
	return 1;
}

Dialog:ShowPlayerItems(playerid, response, listitem, inputtext[])
{
	if(!response) // Anulowa³
		return 1;
	new itemID, count = 0;
	foreach(new i : PlayerItems<playerid>)
	{
		if(count == listitem)
		{
			itemID = i;
			break;
		}
		count++;
	}
	CallLocalFunction("OnPlayerUseItem", "dd", playerid, itemID);
	return 1;
}

YCMD:przedmioty(playerid, params[], help) // TODO: sprawdzanie czy ma jakiekolwiek przedmioty.
{
	new buffer[512];
	buffer = "UID\tNazwa\n";
	foreach(new i : PlayerItems<playerid>)
	{
		if(ItemData[i][iUsed])
		{
			format(buffer, sizeof(buffer), "%s{006400}%d\t{006400}%s\n", buffer, ItemData[i][iUID], ItemData[i][iName]);
		}
		else
		{
			format(buffer, sizeof(buffer), "%s%d\t%s\n", buffer, ItemData[i][iUID], ItemData[i][iName]);
		}
	}
	Dialog_Show(playerid, ShowPlayerItems, DIALOG_STYLE_TABLIST_HEADERS, "Przedmioty", buffer, "U¿yj", "Anuluj");
	return 1;
}

hook OnGroupPermissions()
{
	GROUP_ADD<LoggedPlayers>
	{
		@YCMD:przedmioty;
	}
}

GetItemPropertyBySlot(itemID, slot)
{
	new properties[15][MAX_ITEM_PROPERTIES_LENGTH];
	strexplode(properties, ItemData[itemID][iProperties], "|");
	return properties[slot];
}

SetItemPropertyBySlot(itemID, slot, value[])
{
	new properties[MAX_ITEM_PROPERTIES_LENGTH];
	properties = "";
	for(new i = 0; i <= 14; i++)
	{
		new val[MAX_ITEM_PROPERTIES_LENGTH];
		val = GetItemPropertyBySlot(itemID, i);
		if(isnull(val))
		{
			break;
		}
		if(i != slot)
		{
			format(properties, sizeof(properties), "%s%s|", properties, GetItemPropertyBySlot(itemID, i));
		}
		else
		{
			format(properties, sizeof(properties), "%s%s|", properties, value);
		}
	}
	strtrim(properties, "|", edge_right);
	ItemData[itemID][iProperties] = properties;
}