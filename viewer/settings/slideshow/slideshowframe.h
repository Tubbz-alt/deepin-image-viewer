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
#ifndef SLIDESHOWFRAME_H
#define SLIDESHOWFRAME_H

#include <DFrame>

DWIDGET_USE_NAMESPACE
typedef DFrame QFrToDFrame;

class QVBoxLayout;
class SlideshowFrame : public QFrToDFrame
{
    Q_OBJECT
public:
    explicit SlideshowFrame(QWidget *parent);

signals:
    void requestReset();

private:
    int defaultDuration() const;
    void initPreview();
    void initInterval();

private:
    QVBoxLayout *m_layout;
};

#endif // SLIDESHOWFRAME_H
