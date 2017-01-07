#ifndef THUMBNAILLISTVIEW_H
#define THUMBNAILLISTVIEW_H

#include <QListView>
#include <QFutureWatcher>

class ScrollBar;
class QStandardItemModel;
class ThumbnailDelegate;
class ThumbnailListView : public QListView
{
    Q_OBJECT
public:
    struct ItemInfo {
        QString name = QString();
        QString path = QString();
        QPixmap thumb = QPixmap();
    };

    explicit ThumbnailListView(QWidget *parent = 0);
    ~ThumbnailListView();
    void clearData();
    void updateThumbnail(const QString &path);
    void setIconSize(const QSize &size);
    void insertItem(const ItemInfo &info);
    void updateItem(const ItemInfo &info);
    void removeItems(const QStringList &paths);
    bool contain(const QModelIndex &index) const;
    int indexOf(const QString &path);
    int count() const;
    int hOffset() const;
    const ItemInfo itemInfo(const QModelIndex &index);
    const QList<ItemInfo> ItemInfos();
    const QList<ItemInfo> selectedItemInfos();

signals:
    void mousePressed(QMouseEvent *e);
    void changeItemSize(bool increase);

protected:
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) Q_DECL_OVERRIDE;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags) Q_DECL_OVERRIDE;
    int horizontalOffset() const Q_DECL_OVERRIDE;
    int verticalOffset() const Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *e) Q_DECL_OVERRIDE;

private:
    int contentsHMargin() const;
    int contentsVMargin() const;
    int maxColumn() const;
    QModelIndex movePageUp();
    QModelIndex movePageDown();
    const QVariantList getVariantList(const ItemInfo &info);

private:
    QRect m_selectionRect;
    QStandardItemModel *m_model;
    ThumbnailDelegate *m_delegate;
    ScrollBar *m_scrollbar;
};

#endif // THUMBNAILLISTVIEW_H
