#pragma once
#include <QUndoStack>
#include <QDomDocument>

class CComposerItem;

class CipasComposerItemCommand :public QUndoCommand
{
public:
	CipasComposerItemCommand(CComposerItem* item, const QString& text, QUndoCommand* parent = 0 );
	virtual ~CipasComposerItemCommand(void);
	/**Reverses the command*/
	void undo();
	/**Replays the command*/
	void redo();

	/**Saves current item state as previous state*/
	void savePreviousState();
	/**Saves current item state as after state*/
	void saveAfterState();

	QDomDocument previousState() const { return mPreviousState.cloneNode().toDocument(); }
	QDomDocument afterState() const { return mAfterState.cloneNode().toDocument(); }

	/**Returns true if previous state and after state are valid and different*/
	bool containsChange() const;

protected:
	/**Target item of the command*/
	CComposerItem* mItem;
	/**XML that saves the state before executing the command*/
	QDomDocument mPreviousState;
	/**XML containing the state after executing the command*/
	QDomDocument mAfterState;

	/**Flag to prevent the first redo() if the command is pushed to the undo stack*/
	bool mFirstRun;

	void saveState( QDomDocument& stateDoc ) const;
	void restoreState( QDomDocument& stateDoc ) const;



};



class CipasComposerMergeCommand :
	public CipasComposerItemCommand
{
public:
	enum Context
	{
		Unknown = 0,
		//composer label
		ComposerLabelSetText,
		ComposerLabelSetId,
		//composer map
		ComposerMapRotation,
		ComposerMapAnnotationDistance,
		//composer legend
		ComposerLegendText,
		LegendSymbolWidth,
		LegendSymbolHeight,
		LegendLayerSpace,
		LegendSymbolSpace,
		LegendIconSymbolSpace,
		LegendBoxSpace,
		//composer picture
		ComposerPictureRotation,
		// composer scalebar
		ScaleBarLineWidth,
		ScaleBarHeight,
		ScaleBarSegmentSize,
		ScaleBarSegmentsLeft,
		ScaleBarNSegments,
		ScaleBarUnitText,
		ScaleBarMapUnitsSegment,
		ScaleBarLabelBarSize,
		ScaleBarBoxContentSpace,
		// composer table
		TableMaximumFeatures,
		TableMargin,
		TableGridStrokeWidth,
		//composer shape
		ShapeRotation,
		ShapeOutlineWidth,
		//composer arrow
		ArrowOutlineWidth,
		ArrowHeadWidth,
		//item
		ItemOutlineWidth
	};

	CipasComposerMergeCommand( Context c, CComposerItem* item, const QString& text );
	~CipasComposerMergeCommand();

	bool mergeWith( const QUndoCommand * command );
	int id() const { return ( int )mContext; }

private:
	Context mContext;
};