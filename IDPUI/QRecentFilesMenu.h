#ifndef QRECENTFILESMENU_H
#define QRECENTFILESMENU_H
/** @file 
* @brief   历史菜单
* @author    易家祥
* @date      2013年3月
* @version   version
  @note      2013年 6月
*/
#include <QMenu>
#include <QStringList>

#include "idpui_global.h"
 /** @class   QRecentFilesMenu
    @brief   历史菜单
    @author  易家祥
    @note  历史菜单的对外接口
*/ 
class IDPUI_EXPORT QRecentFilesMenu : public QMenu
{
    Q_OBJECT

    Q_PROPERTY(int maxCount READ maxCount WRITE setMaxCount)
    Q_PROPERTY(QString format READ format WRITE setFormat)
public:
   	/** @brief Constructs a menu with parent parent. */
    QRecentFilesMenu(QWidget * parent = 0 );

 	/** @brief Constructs a menu with a title and a parent. */
    QRecentFilesMenu(const QString & title, QWidget * parent = 0 );

 	/** @brief Returns the maximum number of entries in the menu. */
    int maxCount() const;

    	/** @brief  This property holds the string used to generate the item text.
     * %d is replaced by the item number
     * %s is replaced by the item text
     * The default value is "%d %s".
     */
    void setFormat(const QString &format);

     	/** @brief  Returns the current format. /sa setFormat*/
    const QString & format() const;

 	/** @brief  Saves the state of the recent entries.
     * Typically this is used in conjunction with QSettings to remember entries
     * for a future session. A version number is stored as part of the data.
     * Here is an example:
     * QSettings settings;
     * settings.setValue("recentFiles", recentFilesMenu->saveState());
     */
    void saveRecent(const QString &filePath);

   	/** @brief   Restores the recent entries to the state specified.
     * Typically this is used in conjunction with QSettings to restore entries from a past session.
     * Returns false if there are errors.
     * Here is an example:
     * QSettings settings;
     * recentFilesMenu->restoreState(settings.value("recentFiles").toByteArray());
     */
    void loadRecent(const QString &filePath);

public slots:
    	/** @brief  添加菜单*/
    void addRecentFile(const QString &fileName);

    	/** @brief  Removes all the menu's actions.*/
    void clearMenu();

    	/** @brief  Sets the maximum number of entries int he menu.*/
    void setMaxCount(int);
signals:
    //! This signal is emitted when a recent file in this menu is triggered.
    void recentFileTriggered(const QString & filename);

private slots:
    void menuTriggered(QAction*);
    void updateRecentFileActions();
private:
    int m_maxCount;
    QString m_format;
    QStringList m_files;
};

#endif // QRECENTFILEMENU_H
