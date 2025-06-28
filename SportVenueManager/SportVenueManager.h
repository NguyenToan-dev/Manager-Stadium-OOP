#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SportVenueManager.h"

class SportVenueManager : public QMainWindow
{
    Q_OBJECT

public:
    SportVenueManager(QWidget *parent = nullptr);
    ~SportVenueManager();

private:
    Ui::SportVenueManagerClass ui;
};

