#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <qDebug>
//#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    ui->textBrowser->clear();

    ui->textBrowser->setFontPointSize(14);
    ui->textBrowser->setFontUnderline(false);// подчёрквание текста

  //  connect(ui->actionOpen, SIGNAL(clicked(bool)), this, SLOT(open()) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    QString name = QFileDialog::getOpenFileName(this, "открыть", QDir::currentPath(), "" );

    if(name.isEmpty())
        return;

    QFile f(name);

    ui->textBrowser->clear();

    if(name.endsWith(".txt"))
    {
       openTXT(&f);
    }
    if(name.endsWith(".fb2"))
    {
        openFB2(&f);
    }
}

void MainWindow::openTXT(QFile *f)
{
        QTextCodec* codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
    if (!f->open(QIODevice::ReadOnly | QIODevice::Text))
    {
      qDebug() << "файл не открыт";
      return;
    }
    QTextStream in(f);
     ui->textBrowser->setText(in.readAll());
     ui->textBrowser->verticalScrollBar()->setValue(0);
     f->close();

}

void MainWindow::openFB2(QFile *f)
{
    if (!f->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "файл не открыт";
        return;
    }

    QXmlStreamReader sr(f);
    QString book;
    QString imgId;
    QString imgType;
    while( !sr.atEnd() )
    {
        switch( sr.readNext() )
        {
        case QXmlStreamReader::NoToken:
            qDebug() << "QXmlStreamReader::NoToken";
            break;
        case QXmlStreamReader::StartDocument:
            book = "<!DOCTYPE HTML><html><body style=\"font-size:14px\">";
            break;
        case QXmlStreamReader::EndDocument:
            book.append("</body></html>");
            break;
        case QXmlStreamReader::StartElement:
            thisToken.append( sr.name().toString() );
            if( sr.name().toString() == "image" ) // расположение рисунков
            {
                if(sr.attributes().count() > 0)
                    book.append("<p align=\"center\">"+sr.attributes().at(0).value().toString()+"</p>");
            }
            if(sr.name() == "binary") // хранилище рисунков
            {
                imgId = sr.attributes().at(0).value().toString();
                imgType = sr.attributes().at(1).value().toString();
            }
            break;
        case QXmlStreamReader::EndElement:
            if( thisToken.last() == sr.name().toString() )
                thisToken.removeLast();
            else
                qDebug() << "error token";
            break;
        case QXmlStreamReader::Characters:
            if( sr.text().toString().contains( QRegExp("[A-Z]|[a-z]|[А-Я]|[а-я]") )) // если есть текст в блоке
            {
                if(thisToken.contains("description")) // ОПИСАНИЕ КНИГИ
                {
                    break; // не выводим
                }
                if(thisToken.contains("div"))
                    break;
                if(!thisToken.contains( "binary" ))
                    book.append("<p>" + sr.text().toString() + "</p>");
            }
            if(thisToken.contains( "binary" ) )//для рисунков
            {
                QString image = "<img src=\"data:"
                        + imgType +";base64,"
                        + sr.text().toString()
                        + "\"/>";
                book.replace("#"+imgId, image);
            }
            break;
        }
    }
    f->close();
    ui->textBrowser->setHtml(book);
    ui->textBrowser->verticalScrollBar()->setValue(0);//
}

void MainWindow::on_actionOpen_triggered()
{
    open();
}
