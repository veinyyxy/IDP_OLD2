
#include "ComposerItem.h"
#include "CipasComposition.h"

#include "CipasAddRemoveItemCommand.h"

CCipasAddRemoveItemCommand::CCipasAddRemoveItemCommand( State s, CComposerItem* item, CCipasComposition* c, const QString& text, QUndoCommand* parent ):
QUndoCommand( text, parent ), mItem( item ), mComposition( c ), mState( s ), mFirstRun( true )
{
}


CCipasAddRemoveItemCommand::~CCipasAddRemoveItemCommand()
{
	if ( mState == Removed ) //command class stores the item if removed from the composition
	{
		delete mItem;
	}
}

void CCipasAddRemoveItemCommand::redo()
{
	if ( mFirstRun )
	{
		mFirstRun = false;
		return;
	}
	switchState();
}

void CCipasAddRemoveItemCommand::undo()
{
	if ( mFirstRun )
	{
		mFirstRun = false;
		return;
	}
	switchState();
}

void CCipasAddRemoveItemCommand::switchState()
{
	if ( mState == Added )
	{
		if ( mComposition )
		{
			mComposition->removeItem( mItem );
		}
		emit itemRemoved( mItem );
		mState = Removed;
	}
	else //Removed
	{
		if ( mComposition )
		{
			mComposition->addItem( mItem );
		}
		emit itemAdded( mItem );
		mState = Added;
	}
}

