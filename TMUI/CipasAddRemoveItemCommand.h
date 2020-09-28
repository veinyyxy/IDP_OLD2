#pragma once

#include <QUndoCommand>

#include "tmui_global.h"

class CComposerItem;
class CCipasComposition;

class TMUI_EXPORT CCipasAddRemoveItemCommand :public QObject,public QUndoCommand
{
	Q_OBJECT
public:
	enum State
	{
		Added = 0,
		Removed
	};
	CCipasAddRemoveItemCommand(State s, CComposerItem* item, CCipasComposition* c, const QString& text, QUndoCommand* parent = 0 );
	~CCipasAddRemoveItemCommand();
	void redo();
	void undo();

signals:
	void itemAdded( CComposerItem* item );
	void itemRemoved( CComposerItem* item );

private:
	CComposerItem* mItem;
	CCipasComposition* mComposition;
	State mState;
	bool mFirstRun; //flag to prevent execution when the command is pushed to the QUndoStack

	//changes between added / removed state
	void switchState();

};

