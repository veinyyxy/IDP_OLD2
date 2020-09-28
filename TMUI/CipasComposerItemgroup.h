#include <QSet>

#include "tmui_global.h"
#include "ComposerItem.h"

class TMUI_EXPORT CipasComposerItemGroup: public CComposerItem
{
    Q_OBJECT
  public:
    CipasComposerItemGroup( CCipasComposition* c );
    ~CipasComposerItemGroup();

    /** return correct graphics item type. Added in v1.7 */
    virtual int type() const { return ComposerItemGroup; }

    /**Adds an item to the group. All the group members are deleted
     if the group is deleted*/
    void addItem( CComposerItem* item );
    /**Removes the items but does not delete them*/
    void removeItems();
    /**Draw outline and ev. selection handles*/
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    /**Sets this items bound in scene coordinates such that 1 item size units
       corresponds to 1 scene size unit*/
    void setSceneRect( const QRectF& rectangle );
    /** resizes an item in x- and y direction (scene coordinates)*/
    void resize( double dx, double dy );

    /** stores state in Dom node
       * @param elem is Dom element corresponding to 'Composer' tag
       * @param doc is the Dom document
       */
    bool writeXML( QDomElement& elem, QDomDocument & doc ) const { return true; }

    /** sets state from Dom document
       * @param itemElem is Dom node corresponding to item tag
       * @param doc is the Dom document
       */
    bool readXML( const QDomElement& itemElem, const QDomDocument& doc ) { return true; }

    QSet<CComposerItem*> items() { return mItems; }

  signals:
    void childItemDeleted( CComposerItem* item );

  protected:
    void drawFrame( QPainter* p );

  private:
    QSet<CComposerItem*> mItems;
    QRectF mSceneBoundingRectangle;
};


