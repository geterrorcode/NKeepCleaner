#pragma once

#include <QLayout>

class ContainerLayout : public QLayout
{
    Q_OBJECT

public:
    ContainerLayout(QWidget* parent = nullptr);
    ~ContainerLayout();

    void addLayout(QLayout* layout, Qt::Alignment alignment = Qt::Alignment());
    void addWidget(QWidget* widget, Qt::Alignment alignment = Qt::Alignment());

    void setAlignLayout(QLayout* layout, Qt::Alignment alignment );
    void setAlignWidget(QWidget* widget, Qt::Alignment alignment );

    void addItem(QLayoutItem *item) override;
    QSize sizeHint() const override;
    QSize minimumSize() const override;
    int count() const override;
    QLayoutItem* itemAt(int) const override;
    QLayoutItem* takeAt(int) override;
    void setGeometry(const QRect &rect) override;

private:
    QList<QLayoutItem*> m_list;

};
