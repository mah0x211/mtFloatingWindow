#include"protoType.h"

void  HandleKeyDown( EventRecord theEvent )
{
	long		theChoice;

	if(( theEvent.modifiers & cmdKey  ) == cmdKey
		&& ( theEvent.modifiers & shiftKey  ) != shiftKey
		&& ( theEvent.modifiers & optionKey  ) != optionKey
		&& ( theEvent.modifiers & controlKey  ) != controlKey )
	{
			theChoice = MenuKey( theEvent.message & charCodeMask );
			HandleMenuChoice( theChoice );
	}
}
