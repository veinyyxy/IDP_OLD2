
#ifndef CIPASITEMCOMMAND_H
#define CIPASITEMCOMMAND_H


#include <QUndoCommand>
#include <QDomDocument>

#include "tmui_global.h"

class CComposerItem;

/**Undo command to undo/redo all composer item related changes*/
class TMUI_EXPORT CipasItemCommand : public QUndoCommand
{
  public:
    CipasItemCommand( CComposerItem* item, const QString& text, QUndoCommand* parent = 0 );
    virtual ~CipasItemCommand();

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

/**A composer command that merges together with other commands having the same context (=id). Keeps the oldest previous state and uses the
  newest after state. The purpose is to avoid too many micro changes in the history*/
class TMUI_EXPORT CipasMergeCommand: public CipasItemCommand
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

    CipasMergeCommand( Context c, CComposerItem* item, const QString& text );
    ~CipasMergeCommand();

    bool mergeWith( const QUndoCommand * command );
    int id() const { return ( int )mContext; }

  private:
    Context mContext;
};

#endif // QGSCOMPOSERITEMCOMMAND_H
