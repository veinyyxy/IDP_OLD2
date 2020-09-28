#include "ComposerItem.h"

#include "Cipasitemcommand.h"

CipasItemCommand::CipasItemCommand( CComposerItem* item, const QString& text, QUndoCommand* parent):
    QUndoCommand( text, parent ), mItem( item ), mFirstRun( true )
{
}

CipasItemCommand::~CipasItemCommand()
{
}

void CipasItemCommand::undo()
{
  restoreState( mPreviousState );
}

void CipasItemCommand::redo()
{
  if ( mFirstRun )
  {
    mFirstRun = false;
    return;
  }
  restoreState( mAfterState );
}

bool CipasItemCommand::containsChange() const
{
  return !( mPreviousState.isNull() || mAfterState.isNull() || mPreviousState.toString() == mAfterState.toString() );
}

void CipasItemCommand::savePreviousState()
{
  saveState( mPreviousState );
}

void CipasItemCommand::saveAfterState()
{
  saveState( mAfterState );
}

void CipasItemCommand::saveState( QDomDocument& stateDoc ) const
{
  if ( mItem )
  {
    stateDoc.clear();
    QDomElement documentElement = stateDoc.createElement( "ComposerItemState" );
    mItem->writeXML( documentElement, stateDoc );
    stateDoc.appendChild( documentElement );
  }
}

void CipasItemCommand::restoreState( QDomDocument& stateDoc ) const
{
  if ( mItem )
  {
    mItem->readXML( stateDoc.documentElement().firstChild().toElement(), stateDoc );
    mItem->repaint();
  }
}

CipasMergeCommand::CipasMergeCommand( Context c, CComposerItem* item, const QString& text ): CipasItemCommand( item, text ), mContext( c )
{
}

CipasMergeCommand::~CipasMergeCommand()
{
}

bool CipasMergeCommand::mergeWith( const QUndoCommand * command )
{
  const CipasItemCommand* c = dynamic_cast<const CipasItemCommand*>( command );
  if ( !c )
  {
    return false;
  }
  mAfterState = c->afterState();
  return true;
}

