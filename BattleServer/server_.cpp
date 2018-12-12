#include "server_socket.h"
#include "ui_serverui.h"

ServerUI::ServerUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerUI)
{
    ui->setupUi(this);
}

ServerUI::~ServerUI()
{
    delete ui;
}
