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
	iOwnerType // 1 - postaæ(ekwipunek)
}
new ItemData[MAX_ITEMS][ItemDataEnum];
new Iterator:PlayerItems<MAX_PLAYERS, MAX_ITEMS>;
new Iterator:Items<MAX_ITEMS>;

forward UsePlayerItem(playerid, itemID);
public UsePlayerItem(playerid, itemID) {}

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
	new rows, fields;
	cache_get_data(rows, fields, dbHandler);
	for(new i = 0; i < rows; i++)
	{
		if(i > MAX_ITEMS)
		{
			printf("PRZEKROCZONO MAX_ITEMS(%d)!", rows);
			break;
		}
		ItemData[i][iUID] = cache_get_field_content_int(i, "UID", dbHandler);
		cache_get_field_content(i, "name", ItemData[i][iName], dbHandler, MAX_ITEM_NAME);
		ItemData[i][iType] = cache_get_field_content_int(i, "type", dbHandler);
		cache_get_field_content(i, "properties", ItemData[i][iProperties], dbHandler, MAX_ITEM_PROPERTIES_LENGTH);
		ItemData[i][iOwnerID] = cache_get_field_content_int(i, "ownerID", dbHandler);
		ItemData[i][iOwnerType] = cache_get_field_content_int(i, "ownerType", dbHandler);
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
	CallLocalFunction("UsePlayerItem", "dd", playerid, itemID);
	return 1;
}

YCMD:przedmioty(playerid, params[], help) // TODO: sprawdzanie czy ma jakiekolwiek przedmioty.
{
	new buffer[512];
	buffer = "UID\tNazwa\n";
	foreach(new i : PlayerItems<playerid>)
	{
		format(buffer, sizeof(buffer), "%s%d\t%s\n", buffer, ItemData[i][iUID], ItemData[i][iName]);
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