//= Function for accessing npc variables from another NPC without using 'getvariableofnpc' and can have dynamic names
//= Use Cases:
/* 
//===== getnpc_var("<Variable Name>",{,<Target NPC>});
**
** .@value = getnpc_var("target_var","npc_name");
** .@value$ = getnpc_var("target_var$","npc_name");
** .@value$ = getnpc_var("target_var$[2]","npc_name");
** .@size = getarraysize(getnpcvar("target_array","npc_name"));
**
//===== setnpc_var("<Variable Name>",<New Value>,{,<Target NPC>});
**
** setnpc_var("target_var",10,"npc_name");
** setnpc_var("target_var$","HELLO","npc_name");
** setnpc_var("target_var$[2]","Change Array Value 2","npc_name");
**
*/

function	script	getnpc_var	{
	return getvariableofnpc(getd((compare(getarg(0),".")?"":".") + getarg(0)),getarg(1,strnpcinfo(3)));
}

function	script	setnpc_var	{
	set getvariableofnpc(getd((compare(getarg(0),".")?"":".") + getarg(0)),getarg(2,strnpcinfo(3))),getarg(1);
	return;
}

//= Function for copying an array from another NPC and return the size of the copied array
//= getnpc_array("<Target Array Variable>",<Copied Array Variable>{,<"Target NPC">});
//= .@array_size = getnpc_array(".target_array",.@temp_array,"npc_name");
function	script	getnpc_array	{
	.@type$ = (compare(getarg(0),"$") ? "$" : "");
	copyarray getd(".@temp_array" + .@type$ + "[0]"),getnpc_var(getarg(0),getarg(2,strnpcinfo(3))),getarraysize(getnpc_var(getarg(0),getarg(2,strnpcinfo(3))));
	.@size = getarraysize(getd(".@temp_array" + .@type$));
	if(.@size < 1){
		debugmes "getnpc_array : The target array " + getarg(0) + " is empty."; 
		return 0;
	}
	for(.@i = 0; .@i < .@size; .@i++)
		set getelementofarray(getarg(1),.@i),getd(".@temp_array" + .@type$ + "[" + .@i + "]");
	return .@size;
}

//= Basically inarray but for the target npc
//= getnpc_arrindex("<Target Array Variable>",<Search Value>{,<"Target NPC">});
//= .@array_size = getnpc_array(".target_array",.@temp_array,"npc_name");
function	script	getnpc_arrindex	{
	return inarray(getnpc_var(getarg(0),getarg(2,strnpcinfo(3))),getarg(1));
}

//= Push an element to the target array and returns the current array size.
function	script	array_push	{
	set getelementofarray(getarg(0),(.@size = getarraysize(getarg(0)))), getarg(1);
	return .@size;
}

/* Functions for storing/fetching multiple array of data without using unreadable getd
** SprintVar("<Base Array Variable>",<Value>{,<"NPC Name">};
** SprintIndex"<Base Array Variable>",<Value>,<Index>{,<"NPC Name">};

-- Use Cases
set SprintVar(".var", 1), 20;
Equivalent : .var_1 = 20;
.@value = SprintVar(".var", 1); //Returns 20

set SprintVar(".name$, 1), "Hello";
Equivalent : .name_1$ = "Hello";
.@value$ = SprintVar(".name$, 1); //Returns Hello

//= Usage with array_push function
array_push(SprintVar(".var", 1), 30);
.@index_0 = SprintIndex(".var", 1, 0); //Returns 20
.@index_1 = SprintIndex(".var", 1, 1); //Returns 30

//= Usage with getarraysize
.@size = getarraysize(SprintVar(".var", 1)); // Returns 2
*/

function	script	SprintVar	{
    return getvariableofnpc(getd(sprintf((compare(getarg(0), "$") ? (delchar(getarg(0), getstrlen(getarg(0)) - 1) + "_%d$") : (getarg(0) + "_%d")), getarg(1))), getarg(2,strnpcinfo(3)));
}

function	script	SprintIndex	{
    return getvariableofnpc(getd(sprintf((compare(getarg(0), "$") ? (delchar(getarg(0), getstrlen(getarg(0)) - 1) + "_%d$[%d]") : (getarg(0) + "_%d[%d]")), getarg(1), getarg(2))), getarg(3,strnpcinfo(3)));
}

