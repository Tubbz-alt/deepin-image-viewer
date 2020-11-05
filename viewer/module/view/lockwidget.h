/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LOCKWIDGET_H
#define LOCKWIDGET_H

#include <QLabel>
#include "widgets/themewidget.h"

#include <DLabel>

DWIDGET_USE_NAMESPACE
typedef DLabel QLbtoDLabel;

class LockWidget : public ThemeWidget {
    Q_OBJECT
public:
    LockWidget(const QString &darkFile, const QString &lightFile,
                  QWidget* parent = 0);
    ~LockWidget();

public slots:
    void setContentText(const QString &text);
protected:

private:
    void onThemeChanged(ViewerThemeManager::AppTheme theme);
    QLbtoDLabel* m_thumbnailLabel;
    QPixmap m_logo;
    QString m_picString;
    bool m_theme;
    QLbtoDLabel *m_bgLabel;
    QLbtoDLabel *m_lockTips;
};
#endif // LOCKWIDGET_H
