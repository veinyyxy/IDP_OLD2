#include "ComposerItem.h"

#include "CipasComposerItemCommand.h"

CipasComposerItemCommand::CipasComposerItemCommand(CComposerItem* item, const QString& text, QUndoCommand* parent)
	: QUndoCommand( text, parent ), mItem( item ), mFirstRun( true )
{
}


CipasComposerItemCommand::~CipasComposerItemCommand(void)
{
}

void CipasComposerItemCommand::redo()
{
	if ( mFirstRun )
	{
		mFirstRun = false;
		return;
	}
	restoreState( mAfterState );
}

void CipasComposerItemCommand::undo()
{
	restoreState( mPreviousState );
}

bool CipasComposerItemCommand::containsChange() const
{
	return !( mPreviousState.isNull() || mAfterState.isNull() || mPreviousState.toString() == mAfterState.toString() );
}

void CipasComposerItemCommand::savePreviousState()
{
	saveState( mPreviousState );
}

void CipasComposerItemCommand::saveAfterState()
{
	saveState( mAfterState );
}

void CipasComposerItemCommand::saveState( QDomDocument& stateDoc ) const
{
	if ( mItem )
	{
		stateDoc.clear();
		QDomElement documentElement = stateDoc.createElement( "ComposerItemState" );
		mItem->writeXML( documentElement, stateDoc );
		stateDoc.appendChild( documentElement );
	}
}

void CipasComposerItemCommand::restoreState( QDomDocument& stateDoc ) const
{
	if ( mItem )
	{
		mItem->readXML( stateDoc.documentElement().firstChild().toElement(), stateDoc );
		mItem->repaint();
	}
}

CipasComposerMergeCommand::CipasComposerMergeCommand( Context c, CComposerItem* item, const QString& text ): CipasComposerItemCommand( item, text ), mContext( c )
{
}

CipasComposerMergeCommand::~CipasComposerMergeCommand()
{
}

bool CipasComposerMergeCommand::mergeWith( const QUndoCommand * command )
{
	const CipasComposerItemCommand* c = dynamic_cast<const CipasComposerItemCommand*>( command );
	if ( !c )
	{
		return false;
	}
	mAfterState = c->afterState();
	return true;
}