/* Instance commands shorcuts
instance_warning(<TYPE>);
instance_hide(<NPC Name>,<bool>);
instance_enable(<NPC Name>,<bool>);
instance_event(<NPC Name>,<Event Name>,<Attach Player Bool>);
*/
function	script	instance_warning	{
	.@type = (getargcount() < 1 ? 0 : getarg(0));
	.@md_name$ = (getargcount() == 2 ? getarg(1,"") : "");
	switch(.@type){
		case 0: 
			mes "^FF0000Please note that, any abnormal monster handling inside the instance such as taming is not considered normal and may hinder the instance progress.^000000"; 
			break;
			
		case 1:
			mes "^4D4DFFThis place is a memorial dungeon.";
			mes "Please form a party and try again.^000000";
			mes "To create a party with the following command ^4D4DFF'/organize PARTY NAME'.^000000";
			end;
			
		case 2:
			dispbottom "The reservation of the instance '" + .@md_name$ + "' has failed due to an active instance.";
			break;
			
		case 3:
			mes "^FF0000In beginners mode, the player transform into a monster. Please not that existing transformation effect will disappear after entering and proceeding.^000000"; 
			break;
			
		case 4:
			mes "^4D4DFFThis place is a memorial dungeon.";
			mes "You have to be a party leader to create a memorial dungeon.^000000";
			end;
	}
	return;
}

//= Instance event control so I don't have to type instance_npcname and concatinate something every single time I have to do an event call.
function	script	instance_hide	{
	if(getarg(1))
		hideonnpc instance_npcname(getarg(0));
	else
		hideoffnpc instance_npcname(getarg(0));
	return;
}

function	script	instance_enable	{
	if(getarg(1))
		enablenpc instance_npcname(getarg(0));
	else
		disablenpc instance_npcname(getarg(0));
	return;
}

function	script	instance_event	{
	if(getarg(2))
		doevent instance_npcname(getarg(0)) + "::" + getarg(1);
	else
		donpcevent instance_npcname(getarg(0)) + "::" + getarg(1);
	return;
}

//= Instance variable control because I hate how 'variables breaks my IDE.
function	script	get_instance_var	{
	return getd("'" + getarg(0));
}

function	script	set_instance_var	{
	setd("'" + getarg(0),getarg(1));
	return;
}

function	script	inc_instance_var	{
	setd("'" + getarg(0), getd("'" + getarg(0)) + 1);
	return;
}

//= Shortcut Functions
function	script	cloaknpc	{
	if(getargcount() > 2){
		if(getarg(1))
			cloakonnpc getarg(0),getarg(2);
		else
			cloakoffnpc getarg(0),getarg(2);
	} else {
		if(getarg(1))
			cloakonnpc getarg(0);
		else
			cloakoffnpc getarg(0);
	}
	return;
}

function	script	pctalk	{
	unittalk getcharid(3),strcharinfo(0) + ":" + getarg(0),bc_self;
	return;
}

function	script	pcblock	{
	if(getargcount() < 2)
		setpcblock PCBLOCK_NPC,getarg(0);
	else
		setpcblock PCBLOCK_NPC,getarg(0),getarg(1);
	return;
}

/* Search the index of a value in from another NPC Variable
** npc_resize(<SIZE>);
Sizes :
Size_Small
Size_Medium
Size_Large

-- Use Cases
OnInit:
OnInstanceInit:
	npc_resize(Size_Large);
*/

function	script	npc_resize	{
	.@npc$ = .@name$ = strnpcinfo(0);
	if(instance_live_info(ILI_NAME) != "")
		.@npc$ = instance_npcname(.@npc$);
	getunitdata getnpcid(0,.@npc$),.@npc;
	setnpcdisplay(.@npc$,.@name$,.@npc[UNPC_CLASS],getarg(0));
	return;
}

function	script	mapexists	{
	return getmapusers(getarg(0));
}

function	script	create_menu	{
	return implode(getarg(0),":");
}

function	script	getday	{
	switch(getarg(0)) {
		case SUNDAY: .@d$ = "Sunday"; break;
		case MONDAY: .@d$ = "Monday"; break;
		case TUESDAY: .@d$ = "Tuesday"; break;
		case WEDNESDAY: .@d$ = "Wednesday"; break;
		case THURSDAY: .@d$ = "Thursday"; break;
		case FRIDAY: .@d$ = "Friday"; break;
		case SATURDAY: .@d$ = "Saturday"; break;
	}
	return .@d$;
}

function	script	safe_rand	{
	if ( !getargcount() ) 
		return;
	.@min = getarg(0);
	
	if ( getargcount() > 1 )
		.@max = getarg(1);
	else {
		if ( .@min == 0 ) {
			errormes "safe_rand : Random value can't be 0.";
			end;
		}
		if ( .@min == 1 )
			return 0;
		else
			return rand(.@min);
	}
	
	if ( .@min == .@max )
		return .@min;

	if ( .@max < .@min ) {
		.@temp = .@min;
		.@min = .@max;
		.@max = .@temp;
	}
	
	return rand(.@min, .@max);
}

function	script	validate_itemid	{
	return getiteminfo(getarg(0), ITEMINFO_TYPE);
}

//= DUMMY NPC's
-	script(CLOAKED)	dummynpc	-1,{
	end;
}

-	script	dummynpc2	-1,{ 
	end; 
}